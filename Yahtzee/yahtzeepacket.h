#ifndef INCLUDED_YAHTZEEPACKET
#define INCLUDED_YAHTZEEPACKET

#include "../Network/datablock.h"
#include "yahtzeenetworkhandler.h"

#undef GetMessage

class YahtzeePacket
    : public Network::DataBlock
{
public:
    enum PacketType
    {
        PT_INVALID,
        PT_ACK,
        PT_BROADCAST_GAME_AVAILABLE,
        PT_JOIN_REQUEST,
        PT_JOIN_ACCEPTED,
        PT_JOIN_DECLINED,
        PT_GAME_STARTING,
        PT_THROW_DICE,
        PT_THROW_DONE,
        PT_HOLD_DICE,
        PT_SCORE_TURN,
        PT_END_TURN,
        PT_MESSAGE,
        PT_END_GAME,
        PT_ACTIVATE_APP,
        PT_ENTER_SETTINGS,
        PT_YAHTZEE_DONE,
        PT_GAME_OVER,
        PT_PLAYER_JOINED,
        PT_PLAYER_LEFT
    };

public:
    YahtzeePacket(const Network::DataBlock& db, const Network::IpAddress& from_address);
    YahtzeePacket() {}
    bool IsValid() const;
    PacketType GetType() const;
    unsigned short GetSeqNumber() const;
    void SetSeqNumber(unsigned short seq_number);
    const Network::IpAddress& GetFromAddress() const { return from_address_; }

protected:
    bool ReadString(std::string& text);
    bool ReadByte(char& byte);
    bool ReadWord(short& word);
    bool ReadBool(bool& boolean);

    bool CheckOverrun(int amount) const;

    void InitWrite(int total_size);
    void SetType(PacketType type);

    int WriteString(const std::string& text);
    int WriteByte(char byte);
    int WriteWord(short word);
    int WriteBool(bool boolean);

private:
    char* rd_cursor_;
    char* wt_cursor_;
    Network::IpAddress from_address_;
};



class YahtzeePacketAck
    : public YahtzeePacket
{
public:
    YahtzeePacketAck(const Network::DataBlock& db, const Network::IpAddress& from_address)
        : YahtzeePacket(db, from_address) {}
    YahtzeePacketAck() { InitWrite(0); SetType(PT_ACK); }
};



class YahtzeePacketBroadcastGameAvailable
    : public YahtzeePacket
{
public:
    YahtzeePacketBroadcastGameAvailable(const Network::DataBlock& db, const Network::IpAddress& from_address);
    YahtzeePacketBroadcastGameAvailable(const std::string& player_name, int num_players);
    const std::string& GetPlayerName() const    { return player_name_; }
    int GetNumPlayers() const                   { return num_players_; }
private:
    std::string player_name_;
    int num_players_;
};



class YahtzeePacketJoinRequest
    : public YahtzeePacket
{
public:
    YahtzeePacketJoinRequest(const Network::DataBlock& db, const Network::IpAddress& from_address);
    YahtzeePacketJoinRequest(const std::string& player_name);
    const std::string& GetPlayerName() const       { return player_name_; }
private:
    std::string player_name_;
};



class YahtzeePacketJoinAccepted
    : public YahtzeePacket
{
public:
    YahtzeePacketJoinAccepted(const Network::DataBlock& db, const Network::IpAddress& from_address);
    YahtzeePacketJoinAccepted(const std::vector<std::string>& player_names, int your_player_index);
    const std::vector<std::string>& GetPlayerNames() const  { return player_names_; }
    int GetYourPlayerIndex() const                          { return your_player_index_; }
private:
    std::vector<std::string> player_names_;
    int your_player_index_;
};



class YahtzeePacketJoinDeclined
    : public YahtzeePacket
{
public:
    YahtzeePacketJoinDeclined(const Network::DataBlock& db, const Network::IpAddress& from_address);
    YahtzeePacketJoinDeclined(const std::string& reason);
    const std::string& GetReason() const       { return reason_; }
private:
    std::string reason_;
};


class YahtzeePacketGameStarting
    : public YahtzeePacket
{
public:
    YahtzeePacketGameStarting(const Network::DataBlock& db, const Network::IpAddress& from_address)
        : YahtzeePacket(db, from_address) {}
    YahtzeePacketGameStarting() { InitWrite(0); SetType(PT_GAME_STARTING); }
};



class YahtzeePacketThrowDice
    : public YahtzeePacket
{
public:
    YahtzeePacketThrowDice(const Network::DataBlock& db, const Network::IpAddress& from_address);
    YahtzeePacketThrowDice(int values[5]);
    int GetDiceValue(int index) const
    {
        if(index >= 0 && index < 5)
        {
            return values_[index];
        }
        return -1;
    }
private:
    int values_[5];
};



