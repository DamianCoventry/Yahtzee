#include "stdafx.h"
#include "yahtzee.h"
#include "statemenumain.h"
#include "../Graphics/spritebutton.h"
#include "../Graphics/spriteeditbox.h"
#include "../Graphics/spritelistbox.h"
#include "../Graphics/spritelistctrl.h"
#include "../Graphics/spritecursor.h"
#include "../Graphics/spritestatic.h"
#include "../Graphics/spritecheckbox.h"
#include "../Graphics/spriteslider.h"
#include "../Graphics/bitmap.h"
#include "../Graphics/texture.h"
#include "../Graphics/imagelist.h"
#include "stategameyahtzee.h"
#include "statemenumain.h"
#include "statemenunetgamesetup.h"
#include "statemenunetgameended.h"
#include "statemenuingamesettings.h"
#include "../Utility/logmacros.h"
#include "../Utility/exception.h"
#include "scoreeditbox.h"
#include "../Audio/wave.h"
#include "yahtzeenetworkhandler.h"

Yahtzee::Yahtzee(HINSTANCE instance, Graphics::SplashScreen& splash)
: Core::Engine(instance, splash)
, current_roll_(0)
, is_bonus_yahtzee_(false)
, is_hosting_(false)
, is_player_typing_(false)
, viewport_id_(0)
{
}

Yahtzee::~Yahtzee()
{
}

