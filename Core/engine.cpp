#include "stdafx.h"
#include "engine.h"
#include <mmsystem.h>
#include <stdlib.h>
#include <time.h>

#include "../Graphics/displaymode.h"
#include "../Input/keyboard.h"

#include "../Network/socket.h"
#include "../Network/nic.h"
#include "../Network/ipaddress.h"

#include "../Utility/logcontext.h"
#include "../Utility/logmacros.h"
#include "../Utility/exception.h"
#include "../Utility/logmacros.h"
#include "../Utility/exception.h"
#include "../Utility/strutil.h"

#undef min

using namespace Core;

Engine::Engine(HINSTANCE instance, Graphics::SplashScreen& splash)
: LogContext("Core::Engine")
, instance_(instance)
, app_active_(false)
, restore_needed_(false)
, shutdown_(false)
, splash_(splash)
, do_menu_state_transition_(false)
, do_game_state_transition_(false)
, network_port_(0)
{
}

Engine::~Engine()
{
    Shutdown();
}

void Engine::Initialise()
{
    auto rv = CoInitialize(NULL);
    if (FAILED(rv))
    {
        THROW_COM("CoInitialize failed");
    }
    srand(unsigned int(time(NULL)));

    Utility::OpenLogFile("log.txt");
    LOG("Starting...");

    hw_config_.Load("config.txt");

    if(hw_config_.IsFullscreen())
    {
        SetFullscreen();
    }

    restore_needed_ = hw_config_.IsFullscreen();
    if(restore_needed_)
    {
        ShowBlackWindow();
        splash_.Center();
    }

    splash_.SetTotalItems(200);
    splash_.SetPos(0);

    Graphics::Window::Register(instance_);
    window_.reset(new Graphics::Window(instance_, this, "Yahtzee"));
    window_->Create(&hw_config_);
    splash_.SetPos(5);

    gfx_device_.reset(new Graphics::Device);
    gfx_device_->Initialise(window_->GetHandle(), &hw_config_);
    splash_.SetPos(10);

    viewport_manager_.reset(new Graphics::ViewportManager);
    splash_.SetPos(15);

    sprites_thinkable_.reset(new Graphics::SpriteContainer);
    sprites_drawable_.reset(new Graphics::SpriteContainer);
    sprites_clickable_.reset(new Graphics::SpriteContainer);
    splash_.SetPos(20);

    timeout_manager_.reset(new TimeoutManager);
    splash_.SetPos(25);

    inp_device_.reset(new Input::Device(this));
    inp_device_->Initialise(instance_, window_->GetHandle());
    splash_.SetPos(35);

    try
    {
        aud_device_.reset(new Audio::Device);
        aud_device_->Initialise(window_->GetHandle(), &hw_config_);
    }
    catch (const std::runtime_error& )
    {
        LOG("Continuing without audio...");
        aud_device_->Enable(false);
    }
    splash_.SetPos(40);

    InitialiseNetworking();
    splash_.SetPos(45);
}

void Engine::Shutdown()
{
    if(shutdown_)
    {
        return;
    }

    LOG("Stopping...");

    if(restore_needed_)
    {
        Graphics::RestoreDisplayMode();
    }

    current_menu_state_.reset();
    current_game_state_.reset();

    viewport_manager_.reset();
    sprites_thinkable_.reset();
    sprites_drawable_.reset();
    sprites_clickable_.reset();
    sprite_cursor_.reset();
    sprite_hover_.reset();

    connection_list_.reset();
    socket_list_.reset();
    Network::Socket::ShutdownLibrary();

    aud_device_->Shutdown();
    aud_device_.reset();

    inp_device_->Shutdown();
    inp_device_.reset();

    timeout_manager_.reset();

    gfx_device_->Shutdown(window_->GetHandle());
    gfx_device_.reset();

    window_->AllowCursor();
    window_->Delete();
    window_.reset();
    Graphics::Window::Unregister(instance_);

    hw_config_.Save("config.txt");
    CoUninitialize();

    shutdown_ = true;
}