class YahtzeePacketThrowDone
    : public YahtzeePacket
{
public:
    YahtzeePacketThrowDone(const Network::DataBlock& db, const Network::IpAddress& from_address)
        : YahtzeePacket(db, from_address) {}
    YahtzeePacketThrowDone() { InitWrite(0); SetType(PT_THROW_DONE); }
};



class YahtzeePacketHoldDice
    : public YahtzeePacket
{
public:
    YahtzeePacketHoldDice(const Network::DataBlock& db, const Network::IpAddress& from_address);
    YahtzeePacketHoldDice(bool holds[5]);
    bool GetDiceHold(int index) const
    {
        if(index >= 0 && index < 5)
        {
            return holds_[index];
        }
        return false;
    }
private:
    bool holds_[5];
};



class YahtzeePacketScoreTurn
    : public YahtzeePacket
{
public:
    YahtzeePacketScoreTurn(const Network::DataBlock& db, const Network::IpAddress& from_address);
    YahtzeePacketScoreTurn(int score, YahtzeeNetworkHandler::ScoreCategory score_category);
    int GetScore() const                                            { return score_; }
    YahtzeeNetworkHandler::ScoreCategory GetScoreCategory() const   { return score_category_; }
private:
    int score_;
    YahtzeeNetworkHandler::ScoreCategory score_category_;
};



class YahtzeePacketEndTurn
    : public YahtzeePacket
{
public:
    YahtzeePacketEndTurn(const Network::DataBlock& db, const Network::IpAddress& from_address)
        : YahtzeePacket(db, from_address) {}
    YahtzeePacketEndTurn() { InitWrite(0); SetType(PT_END_TURN); }
};



class YahtzeePacketMessage
    : public YahtzeePacket
{
public:
    YahtzeePacketMessage(const Network::DataBlock& db, const Network::IpAddress& from_address);
    YahtzeePacketMessage(const std::string& message);
    std::string GetMessage() const      { return message_; }
private:
    std::string message_;
};



class YahtzeePacketEndGame
    : public YahtzeePacket
{
public:
    YahtzeePacketEndGame(const Network::DataBlock& db, const Network::IpAddress& from_address);
    YahtzeePacketEndGame(int which_player, const std::string& reason);
    int GetWhichPlayer() const          { return which_player_; }
    std::string GetReason() const       { return reason_; }
private:
    int which_player_;
    std::string reason_;
};



class YahtzeePacketActivateApp
    : public YahtzeePacket
{
public:
    YahtzeePacketActivateApp(const Network::DataBlock& db, const Network::IpAddress& from_address);
    YahtzeePacketActivateApp(int which_player, bool activated);
    int GetWhichPlayer() const      { return which_player_; }
    bool GetActivated() const       { return activated_; }
private:
    int which_player_;
    bool activated_;
};



class YahtzeePacketEnterSettings
    : public YahtzeePacket
{
public:
    YahtzeePacketEnterSettings(const Network::DataBlock& db, const Network::IpAddress& from_address);
    YahtzeePacketEnterSettings(int which_player, bool in_settings);
    int GetWhichPlayer() const      { return which_player_; }
    bool GetInSettings() const      { return in_settings_; }
private:
    int which_player_;
    bool in_settings_;
};



class YahtzeePacketYahtzeeDone
    : public YahtzeePacket
{
public:
    YahtzeePacketYahtzeeDone(const Network::DataBlock& db, const Network::IpAddress& from_address)
        : YahtzeePacket(db, from_address) {}
    YahtzeePacketYahtzeeDone() { InitWrite(0); SetType(PT_YAHTZEE_DONE); }
};



class YahtzeePacketGameOver
    : public YahtzeePacket
{
public:
    YahtzeePacketGameOver(const Network::DataBlock& db, const Network::IpAddress& from_address)
        : YahtzeePacket(db, from_address) {}
    YahtzeePacketGameOver() { InitWrite(0); SetType(PT_GAME_OVER); }
};



class YahtzeePacketPlayerJoined
    : public YahtzeePacket
{
public:
    YahtzeePacketPlayerJoined(const Network::DataBlock& db, const Network::IpAddress& from_address);
    YahtzeePacketPlayerJoined(int which_player, const std::string& player_name);
    int GetWhichPlayer() const          { return which_player_; }
    std::string GetPlayerName() const   { return player_name_; }
private:
    int which_player_;
    std::string player_name_;
};



class YahtzeePacketPlayerLeft
    : public YahtzeePacket
{
public:
    YahtzeePacketPlayerLeft(const Network::DataBlock& db, const Network::IpAddress& from_address);
    YahtzeePacketPlayerLeft(int which_player);
    int GetWhichPlayer() const          { return which_player_; }
private:
    int which_player_;
};



#endif