void Yahtzee::Initialise()
{
    Engine::Initialise();

    GetSplash().SetPos(50);

    LOG("Allocating memory...")
    button_localgame_.reset(new Graphics::SpriteButton(this));
    button_networkgame_.reset(new Graphics::SpriteButton(this));
    button_main_.reset(new Graphics::SpriteButton(this));
    button_settings_.reset(new Graphics::SpriteButton(this));
    button_about_.reset(new Graphics::SpriteButton(this));
    button_viewhighscores_.reset(new Graphics::SpriteButton(this));
    button_exitapplication_.reset(new Graphics::SpriteButton(this));
    button_back_.reset(new Graphics::SpriteButton(this));
    button_ok_.reset(new Graphics::SpriteButton(this));
    button_resethighscores_.reset(new Graphics::SpriteButton(this));
    button_continue_.reset(new Graphics::SpriteButton(this));
    button_addplayer_.reset(new Graphics::SpriteButton(this));
    button_updateplayer_.reset(new Graphics::SpriteButton(this));
    button_editplayer_.reset(new Graphics::SpriteButton(this));
    button_removeplayer_.reset(new Graphics::SpriteButton(this));
    button_clearplayers_.reset(new Graphics::SpriteButton(this));
    editbox_player_.reset(new Graphics::SpriteEditBox(this));
    listbox_players_.reset(new Graphics::SpriteListBox(this));
    button_start_.reset(new Graphics::SpriteButton(this));
    listctrl_highscores_.reset(new Graphics::SpriteListCtrl(this));
    button_savehighscore_.reset(new Graphics::SpriteButton(this));
    button_apply_.reset(new Graphics::SpriteButton(this));
    button_test_.reset(new Graphics::SpriteButton(this));
    button_endgame_.reset(new Graphics::SpriteButton(this));
    button_ingamesettings_.reset(new Graphics::SpriteButton(this));
    button_roll_.reset(new Graphics::SpriteButton(this));
    button_continue2_.reset(new Graphics::SpriteButton(this));
    button_send_.reset(new Graphics::SpriteButton(this));
    checkbox_fullscreenenabled_.reset(new Graphics::SpriteCheckBox(this));
    checkbox_audioenabled_.reset(new Graphics::SpriteCheckBox(this));
    slider_audiovolume_.reset(new Graphics::SpriteSlider(this));
    editbox_highscore_.reset(new Graphics::SpriteEditBox(this));
    editbox_ones_.reset(new ScoreEditBox(this, this));
    editbox_twos_.reset(new ScoreEditBox(this, this));
    editbox_threes_.reset(new ScoreEditBox(this, this));
    editbox_fours_.reset(new ScoreEditBox(this, this));
    editbox_fives_.reset(new ScoreEditBox(this, this));
    editbox_sixes_.reset(new ScoreEditBox(this, this));
    editbox_subtotal_.reset(new Graphics::SpriteEditBox(this));
    editbox_bonus_.reset(new Graphics::SpriteEditBox(this));
    editbox_uppertotal_.reset(new Graphics::SpriteEditBox(this));
    editbox_threeofakind_.reset(new ScoreEditBox(this, this));
    editbox_fourofakind_.reset(new ScoreEditBox(this, this));
    editbox_fullhouse_.reset(new ScoreEditBox(this, this));
    editbox_smallstraight_.reset(new ScoreEditBox(this, this));
    editbox_largestraight_.reset(new ScoreEditBox(this, this));
    editbox_yahtzee_.reset(new ScoreEditBox(this, this));
    editbox_chance_.reset(new ScoreEditBox(this, this));
    editbox_lowertotal_.reset(new Graphics::SpriteEditBox(this));
    editbox_grandtotal_.reset(new Graphics::SpriteEditBox(this));
    static_dice_0_.reset(new Graphics::SpriteStatic(this));
    static_dice_1_.reset(new Graphics::SpriteStatic(this));
    static_dice_2_.reset(new Graphics::SpriteStatic(this));
    static_dice_3_.reset(new Graphics::SpriteStatic(this));
    static_dice_4_.reset(new Graphics::SpriteStatic(this));
    dice_0_held_.reset(new Graphics::SpriteStatic(this));
    dice_1_held_.reset(new Graphics::SpriteStatic(this));
    dice_2_held_.reset(new Graphics::SpriteStatic(this));
    dice_3_held_.reset(new Graphics::SpriteStatic(this));
    dice_4_held_.reset(new Graphics::SpriteStatic(this));
    listctrl_summary_.reset(new Graphics::SpriteListCtrl(this));
    static_netgameinfo1_.reset(new Graphics::SpriteStatic(this));
    static_netgameinfo2_.reset(new Graphics::SpriteStatic(this));
    static_netgameheading1_.reset(new Graphics::SpriteStatic(this));
    static_netgameheading2_.reset(new Graphics::SpriteStatic(this));
    listctrl_ingameplayers_.reset(new Graphics::SpriteListCtrl(this));
    listbox_messages_.reset(new Graphics::SpriteListBox(this));
    editbox_message_.reset(new Graphics::SpriteEditBox(this));
    cursor_normal_.reset(new Graphics::SpriteCursor(this));
    cursor_placescore_.reset(new Graphics::SpriteCursor(this));
    listctrl_netgames_.reset(new Graphics::SpriteListCtrl(this));
    button_joinselectednetgame_.reset(new Graphics::SpriteButton(this));
    button_createnetgame_.reset(new Graphics::SpriteButton(this));
    button_joinspecificnetgame_.reset(new Graphics::SpriteButton(this));
    button_cancel_.reset(new Graphics::SpriteButton(this));
    static_ip_text_.reset(new Graphics::SpriteStatic(this));
    listbox_netplayers_.reset(new Graphics::SpriteListBox(this));
    listbox_netmessages_.reset(new Graphics::SpriteListBox(this));

    GetSplash().SetPos(60);

    // Create a font to use in edit boxes, list boxes and list ctrls
    font_control_.reset(new Graphics::Font);
    font_control_->Create(GetWindow()->GetHandle(), "Tahoma", 10);

    // Load all the bitmaps and create opengl textures out of them.
    Graphics::TexturePtr tex_mainbackground(new Graphics::Texture);
    Graphics::TexturePtr tex_highscoresbackground(new Graphics::Texture);
    Graphics::TexturePtr tex_aboutbackground(new Graphics::Texture);
    Graphics::TexturePtr tex_settingsbackground(new Graphics::Texture);
    Graphics::TexturePtr tex_playersetupbackground(new Graphics::Texture);
    Graphics::TexturePtr tex_enterscorebackground(new Graphics::Texture);
    Graphics::TexturePtr tex_netgameendbackground(new Graphics::Texture);
    Graphics::TexturePtr tex_buttonsleftside(new Graphics::Texture);
    Graphics::TexturePtr tex_listctrl(new Graphics::Texture);
    Graphics::TexturePtr tex_listbox(new Graphics::Texture);
    Graphics::TexturePtr tex_editbox(new Graphics::Texture);
    Graphics::TexturePtr tex_gameboard(new Graphics::Texture);
    Graphics::TexturePtr tex_dice(new Graphics::Texture);
    Graphics::TexturePtr tex_yahtzee(new Graphics::Texture);
    Graphics::TexturePtr tex_gameoverbackground(new Graphics::Texture);
    Graphics::TexturePtr tex_checkbox(new Graphics::Texture);
    Graphics::TexturePtr tex_slider(new Graphics::Texture);
    Graphics::TexturePtr tex_buttons(new Graphics::Texture);
    Graphics::TexturePtr tex_held(new Graphics::Texture);
    Graphics::TexturePtr tex_netgamesetupbackground(new Graphics::Texture);
    Graphics::TexturePtr tex_netgameconnectingbackground(new Graphics::Texture);
    Graphics::TexturePtr tex_netgamebackground(new Graphics::Texture);
    Graphics::TexturePtr tex_staticimages(new Graphics::Texture);

    Graphics::Bitmap bitmap;
    bitmap.Load("Images/MainBackground.bmp");
    bitmap.SetOpaqueAlpha();
    tex_mainbackground->Create(bitmap.GetWidth(), bitmap.GetHeight(), bitmap.GetPixels());

    bitmap.Load("Images/HighscoresBackground.bmp");
    bitmap.SetOpaqueAlpha();
    tex_highscoresbackground->Create(bitmap.GetWidth(), bitmap.GetHeight(), bitmap.GetPixels());

    bitmap.Load("Images/AboutBackground.bmp");
    bitmap.SetOpaqueAlpha();
    tex_aboutbackground->Create(bitmap.GetWidth(), bitmap.GetHeight(), bitmap.GetPixels());

    bitmap.Load("Images/OptionsBackground.bmp");
    bitmap.SetOpaqueAlpha();
    tex_settingsbackground->Create(bitmap.GetWidth(), bitmap.GetHeight(), bitmap.GetPixels());

    bitmap.Load("Images/PlayerSetupBackground.bmp");
    bitmap.SetOpaqueAlpha();
    tex_playersetupbackground->Create(bitmap.GetWidth(), bitmap.GetHeight(), bitmap.GetPixels());

    bitmap.Load("Images/EnterNewHighscoreBackground.bmp");
    bitmap.SetOpaqueAlpha();
    tex_enterscorebackground->Create(bitmap.GetWidth(), bitmap.GetHeight(), bitmap.GetPixels());

    bitmap.Load("Images/NetGameEndBackground.bmp");
    bitmap.SetOpaqueAlpha();
    tex_netgameendbackground->Create(bitmap.GetWidth(), bitmap.GetHeight(), bitmap.GetPixels());

    bitmap.Load("Images/ButtonsLeftSide.bmp");
    bitmap.SetOpaqueAlpha();
    tex_buttonsleftside->Create(bitmap.GetWidth(), bitmap.GetHeight(), bitmap.GetPixels());

    bitmap.Load("Images/Held.bmp");
    bitmap.SetTransparentAlpha(0, 0, 0);
    tex_held->Create(bitmap.GetWidth(), bitmap.GetHeight(), bitmap.GetPixels());

    bitmap.Load("Images/ListCtrl.bmp");
    bitmap.SetOpaqueAlpha();
    tex_listctrl->Create(bitmap.GetWidth(), bitmap.GetHeight(), bitmap.GetPixels());

    bitmap.Load("Images/EditBox.bmp");
    bitmap.SetOpaqueAlpha();
    tex_editbox->Create(bitmap.GetWidth(), bitmap.GetHeight(), bitmap.GetPixels());

    bitmap.Load("Images/ListBox.bmp");
    bitmap.SetOpaqueAlpha();
    tex_listbox->Create(bitmap.GetWidth(), bitmap.GetHeight(), bitmap.GetPixels());

    bitmap.Load("Images/GameBoard.bmp");
    bitmap.SetOpaqueAlpha();
    tex_gameboard->Create(bitmap.GetWidth(), bitmap.GetHeight(), bitmap.GetPixels());

    bitmap.Load("Images/Yahtzee.bmp");
    bitmap.SetOpaqueAlpha();
    tex_yahtzee->Create(bitmap.GetWidth(), bitmap.GetHeight(), bitmap.GetPixels());

    GetSplash().SetPos(70);

    bitmap.Load("Images/GameOverBackground.bmp");
    bitmap.SetOpaqueAlpha();
    tex_gameoverbackground->Create(bitmap.GetWidth(), bitmap.GetHeight(), bitmap.GetPixels());

    bitmap.Load("Images/CheckBox.bmp");
    bitmap.SetOpaqueAlpha();
    tex_checkbox->Create(bitmap.GetWidth(), bitmap.GetHeight(), bitmap.GetPixels());

    bitmap.Load("Images/SliderCtrl.bmp");
    bitmap.SetTransparentAlpha();
    tex_slider->Create(bitmap.GetWidth(), bitmap.GetHeight(), bitmap.GetPixels());

    bitmap.Load("Images/Buttons.bmp");
    bitmap.SetOpaqueAlpha();
    tex_buttons->Create(bitmap.GetWidth(), bitmap.GetHeight(), bitmap.GetPixels());

    bitmap.Load("Images/NetGameSetupBackground.bmp");
    bitmap.SetOpaqueAlpha();
    tex_netgamesetupbackground->Create(bitmap.GetWidth(), bitmap.GetHeight(), bitmap.GetPixels());

    bitmap.Load("Images/NetGameConnectingBackground.bmp");
    bitmap.SetOpaqueAlpha();
    tex_netgameconnectingbackground->Create(bitmap.GetWidth(), bitmap.GetHeight(), bitmap.GetPixels());

    bitmap.Load("Images/NetGameBackground.bmp");
    bitmap.SetOpaqueAlpha();
    tex_netgamebackground->Create(bitmap.GetWidth(), bitmap.GetHeight(), bitmap.GetPixels());

    bitmap.Load("Images/StaticImages.bmp");
    bitmap.SetOpaqueAlpha();
    tex_staticimages->Create(bitmap.GetWidth(), bitmap.GetHeight(), bitmap.GetPixels());

    bitmap.Unload();

    image_mainbackground_.reset(new Graphics::Image);
    image_mainbackground_->Create(0, 0, 640, 480, tex_mainbackground);

    image_highscoresbackground_.reset(new Graphics::Image);
    image_highscoresbackground_->Create(0, 0, 640, 480, tex_highscoresbackground);

    image_aboutbackground_.reset(new Graphics::Image);
    image_aboutbackground_->Create(0, 0, 640, 480, tex_aboutbackground);

    image_settingsbackground_.reset(new Graphics::Image);
    image_settingsbackground_->Create(0, 0, 640, 480, tex_settingsbackground);

    image_playersetupbackground_.reset(new Graphics::Image);
    image_playersetupbackground_->Create(0, 0, 640, 480, tex_playersetupbackground);

    image_enterscorebackground_.reset(new Graphics::Image);
    image_enterscorebackground_->Create(0, 0, 640, 480, tex_enterscorebackground);

    image_gameoverbackground_.reset(new Graphics::Image);
    image_gameoverbackground_->Create(0, 0, 640, 480, tex_gameoverbackground);

    image_netgameendbackground_.reset(new Graphics::Image);
    image_netgameendbackground_->Create(0, 0, 640, 480, tex_netgameendbackground);

    image_netgamesetupbackground_.reset(new Graphics::Image);
    image_netgamesetupbackground_->Create(0, 0, 640, 480, tex_netgamesetupbackground);

    image_netgameconnectingbackground_.reset(new Graphics::Image);
    image_netgameconnectingbackground_->Create(0, 0, 640, 480, tex_netgameconnectingbackground);

    image_netgamebackground_.reset(new Graphics::Image);
    image_netgamebackground_->Create(0, 0, 640, 480, tex_netgamebackground);

    image_netgameinfo1_.reset(new Graphics::Image);
    image_netgameinfo1_->Create(0, 0, 400.0f, 40.0f, tex_staticimages);

    image_netgameinfo2_.reset(new Graphics::Image);
    image_netgameinfo2_->Create(0, 40, 400.0f, 15.0f, tex_staticimages);

    image_netgameheading1_.reset(new Graphics::Image);
    image_netgameheading1_->Create(0, 55, 450.0f, 20.0f, tex_staticimages);

    image_netgameheading2_.reset(new Graphics::Image);
    image_netgameheading2_->Create(0, 75, 512.0f, 20.0f, tex_staticimages);

    Graphics::ImageListPtr cursor_normal_imagelist(new Graphics::ImageList);
    cursor_normal_imagelist->Create("Images/CursorDefault.bmp", 16, 32.0f, 32.0f);
    cursor_normal_->SetImageList(cursor_normal_imagelist);
    SetCursor(cursor_normal_);

    Graphics::ImageListPtr cursor_placescore_imagelist(new Graphics::ImageList);
    cursor_placescore_imagelist->Create("Images/CursorPlaceScore.bmp", 16, 32.0f, 32.0f);
    cursor_placescore_->SetImageList(cursor_placescore_imagelist);

    Utility::HwConfig* hw_config = GetHwConfig();

    GetSplash().SetPos(80);

    shakedice_sound0_.reset(new Audio::Sound(GetAudioDevice()));
    shakedice_sound1_.reset(new Audio::Sound(GetAudioDevice()));
    shakedice_sound2_.reset(new Audio::Sound(GetAudioDevice()));
    shakedice_sound3_.reset(new Audio::Sound(GetAudioDevice()));
    buttonclick_sound_.reset(new Audio::Sound(GetAudioDevice()));
    checkboxclick_sound_.reset(new Audio::Sound(GetAudioDevice()));
    editboxclick_sound_.reset(new Audio::Sound(GetAudioDevice()));
    diceclick_sound_.reset(new Audio::Sound(GetAudioDevice()));
    applause_sound0_.reset(new Audio::Sound(GetAudioDevice()));
    applause_sound1_.reset(new Audio::Sound(GetAudioDevice()));
    yahtzee_sound0_.reset(new Audio::Sound(GetAudioDevice()));
    yahtzee_sound1_.reset(new Audio::Sound(GetAudioDevice()));
    soundtest_sound_.reset(new Audio::Sound(GetAudioDevice()));
    messagereceive_sound_.reset(new Audio::Sound(GetAudioDevice()));

    try
    {
        Audio::Wave wave_file;
        wave_file.Load("Sounds/ShakeDice0.wav");
        shakedice_sound0_->Create(wave_file);
        shakedice_sound0_->SetVolume(hw_config->GetVolume());

        wave_file.Load("Sounds/ShakeDice1.wav");
        shakedice_sound1_->Create(wave_file);
        shakedice_sound1_->SetVolume(hw_config->GetVolume());

        wave_file.Load("Sounds/ShakeDice2.wav");
        shakedice_sound2_->Create(wave_file);
        shakedice_sound2_->SetVolume(hw_config->GetVolume());

        wave_file.Load("Sounds/ShakeDice3.wav");
        shakedice_sound3_->Create(wave_file);
        shakedice_sound3_->SetVolume(hw_config->GetVolume());

        wave_file.Load("Sounds/ButtonClick.wav");
        buttonclick_sound_->Create(wave_file);
        buttonclick_sound_->SetVolume(hw_config->GetVolume());

        wave_file.Load("Sounds/CheckBoxClick.wav");
        checkboxclick_sound_->Create(wave_file);
        checkboxclick_sound_->SetVolume(hw_config->GetVolume());

        wave_file.Load("Sounds/EditBoxClick.wav");
        editboxclick_sound_->Create(wave_file);
        editboxclick_sound_->SetVolume(hw_config->GetVolume());

        wave_file.Load("Sounds/DiceClick.wav");
        diceclick_sound_->Create(wave_file);
        diceclick_sound_->SetVolume(hw_config->GetVolume());

        wave_file.Load("Sounds/Applause0.wav");
        applause_sound0_->Create(wave_file);
        applause_sound0_->SetVolume(hw_config->GetVolume());

        wave_file.Load("Sounds/Applause1.wav");
        applause_sound1_->Create(wave_file);
        applause_sound1_->SetVolume(hw_config->GetVolume());

        wave_file.Load("Sounds/Yahtzee0.wav");
        yahtzee_sound0_->Create(wave_file);
        yahtzee_sound0_->SetVolume(hw_config->GetVolume());

        wave_file.Load("Sounds/Yahtzee1.wav");
        yahtzee_sound1_->Create(wave_file);
        yahtzee_sound1_->SetVolume(hw_config->GetVolume());

        wave_file.Load("Sounds/SoundTest.wav");
        soundtest_sound_->Create(wave_file);
        soundtest_sound_->SetVolume(hw_config->GetVolume());

        wave_file.Load("Sounds/MessageReceive.wav");
        messagereceive_sound_->Create(wave_file);
        messagereceive_sound_->SetVolume(hw_config->GetVolume());
    }
    catch (const std::runtime_error& )
    {
        LOG("Continuing without souds...");
    }

    // The main page
    Graphics::SpriteButton* button = static_cast<Graphics::SpriteButton*>(button_localgame_.get());
    button->SetUpImage(Graphics::MakeImage(0, 0, 150.0f, 40.0f, tex_buttonsleftside));
    button->SetHoverImage(Graphics::MakeImage(150, 0, 150.0f, 40.0f, tex_buttonsleftside));
    button->SetDownImage(Graphics::MakeImage(300, 0, 150.0f, 40.0f, tex_buttonsleftside));
    button->SetPosition(Graphics::Vector(0.0f, 65.0f));
    button->SetBoundingRect(Graphics::Vector(), Graphics::Vector(150.0f, 40.0f));
    button->SetClickSound(buttonclick_sound_);

    button = static_cast<Graphics::SpriteButton*>(button_networkgame_.get());
    button->SetUpImage(Graphics::MakeImage(0, 40, 150.0f, 40.0f, tex_buttonsleftside));
    button->SetHoverImage(Graphics::MakeImage(150, 40, 150.0f, 40.0f, tex_buttonsleftside));
    button->SetDownImage(Graphics::MakeImage(300, 40, 150.0f, 40.0f, tex_buttonsleftside));
    button->SetPosition(Graphics::Vector(0.0f, 105.0f));
    button->SetBoundingRect(Graphics::Vector(), Graphics::Vector(150.0f, 40.0f));
    button->SetClickSound(buttonclick_sound_);

    button = static_cast<Graphics::SpriteButton*>(button_main_.get());
    button->SetUpImage(Graphics::MakeImage(640, 0, 60.0f, 20.0f, tex_mainbackground));
    button->SetHoverImage(Graphics::MakeImage(700, 0, 60.0f, 20.0f, tex_mainbackground));
    button->SetDownImage(Graphics::MakeImage(760, 0, 60.0f, 20.0f, tex_mainbackground));
    button->SetPosition(Graphics::Vector(340.0f, 20.0f));
    button->SetBoundingRect(Graphics::Vector(), Graphics::Vector(60.0f, 20.0f));
    button->SetClickSound(buttonclick_sound_);

    button = static_cast<Graphics::SpriteButton*>(button_viewhighscores_.get());
    button->SetUpImage(Graphics::MakeImage(640, 20, 60.0f, 20.0f, tex_mainbackground));
    button->SetHoverImage(Graphics::MakeImage(700, 20, 60.0f, 20.0f, tex_mainbackground));
    button->SetDownImage(Graphics::MakeImage(760, 20, 60.0f, 20.0f, tex_mainbackground));
    button->SetPosition(Graphics::Vector(400.0f, 20.0f));
    button->SetBoundingRect(Graphics::Vector(), Graphics::Vector(60.0f, 20.0f));
    button->SetClickSound(buttonclick_sound_);

    button = static_cast<Graphics::SpriteButton*>(button_settings_.get());
    button->SetUpImage(Graphics::MakeImage(640, 40, 60.0f, 20.0f, tex_mainbackground));
    button->SetHoverImage(Graphics::MakeImage(700, 40, 60.0f, 20.0f, tex_mainbackground));
    button->SetDownImage(Graphics::MakeImage(760, 40, 60.0f, 20.0f, tex_mainbackground));
    button->SetPosition(Graphics::Vector(460.0f, 20.0f));
    button->SetBoundingRect(Graphics::Vector(), Graphics::Vector(60.0f, 20.0f));
    button->SetClickSound(buttonclick_sound_);

    GetSplash().SetPos(90);

    button = static_cast<Graphics::SpriteButton*>(button_about_.get());
    button->SetUpImage(Graphics::MakeImage(640, 60, 60.0f, 20.0f, tex_mainbackground));
    button->SetHoverImage(Graphics::MakeImage(700, 60, 60.0f, 20.0f, tex_mainbackground));
    button->SetDownImage(Graphics::MakeImage(760, 60, 60.0f, 20.0f, tex_mainbackground));
    button->SetPosition(Graphics::Vector(520.0f, 20.0f));
    button->SetBoundingRect(Graphics::Vector(), Graphics::Vector(60.0f, 20.0f));
    button->SetClickSound(buttonclick_sound_);

    button = static_cast<Graphics::SpriteButton*>(button_exitapplication_.get());
    button->SetUpImage(Graphics::MakeImage(640, 80, 60.0f, 20.0f, tex_mainbackground));
    button->SetHoverImage(Graphics::MakeImage(700, 80, 60.0f, 20.0f, tex_mainbackground));
    button->SetDownImage(Graphics::MakeImage(760, 80, 60.0f, 20.0f, tex_mainbackground));
    button->SetPosition(Graphics::Vector(580.0f, 20.0f));
    button->SetBoundingRect(Graphics::Vector(), Graphics::Vector(60.0f, 20.0f));
    button->SetClickSound(buttonclick_sound_);

    button = static_cast<Graphics::SpriteButton*>(button_back_.get());
    button->SetUpImage(Graphics::MakeImage(640, 100, 60.0f, 20.0f, tex_mainbackground));
    button->SetHoverImage(Graphics::MakeImage(700, 100, 60.0f, 20.0f, tex_mainbackground));
    button->SetDownImage(Graphics::MakeImage(760, 100, 60.0f, 20.0f, tex_mainbackground));
    button->SetPosition(Graphics::Vector(580.0f, 20.0f));
    button->SetBoundingRect(Graphics::Vector(), Graphics::Vector(60.0f, 20.0f));
    button->SetClickSound(buttonclick_sound_);

    // The view highscores page
    Graphics::SpriteListCtrl* listctrl = static_cast<Graphics::SpriteListCtrl*>(listctrl_highscores_.get());
    listctrl->SetBackImage(Graphics::MakeImage(0, 0, 470.0f, 250.0f, tex_listctrl));
    listctrl->SetDisabledImage(Graphics::MakeImage(500, 0, 430.0f, 250.0f, tex_listctrl));
    listctrl->SetStringFont(font_control_);
    listctrl->SetStringDefaultColor(Graphics::Color(0.0f, 0.0f, 0.0f));
    listctrl->SetStringSelectedColor(Graphics::Color(1.0f, 1.0f, 1.0f));
    listctrl->SetStringHighlightedColor(Graphics::Color(0.0f, 0.0f, 1.0f));
    listctrl->SetSelectionRectColor(Graphics::Color(0.368f, 0.368f, 0.368f));
    listctrl->SetHeaderColor(Graphics::Color(1.0f, 1.0f, 0.0f));
    listctrl->SetItemHeight(20);
    listctrl->SetPageSize(30);
    listctrl->SetRightMargin(10);
    listctrl->SetLeftMargin(10);
    listctrl->SetPosition(Graphics::Vector(10.0f, 140.0f));
    listctrl->SetBoundingRect(Graphics::Vector(), Graphics::Vector(470.0f, 250.0f));

    button = static_cast<Graphics::SpriteButton*>(button_resethighscores_.get());
    button->SetUpImage(Graphics::MakeImage(0, 80, 150.0f, 40.0f, tex_buttonsleftside));
    button->SetHoverImage(Graphics::MakeImage(150, 80, 150.0f, 40.0f, tex_buttonsleftside));
    button->SetDownImage(Graphics::MakeImage(300, 80, 150.0f, 40.0f, tex_buttonsleftside));
    button->SetPosition(Graphics::Vector(490.0f, 65.0f));
    button->SetBoundingRect(Graphics::Vector(), Graphics::Vector(150.0f, 40.0f));
    button->SetClickSound(buttonclick_sound_);

    button = static_cast<Graphics::SpriteButton*>(button_ok_.get());
    button->SetUpImage(Graphics::MakeImage(0, 225, 70.0f, 25.0f, tex_buttons));
    button->SetHoverImage(Graphics::MakeImage(70, 225, 70.0f, 25.0f, tex_buttons));
    button->SetDownImage(Graphics::MakeImage(140, 225, 70.0f, 25.0f, tex_buttons));
    button->SetDisabledImage(Graphics::MakeImage(210, 225, 70.0f, 25.0f, tex_buttons));
    button->SetPosition(Graphics::Vector(550.0f, 150.0f));
    button->SetBoundingRect(Graphics::Vector(), Graphics::Vector(70.0f, 25.0f));
    button->SetClickSound(buttonclick_sound_);

    button = static_cast<Graphics::SpriteButton*>(button_send_.get());
    button->SetUpImage(Graphics::MakeImage(0, 200, 70.0f, 25.0f, tex_buttons));
    button->SetHoverImage(Graphics::MakeImage(70, 200, 70.0f, 25.0f, tex_buttons));
    button->SetDownImage(Graphics::MakeImage(140, 200, 70.0f, 25.0f, tex_buttons));
    button->SetDisabledImage(Graphics::MakeImage(210, 200, 70.0f, 25.0f, tex_buttons));
    button->SetPosition(Graphics::Vector(564.0f, 404.0f));
    button->SetBoundingRect(Graphics::Vector(), Graphics::Vector(70.0f, 25.0f));
    button->SetClickSound(buttonclick_sound_);

    // The player setup page
    Graphics::SpriteEditBox* editbox = static_cast<Graphics::SpriteEditBox*>(editbox_player_.get());
    editbox->SetBackImage(Graphics::MakeImage(0, 0, 250.0f, 30.0f, tex_editbox));
    editbox->SetDisabledImage(Graphics::MakeImage(250, 0, 250.0f, 30.0f, tex_editbox));
    editbox->SetFont(font_control_);
    editbox->SetFontColor(Graphics::Color(0.0f, 0.0f, 0.0f));
    editbox->SetPosition(Graphics::Vector(165.0f, 120.0f));
    editbox->SetBoundingRect(Graphics::Vector(), Graphics::Vector(250.0f, 30.0f));

    Graphics::SpriteListBox* listbox = static_cast<Graphics::SpriteListBox*>(listbox_players_.get());
    listbox->SetBackImage(Graphics::MakeImage(0, 0, 250.0f, 200.0f, tex_listbox));
    listbox->SetDisabledImage(Graphics::MakeImage(250, 0, 250.0f, 200.0f, tex_listbox));
    listbox->SetStringFont(font_control_);
    listbox->SetStringNormalColor(Graphics::Color(0.0f, 0.0f, 0.0f));
    listbox->SetStringSelectedColor(Graphics::Color(1.0f, 1.0f, 1.0f));
    listbox->SetStringHighlightedColor(Graphics::Color(0.0f, 0.0f, 1.0f));
    listbox->SetSelectionRectColor(Graphics::Color(0.368f, 0.368f, 0.368f));
    listbox->SetItemHeight(20);
    listbox->SetPageSize(30);
    listbox->SetPosition(Graphics::Vector(165.0f, 160.0f));
    listbox->SetBoundingRect(Graphics::Vector(), Graphics::Vector(250.0f, 200.0f));

    GetSplash().SetPos(105);

    button = static_cast<Graphics::SpriteButton*>(button_addplayer_.get());
    button->SetUpImage(Graphics::MakeImage(0, 0, 70.0f, 25.0f, tex_buttons));
    button->SetHoverImage(Graphics::MakeImage(70, 0, 70.0f, 25.0f, tex_buttons));
    button->SetDownImage(Graphics::MakeImage(140, 0, 70.0f, 25.0f, tex_buttons));
    button->SetDisabledImage(Graphics::MakeImage(210, 0, 70.0f, 25.0f, tex_buttons));
    button->SetPosition(Graphics::Vector(430.0f, 120.0f));
    button->SetBoundingRect(Graphics::Vector(), Graphics::Vector(70.0f, 25.0f));
    button->SetClickSound(buttonclick_sound_);

    button = static_cast<Graphics::SpriteButton*>(button_updateplayer_.get());
    button->SetUpImage(Graphics::MakeImage(0, 25, 70.0f, 25.0f, tex_buttons));
    button->SetHoverImage(Graphics::MakeImage(70, 25, 70.0f, 25.0f, tex_buttons));
    button->SetDownImage(Graphics::MakeImage(140, 25, 70.0f, 25.0f, tex_buttons));
    button->SetDisabledImage(Graphics::MakeImage(210, 25, 70.0f, 25.0f, tex_buttons));
    button->SetPosition(Graphics::Vector(430.0f, 120.0f));
    button->SetBoundingRect(Graphics::Vector(), Graphics::Vector(70.0f, 25.0f));
    button->SetClickSound(buttonclick_sound_);

    button = static_cast<Graphics::SpriteButton*>(button_editplayer_.get());
    button->SetUpImage(Graphics::MakeImage(0, 50, 70.0f, 25.0f, tex_buttons));
    button->SetHoverImage(Graphics::MakeImage(70, 50, 70.0f, 25.0f, tex_buttons));
    button->SetDownImage(Graphics::MakeImage(140, 50, 70.0f, 25.0f, tex_buttons));
    button->SetDisabledImage(Graphics::MakeImage(210, 50, 70.0f, 25.0f, tex_buttons));
    button->SetPosition(Graphics::Vector(430.0f, 160.0f));
    button->SetBoundingRect(Graphics::Vector(), Graphics::Vector(70.0f, 25.0f));
    button->SetClickSound(buttonclick_sound_);

    button = static_cast<Graphics::SpriteButton*>(button_removeplayer_.get());
    button->SetUpImage(Graphics::MakeImage(0, 75, 70.0f, 25.0f, tex_buttons));
    button->SetHoverImage(Graphics::MakeImage(70, 75, 70.0f, 25.0f, tex_buttons));
    button->SetDownImage(Graphics::MakeImage(140, 75, 70.0f, 25.0f, tex_buttons));
    button->SetDisabledImage(Graphics::MakeImage(210, 75, 70.0f, 25.0f, tex_buttons));
    button->SetPosition(Graphics::Vector(430.0f, 200.0f));
    button->SetBoundingRect(Graphics::Vector(), Graphics::Vector(70.0f, 25.0f));
    button->SetClickSound(buttonclick_sound_);

    button = static_cast<Graphics::SpriteButton*>(button_clearplayers_.get());
    button->SetUpImage(Graphics::MakeImage(0, 100, 70.0f, 25.0f, tex_buttons));
    button->SetHoverImage(Graphics::MakeImage(70, 100, 70.0f, 25.0f, tex_buttons));
    button->SetDownImage(Graphics::MakeImage(140, 100, 70.0f, 25.0f, tex_buttons));
    button->SetDisabledImage(Graphics::MakeImage(210, 100, 70.0f, 25.0f, tex_buttons));
    button->SetPosition(Graphics::Vector(430.0f, 240.0f));
    button->SetBoundingRect(Graphics::Vector(), Graphics::Vector(70.0f, 25.0f));
    button->SetClickSound(buttonclick_sound_);

    button = static_cast<Graphics::SpriteButton*>(button_start_.get());
    button->SetUpImage(Graphics::MakeImage(0, 125, 70.0f, 25.0f, tex_buttons));
    button->SetHoverImage(Graphics::MakeImage(70, 125, 70.0f, 25.0f, tex_buttons));
    button->SetDownImage(Graphics::MakeImage(140, 125, 70.0f, 25.0f, tex_buttons));
    button->SetDisabledImage(Graphics::MakeImage(210, 125, 70.0f, 25.0f, tex_buttons));
    button->SetPosition(Graphics::Vector(430.0f, 330.0f));
    button->SetBoundingRect(Graphics::Vector(), Graphics::Vector(70.0f, 25.0f));
    button->SetClickSound(buttonclick_sound_);

    // The enter new highscore page.
    editbox = static_cast<Graphics::SpriteEditBox*>(editbox_highscore_.get());
    editbox->SetBackImage(Graphics::MakeImage(0, 0, 250.0f, 30.0f, tex_editbox));
    editbox->SetDisabledImage(Graphics::MakeImage(250, 0, 250.0f, 30.0f, tex_editbox));
    editbox->SetFont(font_control_);
    editbox->SetFontColor(Graphics::Color(0.0f, 0.0f, 0.0f));
    editbox->SetPosition(Graphics::Vector(30.0f, 380.0f));
    editbox->SetBoundingRect(Graphics::Vector(), Graphics::Vector(250.0f, 30.0f));

    button = static_cast<Graphics::SpriteButton*>(button_savehighscore_.get());
    button->SetUpImage(Graphics::MakeImage(0, 150, 70.0f, 25.0f, tex_buttons));
    button->SetHoverImage(Graphics::MakeImage(70, 150, 70.0f, 25.0f, tex_buttons));
    button->SetDownImage(Graphics::MakeImage(140, 150, 70.0f, 25.0f, tex_buttons));
    button->SetDisabledImage(Graphics::MakeImage(210, 150, 70.0f, 25.0f, tex_buttons));
    button->SetPosition(Graphics::Vector(320.0f, 380.0f));
    button->SetBoundingRect(Graphics::Vector(), Graphics::Vector(70.0f, 25.0f));
    button->SetClickSound(buttonclick_sound_);

    button = static_cast<Graphics::SpriteButton*>(button_continue_.get());
    button->SetUpImage(Graphics::MakeImage(0, 120, 150.0f, 40.0f, tex_buttonsleftside));
    button->SetHoverImage(Graphics::MakeImage(150, 120, 150.0f, 40.0f, tex_buttonsleftside));
    button->SetDownImage(Graphics::MakeImage(300, 120, 150.0f, 40.0f, tex_buttonsleftside));
    button->SetPosition(Graphics::Vector(490.0f, 65.0f));
    button->SetBoundingRect(Graphics::Vector(), Graphics::Vector(150.0f, 40.0f));
    button->SetClickSound(buttonclick_sound_);

    // The options page.
    Graphics::SpriteCheckBox* checkbox = static_cast<Graphics::SpriteCheckBox*>(checkbox_fullscreenenabled_.get());
    checkbox->SetUpImage(Graphics::MakeImage(0, 0, 200.0f, 30.0f, tex_checkbox), Graphics::SpriteCheckBox::UNCHECKED);
    checkbox->SetHoverImage(Graphics::MakeImage(200, 0, 200.0f, 30.0f, tex_checkbox), Graphics::SpriteCheckBox::UNCHECKED);
    checkbox->SetDownImage(Graphics::MakeImage(400, 0, 200.0f, 30.0f, tex_checkbox), Graphics::SpriteCheckBox::UNCHECKED);
    checkbox->SetUpImage(Graphics::MakeImage(0, 30, 200.0f, 30.0f, tex_checkbox), Graphics::SpriteCheckBox::CHECKED);
    checkbox->SetHoverImage(Graphics::MakeImage(200, 30, 200.0f, 30.0f, tex_checkbox), Graphics::SpriteCheckBox::CHECKED);
    checkbox->SetDownImage(Graphics::MakeImage(400, 30, 200.0f, 30.0f, tex_checkbox), Graphics::SpriteCheckBox::CHECKED);
    checkbox->SetPosition(Graphics::Vector(275.0f, 125.0f));
    checkbox->SetBoundingRect(Graphics::Vector(), Graphics::Vector(200.0f, 30.0f));
    checkbox->SetClickSound(checkboxclick_sound_);

    button = static_cast<Graphics::SpriteButton*>(button_apply_.get());
    button->SetUpImage(Graphics::MakeImage(0, 175, 70.0f, 25.0f, tex_buttons));
    button->SetHoverImage(Graphics::MakeImage(70, 175, 70.0f, 25.0f, tex_buttons));
    button->SetDownImage(Graphics::MakeImage(140, 175, 70.0f, 25.0f, tex_buttons));
    button->SetDisabledImage(Graphics::MakeImage(210, 175, 70.0f, 25.0f, tex_buttons));
    button->SetPosition(Graphics::Vector(480.0f, 125.0f));
    button->SetBoundingRect(Graphics::Vector(), Graphics::Vector(70.0f, 25.0f));
    button->SetClickSound(buttonclick_sound_);

    button = static_cast<Graphics::SpriteButton*>(button_test_.get());
    button->SetUpImage(Graphics::MakeImage(0, 250, 70.0f, 25.0f, tex_buttons));
    button->SetHoverImage(Graphics::MakeImage(70, 250, 70.0f, 25.0f, tex_buttons));
    button->SetDownImage(Graphics::MakeImage(140, 250, 70.0f, 25.0f, tex_buttons));
    button->SetDisabledImage(Graphics::MakeImage(210, 250, 70.0f, 25.0f, tex_buttons));
    button->SetPosition(Graphics::Vector(480.0f, 205.0f));
    button->SetBoundingRect(Graphics::Vector(), Graphics::Vector(70.0f, 25.0f));
    button->SetClickSound(soundtest_sound_);

    checkbox = static_cast<Graphics::SpriteCheckBox*>(checkbox_audioenabled_.get());
    checkbox->SetUpImage(Graphics::MakeImage(0, 60, 200.0f, 30.0f, tex_checkbox), Graphics::SpriteCheckBox::UNCHECKED);
    checkbox->SetHoverImage(Graphics::MakeImage(200, 60, 200.0f, 30.0f, tex_checkbox), Graphics::SpriteCheckBox::UNCHECKED);
    checkbox->SetDownImage(Graphics::MakeImage(400, 60, 200.0f, 30.0f, tex_checkbox), Graphics::SpriteCheckBox::UNCHECKED);
    checkbox->SetUpImage(Graphics::MakeImage(0, 90, 200.0f, 30.0f, tex_checkbox), Graphics::SpriteCheckBox::CHECKED);
    checkbox->SetHoverImage(Graphics::MakeImage(200, 90, 200.0f, 30.0f, tex_checkbox), Graphics::SpriteCheckBox::CHECKED);
    checkbox->SetDownImage(Graphics::MakeImage(400, 90, 200.0f, 30.0f, tex_checkbox), Graphics::SpriteCheckBox::CHECKED);
    checkbox->SetPosition(Graphics::Vector(275.0f, 165.0f));
    checkbox->SetBoundingRect(Graphics::Vector(), Graphics::Vector(200.0f, 30.0f));
    checkbox->SetClickSound(checkboxclick_sound_);

    Graphics::SpriteSlider* slider = static_cast<Graphics::SpriteSlider*>(slider_audiovolume_.get());
    slider->SetBackNormalImage(Graphics::MakeImage(0, 0, 200.0f, 30.0f, tex_slider));
    slider->SetBackDisabledImage(Graphics::MakeImage(200, 0, 200.0f, 30.0f, tex_slider));
    slider->SetGrabberNormalImage(Graphics::MakeImage(0, 30, 12.0f, 20.0f, tex_slider));
    slider->SetGrabberDisabledImage(Graphics::MakeImage(12, 30, 12.0f, 20.0f, tex_slider));
    slider->SetPosition(Graphics::Vector(275.0f, 205.0f));
    slider->SetBoundingRect(Graphics::Vector(), Graphics::Vector(200.0f, 30.0f));
    slider->SetRange(0, 100);
    slider->SetPos(100);
    slider->SetGrabberXCoords(Graphics::Vector(11.0f, 189.0f));
    slider->SetGrabberYCoord(9.0f);

    GetSplash().SetPos(120);

    // The game board.
    image_gameboard_.reset(new Graphics::Image);
    image_gameboard_->Create(0, 0, 640, 480, tex_gameboard);

    editbox = static_cast<Graphics::SpriteEditBox*>(editbox_ones_.get());
    editbox->SetReadOnly();
    editbox->SetLeftMargin(30.0f);
    editbox->SetBackImage(Graphics::MakeImage(0, 90, 120.0f, 20.0f, tex_editbox));
    editbox->SetDisabledImage(Graphics::MakeImage(120, 90, 120.0f, 20.0f, tex_editbox));
    editbox->SetFont(font_control_);
    editbox->SetFontColor(Graphics::Color(0.0f, 0.0f, 0.0f));
    editbox->SetPosition(Graphics::Vector(245.0f, 10.0f));
    editbox->SetBoundingRect(Graphics::Vector(), Graphics::Vector(120.0f, 20.0f));
    editbox->SetClickSound(editboxclick_sound_);

    editbox = static_cast<Graphics::SpriteEditBox*>(editbox_twos_.get());
    editbox->SetReadOnly();
    editbox->SetLeftMargin(30.0f);
    editbox->SetBackImage(Graphics::MakeImage(0, 90, 120.0f, 20.0f, tex_editbox));
    editbox->SetDisabledImage(Graphics::MakeImage(120, 90, 120.0f, 20.0f, tex_editbox));
    editbox->SetFont(font_control_);
    editbox->SetFontColor(Graphics::Color(0.0f, 0.0f, 0.0f));
    editbox->SetPosition(Graphics::Vector(245.0f, 33.0f));
    editbox->SetBoundingRect(Graphics::Vector(), Graphics::Vector(120.0f, 20.0f));
    editbox->SetClickSound(editboxclick_sound_);

    editbox = static_cast<Graphics::SpriteEditBox*>(editbox_threes_.get());
    editbox->SetReadOnly();
    editbox->SetLeftMargin(30.0f);
    editbox->SetBackImage(Graphics::MakeImage(0, 90, 120.0f, 20.0f, tex_editbox));
    editbox->SetDisabledImage(Graphics::MakeImage(120, 90, 120.0f, 20.0f, tex_editbox));
    editbox->SetFont(font_control_);
    editbox->SetFontColor(Graphics::Color(0.0f, 0.0f, 0.0f));
    editbox->SetPosition(Graphics::Vector(245.0f, 56.0f));
    editbox->SetBoundingRect(Graphics::Vector(), Graphics::Vector(120.0f, 20.0f));
    editbox->SetClickSound(editboxclick_sound_);

    editbox = static_cast<Graphics::SpriteEditBox*>(editbox_fours_.get());
    editbox->SetReadOnly();
    editbox->SetLeftMargin(30.0f);
    editbox->SetBackImage(Graphics::MakeImage(0, 90, 120.0f, 20.0f, tex_editbox));
    editbox->SetDisabledImage(Graphics::MakeImage(120, 90, 120.0f, 20.0f, tex_editbox));
    editbox->SetFont(font_control_);
    editbox->SetFontColor(Graphics::Color(0.0f, 0.0f, 0.0f));
    editbox->SetPosition(Graphics::Vector(245.0f, 79.0f));
    editbox->SetBoundingRect(Graphics::Vector(), Graphics::Vector(120.0f, 20.0f));
    editbox->SetClickSound(editboxclick_sound_);

    editbox = static_cast<Graphics::SpriteEditBox*>(editbox_fives_.get());
    editbox->SetReadOnly();
    editbox->SetLeftMargin(30.0f);
    editbox->SetBackImage(Graphics::MakeImage(0, 90, 120.0f, 20.0f, tex_editbox));
    editbox->SetDisabledImage(Graphics::MakeImage(120, 90, 120.0f, 20.0f, tex_editbox));
    editbox->SetFont(font_control_);
    editbox->SetFontColor(Graphics::Color(0.0f, 0.0f, 0.0f));
    editbox->SetPosition(Graphics::Vector(245.0f, 102.0f));
    editbox->SetBoundingRect(Graphics::Vector(), Graphics::Vector(120.0f, 20.0f));
    editbox->SetClickSound(editboxclick_sound_);

    GetSplash().SetPos(130);

    editbox = static_cast<Graphics::SpriteEditBox*>(editbox_sixes_.get());
    editbox->SetReadOnly();
    editbox->SetLeftMargin(30.0f);
    editbox->SetBackImage(Graphics::MakeImage(0, 90, 120.0f, 20.0f, tex_editbox));
    editbox->SetDisabledImage(Graphics::MakeImage(120, 90, 120.0f, 20.0f, tex_editbox));
    editbox->SetFont(font_control_);
    editbox->SetFontColor(Graphics::Color(0.0f, 0.0f, 0.0f));
    editbox->SetPosition(Graphics::Vector(245.0f, 125.0f));
    editbox->SetBoundingRect(Graphics::Vector(), Graphics::Vector(120.0f, 20.0f));
    editbox->SetClickSound(editboxclick_sound_);

    editbox = static_cast<Graphics::SpriteEditBox*>(editbox_subtotal_.get());
    editbox->SetReadOnly();
    editbox->SetLeftMargin(30.0f);
    editbox->SetBackImage(Graphics::MakeImage(0, 90, 120.0f, 20.0f, tex_editbox));
    editbox->SetDisabledImage(Graphics::MakeImage(120, 90, 120.0f, 20.0f, tex_editbox));
    editbox->SetFont(font_control_);
    editbox->SetFontColor(Graphics::Color(0.0f, 0.0f, 0.0f));
    editbox->SetPosition(Graphics::Vector(245.0f, 148.0f));
    editbox->SetBoundingRect(Graphics::Vector(), Graphics::Vector(120.0f, 20.0f));

    editbox = static_cast<Graphics::SpriteEditBox*>(editbox_bonus_.get());
    editbox->SetReadOnly();
    editbox->SetLeftMargin(30.0f);
    editbox->SetBackImage(Graphics::MakeImage(0, 90, 120.0f, 20.0f, tex_editbox));
    editbox->SetDisabledImage(Graphics::MakeImage(120, 90, 120.0f, 20.0f, tex_editbox));
    editbox->SetFont(font_control_);
    editbox->SetFontColor(Graphics::Color(0.0f, 0.0f, 0.0f));
    editbox->SetPosition(Graphics::Vector(245.0f, 171.0f));
    editbox->SetBoundingRect(Graphics::Vector(), Graphics::Vector(120.0f, 20.0f));

    GetSplash().SetPos(140);

    editbox = static_cast<Graphics::SpriteEditBox*>(editbox_uppertotal_.get());
    editbox->SetReadOnly();
    editbox->SetLeftMargin(30.0f);
    editbox->SetBackImage(Graphics::MakeImage(0, 90, 120.0f, 20.0f, tex_editbox));
    editbox->SetDisabledImage(Graphics::MakeImage(120, 90, 120.0f, 20.0f, tex_editbox));
    editbox->SetFont(font_control_);
    editbox->SetFontColor(Graphics::Color(0.0f, 0.0f, 0.0f));
    editbox->SetPosition(Graphics::Vector(245.0f, 194.0f));
    editbox->SetBoundingRect(Graphics::Vector(), Graphics::Vector(120.0f, 20.0f));

    editbox = static_cast<Graphics::SpriteEditBox*>(editbox_threeofakind_.get());
    editbox->SetReadOnly();
    editbox->SetLeftMargin(30.0f);
    editbox->SetBackImage(Graphics::MakeImage(0, 90, 120.0f, 20.0f, tex_editbox));
    editbox->SetDisabledImage(Graphics::MakeImage(120, 90, 120.0f, 20.0f, tex_editbox));
    editbox->SetFont(font_control_);
    editbox->SetFontColor(Graphics::Color(0.0f, 0.0f, 0.0f));
    editbox->SetPosition(Graphics::Vector(245.0f, 228.0f));
    editbox->SetBoundingRect(Graphics::Vector(), Graphics::Vector(120.0f, 20.0f));
    editbox->SetClickSound(editboxclick_sound_);

    editbox = static_cast<Graphics::SpriteEditBox*>(editbox_fourofakind_.get());
    editbox->SetReadOnly();
    editbox->SetLeftMargin(30.0f);
    editbox->SetBackImage(Graphics::MakeImage(0, 90, 120.0f, 20.0f, tex_editbox));
    editbox->SetDisabledImage(Graphics::MakeImage(120, 90, 120.0f, 20.0f, tex_editbox));
    editbox->SetFont(font_control_);
    editbox->SetFontColor(Graphics::Color(0.0f, 0.0f, 0.0f));
    editbox->SetPosition(Graphics::Vector(245.0f, 251.0f));
    editbox->SetBoundingRect(Graphics::Vector(), Graphics::Vector(120.0f, 20.0f));
    editbox->SetClickSound(editboxclick_sound_);

    editbox = static_cast<Graphics::SpriteEditBox*>(editbox_fullhouse_.get());
    editbox->SetReadOnly();
    editbox->SetLeftMargin(30.0f);
    editbox->SetBackImage(Graphics::MakeImage(0, 90, 120.0f, 20.0f, tex_editbox));
    editbox->SetDisabledImage(Graphics::MakeImage(120, 90, 120.0f, 20.0f, tex_editbox));
    editbox->SetFont(font_control_);
    editbox->SetFontColor(Graphics::Color(0.0f, 0.0f, 0.0f));
    editbox->SetPosition(Graphics::Vector(245.0f, 274.0f));
    editbox->SetBoundingRect(Graphics::Vector(), Graphics::Vector(120.0f, 20.0f));
    editbox->SetClickSound(editboxclick_sound_);

    editbox = static_cast<Graphics::SpriteEditBox*>(editbox_smallstraight_.get());
    editbox->SetReadOnly();
    editbox->SetLeftMargin(30.0f);
    editbox->SetBackImage(Graphics::MakeImage(0, 90, 120.0f, 20.0f, tex_editbox));
    editbox->SetDisabledImage(Graphics::MakeImage(120, 90, 120.0f, 20.0f, tex_editbox));
    editbox->SetFont(font_control_);
    editbox->SetFontColor(Graphics::Color(0.0f, 0.0f, 0.0f));
    editbox->SetPosition(Graphics::Vector(245.0f, 297.0f));
    editbox->SetBoundingRect(Graphics::Vector(), Graphics::Vector(120.0f, 20.0f));
    editbox->SetClickSound(editboxclick_sound_);

    editbox = static_cast<Graphics::SpriteEditBox*>(editbox_largestraight_.get());
    editbox->SetReadOnly();
    editbox->SetLeftMargin(30.0f);
    editbox->SetBackImage(Graphics::MakeImage(0, 90, 120.0f, 20.0f, tex_editbox));
    editbox->SetDisabledImage(Graphics::MakeImage(120, 90, 120.0f, 20.0f, tex_editbox));
    editbox->SetFont(font_control_);
    editbox->SetFontColor(Graphics::Color(0.0f, 0.0f, 0.0f));
    editbox->SetPosition(Graphics::Vector(245.0f, 320.0f));
    editbox->SetBoundingRect(Graphics::Vector(), Graphics::Vector(120.0f, 20.0f));
    editbox->SetClickSound(editboxclick_sound_);

    editbox = static_cast<Graphics::SpriteEditBox*>(editbox_yahtzee_.get());
    editbox->SetReadOnly();
    editbox->SetLeftMargin(30.0f);
    editbox->SetBackImage(Graphics::MakeImage(0, 90, 120.0f, 20.0f, tex_editbox));
    editbox->SetDisabledImage(Graphics::MakeImage(120, 90, 120.0f, 20.0f, tex_editbox));
    editbox->SetFont(font_control_);
    editbox->SetFontColor(Graphics::Color(0.0f, 0.0f, 0.0f));
    editbox->SetPosition(Graphics::Vector(245.0f, 343.0f));
    editbox->SetBoundingRect(Graphics::Vector(), Graphics::Vector(120.0f, 20.0f));
    editbox->SetClickSound(editboxclick_sound_);

    GetSplash().SetPos(150);

    editbox = static_cast<Graphics::SpriteEditBox*>(editbox_chance_.get());
    editbox->SetReadOnly();
    editbox->SetLeftMargin(30.0f);
    editbox->SetBackImage(Graphics::MakeImage(0, 90, 120.0f, 20.0f, tex_editbox));
    editbox->SetDisabledImage(Graphics::MakeImage(120, 90, 120.0f, 20.0f, tex_editbox));
    editbox->SetFont(font_control_);
    editbox->SetFontColor(Graphics::Color(0.0f, 0.0f, 0.0f));
    editbox->SetPosition(Graphics::Vector(245.0f, 366.0f));
    editbox->SetBoundingRect(Graphics::Vector(), Graphics::Vector(120.0f, 20.0f));
    editbox->SetClickSound(editboxclick_sound_);

    editbox = static_cast<Graphics::SpriteEditBox*>(editbox_lowertotal_.get());
    editbox->SetReadOnly();
    editbox->SetLeftMargin(30.0f);
    editbox->SetBackImage(Graphics::MakeImage(0, 90, 120.0f, 20.0f, tex_editbox));
    editbox->SetDisabledImage(Graphics::MakeImage(120, 90, 120.0f, 20.0f, tex_editbox));
    editbox->SetFont(font_control_);
    editbox->SetFontColor(Graphics::Color(0.0f, 0.0f, 0.0f));
    editbox->SetPosition(Graphics::Vector(245.0f, 389.0f));
    editbox->SetBoundingRect(Graphics::Vector(), Graphics::Vector(120.0f, 20.0f));

    editbox = static_cast<Graphics::SpriteEditBox*>(editbox_grandtotal_.get());
    editbox->SetReadOnly();
    editbox->SetLeftMargin(30.0f);
    editbox->SetBackImage(Graphics::MakeImage(0, 90, 120.0f, 20.0f, tex_editbox));
    editbox->SetDisabledImage(Graphics::MakeImage(120, 90, 120.0f, 20.0f, tex_editbox));
    editbox->SetFont(font_control_);
    editbox->SetFontColor(Graphics::Color(0.0f, 0.0f, 0.0f));
    editbox->SetPosition(Graphics::Vector(245.0f, 412.0f));
    editbox->SetBoundingRect(Graphics::Vector(), Graphics::Vector(120.0f, 20.0f));

    dice1fromstilla_imagelist_.reset(new Graphics::ImageList);
    dice1fromstilla_imagelist_->Create("Images/Dice1 - from still (a).bmp", 16, 64.0f, 64.0f, false);

    dice1fromstillb_imagelist_.reset(new Graphics::ImageList);
    dice1fromstillb_imagelist_->Create("Images/Dice1 - from still (b).bmp", 16, 64.0f, 64.0f, false);

    dice1fromstillc_imagelist_.reset(new Graphics::ImageList);
    dice1fromstillc_imagelist_->Create("Images/Dice1 - from still (c).bmp", 16, 64.0f, 64.0f, false);

    dice1tostilla_imagelist_.reset(new Graphics::ImageList);
    dice1tostilla_imagelist_->Create("Images/Dice1 - to still (a).bmp", 16, 64.0f, 64.0f, false);

    dice1tostillb_imagelist_.reset(new Graphics::ImageList);
    dice1tostillb_imagelist_->Create("Images/Dice1 - to still (b).bmp", 16, 64.0f, 64.0f, false);

    dice1tostillc_imagelist_.reset(new Graphics::ImageList);
    dice1tostillc_imagelist_->Create("Images/Dice1 - to still (c).bmp", 16, 64.0f, 64.0f, false);

    dice2fromstilla_imagelist_.reset(new Graphics::ImageList);
    dice2fromstilla_imagelist_->Create("Images/Dice2 - from still (a).bmp", 16, 64.0f, 64.0f, false);

    dice2fromstillb_imagelist_.reset(new Graphics::ImageList);
    dice2fromstillb_imagelist_->Create("Images/Dice2 - from still (b).bmp", 16, 64.0f, 64.0f, false);

    dice2fromstillc_imagelist_.reset(new Graphics::ImageList);
    dice2fromstillc_imagelist_->Create("Images/Dice2 - from still (c).bmp", 16, 64.0f, 64.0f, false);

    dice2tostilla_imagelist_.reset(new Graphics::ImageList);
    dice2tostilla_imagelist_->Create("Images/Dice2 - to still (a).bmp", 16, 64.0f, 64.0f, false);

    dice2tostillb_imagelist_.reset(new Graphics::ImageList);
    dice2tostillb_imagelist_->Create("Images/Dice2 - to still (b).bmp", 16, 64.0f, 64.0f, false);

    dice2tostillc_imagelist_.reset(new Graphics::ImageList);
    dice2tostillc_imagelist_->Create("Images/Dice2 - to still (c).bmp", 16, 64.0f, 64.0f, false);

    dice3fromstilla_imagelist_.reset(new Graphics::ImageList);
    dice3fromstilla_imagelist_->Create("Images/Dice3 - from still (a).bmp", 16, 64.0f, 64.0f, false);

    dice3fromstillb_imagelist_.reset(new Graphics::ImageList);
    dice3fromstillb_imagelist_->Create("Images/Dice3 - from still (b).bmp", 16, 64.0f, 64.0f, false);

    dice3fromstillc_imagelist_.reset(new Graphics::ImageList);
    dice3fromstillc_imagelist_->Create("Images/Dice3 - from still (c).bmp", 16, 64.0f, 64.0f, false);

    dice3tostilla_imagelist_.reset(new Graphics::ImageList);
    dice3tostilla_imagelist_->Create("Images/Dice3 - to still (a).bmp", 16, 64.0f, 64.0f, false);

    dice3tostillb_imagelist_.reset(new Graphics::ImageList);
    dice3tostillb_imagelist_->Create("Images/Dice3 - to still (b).bmp", 16, 64.0f, 64.0f, false);

    dice3tostillc_imagelist_.reset(new Graphics::ImageList);
    dice3tostillc_imagelist_->Create("Images/Dice3 - to still (c).bmp", 16, 64.0f, 64.0f, false);

    dice4fromstilla_imagelist_.reset(new Graphics::ImageList);
    dice4fromstilla_imagelist_->Create("Images/Dice4 - from still (a).bmp", 16, 64.0f, 64.0f, false);

    dice4fromstillb_imagelist_.reset(new Graphics::ImageList);
    dice4fromstillb_imagelist_->Create("Images/Dice4 - from still (b).bmp", 16, 64.0f, 64.0f, false);

    dice4fromstillc_imagelist_.reset(new Graphics::ImageList);
    dice4fromstillc_imagelist_->Create("Images/Dice4 - from still (c).bmp", 16, 64.0f, 64.0f, false);

    GetSplash().SetPos(160);

    dice4tostilla_imagelist_.reset(new Graphics::ImageList);
    dice4tostilla_imagelist_->Create("Images/Dice4 - to still (a).bmp", 16, 64.0f, 64.0f, false);

    dice4tostillb_imagelist_.reset(new Graphics::ImageList);
    dice4tostillb_imagelist_->Create("Images/Dice4 - to still (b).bmp", 16, 64.0f, 64.0f, false);

    dice4tostillc_imagelist_.reset(new Graphics::ImageList);
    dice4tostillc_imagelist_->Create("Images/Dice4 - to still (c).bmp", 16, 64.0f, 64.0f, false);

    dice5fromstilla_imagelist_.reset(new Graphics::ImageList);
    dice5fromstilla_imagelist_->Create("Images/Dice5 - from still (a).bmp", 16, 64.0f, 64.0f, false);

    dice5fromstillb_imagelist_.reset(new Graphics::ImageList);
    dice5fromstillb_imagelist_->Create("Images/Dice5 - from still (b).bmp", 16, 64.0f, 64.0f, false);

    dice5fromstillc_imagelist_.reset(new Graphics::ImageList);
    dice5fromstillc_imagelist_->Create("Images/Dice5 - from still (c).bmp", 16, 64.0f, 64.0f, false);

    dice5tostilla_imagelist_.reset(new Graphics::ImageList);
    dice5tostilla_imagelist_->Create("Images/Dice5 - to still (a).bmp", 16, 64.0f, 64.0f, false);

    dice5tostillb_imagelist_.reset(new Graphics::ImageList);
    dice5tostillb_imagelist_->Create("Images/Dice5 - to still (b).bmp", 16, 64.0f, 64.0f, false);

    dice5tostillc_imagelist_.reset(new Graphics::ImageList);
    dice5tostillc_imagelist_->Create("Images/Dice5 - to still (c).bmp", 16, 64.0f, 64.0f, false);

    dice6fromstilla_imagelist_.reset(new Graphics::ImageList);
    dice6fromstilla_imagelist_->Create("Images/Dice6 - from still (a).bmp", 16, 64.0f, 64.0f, false);

    dice6fromstillb_imagelist_.reset(new Graphics::ImageList);
    dice6fromstillb_imagelist_->Create("Images/Dice6 - from still (b).bmp", 16, 64.0f, 64.0f, false);

    dice6fromstillc_imagelist_.reset(new Graphics::ImageList);
    dice6fromstillc_imagelist_->Create("Images/Dice6 - from still (c).bmp", 16, 64.0f, 64.0f, false);

    dice6tostilla_imagelist_.reset(new Graphics::ImageList);
    dice6tostilla_imagelist_->Create("Images/Dice6 - to still (a).bmp", 16, 64.0f, 64.0f, false);

    dice6tostillb_imagelist_.reset(new Graphics::ImageList);
    dice6tostillb_imagelist_->Create("Images/Dice6 - to still (b).bmp", 16, 64.0f, 64.0f, false);

    GetSplash().SetPos(170);

    dice6tostillc_imagelist_.reset(new Graphics::ImageList);
    dice6tostillc_imagelist_->Create("Images/Dice6 - to still (c).bmp", 16, 64.0f, 64.0f, false);

    randomspina0_imagelist_.reset(new Graphics::ImageList);
    randomspina0_imagelist_->Create("Images/Random spin (a0).bmp", 16, 64.0f, 64.0f, false);

    randomspina1_imagelist_.reset(new Graphics::ImageList);
    randomspina1_imagelist_->Create("Images/Random spin (a1).bmp", 16, 64.0f, 64.0f, false);

    randomspinb0_imagelist_.reset(new Graphics::ImageList);
    randomspinb0_imagelist_->Create("Images/Random spin (b0).bmp", 16, 64.0f, 64.0f, false);

    randomspinb1_imagelist_.reset(new Graphics::ImageList);
    randomspinb1_imagelist_->Create("Images/Random spin (b1).bmp", 16, 64.0f, 64.0f, false);

    randomspinc0_imagelist_.reset(new Graphics::ImageList);
    randomspinc0_imagelist_->Create("Images/Random spin (c0).bmp", 16, 64.0f, 64.0f, false);

    randomspinc1_imagelist_.reset(new Graphics::ImageList);
    randomspinc1_imagelist_->Create("Images/Random spin (c1).bmp", 16, 64.0f, 64.0f, false);

    Graphics::SpriteStatic* sprite_static = static_cast<Graphics::SpriteStatic*>(static_dice_0_.get());
    sprite_static->SetPosition(Graphics::Vector(25.0f, 20.0f));
    sprite_static->SetCurrentFrame(15);
    sprite_static->SetBoundingRect(Graphics::Vector(), Graphics::Vector(64.0f, 64.0f));

    sprite_static = static_cast<Graphics::SpriteStatic*>(static_dice_1_.get());
    sprite_static->SetPosition(Graphics::Vector(25.0f, 87.0f));
    sprite_static->SetCurrentFrame(15);
    sprite_static->SetBoundingRect(Graphics::Vector(), Graphics::Vector(64.0f, 64.0f));

    sprite_static = static_cast<Graphics::SpriteStatic*>(static_dice_2_.get());
    sprite_static->SetPosition(Graphics::Vector(25.0f, 154.0f));
    sprite_static->SetCurrentFrame(15);
    sprite_static->SetBoundingRect(Graphics::Vector(), Graphics::Vector(64.0f, 64.0f));

    sprite_static = static_cast<Graphics::SpriteStatic*>(static_dice_3_.get());
    sprite_static->SetPosition(Graphics::Vector(25.0f, 221.0f));
    sprite_static->SetCurrentFrame(15);
    sprite_static->SetBoundingRect(Graphics::Vector(), Graphics::Vector(64.0f, 64.0f));

    sprite_static = static_cast<Graphics::SpriteStatic*>(static_dice_4_.get());
    sprite_static->SetPosition(Graphics::Vector(25.0f, 288.0f));
    sprite_static->SetCurrentFrame(15);
    sprite_static->SetBoundingRect(Graphics::Vector(), Graphics::Vector(64.0f, 64.0f));

    Graphics::ImagePtr img_held(new Graphics::Image);
    img_held->Create(0, 0, 83.0f, 66.0f, tex_held);

    sprite_static = static_cast<Graphics::SpriteStatic*>(dice_0_held_.get());
    sprite_static->SetImage(img_held);
    sprite_static->SetPosition(Graphics::Vector(24.0f, 19.0f));
    sprite_static->SetBoundingRect(Graphics::Vector(), Graphics::Vector(83.0f, 66.0f));

    sprite_static = static_cast<Graphics::SpriteStatic*>(dice_1_held_.get());
    sprite_static->SetImage(img_held);
    sprite_static->SetPosition(Graphics::Vector(24.0f, 86.0f));
    sprite_static->SetBoundingRect(Graphics::Vector(), Graphics::Vector(83.0f, 66.0f));

    sprite_static = static_cast<Graphics::SpriteStatic*>(dice_2_held_.get());
    sprite_static->SetImage(img_held);
    sprite_static->SetPosition(Graphics::Vector(24.0f, 153.0f));
    sprite_static->SetBoundingRect(Graphics::Vector(), Graphics::Vector(83.0f, 66.0f));

    sprite_static = static_cast<Graphics::SpriteStatic*>(dice_3_held_.get());
    sprite_static->SetImage(img_held);
    sprite_static->SetPosition(Graphics::Vector(24.0f, 220.0f));
    sprite_static->SetBoundingRect(Graphics::Vector(), Graphics::Vector(83.0f, 66.0f));

    sprite_static = static_cast<Graphics::SpriteStatic*>(dice_4_held_.get());
    sprite_static->SetImage(img_held);
    sprite_static->SetPosition(Graphics::Vector(24.0f, 287.0f));
    sprite_static->SetBoundingRect(Graphics::Vector(), Graphics::Vector(83.0f, 66.0f));

    button = static_cast<Graphics::SpriteButton*>(button_roll_.get());
    button->SetUpImage(Graphics::MakeImage(640, 84, 64.0f, 64.0f, tex_gameboard));
    button->SetHoverImage(Graphics::MakeImage(704, 84, 64.0f, 64.0f, tex_gameboard));
    button->SetDownImage(Graphics::MakeImage(768, 84, 64.0f, 64.0f, tex_gameboard));
    button->SetDisabledImage(Graphics::MakeImage(832, 84, 64.0f, 64.0f, tex_gameboard));
    button->SetPosition(Graphics::Vector(25.0f, 355.0f));
    button->SetBoundingRect(Graphics::Vector(), Graphics::Vector(64.0f, 64.0f));

    button = static_cast<Graphics::SpriteButton*>(button_continue2_.get());
    button->SetUpImage(Graphics::MakeImage(640, 148, 64.0f, 64.0f, tex_gameboard));
    button->SetHoverImage(Graphics::MakeImage(704, 148, 64.0f, 64.0f, tex_gameboard));
    button->SetDownImage(Graphics::MakeImage(768, 148, 64.0f, 64.0f, tex_gameboard));
    button->SetDisabledImage(Graphics::MakeImage(832, 148, 64.0f, 64.0f, tex_gameboard));
    button->SetPosition(Graphics::Vector(25.0f, 355.0f));
    button->SetBoundingRect(Graphics::Vector(), Graphics::Vector(64.0f, 64.0f));
    button->SetClickSound(buttonclick_sound_);

    GetSplash().SetPos(180);

    button = static_cast<Graphics::SpriteButton*>(button_endgame_.get());
    button->SetUpImage(Graphics::MakeImage(640, 64, 60.0f, 20.0f, tex_gameboard));
    button->SetHoverImage(Graphics::MakeImage(700, 64, 60.0f, 20.0f, tex_gameboard));
    button->SetDownImage(Graphics::MakeImage(760, 64, 60.0f, 20.0f, tex_gameboard));
    button->SetDisabledImage(Graphics::MakeImage(820, 64, 60.0f, 20.0f, tex_gameboard));
    button->SetPosition(Graphics::Vector(60.0f, 450.0f));
    button->SetBoundingRect(Graphics::Vector(), Graphics::Vector(60.0f, 20.0f));
    button->SetClickSound(buttonclick_sound_);

    button = static_cast<Graphics::SpriteButton*>(button_ingamesettings_.get());
    button->SetUpImage(Graphics::MakeImage(640, 44, 60.0f, 20.0f, tex_gameboard));
    button->SetHoverImage(Graphics::MakeImage(700, 44, 60.0f, 20.0f, tex_gameboard));
    button->SetDownImage(Graphics::MakeImage(760, 44, 60.0f, 20.0f, tex_gameboard));
    button->SetDisabledImage(Graphics::MakeImage(820, 44, 60.0f, 20.0f, tex_gameboard));
    button->SetPosition(Graphics::Vector(0.0f, 450.0f));
    button->SetBoundingRect(Graphics::Vector(), Graphics::Vector(60.0f, 20.0f));
    button->SetClickSound(buttonclick_sound_);

    listctrl = static_cast<Graphics::SpriteListCtrl*>(listctrl_ingameplayers_.get());
    listctrl->SetBackImage(Graphics::MakeImage(0, 250, 252.0f, 214.0f, tex_listctrl));
    listctrl->SetDisabledImage(Graphics::MakeImage(252, 250, 252.0f, 214.0f, tex_listctrl));
    listctrl->SetStringFont(font_control_);
    listctrl->SetStringDefaultColor(Graphics::Color(0.0f, 0.0f, 0.0f));
    listctrl->SetStringSelectedColor(Graphics::Color(1.0f, 1.0f, 1.0f));
    listctrl->SetStringHighlightedColor(Graphics::Color(0.0f, 0.0f, 1.0f));
    listctrl->SetSelectionRectColor(Graphics::Color(0.368f, 0.368f, 0.368f));
    listctrl->SetHeaderColor(Graphics::Color(1.0f, 1.0f, 0.0f));
    listctrl->SetItemHeight(20);
    listctrl->SetPageSize(30);
    listctrl->SetRightMargin(10);
    listctrl->SetLeftMargin(10);
    listctrl->SetPosition(Graphics::Vector(382.0f, 5.0f));
    listctrl->SetBoundingRect(Graphics::Vector(), Graphics::Vector(252.0f, 214.0f));

    listbox = static_cast<Graphics::SpriteListBox*>(listbox_messages_.get());
    listbox->SetBackImage(Graphics::MakeImage(0, 200, 252.0f, 182.0f, tex_listbox));
    listbox->SetDisabledImage(Graphics::MakeImage(252, 200, 252.0f, 182.0f, tex_listbox));
    listbox->SetStringFont(font_control_);
    listbox->SetStringNormalColor(Graphics::Color(0.45f, 0.45f, 0.45f));
    listbox->SetStringSelectedColor(Graphics::Color(1.0f, 1.0f, 1.0f));
    listbox->SetStringHighlightedColor(Graphics::Color(0.0f, 0.0f, 1.0f));
    listbox->SetSelectionRectColor(Graphics::Color(0.368f, 0.368f, 0.368f));
    listbox->SetItemHeight(20);
    listbox->SetPageSize(30);
    listbox->SetPosition(Graphics::Vector(382.0f, 220.0f));
    listbox->SetBoundingRect(Graphics::Vector(), Graphics::Vector(252.0f, 182.0f));

    GetSplash().SetPos(190);

    editbox = static_cast<Graphics::SpriteEditBox*>(editbox_message_.get());
    editbox->SetBackImage(Graphics::MakeImage(0, 110, 180.0f, 25.0f, tex_editbox));
    editbox->SetDisabledImage(Graphics::MakeImage(252, 110, 180.0f, 25.0f, tex_editbox));
    editbox->SetFont(font_control_);
    editbox->SetFontColor(Graphics::Color(0.0f, 0.0f, 0.0f));
    editbox->SetPosition(Graphics::Vector(382.0f, 404.0f));
    editbox->SetBoundingRect(Graphics::Vector(), Graphics::Vector(180.0f, 25.0f));

    listctrl = static_cast<Graphics::SpriteListCtrl*>(listctrl_summary_.get());
    listctrl->SetBackImage(Graphics::MakeImage(0, 0, 250.0f, 200.0f, tex_listbox));
    listctrl->SetDisabledImage(Graphics::MakeImage(250, 0, 500.0f, 250.0f, tex_listbox));
    listctrl->SetStringFont(font_control_);
    listctrl->SetStringDefaultColor(Graphics::Color(0.0f, 0.0f, 0.0f));
    listctrl->SetStringSelectedColor(Graphics::Color(1.0f, 1.0f, 1.0f));
    listctrl->SetStringHighlightedColor(Graphics::Color(0.0f, 0.0f, 1.0f));
    listctrl->SetSelectionRectColor(Graphics::Color(0.368f, 0.368f, 0.368f));
    listctrl->SetHeaderColor(Graphics::Color(1.0f, 1.0f, 0.0f));
    listctrl->SetItemHeight(20);
    listctrl->SetPageSize(30);
    listctrl->SetRightMargin(10);
    listctrl->SetLeftMargin(10);
    listctrl->SetPosition(Graphics::Vector(195.0f, 145.0f));
    listctrl->SetBoundingRect(Graphics::Vector(), Graphics::Vector(250.0f, 200.0f));

    // The network game setup
    sprite_static = static_cast<Graphics::SpriteStatic*>(static_netgameinfo1_.get());
    sprite_static->SetPosition(Graphics::Vector(160.0f, 106.0f));
    sprite_static->SetImage(image_netgameinfo1_);

    sprite_static = static_cast<Graphics::SpriteStatic*>(static_netgameinfo2_.get());
    sprite_static->SetPosition(Graphics::Vector(160.0f, 350.0f));
    sprite_static->SetImage(image_netgameinfo2_);

    sprite_static = static_cast<Graphics::SpriteStatic*>(static_netgameheading1_.get());
    sprite_static->SetPosition(Graphics::Vector(15.0f, 106.0f));
    sprite_static->SetImage(image_netgameheading1_);

    sprite_static = static_cast<Graphics::SpriteStatic*>(static_netgameheading2_.get());
    sprite_static->SetPosition(Graphics::Vector(15.0f, 106.0f));
    sprite_static->SetImage(image_netgameheading2_);

    listctrl = static_cast<Graphics::SpriteListCtrl*>(listctrl_netgames_.get());
    listctrl->SetBackImage(Graphics::MakeImage(0, 464, 390.0f, 200.0f, tex_listctrl));
    listctrl->SetDisabledImage(Graphics::MakeImage(500, 0, 390.0f, 200.0f, tex_listctrl));
    listctrl->SetStringFont(font_control_);
    listctrl->SetStringDefaultColor(Graphics::Color(0.0f, 0.0f, 0.0f));
    listctrl->SetStringSelectedColor(Graphics::Color(1.0f, 1.0f, 1.0f));
    listctrl->SetStringHighlightedColor(Graphics::Color(0.0f, 0.0f, 1.0f));
    listctrl->SetSelectionRectColor(Graphics::Color(0.368f, 0.368f, 0.368f));
    listctrl->SetHeaderColor(Graphics::Color(1.0f, 1.0f, 0.0f));
    listctrl->SetItemHeight(20);
    listctrl->SetPageSize(30);
    listctrl->SetRightMargin(10);
    listctrl->SetLeftMargin(10);
    listctrl->SetPosition(Graphics::Vector(160.0f, 145.0f));
    listctrl->SetBoundingRect(Graphics::Vector(), Graphics::Vector(390.0f, 200.0f));

    button = static_cast<Graphics::SpriteButton*>(button_joinselectednetgame_.get());
    button->SetUpImage(Graphics::MakeImage(0, 275, 70.0f, 25.0f, tex_buttons));
    button->SetHoverImage(Graphics::MakeImage(70, 275, 70.0f, 25.0f, tex_buttons));
    button->SetDownImage(Graphics::MakeImage(140, 275, 70.0f, 25.0f, tex_buttons));
    button->SetDisabledImage(Graphics::MakeImage(210, 275, 70.0f, 25.0f, tex_buttons));
    button->SetPosition(Graphics::Vector(556.0f, 145.0f));
    button->SetBoundingRect(Graphics::Vector(), Graphics::Vector(70.0f, 25.0f));
    button->SetClickSound(buttonclick_sound_);

    button = static_cast<Graphics::SpriteButton*>(button_createnetgame_.get());
    button->SetUpImage(Graphics::MakeImage(0, 300, 70.0f, 25.0f, tex_buttons));
    button->SetHoverImage(Graphics::MakeImage(70, 300, 70.0f, 25.0f, tex_buttons));
    button->SetDownImage(Graphics::MakeImage(140, 300, 70.0f, 25.0f, tex_buttons));
    button->SetDisabledImage(Graphics::MakeImage(210, 300, 70.0f, 25.0f, tex_buttons));
    button->SetPosition(Graphics::Vector(556.0f, 185.0f));
    button->SetBoundingRect(Graphics::Vector(), Graphics::Vector(70.0f, 25.0f));
    button->SetClickSound(buttonclick_sound_);

    button = static_cast<Graphics::SpriteButton*>(button_joinspecificnetgame_.get());
    button->SetUpImage(Graphics::MakeImage(0, 275, 70.0f, 25.0f, tex_buttons));
    button->SetHoverImage(Graphics::MakeImage(70, 275, 70.0f, 25.0f, tex_buttons));
    button->SetDownImage(Graphics::MakeImage(140, 275, 70.0f, 25.0f, tex_buttons));
    button->SetDisabledImage(Graphics::MakeImage(210, 275, 70.0f, 25.0f, tex_buttons));
    button->SetPosition(Graphics::Vector(415.0f, 364.0f));
    button->SetBoundingRect(Graphics::Vector(), Graphics::Vector(70.0f, 25.0f));
    button->SetClickSound(buttonclick_sound_);

    // The net connecting screen
    sprite_static = static_cast<Graphics::SpriteStatic*>(static_ip_text_.get());
    sprite_static->SetPosition(Graphics::Vector(205.0f, 218.0f));
    sprite_static->SetFont(font_control_);
    sprite_static->SetFontColor(Graphics::Color(0.0f, 0.0f, 0.0f));
    sprite_static->SetText("Server: 192.168.0.1");
    sprite_static->SetBoundingRect(Graphics::Vector(), Graphics::Vector(230.0f, 36.0f));

    button = static_cast<Graphics::SpriteButton*>(button_cancel_.get());
    button->SetUpImage(Graphics::MakeImage(0, 325, 70.0f, 25.0f, tex_buttons));
    button->SetHoverImage(Graphics::MakeImage(70, 325, 70.0f, 25.0f, tex_buttons));
    button->SetDownImage(Graphics::MakeImage(140, 325, 70.0f, 25.0f, tex_buttons));
    button->SetDisabledImage(Graphics::MakeImage(210, 325, 70.0f, 25.0f, tex_buttons));
    button->SetPosition(Graphics::Vector(366.0f, 258.0f));
    button->SetBoundingRect(Graphics::Vector(), Graphics::Vector(70.0f, 25.0f));
    button->SetClickSound(buttonclick_sound_);

    // The joinedremote and hostinggame screens
    listbox = static_cast<Graphics::SpriteListBox*>(listbox_netplayers_.get());
    listbox->SetBackImage(Graphics::MakeImage(0, 382, 125.0f, 200.0f, tex_listbox));
    listbox->SetDisabledImage(Graphics::MakeImage(125, 382, 125.0f, 200.0f, tex_listbox));
    listbox->SetStringFont(font_control_);
    listbox->SetStringNormalColor(Graphics::Color(0.0f, 0.0f, 0.0f));
    listbox->SetStringSelectedColor(Graphics::Color(1.0f, 1.0f, 1.0f));
    listbox->SetStringHighlightedColor(Graphics::Color(0.0f, 0.0f, 1.0f));
    listbox->SetSelectionRectColor(Graphics::Color(0.368f, 0.368f, 0.368f));
    listbox->SetItemHeight(20);
    listbox->SetPageSize(30);
    listbox->SetPosition(Graphics::Vector(15.0f, 141.0f));
    listbox->SetBoundingRect(Graphics::Vector(), Graphics::Vector(125.0f, 200.0f));

    listbox = static_cast<Graphics::SpriteListBox*>(listbox_netmessages_.get());
    listbox->SetBackImage(Graphics::MakeImage(0, 464, 390.0f, 200.0f, tex_listctrl));
    listbox->SetDisabledImage(Graphics::MakeImage(500, 0, 390.0f, 200.0f, tex_listctrl));
    listbox->SetStringFont(font_control_);
    listbox->SetStringNormalColor(Graphics::Color(0.0f, 0.0f, 0.0f));
    listbox->SetStringSelectedColor(Graphics::Color(1.0f, 1.0f, 1.0f));
    listbox->SetStringHighlightedColor(Graphics::Color(0.0f, 0.0f, 1.0f));
    listbox->SetSelectionRectColor(Graphics::Color(0.368f, 0.368f, 0.368f));
    listbox->SetItemHeight(20);
    listbox->SetPageSize(30);
    listbox->SetPosition(Graphics::Vector(145.0f, 141.0f));
    listbox->SetBoundingRect(Graphics::Vector(), Graphics::Vector(390.0f, 200.0f));

    // Load other graphics.
    image_yahtzee_.reset(new Graphics::Image);
    image_yahtzee_->Create(0, 0, 640, 480, tex_yahtzee);

    player_container_.reset(new PlayerContainer);
    highscores_.reset(new Highscores("highscores.bin"));

    LOG("Adding default player");
    player_container_->push_back(PlayerPtr(new Player("Player")));
    LOG("Player container has [" << int(player_container_->size()) << "] items");

    //SetClipRect(Graphics::Vector(0.0f, 0.0f), Graphics::Vector(float(hw_config->GetWindowWidth()), float(hw_config->GetWindowHeight())));
    //cursor_normal_->SetPosition(
    //    Graphics::Vector(float(hw_config->GetWindowWidth()/2), float(hw_config->GetWindowHeight()/2)));
    POINT point;
    GetCursorPos(&point);
    ScreenToClient(GetWindow()->GetHandle(), &point);
    cursor_normal_->SetPosition(Graphics::Vector(float(point.x), float(point.y)));

    // Add a single viewport
    Graphics::ViewportManagerPtr vpm = GetViewportManager();
    viewport_id_ = vpm->Add(Graphics::ViewportPtr(new Graphics::Viewport(0, 0, hw_config->GetWindowWidth(), hw_config->GetWindowHeight())));

    // Randomize the dice
    roll_.ThrowDice();
    SyncDiceImagesToCurrentRoll();

    LOG("Setting initial state");
    SetCurrentMenuState(Core::StatePtr(new StateMenuMain(this)));

    GetSplash().SetPos(200);

    LOG("Showing main window");
    GetWindow()->Show();
    HideBlackWindow();
}