void Engine::Run()
{
    MSG msg;
    bool exit = false;
    unsigned long time_now;
    unsigned long time_last = timeGetTime();
    float time_delta;

    LOG("Entering core loop");
    for( ; ; )
    {
        // Pump any window messages.
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if(msg.message == WM_QUIT)
            {
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // If the main window is not minimised and is active then run
        // a game frame, otherwise sleep until another window message arrives.
        //if(!window_->IsMinimised() && app_active_)
        //{
            time_now = timeGetTime();
            inp_device_->OnBeginFrame();
            gfx_device_->OnBeginFrame();

            time_delta = float(time_now - time_last) / 1000.0f;

            inp_device_->Poll();
            timeout_manager_->CheckTimeouts();

            try
            {
                ScanForIncomingPackets();

                if(current_menu_state_)
                {
                    // There's a menu page active, run its logic code as 
                    // well as the game's, if the game is active.
                    current_menu_state_->DoFrameLogic(time_delta);
                    if(current_game_state_)
                    {
                        current_game_state_->DoFrameLogic(time_delta);
                    }

                    // Only draw the menu page though!
                    current_menu_state_->DrawFrame(time_delta);
                }
                else
                {
                    // No menu page is active, run the game logic and
                    // draw it.
                    if(current_game_state_)
                    {
                        current_game_state_->DoFrameLogic(time_delta);
                        current_game_state_->DrawFrame(time_delta);
                    }
                }
            }
            catch(Network::SocketException& e)
            {
                HandleSocketException(e);
            }

            if(sprite_cursor_)
            {
                sprite_cursor_->Think(time_delta);
                sprite_cursor_->Draw(time_delta, gfx_device_, Graphics::SpritePtr());
            }

            gfx_device_->OnEndFrame();
            inp_device_->OnEndFrame();
            DoStateTransition();

            Sleep(1);
            time_last = time_now;
        //}
        //else
        //{
        //    LOG("Core loop going to sleep");
        //    WaitMessage();
        //    LOG("Core loop woke up, continuing.");
        //}
    }

    LOG("Core loop ended");
}

void Engine::SetCurrentMenuState(StatePtr state)
{
    if(current_menu_state_)
    {
        current_menu_state_->OnEnd();
    }
    current_menu_state_ = state;
    current_menu_state_->OnBegin();
    current_menu_state_->SetupLists();
}

void Engine::SetCurrentGameState(StatePtr state)
{
    if(current_game_state_)
    {
        current_game_state_->OnEnd();
    }
    current_game_state_ = state;
    current_game_state_->OnBegin();
    current_game_state_->SetupLists();
}

void Engine::DoStateTransition()
{
    if(do_menu_state_transition_)
    {
        do_menu_state_transition_ = false;
        if(current_menu_state_)
        {
            current_menu_state_->OnEnd();
        }
        current_menu_state_ = next_menu_state_;
        next_menu_state_.reset();
        if(current_menu_state_)
        {
            current_menu_state_->OnBegin();
            current_menu_state_->SetupLists();
        }
        else
        {
            // The current menu page has been closed, allow
            // the game to add its sprites to the visible and
            // clickable lists
            if(current_game_state_)
            {
                current_game_state_->SetupLists();
            }
        }
    }

    if(do_game_state_transition_)
    {
        do_game_state_transition_ = false;
        if(current_game_state_)
        {
            current_game_state_->OnEnd();
        }
        current_game_state_ = next_game_state_;
        next_game_state_.reset();
        if(current_game_state_)
        {
            current_game_state_->OnBegin();
            // Don't allow the game to add its sprites to the
            // clickable and visible lists if there's a menu
            // page active
            if(!current_menu_state_)
            {
                current_game_state_->SetupLists();
            }
        }
    }
}

void Engine::SetCursor(Graphics::SpriteCursorPtr sprite_cursor)
{
    Graphics::Vector position;
    if(sprite_cursor_)
    {
        position = sprite_cursor_->GetPosition();
    }
    sprite_cursor_ = sprite_cursor;
    sprite_cursor_->SetPosition(position);
}

void Engine::SetFullscreen()
{
    hw_config_.SetFullscreen(true);

    Graphics::DisplayModes modes = Graphics::GetDisplayModes();
    Graphics::DisplayModes::iterator m;
    for(m = modes.begin(); m != modes.end(); ++m)
    {
        if(m->Equals(hw_config_.GetWindowWidth(), hw_config_.GetWindowHeight(),
                     hw_config_.GetBpp(), hw_config_.GetRefreshRate()))
        {
            m->Set();
            break;
        }
    }
    if(m == modes.end())
    {
        LOG("Couldn't find the graphics mode [" << hw_config_.GetWindowWidth() << "x"
            << hw_config_.GetWindowHeight() << ", " << hw_config_.GetBpp() << " BPP, "
            << hw_config_.GetRefreshRate() << " Hz], setting windowed mode instead");
        hw_config_.SetFullscreen(false);
        return;
    }

    LOG("The screen mode changed successfully");
    if(window_)
    {
        LOG("Changing the window style to fullscreen");
        window_->SetStyle(Graphics::Window::STYLE_FULLSCREEN);
        window_->DisallowCursor();
    }
}

void Engine::SetWindowed()
{
    hw_config_.SetFullscreen(false);
    Graphics::RestoreDisplayMode();

    if(window_)
    {
        window_->SetStyle(Graphics::Window::STYLE_WINDOWED);
        window_->DisallowCursor();
    }
}

void Engine::ShowBlackWindow()
{
    black_window_.reset(new Graphics::BlackWindow(instance_));
}

void Engine::HideBlackWindow()
{
    black_window_.reset();
}

void Engine::OnSpriteClicked(Graphics::SpritePtr sprite)
{
    if(current_menu_state_)
    {
        current_menu_state_->OnSpriteClicked(sprite);
    }
    else if(current_game_state_)
    {
        current_game_state_->OnSpriteClicked(sprite);
    }
}

void Engine::OnSpriteGetFocus(Graphics::SpritePtr sprite)
{
    if(current_menu_state_)
    {
        current_menu_state_->OnSpriteGetFocus(sprite);
    }
    else if(current_game_state_)
    {
        current_game_state_->OnSpriteGetFocus(sprite);
    }
}

void Engine::OnSpriteLoseFocus(Graphics::SpritePtr sprite)
{
    if(current_menu_state_)
    {
        current_menu_state_->OnSpriteLoseFocus(sprite);
    }
    else if(current_game_state_)
    {
        current_game_state_->OnSpriteLoseFocus(sprite);
    }
}

void Engine::OnSpriteTextChanged(Graphics::SpritePtr sprite)
{
    if(current_menu_state_)
    {
        current_menu_state_->OnSpriteTextChanged(sprite);
    }
    else if(current_game_state_)
    {
        current_game_state_->OnSpriteTextChanged(sprite);
    }
}

void Engine::OnSpriteSelectionChanged(Graphics::SpritePtr sprite)
{
    if(current_menu_state_)
    {
        current_menu_state_->OnSpriteSelectionChanged(sprite);
    }
    else if(current_game_state_)
    {
        current_game_state_->OnSpriteSelectionChanged(sprite);
    }
}

void Engine::OnSpriteStringAdded(Graphics::SpritePtr sprite)
{
    if(current_menu_state_)
    {
        current_menu_state_->OnSpriteStringAdded(sprite);
    }
    else if(current_game_state_)
    {
        current_game_state_->OnSpriteStringAdded(sprite);
    }
}

void Engine::OnSpriteStringRemoved(Graphics::SpritePtr sprite)
{
    if(current_menu_state_)
    {
        current_menu_state_->OnSpriteStringRemoved(sprite);
    }
    else if(current_game_state_)
    {
        current_game_state_->OnSpriteStringRemoved(sprite);
    }
}

void Engine::OnSpriteSliderMoved(Graphics::SpritePtr sprite)
{
    if(current_menu_state_)
    {
        current_menu_state_->OnSpriteSliderMoved(sprite);
    }
    else if(current_game_state_)
    {
        current_game_state_->OnSpriteSliderMoved(sprite);
    }
}

void Engine::OnSpriteSliderReleased(Graphics::SpritePtr sprite)
{
    if(current_menu_state_)
    {
        current_menu_state_->OnSpriteSliderReleased(sprite);
    }
    else if(current_game_state_)
    {
        current_game_state_->OnSpriteSliderReleased(sprite);
    }
}

void Engine::OnSpriteEnterPressed(Graphics::SpritePtr sprite)
{
    if(current_menu_state_)
    {
        current_menu_state_->OnSpriteEnterPressed(sprite);
    }
    else if(current_game_state_)
    {
        current_game_state_->OnSpriteEnterPressed(sprite);
    }
}

void Engine::OnSpriteDoubleClicked(Graphics::SpritePtr sprite)
{
    if(current_menu_state_)
    {
        current_menu_state_->OnSpriteDoubleClicked(sprite);
    }
    else if(current_game_state_)
    {
        current_game_state_->OnSpriteDoubleClicked(sprite);
    }
}

void Engine::OnWindowCreated()
{
    OutputDebugString("OnWindowCreated\n");
    LOG("OnWindowCreated");
}

void Engine::OnWindowDeleted()
{
    OutputDebugString("OnWindowDeleted\n");
    LOG("OnWindowDeleted");
}

bool Engine::OnWindowQueryClose()
{
    OutputDebugString("OnWindowQueryClose\n");
    LOG("OnWindowQueryClose");

    PostQuitMessage(0);
    return true;
}

void Engine::OnWindowActivated()
{
    OutputDebugString("OnWindowActivated\n");
    LOG("OnWindowActivated");

    app_active_ = true;
    if(inp_device_)
    {
        inp_device_->Activate();
    }
    if(window_ /*&& window_->IsForeground()*/)
    {
        window_->DisallowCursor();
    }

    if(current_menu_state_)
    {
        current_menu_state_->OnWindowActivated();
    }
    if(current_game_state_)
    {
        current_game_state_->OnWindowActivated();
    }
}

void Engine::OnWindowDeactivated()
{
    OutputDebugString("OnWindowDeactivated\n");
    LOG("OnWindowDeactivated");

    app_active_ = false;
    if(inp_device_)
    {
        inp_device_->Deactivate();
    }
    if(window_)
    {
        window_->AllowCursor();
    }

    if(current_menu_state_)
    {
        current_menu_state_->OnWindowDeactivated();
    }
    if(current_game_state_)
    {
        current_game_state_->OnWindowDeactivated();
    }
}

void Engine::OnWindowPaint(HDC dc)
{
}

void Engine::OnWindowEraseBackground()
{
    //if(gfx_device_)
    //{
    //    if(current_menu_state_)
    //    {
    //        current_menu_state_->DrawFrame(0);
    //        gfx_device_->OnEndFrame();
    //    }
    //    else if(current_game_state_)
    //    {
    //        current_game_state_->DrawFrame(0);
    //        gfx_device_->OnEndFrame();
    //    }
    //}
}

void Engine::OnWindowMouseMove(const Graphics::Vector& pos)
{
    if(!sprite_cursor_)
    {
        return;
    }

    sprite_cursor_->SetPosition(pos);

    if(sprites_clickable_->GetCapture())
    {
        sprites_clickable_->GetCapture()->OnMouseMoved(sprite_cursor_, sprites_clickable_);
    }
    else
    {
        Graphics::SpriteContainer::Sprites::iterator sprite;
        for(sprite = sprites_clickable_->begin(); sprite != sprites_clickable_->end(); ++sprite)
        {
            if((*sprite)->IsEnabled() && (*sprite)->IsPointColliding(sprite_cursor_->GetPosition()))
            {
                if(*sprite != sprite_hover_)
                {
                    if(sprite_hover_)
                    {
                        sprite_hover_->OnMouseLeave(sprite_cursor_, sprites_clickable_);
                    }
                    sprite_hover_ = *sprite;
                    sprite_hover_->OnMouseEnter(sprite_cursor_, sprites_clickable_);
                }

                (*sprite)->OnMouseMoved(sprite_cursor_, sprites_clickable_);
                break;
            }
        }

        if(sprite == sprites_clickable_->end() && sprite_hover_)
        {
            sprite_hover_->OnMouseLeave(sprite_cursor_, sprites_clickable_);
            sprite_hover_.reset();
        }
    }
}

void Engine::OnWindowMouseLeave()
{
    if(sprite_cursor_)
    {
        // Hide the cursor by moving it off screen
        sprite_cursor_->SetPosition(Graphics::Vector(-100.0f, -100.0f));
    }
}

void Engine::OnKeyPressed(int key, bool shift_held, bool ctrl_held, bool alt_held, bool win_held)
{
    if(key == DIK_TAB)
    {
        shift_held ? sprites_clickable_->MoveFocusPrev() : sprites_clickable_->MoveFocusNext();
    }
    else
    {
        Graphics::SpritePtr sprite = sprites_clickable_->GetFocus();
        if(sprite && sprite->IsEnabled())
        {
            sprite->OnKeyPressed(key, shift_held, ctrl_held, alt_held, win_held);
        }
    }
}

void Engine::OnKeyHeld(int key, bool shift_held, bool ctrl_held, bool alt_held, bool win_held)
{
    Graphics::SpritePtr sprite = sprites_clickable_->GetFocus();
    if(sprite && sprite->IsEnabled())
    {
        sprite->OnKeyHeld(key, shift_held, ctrl_held, alt_held, win_held);
    }
}

void Engine::OnKeyReleased(int key, bool shift_held, bool ctrl_held, bool alt_held, bool win_held)
{
    Graphics::SpritePtr sprite = sprites_clickable_->GetFocus();
    if(sprite && sprite->IsEnabled())
    {
        sprite->OnKeyReleased(key, shift_held, ctrl_held, alt_held, win_held);
    }
}

void Engine::OnMouseButtonPressed(int button)
{
    if(sprites_clickable_->GetCapture())
    {
        sprites_clickable_->SetFocus(sprites_clickable_->GetCapture());
        sprites_clickable_->GetCapture()->OnMouseButtonPressed(button, sprite_cursor_, sprites_clickable_);
    }
    else if(sprite_cursor_)
    {
        Graphics::SpriteContainer::Sprites::reverse_iterator sprite;
        for(sprite = sprites_clickable_->rbegin(); sprite != sprites_clickable_->rend(); ++sprite)
        {
            if((*sprite)->IsEnabled() && (*sprite)->IsPointColliding(sprite_cursor_->GetPosition()))
            {
                sprites_clickable_->SetFocus(*sprite);
                (*sprite)->OnMouseButtonPressed(button, sprite_cursor_, sprites_clickable_);
                break;
            }
        }
    }
}

void Engine::OnMouseButtonHeld(int button)
{
    if(sprites_clickable_->GetCapture())
    {
        sprites_clickable_->GetCapture()->OnMouseButtonHeld(button, sprite_cursor_, sprites_clickable_);
    }
    else if(sprite_cursor_)
    {
        Graphics::SpriteContainer::Sprites::reverse_iterator sprite;
        for(sprite = sprites_clickable_->rbegin(); sprite != sprites_clickable_->rend(); ++sprite)
        {
            if((*sprite)->IsEnabled() && (*sprite)->IsPointColliding(sprite_cursor_->GetPosition()))
            {
                (*sprite)->OnMouseButtonHeld(button, sprite_cursor_, sprites_clickable_);
                break;
            }
        }
    }
}

void Engine::OnMouseButtonReleased(int button)
{
    if(sprites_clickable_->GetCapture())
    {
        sprites_clickable_->GetCapture()->OnMouseButtonReleased(button, sprite_cursor_, sprites_clickable_);
    }
    else if(sprite_cursor_)
    {
        Graphics::SpriteContainer::Sprites::reverse_iterator sprite;
        for(sprite = sprites_clickable_->rbegin(); sprite != sprites_clickable_->rend(); ++sprite)
        {
            if((*sprite)->IsEnabled() && (*sprite)->IsPointColliding(sprite_cursor_->GetPosition()))
            {
                (*sprite)->OnMouseButtonReleased(button, sprite_cursor_, sprites_clickable_);
                break;
            }
        }
    }
}

void Engine::OnMouseMoved(int x_delta, int y_delta)
{
    //if(!sprite_cursor_)
    //{
    //    return;
    //}

    //// Move the cursor and make sure it stays within the clipping rectangle
    //Graphics::Vector position = Graphics::Vector(float(x_delta), float(y_delta)) + sprite_cursor_->GetPosition();
    //if(position.x_ < clip_rect_min_.x_)
    //{
    //    position.x_ = clip_rect_min_.x_;
    //}
    //else if(position.x_ > clip_rect_max_.x_)
    //{
    //    position.x_ = clip_rect_max_.x_;
    //}

    //if(position.y_ < clip_rect_min_.y_)
    //{
    //    position.y_ = clip_rect_min_.y_;
    //}
    //else if(position.y_ > clip_rect_max_.y_)
    //{
    //    position.y_ = clip_rect_max_.y_;
    //}
    //sprite_cursor_->SetPosition(position);

    //if(sprites_clickable_->GetCapture())
    //{
    //    sprites_clickable_->GetCapture()->OnMouseMoved(sprite_cursor_, sprites_clickable_);
    //}
    //else
    //{
    //    Graphics::SpriteContainer::Sprites::iterator sprite;
    //    for(sprite = sprites_clickable_->begin(); sprite != sprites_clickable_->end(); ++sprite)
    //    {
    //        if((*sprite)->IsEnabled() && (*sprite)->IsPointColliding(sprite_cursor_->GetPosition()))
    //        {
    //            if(*sprite != sprite_hover_)
    //            {
    //                if(sprite_hover_)
    //                {
    //                    sprite_hover_->OnMouseLeave(sprite_cursor_, sprites_clickable_);
    //                }
    //                sprite_hover_ = *sprite;
    //                sprite_hover_->OnMouseEnter(sprite_cursor_, sprites_clickable_);
    //            }

    //            (*sprite)->OnMouseMoved(sprite_cursor_, sprites_clickable_);
    //            break;
    //        }
    //    }

    //    if(sprite == sprites_clickable_->end() && sprite_hover_)
    //    {
    //        sprite_hover_->OnMouseLeave(sprite_cursor_, sprites_clickable_);
    //        sprite_hover_.reset();
    //    }
    //}
}

void Engine::OnJoyButtonPressed(int button)
{
}

void Engine::OnJoyButtonHeld(int button)
{
}

void Engine::OnJoyButtonReleased(int button)
{
}

void Engine::OnJoyPovButtonPressed(JoyPovDirection dir, int pov)
{
}

void Engine::OnJoyPovButtonHeld(JoyPovDirection dir, int pov)
{
}

void Engine::OnJoyPovButtonReleased(JoyPovDirection dir, int pov)
{
}

void Engine::OnJoyMoved(int x_delta, int y_delta, int z_delta)
{
}

void Engine::OnJoyRotated(int x_delta, int y_delta, int z_delta)
{
}

void Engine::OnJoySliderMoved(int slider, int pos)
{
}

void Engine::AddNetEventHandler(Network::EventHandler* event_handler)
{
    Network::EventHandlerList::iterator itor = std::find(net_event_handlers_.begin(),
                                                         net_event_handlers_.end(),
                                                         event_handler);
    if(itor == net_event_handlers_.end())
    {
        net_event_handlers_.push_back(event_handler);
    }
}

void Engine::RemoveNetEventHandler(Network::EventHandler* event_handler)
{
    Network::EventHandlerList::iterator itor = std::find(net_event_handlers_.begin(),
                                                         net_event_handlers_.end(),
                                                         event_handler);
    if(itor != net_event_handlers_.end())
    {
        net_event_handlers_.erase(itor);
    }
}

void Engine::ClearNetEventHandlers()
{
    net_event_handlers_.clear();
}

void Engine::ScanForIncomingPackets()
{
    Network::SocketList::iterator sock;
    for(sock = socket_list_->begin(); sock != socket_list_->end(); ++sock)
    {
        Network::IpAddress ip_address;
        Network::DataBlock db = (*sock)->Read(ip_address);
        if(db.num_bytes_ > 0)
        {
            Network::EventHandlerList::iterator itor;
            for(itor = net_event_handlers_.begin(); itor != net_event_handlers_.end(); ++itor)
            {
                (*itor)->OnDataBlockReceived(*sock, db, ip_address);
            }
        }
    }
}

void Engine::InitialiseNetworking()
{
    connection_list_.reset(new Network::ConnectionList);
    network_port_ = hw_config_.GetNetworkPort();

    Network::Socket::InitialiseLibrary();
    socket_list_.reset(new Network::SocketList);
    broadcast_addresses_.clear();
    local_addresses_.clear();

    // For each NIC in the machine we need to:
    //  1. Build a Directed Broadcast address out each IP address and subnet mask
    //  2. Create a socket

    Network::Nics nics;
    Network::NicPtr nic = nics.MoveFirst();
    while(nic)
    {
        // Build the Directed Broadcast address
        std::vector<std::string> ips    = nic->GetIpAddresses();
        std::vector<std::string> masks  = nic->GetIpMasks();

        std::size_t size = std::min(ips.size(), masks.size());
        for(std::size_t i = 0; i < size; i++)
        {
            if(ips[i] != "0.0.0.0")
            {
                in_addr bcast_addr;

                unsigned long host_addr = inet_addr(ips[i].c_str());        // 192.168.1.2
                unsigned long net_mask  = inet_addr(masks[i].c_str());      // 255.255.255.0
                unsigned long net_addr  = host_addr & net_mask;             // 192.168.1.0
                bcast_addr.s_addr       = net_addr | (~net_mask);           // 192.168.1.255

                std::string watch_me(inet_ntoa(bcast_addr));
                broadcast_addresses_.push_back(Network::IpAddress(watch_me, network_port_));
            }
        }

        if(!ips.empty())
        {
            // Create a socket and bind it to the first IP address
            // on this device that is not 0.0.0.0
            std::size_t size = std::min(ips.size(), masks.size());
            for(std::size_t i = 0; i < size; i++)
            {
                if(ips[i] != "0.0.0.0")
                {
                    Network::SocketPtr sock(new Network::Socket(Network::IpAddress(ips[i], network_port_)));
                    socket_list_->push_back(sock);
                    break;
                }
            }
        }

        // We also collect all local addresses
        std::copy(ips.begin(), ips.end(), std::back_inserter(local_addresses_));

        nic = nics.MoveNext();
    }

    LOG("Available broadcast addresses [" << Network::ToString(broadcast_addresses_) << "]");
}

bool Engine::IsLocalAddress(const Network::IpAddress& ip_address) const
{
    std::vector<std::string>::const_iterator itor = std::find(local_addresses_.begin(), local_addresses_.end(), ip_address.ToString());
    return (itor != local_addresses_.end());
}
