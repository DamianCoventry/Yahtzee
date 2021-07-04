#ifndef INCLUDED_CORE_ENGINE
#define INCLUDED_CORE_ENGINE

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "state.h"
#include "timeoutmanager.h"

#include "../Graphics/device.h"
#include "../Graphics/sprite.h"
#include "../Graphics/spritecursor.h"
#include "../Graphics/spritecontainer.h"
#include "../Graphics/window.h"
#include "../Graphics/viewport.h"
#include "../Graphics/blackwindow.h"
#include "../Graphics/splashscreen.h"

#include "../Input/device.h"
#include "../Audio/device.h"

#include "../Utility/logcontext.h"
#include "../Utility/hwconfig.h"

#include "../Network/ipaddress.h"
#include "../Network/eventhandler.h"
#include "../Network/connection.h"

namespace Network
{
class SocketException;
}

namespace Core
{

class Engine :
    public Graphics::SpriteEventHandler,
    public Graphics::WindowEventHandler,
    public Input::InputEventHandler,
    public Utility::LogContext
{
public:
    Engine(HINSTANCE instance, Graphics::SplashScreen& splash);
    virtual ~Engine();

    virtual void Initialise();
    virtual void Shutdown();
    virtual void HandleSocketException(Network::SocketException& e) {}

    void Run();

    void SetCurrentMenuState(StatePtr state);
    void SetCurrentGameState(StatePtr state);
    void TransitionMenuState(StatePtr state)        { do_menu_state_transition_ = true; next_menu_state_ = state; }
    void TransitionGameState(StatePtr state)        { do_game_state_transition_ = true; next_game_state_ = state; }
    void DoStateTransition();

    void SetCursor(Graphics::SpriteCursorPtr sprite_cursor);
    void SetClipRect(const Graphics::Vector& clip_rect_min, const Graphics::Vector& clip_rect_max) { clip_rect_min_ = clip_rect_min; clip_rect_max_ = clip_rect_max; }

    void SetFullscreen();
    void SetWindowed();

    StatePtr GetCurrentMenuState() const                { return current_menu_state_; }
    StatePtr GetCurrentGameState() const                { return current_game_state_; }

    Utility::HwConfig* GetHwConfig() const              { return (Utility::HwConfig*)&hw_config_; }

    Input::DevicePtr GetInputDevice() const             { return inp_device_; }
    Audio::DevicePtr GetAudioDevice() const             { return aud_device_; }
    Network::SocketListPtr GetSocketList() const        { return socket_list_; }

    Graphics::DevicePtr GetGraphicsDevice() const       { return gfx_device_; }
    Graphics::WindowPtr GetWindow() const               { return window_; }
    Graphics::ViewportManagerPtr GetViewportManager() const { return viewport_manager_; }
    Graphics::SpriteContainerPtr GetThinkable() const   { return sprites_thinkable_; }
    Graphics::SpriteContainerPtr GetDrawable() const    { return sprites_drawable_; }
    Graphics::SpriteContainerPtr GetClickable() const   { return sprites_clickable_; }

    Graphics::SpriteCursorPtr GetSpriteCursor() const   { return sprite_cursor_; }

    TimeoutManagerPtr GetTimeoutManager() const         { return timeout_manager_; }

    void ShowBlackWindow();
    void HideBlackWindow();

    const Network::IpAddresses& GetBroadcastAddresses() const   { return broadcast_addresses_; }
    bool IsLocalAddress(const Network::IpAddress& ip_address) const;
    Network::ConnectionListPtr GetConnectionList() const        { return connection_list_; }

    void SetNetworkPort(unsigned short port)    { network_port_ = port; }
    unsigned short GetNetworkPort() const       { return network_port_; }

    void AddNetEventHandler(Network::EventHandler* event_handler);
    void RemoveNetEventHandler(Network::EventHandler* event_handler);
    void ClearNetEventHandlers();
    void ScanForIncomingPackets();

    // Graphics::SpriteEventHandler
    void OnSpriteClicked(Graphics::SpritePtr sprite);
    void OnSpriteGetFocus(Graphics::SpritePtr sprite);
    void OnSpriteLoseFocus(Graphics::SpritePtr sprite);
    void OnSpriteTextChanged(Graphics::SpritePtr sprite);
    void OnSpriteSelectionChanged(Graphics::SpritePtr sprite);
    void OnSpriteStringAdded(Graphics::SpritePtr sprite);
    void OnSpriteStringRemoved(Graphics::SpritePtr sprite);
    void OnSpriteSliderMoved(Graphics::SpritePtr sprite);
    void OnSpriteSliderReleased(Graphics::SpritePtr sprite);
    void OnSpriteEnterPressed(Graphics::SpritePtr sprite);
    void OnSpriteDoubleClicked(Graphics::SpritePtr sprite);

    // Graphics::WindowEventHandler
    void OnWindowCreated();
    void OnWindowDeleted();
    bool OnWindowQueryClose();
    void OnWindowActivated();
    void OnWindowDeactivated();
    void OnWindowPaint(HDC dc);
    void OnWindowEraseBackground();
    void OnWindowMouseMove(const Graphics::Vector& pos);
    void OnWindowMouseLeave();

    // Input::InputEventHandler
    void OnKeyPressed(int key, bool shift_held, bool ctrl_held, bool alt_held, bool win_held);
    void OnKeyHeld(int key, bool shift_held, bool ctrl_held, bool alt_held, bool win_held);
    void OnKeyReleased(int key, bool shift_held, bool ctrl_held, bool alt_held, bool win_held);
    void OnMouseButtonPressed(int button);
    void OnMouseButtonHeld(int button);
    void OnMouseButtonReleased(int button);
    void OnMouseMoved(int x_delta, int y_delta);
    void OnJoyButtonPressed(int button);
    void OnJoyButtonHeld(int button);
    void OnJoyButtonReleased(int button);
    void OnJoyPovButtonPressed(JoyPovDirection dir, int pov);
    void OnJoyPovButtonHeld(JoyPovDirection dir, int pov);
    void OnJoyPovButtonReleased(JoyPovDirection dir, int pov);
    void OnJoyMoved(int x_delta, int y_delta, int z_delta);
    void OnJoyRotated(int x_delta, int y_delta, int z_delta);
    void OnJoySliderMoved(int slider, int pos);

protected:
    StatePtr GetCurrentMenuState() { return current_menu_state_; }
    StatePtr GetCurrentGameState() { return current_game_state_; }

private:
    void InitialiseNetworking();

protected:
    Graphics::SplashScreen& GetSplash() const { return (Graphics::SplashScreen&)splash_; }

private:
    StatePtr current_menu_state_;
    StatePtr current_game_state_;
    StatePtr next_game_state_;
    StatePtr next_menu_state_;
    bool do_menu_state_transition_;
    bool do_game_state_transition_;

    HINSTANCE instance_;
    bool shutdown_;
    bool app_active_;
    bool restore_needed_;

    Utility::HwConfig hw_config_;

    Graphics::SplashScreen& splash_;
    Graphics::WindowPtr window_;
    Graphics::DevicePtr gfx_device_;
    Graphics::ViewportManagerPtr viewport_manager_;
    Graphics::SpriteContainerPtr sprites_thinkable_;
    Graphics::SpriteContainerPtr sprites_drawable_;
    Graphics::SpriteContainerPtr sprites_clickable_;
    Graphics::SpriteCursorPtr sprite_cursor_;
    Graphics::SpritePtr sprite_hover_;
    Graphics::Vector clip_rect_min_;
    Graphics::Vector clip_rect_max_;
    Graphics::BlackWindowPtr black_window_;

    TimeoutManagerPtr timeout_manager_;
    Input::DevicePtr inp_device_;
    Audio::DevicePtr aud_device_;
    Network::SocketListPtr socket_list_;
    Network::EventHandlerList net_event_handlers_;

    Network::ConnectionListPtr connection_list_;
    Network::IpAddresses broadcast_addresses_;
    std::vector<std::string> local_addresses_;
    unsigned short network_port_;
};

}       // namespace Core

#endif  // INCLUDED_CORE_ENGINE