void Yahtzee::Shutdown()
{
    LOG("Freeing memory...")

    button_localgame_.reset();
    button_networkgame_.reset();
    button_main_.reset();
    button_settings_.reset();
    button_about_.reset();
    button_viewhighscores_.reset();
    button_exitapplication_.reset();
    button_back_.reset();
    button_ok_.reset();
    button_resethighscores_.reset();
    button_continue_.reset();
    button_addplayer_.reset();
    button_updateplayer_.reset();
    button_editplayer_.reset();
    button_removeplayer_.reset();
    button_clearplayers_.reset();
    editbox_player_.reset();
    listbox_players_.reset();
    button_start_.reset();
    listctrl_highscores_.reset();
    button_savehighscore_.reset();
    button_apply_.reset();
    button_test_.reset();
    button_ingamesettings_.reset();
    button_endgame_.reset();
    button_roll_.reset();
    button_continue2_.reset();
    button_send_.reset();
    checkbox_fullscreenenabled_.reset();
    slider_audiovolume_.reset();
    checkbox_audioenabled_.reset();
    editbox_highscore_.reset();
    editbox_ones_.reset();
    editbox_twos_.reset();
    editbox_threes_.reset();
    editbox_fours_.reset();
    editbox_fives_.reset();
    editbox_sixes_.reset();
    editbox_subtotal_.reset();
    editbox_bonus_.reset();
    editbox_uppertotal_.reset();
    editbox_threeofakind_.reset();
    editbox_fourofakind_.reset();
    editbox_fullhouse_.reset();
    editbox_smallstraight_.reset();
    editbox_largestraight_.reset();
    editbox_yahtzee_.reset();
    editbox_chance_.reset();
    editbox_lowertotal_.reset();
    editbox_grandtotal_.reset();
    static_dice_0_.reset();
    static_dice_1_.reset();
    static_dice_2_.reset();
    static_dice_3_.reset();
    static_dice_4_.reset();
    dice_0_held_.reset();
    dice_1_held_.reset();
    dice_2_held_.reset();
    dice_3_held_.reset();
    dice_4_held_.reset();
    listctrl_summary_.reset();
    listctrl_ingameplayers_.reset();
    listbox_messages_.reset();
    editbox_message_.reset();
    listctrl_netgames_.reset();
    button_joinselectednetgame_.reset();
    static_netgameinfo1_.reset();
    static_netgameinfo2_.reset();
    button_createnetgame_.reset();
    button_joinspecificnetgame_.reset();
    button_cancel_.reset();
    static_ip_text_.reset();
    listbox_netplayers_.reset();
    listbox_netmessages_.reset();
    cursor_normal_.reset();
    cursor_placescore_.reset();
    image_mainbackground_.reset();
    image_highscoresbackground_.reset();
    image_gameoverbackground_.reset();
    image_aboutbackground_.reset();
    image_settingsbackground_.reset();
    image_playersetupbackground_.reset();
    image_enterscorebackground_.reset();
    image_gameboard_.reset();
    image_yahtzee_.reset();
    image_netgameendbackground_.reset();
    image_netgamesetupbackground_.reset();
    dice1fromstilla_imagelist_.reset();
    dice1fromstillb_imagelist_.reset();
    dice1fromstillc_imagelist_.reset();
    dice1tostilla_imagelist_.reset();
    dice1tostillb_imagelist_.reset();
    dice1tostillc_imagelist_.reset();
    dice2fromstilla_imagelist_.reset();
    dice2fromstillb_imagelist_.reset();
    dice2fromstillc_imagelist_.reset();
    dice2tostilla_imagelist_.reset();
    dice2tostillb_imagelist_.reset();
    dice2tostillc_imagelist_.reset();
    dice3fromstilla_imagelist_.reset();
    dice3fromstillb_imagelist_.reset();
    dice3fromstillc_imagelist_.reset();
    dice3tostilla_imagelist_.reset();
    dice3tostillb_imagelist_.reset();
    dice3tostillc_imagelist_.reset();
    dice4fromstilla_imagelist_.reset();
    dice4fromstillb_imagelist_.reset();
    dice4fromstillc_imagelist_.reset();
    dice4tostilla_imagelist_.reset();
    dice4tostillb_imagelist_.reset();
    dice4tostillc_imagelist_.reset();
    dice5fromstilla_imagelist_.reset();
    dice5fromstillb_imagelist_.reset();
    dice5fromstillc_imagelist_.reset();
    dice5tostilla_imagelist_.reset();
    dice5tostillb_imagelist_.reset();
    dice5tostillc_imagelist_.reset();
    dice6fromstilla_imagelist_.reset();
    dice6fromstillb_imagelist_.reset();
    dice6fromstillc_imagelist_.reset();
    dice6tostilla_imagelist_.reset();
    dice6tostillb_imagelist_.reset();
    dice6tostillc_imagelist_.reset();
    randomspina0_imagelist_.reset();
    randomspina1_imagelist_.reset();
    randomspinb0_imagelist_.reset();
    randomspinb1_imagelist_.reset();
    randomspinc0_imagelist_.reset();
    randomspinc1_imagelist_.reset();
    font_control_.reset();

    shakedice_sound0_.reset();
    shakedice_sound1_.reset();
    shakedice_sound2_.reset();
    shakedice_sound3_.reset();
    buttonclick_sound_.reset();
    checkboxclick_sound_.reset();
    editboxclick_sound_.reset();
    diceclick_sound_.reset();
    applause_sound0_.reset();
    applause_sound1_.reset();
    yahtzee_sound0_.reset();
    yahtzee_sound1_.reset();
    soundtest_sound_.reset();
    messagereceive_sound_.reset();

    Engine::Shutdown();
}

