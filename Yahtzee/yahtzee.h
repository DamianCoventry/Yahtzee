#ifndef INCLUDED_YAHTZEE
#define INCLUDED_YAHTZEE

#include "../Core/engine.h"
#include "../Graphics/image.h"
#include "../Graphics/font.h"
#include "../Audio/sound.h"
#include "player.h"
#include "highscores.h"
#include "roll.h"
#include "../Utility/logcontext.h"

class YahtzeeNetworkHandler;

class Yahtzee :
    public Core::Engine
{
public:
    Yahtzee(HINSTANCE instance, Graphics::SplashScreen& splash);
    ~Yahtzee();

    void Initialise();
    void Shutdown();
    void HandleSocketException(Network::SocketException& e);

    void StartLocalGame();
    void StartNetworkGame();
    void EndNetworkGame(YahtzeeNetworkHandler* net_handler);

    Graphics::SpritePtr GetButtonMain() const               { return button_main_; }
    Graphics::SpritePtr GetButtonViewHighscores() const     { return button_viewhighscores_; }
    Graphics::SpritePtr GetButtonSettings() const           { return button_settings_; }
    Graphics::SpritePtr GetButtonAbout() const              { return button_about_; }
    Graphics::SpritePtr GetButtonExitApplication() const    { return button_exitapplication_; }
    Graphics::SpritePtr GetButtonBack() const               { return button_back_; }
    Graphics::SpritePtr GetButtonLocalGame() const          { return button_localgame_; }
    Graphics::SpritePtr GetButtonNetworkGame() const        { return button_networkgame_; }
    Graphics::SpritePtr GetButtonOK() const                 { return button_ok_; }
    Graphics::SpritePtr GetButtonResetHighscores() const    { return button_resethighscores_; }
    Graphics::SpritePtr GetButtonContinue() const           { return button_continue_; }
    Graphics::SpritePtr GetButtonAddPlayer() const          { return button_addplayer_; }
    Graphics::SpritePtr GetButtonUpdatePlayer() const       { return button_updateplayer_; }
    Graphics::SpritePtr GetButtonEditPlayer() const         { return button_editplayer_; }
    Graphics::SpritePtr GetButtonRemovePlayer() const       { return button_removeplayer_; }
    Graphics::SpritePtr GetButtonClearPlayers() const       { return button_clearplayers_; }
    Graphics::SpritePtr GetEditBoxPlayer() const            { return editbox_player_; }
    Graphics::SpritePtr GetListBoxPlayers() const           { return listbox_players_; }
    Graphics::SpritePtr GetButtonStart() const              { return button_start_; }
    Graphics::SpritePtr GetListCtrlHighscores() const       { return listctrl_highscores_; }
    Graphics::SpritePtr GetButtonSaveHighscore() const      { return button_savehighscore_; }
    Graphics::SpritePtr GetButtonApply() const              { return button_apply_; }
    Graphics::SpritePtr GetButtonTest() const               { return button_test_; }
    Graphics::SpritePtr GetButtonEndGame() const            { return button_endgame_; }
    Graphics::SpritePtr GetButtonInGameSettings() const     { return button_ingamesettings_; }
    Graphics::SpritePtr GetButtonRoll() const               { return button_roll_; }
    Graphics::SpritePtr GetButtonContinue2() const          { return button_continue2_; }
    Graphics::SpritePtr GetButtonSend() const               { return button_send_; }
    Graphics::SpritePtr GetCheckBoxFullscreenEnabled() const{ return checkbox_fullscreenenabled_; }
    Graphics::SpritePtr GetCheckBoxAudioEnabled() const     { return checkbox_audioenabled_; }
    Graphics::SpritePtr GetSliderAudioVolume() const        { return slider_audiovolume_; }
    Graphics::SpritePtr GetEditBoxHighscore() const         { return editbox_highscore_; }
    Graphics::SpritePtr GetEditBoxOnes() const              { return editbox_ones_; }
    Graphics::SpritePtr GetEditBoxTwos() const              { return editbox_twos_; }
    Graphics::SpritePtr GetEditBoxThrees() const            { return editbox_threes_; }
    Graphics::SpritePtr GetEditBoxFours() const             { return editbox_fours_; }
    Graphics::SpritePtr GetEditBoxFives() const             { return editbox_fives_; }
    Graphics::SpritePtr GetEditBoxSixes() const             { return editbox_sixes_; }
    Graphics::SpritePtr GetEditBoxSubTotal() const          { return editbox_subtotal_; }
    Graphics::SpritePtr GetEditBoxBonus() const             { return editbox_bonus_; }
    Graphics::SpritePtr GetEditBoxUpperTotal() const        { return editbox_uppertotal_; }
    Graphics::SpritePtr GetEditBoxThreeOfAKind() const      { return editbox_threeofakind_; }
    Graphics::SpritePtr GetEditBoxFourOfAKind() const       { return editbox_fourofakind_; }
    Graphics::SpritePtr GetEditBoxFullHouse() const         { return editbox_fullhouse_; }
    Graphics::SpritePtr GetEditBoxSmallStraight() const     { return editbox_smallstraight_; }
    Graphics::SpritePtr GetEditBoxLargeStraight() const     { return editbox_largestraight_; }
    Graphics::SpritePtr GetEditBoxYahtzee() const           { return editbox_yahtzee_; }
    Graphics::SpritePtr GetEditBoxChance() const            { return editbox_chance_; }
    Graphics::SpritePtr GetEditBoxLowerTotal() const        { return editbox_lowertotal_; }
    Graphics::SpritePtr GetEditBoxGrandTotal() const        { return editbox_grandtotal_; }
    Graphics::SpritePtr GetStaticDice0() const              { return static_dice_0_; }
    Graphics::SpritePtr GetStaticDice1() const              { return static_dice_1_; }
    Graphics::SpritePtr GetStaticDice2() const              { return static_dice_2_; }
    Graphics::SpritePtr GetStaticDice3() const              { return static_dice_3_; }
    Graphics::SpritePtr GetStaticDice4() const              { return static_dice_4_; }
    Graphics::SpritePtr GetDice0Held() const                { return dice_0_held_; }
    Graphics::SpritePtr GetDice1Held() const                { return dice_1_held_; }
    Graphics::SpritePtr GetDice2Held() const                { return dice_2_held_; }
    Graphics::SpritePtr GetDice3Held() const                { return dice_3_held_; }
    Graphics::SpritePtr GetDice4Held() const                { return dice_4_held_; }
    Graphics::SpritePtr GetListCtrlSummary() const          { return listctrl_summary_; }
    Graphics::SpritePtr GetListCtrlInGamePlayers() const    { return listctrl_ingameplayers_; }
    Graphics::SpritePtr GetListBoxMessages() const          { return listbox_messages_; }
    Graphics::SpritePtr GetEditBoxMessage() const           { return editbox_message_; }
    Graphics::SpritePtr GetListCtrlNetGames() const         { return listctrl_netgames_; }
    Graphics::SpritePtr GetStaticNetGameInfo1() const       { return static_netgameinfo1_; }
    Graphics::SpritePtr GetStaticNetGameInfo2() const       { return static_netgameinfo2_; }
    Graphics::SpritePtr GetStaticNetGameHeading1() const    { return static_netgameheading1_; }
    Graphics::SpritePtr GetStaticNetGameHeading2() const    { return static_netgameheading2_; }
    Graphics::SpritePtr GetButtonJoinSelectedNetGame() const{ return button_joinselectednetgame_; }
    Graphics::SpritePtr GetButtonCreateNetGame() const      { return button_createnetgame_; }
    Graphics::SpritePtr GetButtonJoinSpecificNetGame() const{ return button_joinspecificnetgame_; }
    Graphics::SpritePtr GetButtonCancel() const             { return button_cancel_; }
    Graphics::SpritePtr GetStaticIpText() const             { return static_ip_text_; }
    Graphics::SpritePtr GetListBoxNetPlayers() const        { return listbox_netplayers_; }
    Graphics::SpritePtr GetListBoxNetMessages() const       { return listbox_netmessages_; }

    Graphics::SpriteCursorPtr GetCursorNormal() const       { return cursor_normal_; }
    Graphics::SpriteCursorPtr GetCursorPlaceScore() const   { return cursor_placescore_; }

    Graphics::ImagePtr GetImageMainBackground() const       { return image_mainbackground_; }
    Graphics::ImagePtr GetImageHighscoresBackground() const { return image_highscoresbackground_; }
    Graphics::ImagePtr GetImageGameOverBackground() const   { return image_gameoverbackground_; }
    Graphics::ImagePtr GetImageAboutBackground() const      { return image_aboutbackground_; }
    Graphics::ImagePtr GetImageSettingsBackground() const   { return image_settingsbackground_; }
    Graphics::ImagePtr GetImagePlayerSetupBackground() const{ return image_playersetupbackground_; }
    Graphics::ImagePtr GetImageEnterScoreBackground() const { return image_enterscorebackground_; }
    Graphics::ImagePtr GetImageGameBoard() const            { return image_gameboard_; }
    Graphics::ImagePtr GetImageYahtzee() const              { return image_yahtzee_; }
    Graphics::ImagePtr GetImageNetGameEndBackground() const { return image_netgameendbackground_; }
    Graphics::ImagePtr GetImageNetGameSetupBackground() const       { return image_netgamesetupbackground_; }
    Graphics::ImagePtr GetImageNetGameConnectingBackground() const  { return image_netgameconnectingbackground_; }
    Graphics::ImagePtr GetImageNetGameBackground() const            { return image_netgamebackground_; }

    Graphics::ImageListPtr GetDice1FromStillAImageList() const      { return dice1fromstilla_imagelist_; }
    Graphics::ImageListPtr GetDice1FromStillBImageList() const      { return dice1fromstillb_imagelist_; }
    Graphics::ImageListPtr GetDice1FromStillCImageList() const      { return dice1fromstillc_imagelist_; }
    Graphics::ImageListPtr GetDice1ToStillAImageList() const        { return dice1tostilla_imagelist_; }
    Graphics::ImageListPtr GetDice1ToStillBImageList() const        { return dice1tostillb_imagelist_; }
    Graphics::ImageListPtr GetDice1ToStillCImageList() const        { return dice1tostillc_imagelist_; }
    Graphics::ImageListPtr GetDice2FromStillAImageList() const      { return dice2fromstilla_imagelist_; }
    Graphics::ImageListPtr GetDice2FromStillBImageList() const      { return dice2fromstillb_imagelist_; }
    Graphics::ImageListPtr GetDice2FromStillCImageList() const      { return dice2fromstillc_imagelist_; }
    Graphics::ImageListPtr GetDice2ToStillAImageList() const        { return dice2tostilla_imagelist_; }
    Graphics::ImageListPtr GetDice2ToStillBImageList() const        { return dice2tostillb_imagelist_; }
    Graphics::ImageListPtr GetDice2ToStillCImageList() const        { return dice2tostillc_imagelist_; }
    Graphics::ImageListPtr GetDice3FromStillAImageList() const      { return dice3fromstilla_imagelist_; }
    Graphics::ImageListPtr GetDice3FromStillBImageList() const      { return dice3fromstillb_imagelist_; }
    Graphics::ImageListPtr GetDice3FromStillCImageList() const      { return dice3fromstillc_imagelist_; }
    Graphics::ImageListPtr GetDice3ToStillAImageList() const        { return dice3tostilla_imagelist_; }
    Graphics::ImageListPtr GetDice3ToStillBImageList() const        { return dice3tostillb_imagelist_; }
    Graphics::ImageListPtr GetDice3ToStillCImageList() const        { return dice3tostillc_imagelist_; }
    Graphics::ImageListPtr GetDice4FromStillAImageList() const      { return dice4fromstilla_imagelist_; }
    Graphics::ImageListPtr GetDice4FromStillBImageList() const      { return dice4fromstillb_imagelist_; }
    Graphics::ImageListPtr GetDice4FromStillCImageList() const      { return dice4fromstillc_imagelist_; }
    Graphics::ImageListPtr GetDice4ToStillAImageList() const        { return dice4tostilla_imagelist_; }
    Graphics::ImageListPtr GetDice4ToStillBImageList() const        { return dice4tostillb_imagelist_; }
    Graphics::ImageListPtr GetDice4ToStillCImageList() const        { return dice4tostillc_imagelist_; }
    Graphics::ImageListPtr GetDice5FromStillAImageList() const      { return dice5fromstilla_imagelist_; }
    Graphics::ImageListPtr GetDice5FromStillBImageList() const      { return dice5fromstillb_imagelist_; }
    Graphics::ImageListPtr GetDice5FromStillCImageList() const      { return dice5fromstillc_imagelist_; }
    Graphics::ImageListPtr GetDice5ToStillAImageList() const        { return dice5tostilla_imagelist_; }
    Graphics::ImageListPtr GetDice5ToStillBImageList() const        { return dice5tostillb_imagelist_; }
    Graphics::ImageListPtr GetDice5ToStillCImageList() const        { return dice5tostillc_imagelist_; }
    Graphics::ImageListPtr GetDice6FromStillAImageList() const      { return dice6fromstilla_imagelist_; }
    Graphics::ImageListPtr GetDice6FromStillBImageList() const      { return dice6fromstillb_imagelist_; }
    Graphics::ImageListPtr GetDice6FromStillCImageList() const      { return dice6fromstillc_imagelist_; }
    Graphics::ImageListPtr GetDice6ToStillAImageList() const        { return dice6tostilla_imagelist_; }
    Graphics::ImageListPtr GetDice6ToStillBImageList() const        { return dice6tostillb_imagelist_; }
    Graphics::ImageListPtr GetDice6ToStillCImageList() const        { return dice6tostillc_imagelist_; }
    Graphics::ImageListPtr GetRandomSpinA0ImageList() const         { return randomspina0_imagelist_; }
    Graphics::ImageListPtr GetRandomSpinA1ImageList() const         { return randomspina1_imagelist_; }
    Graphics::ImageListPtr GetRandomSpinB0ImageList() const         { return randomspinb0_imagelist_; }
    Graphics::ImageListPtr GetRandomSpinB1ImageList() const         { return randomspinb1_imagelist_; }
    Graphics::ImageListPtr GetRandomSpinC0ImageList() const         { return randomspinc0_imagelist_; }
    Graphics::ImageListPtr GetRandomSpinC1ImageList() const         { return randomspinc1_imagelist_; }

    Graphics::FontPtr   GetFontControl() const              { return font_control_; }

    Audio::SoundPtr GetShakeDiceSound() const;
    Audio::SoundPtr GetButtonClickSound() const             { return buttonclick_sound_; }
    Audio::SoundPtr GetCheckBoxClickSound() const           { return checkboxclick_sound_; }
    Audio::SoundPtr GetEditBoxClickSound() const            { return editboxclick_sound_; }
    Audio::SoundPtr GetDiceClickSound() const               { return diceclick_sound_; }
    Audio::SoundPtr GetApplauseSound() const;
    Audio::SoundPtr GetYahtzeeSound() const;
    Audio::SoundPtr GetMessageReceiveSound() const          { return messagereceive_sound_; }

    PlayerContainerPtr  GetPlayerContainer() const          { return player_container_; }
    PlayerPtr           GetCurrentPlayer() const            { return current_player_; }
    PlayerPtr           GetLocalPlayer() const              { return local_player_; }
    void                SetLocalPlayer(int index);
    int                 GetCurrentPlayerIndex() const;
    int                 GetLocalPlayerIndex() const;
    HighscoresPtr       GetHighscores() const               { return highscores_; }
    Roll&               GetCurrentRoll() const              { return (Roll&)roll_; }
    PlayerPtr           GetFirstHighscoringPlayer() const;
    PlayerPtr           GetNextHighscoringPlayer(PlayerPtr current_player) const;

    int GetRollCount() const    { return current_roll_; }
    int GetViewportID() const   { return viewport_id_; }

    void StartNewRoll();
    void StartNewTurn();
    void ThrowAYahtzee();
    void ThrowFromNet(int dice_values[5]);
    void ThrowDice();
    void SetBonusYahtzee(bool is_bonus_yahtzee) { is_bonus_yahtzee_ = is_bonus_yahtzee; }

    void SetVolume(int volume);

    void SyncDiceImagesToCurrentRoll();
    Graphics::ImageListPtr ChooseDiceFromImageList(int dice_value) const;
    Graphics::ImageListPtr ChooseDiceToImageList(int dice_value) const;
    Graphics::ImageListPtr ChooseDiceSpinImageList(Graphics::ImageListPtr current_imagelist) const;
    Graphics::ImageListPtr SyncDiceToImageListToCurrentRoll(int dice_value, Graphics::ImageListPtr current_imagelist) const;

    bool IsFinalRoll() const;
    bool IsGameOver() const;
    bool IsBonusYahtzee() const { return is_bonus_yahtzee_; }
    bool IsHosting() const      { return is_hosting_; }
    bool IsNetworkGame() const  { return !GetConnectionList()->empty(); }
    bool IsPlayerTyping() const { return is_player_typing_; }

    void SetHosting(bool hosting) { is_hosting_ = hosting; }

    void AddMessage(const std::string& message);
    void SetPlayerLocation(int which_player, bool present, const std::string& location = std::string());

public:
    static const int MAX_PLAYERS = 8;
    static const int MAX_ROLLS = 3;

private:
    void OnSpriteGetFocus(Graphics::SpritePtr sprite) override;
    void OnSpriteLoseFocus(Graphics::SpritePtr sprite) override;

    bool IsSettingsPageActive() const;
    bool IsYahtzeePageActive() const;

private:
    Graphics::SpritePtr button_localgame_;
    Graphics::SpritePtr button_networkgame_;
    Graphics::SpritePtr button_main_;
    Graphics::SpritePtr button_settings_;
    Graphics::SpritePtr button_about_;
    Graphics::SpritePtr button_viewhighscores_;
    Graphics::SpritePtr button_exitapplication_;
    Graphics::SpritePtr button_back_;
    Graphics::SpritePtr button_ok_;
    Graphics::SpritePtr button_resethighscores_;
    Graphics::SpritePtr button_continue_;
    Graphics::SpritePtr button_addplayer_;
    Graphics::SpritePtr button_updateplayer_;
    Graphics::SpritePtr button_editplayer_;
    Graphics::SpritePtr button_removeplayer_;
    Graphics::SpritePtr button_clearplayers_;
    Graphics::SpritePtr editbox_player_;
    Graphics::SpritePtr listbox_players_;
    Graphics::SpritePtr button_start_;
    Graphics::SpritePtr listctrl_highscores_;
    Graphics::SpritePtr button_savehighscore_;
    Graphics::SpritePtr button_apply_;
    Graphics::SpritePtr button_test_;
    Graphics::SpritePtr button_ingamesettings_;
    Graphics::SpritePtr button_endgame_;
    Graphics::SpritePtr button_roll_;
    Graphics::SpritePtr button_continue2_;
    Graphics::SpritePtr button_send_;
    Graphics::SpritePtr checkbox_fullscreenenabled_;
    Graphics::SpritePtr slider_audiovolume_;
    Graphics::SpritePtr checkbox_audioenabled_;
    Graphics::SpritePtr editbox_highscore_;
    Graphics::SpritePtr editbox_ones_;
    Graphics::SpritePtr editbox_twos_;
    Graphics::SpritePtr editbox_threes_;
    Graphics::SpritePtr editbox_fours_;
    Graphics::SpritePtr editbox_fives_;
    Graphics::SpritePtr editbox_sixes_;
    Graphics::SpritePtr editbox_subtotal_;
    Graphics::SpritePtr editbox_bonus_;
    Graphics::SpritePtr editbox_uppertotal_;
    Graphics::SpritePtr editbox_threeofakind_;
    Graphics::SpritePtr editbox_fourofakind_;
    Graphics::SpritePtr editbox_fullhouse_;
    Graphics::SpritePtr editbox_smallstraight_;
    Graphics::SpritePtr editbox_largestraight_;
    Graphics::SpritePtr editbox_yahtzee_;
    Graphics::SpritePtr editbox_chance_;
    Graphics::SpritePtr editbox_lowertotal_;
    Graphics::SpritePtr editbox_grandtotal_;
    Graphics::SpritePtr static_dice_0_;
    Graphics::SpritePtr static_dice_1_;
    Graphics::SpritePtr static_dice_2_;
    Graphics::SpritePtr static_dice_3_;
    Graphics::SpritePtr static_dice_4_;
    Graphics::SpritePtr dice_0_held_;
    Graphics::SpritePtr dice_1_held_;
    Graphics::SpritePtr dice_2_held_;
    Graphics::SpritePtr dice_3_held_;
    Graphics::SpritePtr dice_4_held_;
    Graphics::SpritePtr listctrl_summary_;
    Graphics::SpritePtr listctrl_ingameplayers_;
    Graphics::SpritePtr listbox_messages_;
    Graphics::SpritePtr editbox_message_;
    Graphics::SpritePtr listctrl_netgames_;
    Graphics::SpritePtr static_netgameinfo1_;
    Graphics::SpritePtr static_netgameinfo2_;
    Graphics::SpritePtr static_netgameheading1_;
    Graphics::SpritePtr static_netgameheading2_;
    Graphics::SpritePtr button_joinselectednetgame_;
    Graphics::SpritePtr button_createnetgame_;
    Graphics::SpritePtr button_joinspecificnetgame_;
    Graphics::SpritePtr button_cancel_;
    Graphics::SpritePtr static_ip_text_;
    Graphics::SpritePtr listbox_netplayers_;
    Graphics::SpritePtr listbox_netmessages_;
    Graphics::SpriteCursorPtr cursor_normal_;
    Graphics::SpriteCursorPtr cursor_placescore_;
    Graphics::ImagePtr image_mainbackground_;
    Graphics::ImagePtr image_highscoresbackground_;
    Graphics::ImagePtr image_gameoverbackground_;
    Graphics::ImagePtr image_aboutbackground_;
    Graphics::ImagePtr image_settingsbackground_;
    Graphics::ImagePtr image_playersetupbackground_;
    Graphics::ImagePtr image_enterscorebackground_;
    Graphics::ImagePtr image_gameboard_;
    Graphics::ImagePtr image_yahtzee_;
    Graphics::ImagePtr image_netgameendbackground_;
    Graphics::ImagePtr image_netgamesetupbackground_;
    Graphics::ImagePtr image_netgameconnectingbackground_;
    Graphics::ImagePtr image_netgamebackground_;
    Graphics::ImagePtr image_netgameinfo1_;
    Graphics::ImagePtr image_netgameinfo2_;
    Graphics::ImagePtr image_netgameheading1_;
    Graphics::ImagePtr image_netgameheading2_;
    Graphics::ImageListPtr dice1fromstilla_imagelist_;
    Graphics::ImageListPtr dice1fromstillb_imagelist_;
    Graphics::ImageListPtr dice1fromstillc_imagelist_;
    Graphics::ImageListPtr dice1tostilla_imagelist_;
    Graphics::ImageListPtr dice1tostillb_imagelist_;
    Graphics::ImageListPtr dice1tostillc_imagelist_;
    Graphics::ImageListPtr dice2fromstilla_imagelist_;
    Graphics::ImageListPtr dice2fromstillb_imagelist_;
    Graphics::ImageListPtr dice2fromstillc_imagelist_;
    Graphics::ImageListPtr dice2tostilla_imagelist_;
    Graphics::ImageListPtr dice2tostillb_imagelist_;
    Graphics::ImageListPtr dice2tostillc_imagelist_;
    Graphics::ImageListPtr dice3fromstilla_imagelist_;
    Graphics::ImageListPtr dice3fromstillb_imagelist_;
    Graphics::ImageListPtr dice3fromstillc_imagelist_;
    Graphics::ImageListPtr dice3tostilla_imagelist_;
    Graphics::ImageListPtr dice3tostillb_imagelist_;
    Graphics::ImageListPtr dice3tostillc_imagelist_;
    Graphics::ImageListPtr dice4fromstilla_imagelist_;
    Graphics::ImageListPtr dice4fromstillb_imagelist_;
    Graphics::ImageListPtr dice4fromstillc_imagelist_;
    Graphics::ImageListPtr dice4tostilla_imagelist_;
    Graphics::ImageListPtr dice4tostillb_imagelist_;
    Graphics::ImageListPtr dice4tostillc_imagelist_;
    Graphics::ImageListPtr dice5fromstilla_imagelist_;
    Graphics::ImageListPtr dice5fromstillb_imagelist_;
    Graphics::ImageListPtr dice5fromstillc_imagelist_;
    Graphics::ImageListPtr dice5tostilla_imagelist_;
    Graphics::ImageListPtr dice5tostillb_imagelist_;
    Graphics::ImageListPtr dice5tostillc_imagelist_;
    Graphics::ImageListPtr dice6fromstilla_imagelist_;
    Graphics::ImageListPtr dice6fromstillb_imagelist_;
    Graphics::ImageListPtr dice6fromstillc_imagelist_;
    Graphics::ImageListPtr dice6tostilla_imagelist_;
    Graphics::ImageListPtr dice6tostillb_imagelist_;
    Graphics::ImageListPtr dice6tostillc_imagelist_;
    Graphics::ImageListPtr randomspina0_imagelist_;
    Graphics::ImageListPtr randomspina1_imagelist_;
    Graphics::ImageListPtr randomspinb0_imagelist_;
    Graphics::ImageListPtr randomspinb1_imagelist_;
    Graphics::ImageListPtr randomspinc0_imagelist_;
    Graphics::ImageListPtr randomspinc1_imagelist_;
    Graphics::FontPtr   font_control_;

    Audio::SoundPtr shakedice_sound0_;
    Audio::SoundPtr shakedice_sound1_;
    Audio::SoundPtr shakedice_sound2_;
    Audio::SoundPtr shakedice_sound3_;
    Audio::SoundPtr buttonclick_sound_;
    Audio::SoundPtr checkboxclick_sound_;
    Audio::SoundPtr editboxclick_sound_;
    Audio::SoundPtr diceclick_sound_;
    Audio::SoundPtr applause_sound0_;
    Audio::SoundPtr applause_sound1_;
    Audio::SoundPtr yahtzee_sound0_;
    Audio::SoundPtr yahtzee_sound1_;
    Audio::SoundPtr soundtest_sound_;
    Audio::SoundPtr messagereceive_sound_;

    HighscoresPtr       highscores_;
    PlayerContainerPtr  player_container_;
    PlayerPtr           current_player_;
    PlayerPtr           local_player_;
    int                 current_roll_;
    Roll                roll_;
    int                 viewport_id_;
    bool                is_bonus_yahtzee_;
    bool                is_hosting_;
    bool                is_player_typing_;
};

#endif