void Yahtzee::HandleSocketException(Network::SocketException& e)
{
    GetConnectionList()->clear();
    TransitionGameState(Core::StatePtr());
    TransitionMenuState(Core::StatePtr(new StateMenuNetGameEnded(this, -1, e.what())));
}

void Yahtzee::StartLocalGame()
{
    LOG("Starting a new local game");

    LOG("Resetting score cards");
    PlayerContainer::iterator player;
    for(player = player_container_->begin(); player != player_container_->end(); ++player)
    {
        (*player)->GetScorecard().Reset();
    }

    // Stick all the players in the in game list control
    Graphics::SpriteListCtrl* list_ctrl = static_cast<Graphics::SpriteListCtrl*>(listctrl_ingameplayers_.get());
    list_ctrl->DeleteAllColumns();
    list_ctrl->AddColumn("Player", 150);
    list_ctrl->AddColumn("Score", 100);
    list_ctrl->DeleteAllRows();
    for(player = player_container_->begin(); player != player_container_->end(); ++player)
    {
        int row = list_ctrl->AddRow((*player)->GetName());
        list_ctrl->SetString(row, 1, "0");
    }

    Graphics::SpriteListBox* list_box = static_cast<Graphics::SpriteListBox*>(listbox_messages_.get());
    list_box->ResetContent();

    current_player_ = player_container_->front();
    local_player_.reset();          // TODO: Is this correct for a local game?
    current_roll_   =  0;
    roll_.ClearHolds();
}

void Yahtzee::StartNetworkGame()
{
    LOG("Starting a new network game");

    LOG("Resetting score cards");
    PlayerContainer::iterator player;
    for(player = player_container_->begin(); player != player_container_->end(); ++player)
    {
        (*player)->GetScorecard().Reset();
    }

    // Stick all the players in the in game list control
    Graphics::SpriteListCtrl* list_ctrl = static_cast<Graphics::SpriteListCtrl*>(listctrl_ingameplayers_.get());
    list_ctrl->DeleteAllColumns();
    list_ctrl->AddColumn("Player", 150);
    list_ctrl->AddColumn("Score", 100);
    list_ctrl->DeleteAllRows();
    for(player = player_container_->begin(); player != player_container_->end(); ++player)
    {
        int row = list_ctrl->AddRow((*player)->GetName());
        list_ctrl->SetString(row, 1, "0");
    }

    Graphics::SpriteListBox* list_box = static_cast<Graphics::SpriteListBox*>(listbox_messages_.get());
    list_box->ResetContent();

    current_player_ = player_container_->front();
    current_roll_   =  0;
    roll_.ClearHolds();
}

void Yahtzee::EndNetworkGame(YahtzeeNetworkHandler* net_handler)
{
    TransitionGameState(Core::StatePtr());

    if(IsHosting())
        net_handler->BroadcastNetworkEventEndGame(GetLocalPlayerIndex(), local_player_->GetName() + " aborted the game");
    else
        net_handler->BroadcastNetworkEventPlayerLeft(GetLocalPlayerIndex());

    GetConnectionList()->clear();
    TransitionMenuState(Core::StatePtr(new StateMenuNetGameSetup(this, local_player_->GetName())));
}

bool Yahtzee::IsFinalRoll() const
{
    return (current_roll_ == (MAX_ROLLS-1));
}

void Yahtzee::StartNewRoll()
{
    ++current_roll_;
}

void Yahtzee::StartNewTurn()
{
    PlayerContainer::iterator player_itor;
    for(player_itor = player_container_->begin(); player_itor != player_container_->end(); ++player_itor)
    {
        if(current_player_ == *player_itor)
        {
            break;
        }
    }

    ++player_itor;
    current_player_ = (player_itor == player_container_->end() ? player_container_->front() : *player_itor);

    current_roll_ = 0;
    is_bonus_yahtzee_ = false;

    roll_.ClearHolds();
}

void Yahtzee::ThrowAYahtzee()
{
    roll_.ThrowAYahtzee();
}

void Yahtzee::ThrowFromNet(int dice_values[5])
{
    roll_.SetDiceValues(dice_values);
}

void Yahtzee::ThrowDice()
{
    roll_.ThrowDice();
}

void Yahtzee::SyncDiceImagesToCurrentRoll()
{
    Graphics::SpriteStatic* sprite_static = static_cast<Graphics::SpriteStatic*>(static_dice_0_.get());
    sprite_static->SetImageList(ChooseDiceToImageList(roll_.GetValue(0)));

    sprite_static = static_cast<Graphics::SpriteStatic*>(static_dice_1_.get());
    sprite_static->SetImageList(ChooseDiceToImageList(roll_.GetValue(1)));

    sprite_static = static_cast<Graphics::SpriteStatic*>(static_dice_2_.get());
    sprite_static->SetImageList(ChooseDiceToImageList(roll_.GetValue(2)));

    sprite_static = static_cast<Graphics::SpriteStatic*>(static_dice_3_.get());
    sprite_static->SetImageList(ChooseDiceToImageList(roll_.GetValue(3)));

    sprite_static = static_cast<Graphics::SpriteStatic*>(static_dice_4_.get());
    sprite_static->SetImageList(ChooseDiceToImageList(roll_.GetValue(4)));
}

Graphics::ImageListPtr Yahtzee::ChooseDiceFromImageList(int dice_value) const
{
    int variation = rand()%3;
    Graphics::ImageListPtr il;

    switch(dice_value)
    {
    case 1:
        switch(variation)
        {
        case 0: il = dice1fromstilla_imagelist_; break;
        case 1: il = dice1fromstillb_imagelist_; break;
        case 2: il = dice1fromstillc_imagelist_; break;
        }
        break;
    case 2:
        switch(variation)
        {
        case 0: il = dice2fromstilla_imagelist_; break;
        case 1: il = dice2fromstillb_imagelist_; break;
        case 2: il = dice2fromstillc_imagelist_; break;
        }
        break;
    case 3:
        switch(variation)
        {
        case 0: il = dice3fromstilla_imagelist_; break;
        case 1: il = dice3fromstillb_imagelist_; break;
        case 2: il = dice3fromstillc_imagelist_; break;
        }
        break;
    case 4:
        switch(variation)
        {
        case 0: il = dice4fromstilla_imagelist_; break;
        case 1: il = dice4fromstillb_imagelist_; break;
        case 2: il = dice4fromstillc_imagelist_; break;
        }
        break;
    case 5:
        switch(variation)
        {
        case 0: il = dice5fromstilla_imagelist_; break;
        case 1: il = dice5fromstillb_imagelist_; break;
        case 2: il = dice5fromstillc_imagelist_; break;
        }
        break;
    case 6:
    default:
        switch(variation)
        {
        case 0: il = dice6fromstilla_imagelist_; break;
        case 1: il = dice6fromstillb_imagelist_; break;
        case 2: il = dice6fromstillc_imagelist_; break;
        }
        break;
    }

    return il;
}

Graphics::ImageListPtr Yahtzee::ChooseDiceToImageList(int dice_value) const
{
    int variation = rand()%3;
    Graphics::ImageListPtr il;

    switch(dice_value)
    {
    case 1:
        switch(variation)
        {
        case 0: il = dice1tostilla_imagelist_; break;
        case 1: il = dice1tostillb_imagelist_; break;
        case 2: il = dice1tostillc_imagelist_; break;
        }
        break;
    case 2:
        switch(variation)
        {
        case 0: il = dice2tostilla_imagelist_; break;
        case 1: il = dice2tostillb_imagelist_; break;
        case 2: il = dice2tostillc_imagelist_; break;
        }
        break;
    case 3:
        switch(variation)
        {
        case 0: il = dice3tostilla_imagelist_; break;
        case 1: il = dice3tostillb_imagelist_; break;
        case 2: il = dice3tostillc_imagelist_; break;
        }
        break;
    case 4:
        switch(variation)
        {
        case 0: il = dice4tostilla_imagelist_; break;
        case 1: il = dice4tostillb_imagelist_; break;
        case 2: il = dice4tostillc_imagelist_; break;
        }
        break;
    case 5:
        switch(variation)
        {
        case 0: il = dice5tostilla_imagelist_; break;
        case 1: il = dice5tostillb_imagelist_; break;
        case 2: il = dice5tostillc_imagelist_; break;
        }
        break;
    case 6:
    default:
        switch(variation)
        {
        case 0: il = dice6tostilla_imagelist_; break;
        case 1: il = dice6tostillb_imagelist_; break;
        case 2: il = dice6tostillc_imagelist_; break;
        }
        break;
    }

    return il;
}

Graphics::ImageListPtr Yahtzee::ChooseDiceSpinImageList(Graphics::ImageListPtr current_imagelist) const
{
    if(current_imagelist == dice1fromstilla_imagelist_ || current_imagelist == dice2fromstilla_imagelist_ ||
       current_imagelist == dice3fromstilla_imagelist_ || current_imagelist == dice4fromstilla_imagelist_ ||
       current_imagelist == dice5fromstilla_imagelist_ || current_imagelist == dice6fromstilla_imagelist_)
    {
        return (rand()%2 == 0 ? GetRandomSpinA0ImageList() : GetRandomSpinA1ImageList());
    }
    if(current_imagelist == dice1fromstillb_imagelist_ || current_imagelist == dice2fromstillb_imagelist_ ||
       current_imagelist == dice3fromstillb_imagelist_ || current_imagelist == dice4fromstillb_imagelist_ ||
       current_imagelist == dice5fromstillb_imagelist_ || current_imagelist == dice6fromstillb_imagelist_)
    {
        return (rand()%2 == 0 ? GetRandomSpinB0ImageList() : GetRandomSpinB1ImageList());
    }
    if(current_imagelist == dice1fromstillc_imagelist_ || current_imagelist == dice2fromstillc_imagelist_ ||
       current_imagelist == dice3fromstillc_imagelist_ || current_imagelist == dice4fromstillc_imagelist_ ||
       current_imagelist == dice5fromstillc_imagelist_ || current_imagelist == dice6fromstillc_imagelist_)
    {
        return (rand()%2 == 0 ? GetRandomSpinC0ImageList() : GetRandomSpinC1ImageList());
    }

    return Graphics::ImageListPtr();
}

Graphics::ImageListPtr Yahtzee::SyncDiceToImageListToCurrentRoll(int dice_value, Graphics::ImageListPtr current_imagelist) const
{
    if(current_imagelist == randomspina0_imagelist_ || current_imagelist == randomspina1_imagelist_)
    {
        switch(dice_value)
        {
        case 1: return dice1tostilla_imagelist_;
        case 2: return dice2tostilla_imagelist_;
        case 3: return dice3tostilla_imagelist_;
        case 4: return dice4tostilla_imagelist_;
        case 5: return dice5tostilla_imagelist_;
        case 6: return dice6tostilla_imagelist_;
        }
    }
    if(current_imagelist == randomspinb0_imagelist_ || current_imagelist == randomspinb1_imagelist_)
    {
        switch(dice_value)
        {
        case 1: return dice1tostillb_imagelist_;
        case 2: return dice2tostillb_imagelist_;
        case 3: return dice3tostillb_imagelist_;
        case 4: return dice4tostillb_imagelist_;
        case 5: return dice5tostillb_imagelist_;
        case 6: return dice6tostillb_imagelist_;
        }
    }
    if(current_imagelist == randomspinc0_imagelist_ || current_imagelist == randomspinc1_imagelist_)
    {
        switch(dice_value)
        {
        case 1: return dice1tostillc_imagelist_;
        case 2: return dice2tostillc_imagelist_;
        case 3: return dice3tostillc_imagelist_;
        case 4: return dice4tostillc_imagelist_;
        case 5: return dice5tostillc_imagelist_;
        case 6: return dice6tostillc_imagelist_;
        }
    }

    return Graphics::ImageListPtr();
}

bool Yahtzee::IsGameOver() const
{
    PlayerContainer::iterator player;
    for(player = player_container_->begin(); player != player_container_->end(); ++player)
    {
        if(!(*player)->GetScorecard().IsEverythingComplete())
        {
            return false;
        }
    }
    return true;
}

PlayerPtr Yahtzee::GetFirstHighscoringPlayer() const
{
    PlayerContainer::iterator player;
    for(player = player_container_->begin(); player != player_container_->end(); ++player)
    {
        if(highscores_->IsNewHighscore((*player)->GetScorecard().GetGrandTotal_i()))
        {
            if((*player)->IsRemote())
            {
                highscores_->Insert((*player)->GetName(), (*player)->GetScorecard().GetGrandTotal_i());
            }
            else
            {
                return *player;
            }
        }
    }
    return PlayerPtr();
}

PlayerPtr Yahtzee::GetNextHighscoringPlayer(PlayerPtr current_player) const
{
    PlayerContainer::iterator start_itor;
    for(start_itor = player_container_->begin(); start_itor != player_container_->end(); ++start_itor)
    {
        if(current_player == *start_itor)
        {
            break;
        }
    }
    if(start_itor == player_container_->end())
    {
        return PlayerPtr();
    }

    PlayerContainer::iterator next_itor = start_itor;
    ++next_itor;
    for( ; next_itor != player_container_->end(); ++next_itor)
    {
        if(highscores_->IsNewHighscore((*next_itor)->GetScorecard().GetGrandTotal_i()))
        {
            if((*next_itor)->IsRemote())
            {
                highscores_->Insert((*next_itor)->GetName(), (*next_itor)->GetScorecard().GetGrandTotal_i());
            }
            else
            {
                return *next_itor;
            }
        }
    }
    return PlayerPtr();
}

void Yahtzee::SetLocalPlayer(int index)
{
    if(index < 0 || index > int(player_container_->size()))
    {
        return;
    }

    PlayerContainer::iterator itor = player_container_->begin();
    std::advance(itor, index);
    local_player_ = *itor;
}

int Yahtzee::GetCurrentPlayerIndex() const
{
    int i = 0;
    PlayerContainer::iterator itor;
    for(itor = player_container_->begin(); itor != player_container_->end(); ++itor, ++i)
    {
        if(*itor == current_player_) return i;
    }
    return -1;
}

int Yahtzee::GetLocalPlayerIndex() const
{
    int i = 0;
    PlayerContainer::iterator itor;
    for(itor = player_container_->begin(); itor != player_container_->end(); ++itor, ++i)
    {
        if(*itor == local_player_) return i;
    }
    return -1;
}

void Yahtzee::AddMessage(const std::string& message)
{
    if(message.empty())
    {
        return;
    }

    messagereceive_sound_->Play();

    Graphics::SpriteListBox* listbox = static_cast<Graphics::SpriteListBox*>(listbox_messages_.get());
    listbox->AddWrappedString(message, Graphics::Color(0.9f, 0.4f, 0.0f), Graphics::Color(0.368f, 0.368f, 0.368f));
    listbox->ScrollToBottom();
}

void Yahtzee::SetPlayerLocation(int which_player, bool present, const std::string& location)
{
    if(which_player < 0 || which_player >= int(player_container_->size()))
    {
        return;
    }

    PlayerContainer::iterator player = player_container_->begin();
    std::advance(player, which_player);

    std::ostringstream oss;
    if(present)
    {
        oss << (*player)->GetName();
    }
    else
    {
        oss << "(" << location << ") " << (*player)->GetName();
    }

    Graphics::SpriteListCtrl* list_ctrl = static_cast<Graphics::SpriteListCtrl*>(listctrl_ingameplayers_.get());
    list_ctrl->SetString(which_player, 0, oss.str());
}

void Yahtzee::OnSpriteGetFocus(Graphics::SpritePtr sprite)
{
    Engine::OnSpriteGetFocus(sprite);

    if (!IsSettingsPageActive() && !IsYahtzeePageActive())
        is_player_typing_ = (sprite == GetEditBoxMessage());
}

void Yahtzee::OnSpriteLoseFocus(Graphics::SpritePtr sprite)
{
    Engine::OnSpriteLoseFocus(sprite);

    if (!IsSettingsPageActive() && !IsYahtzeePageActive())
        is_player_typing_ = !(sprite == GetEditBoxMessage());
}

bool Yahtzee::IsSettingsPageActive() const
{
    if (GetCurrentMenuState())
        return dynamic_cast<StateMenuInGameSettings*>(GetCurrentMenuState().get()) != nullptr;
    return false;
}

bool Yahtzee::IsYahtzeePageActive() const
{
    if (GetCurrentGameState())
        return dynamic_cast<StateGameYahtzee*>(GetCurrentGameState().get()) != nullptr;
    return false;
}

Audio::SoundPtr Yahtzee::GetShakeDiceSound() const
{
    switch(rand()%4)
    {
    case 0: return shakedice_sound0_;
    case 1: return shakedice_sound1_;
    case 2: return shakedice_sound2_;
    }
    return shakedice_sound3_;
}

Audio::SoundPtr Yahtzee::GetApplauseSound() const
{
    return (rand()%2) == 0 ? applause_sound0_ : applause_sound1_;
}

Audio::SoundPtr Yahtzee::GetYahtzeeSound() const
{
    return (rand()%2) == 0 ? yahtzee_sound0_ : yahtzee_sound1_;
}

void Yahtzee::SetVolume(int volume)
{
    if(shakedice_sound0_) shakedice_sound0_->SetVolume(volume);
    if(shakedice_sound1_) shakedice_sound1_->SetVolume(volume);
    if(shakedice_sound2_) shakedice_sound2_->SetVolume(volume);
    if(shakedice_sound3_) shakedice_sound3_->SetVolume(volume);
    if(buttonclick_sound_) buttonclick_sound_->SetVolume(volume);
    if(checkboxclick_sound_) checkboxclick_sound_->SetVolume(volume);
    if(editboxclick_sound_) editboxclick_sound_->SetVolume(volume);
    if(diceclick_sound_) diceclick_sound_->SetVolume(volume);
    if(applause_sound0_) applause_sound0_->SetVolume(volume);
    if(applause_sound1_) applause_sound1_->SetVolume(volume);
    if(yahtzee_sound0_) yahtzee_sound0_->SetVolume(volume);
    if(yahtzee_sound1_) yahtzee_sound1_->SetVolume(volume);
    if(soundtest_sound_) soundtest_sound_->SetVolume(volume);
    if(messagereceive_sound_) messagereceive_sound_->SetVolume(volume);
}
