#include "stdafx.h"
#include "yahtzeepacket.h"

#undef min

namespace
{
    const char UNIQUE_ID[4] = { 'O', 'W', 'Y', 'Z' };

#pragma pack(push, 1)
    struct Header
    {
        char unique_id_[4];
        char type_;
        unsigned short seq_number_;
    };
#pragma pack(pop)

}

YahtzeePacket::YahtzeePacket(const Network::DataBlock& db, const Network::IpAddress& from_address)
: DataBlock(db)
, rd_cursor_(db.bytes_.get() + sizeof(Header))
, wt_cursor_(NULL)
, from_address_(from_address)
{
}

bool YahtzeePacket::IsValid() const
{
    if(bytes_ == NULL)
    {
        return false;
    }
    Header* header = reinterpret_cast<Header*>(bytes_.get());
    return (memcmp(header->unique_id_, UNIQUE_ID, sizeof(char)*4) == 0);
}

YahtzeePacket::PacketType YahtzeePacket::GetType() const
{
    if(bytes_ == NULL)
    {
        return PT_INVALID;
    }
    Header* header = reinterpret_cast<Header*>(bytes_.get());
    return (YahtzeePacket::PacketType)header->type_;
}

unsigned short YahtzeePacket::GetSeqNumber() const
{
    if(bytes_ == NULL)
    {
        return 0xffff;
    }
    Header* header = reinterpret_cast<Header*>(bytes_.get());
    return header->seq_number_;
}

bool YahtzeePacket::CheckOverrun(int amount) const
{
    return ((rd_cursor_ - bytes_.get()) + amount) > num_bytes_;
}

void YahtzeePacket::SetSeqNumber(unsigned short seq_number)
{
    if(bytes_)
    {
        Header* header = reinterpret_cast<Header*>(bytes_.get());
        header->seq_number_ = seq_number;
    }
}

bool YahtzeePacket::ReadString(std::string& text)
{
    if(CheckOverrun(1))
    {
        return false;
    }
    char raw_length = *rd_cursor_;
    ++rd_cursor_;

    if(CheckOverrun(raw_length))
    {
        return false;
    }
    char buffer[64];
    char length = std::min(raw_length, (char)63);
    memcpy(buffer, rd_cursor_, length);
    buffer[length] = 0;
    text = buffer;
    rd_cursor_ += raw_length;
    return true;
}

bool YahtzeePacket::ReadByte(char& byte)
{
    if(CheckOverrun(1))
    {
        return false;
    }
    byte = *rd_cursor_;
    ++rd_cursor_;
    return true;
}

bool YahtzeePacket::ReadWord(short& word)
{
    if(CheckOverrun(2))
    {
        return false;
    }
    word = *(short*)rd_cursor_;
    rd_cursor_ += sizeof(short);
    return true;
}

bool YahtzeePacket::ReadBool(bool& boolean)
{
    if(CheckOverrun(1))
    {
        return false;
    }
    boolean = (*rd_cursor_ != 0);
    ++rd_cursor_;
    return true;
}

void YahtzeePacket::InitWrite(int total_size)
{
    num_bytes_ = sizeof(Header) + total_size;
    bytes_.reset(new char[num_bytes_]);
    wt_cursor_ = bytes_.get() + sizeof(Header);

    Header* header = reinterpret_cast<Header*>(bytes_.get());
    header->seq_number_ = 0;
    memcpy(header->unique_id_, UNIQUE_ID, sizeof(char)*4);
}

void YahtzeePacket::SetType(PacketType type)
{
    Header* header = reinterpret_cast<Header*>(bytes_.get());
    header->type_ = type;
}

int YahtzeePacket::WriteString(const std::string& text)
{
    char length = char(text.size());
    *wt_cursor_ = length;
    ++wt_cursor_;

    std::string::const_iterator itor;
    for(itor = text.begin(); itor != text.end(); ++itor)
    {
        *wt_cursor_ = *itor;
        ++wt_cursor_;
    }

    return 1 + int(text.size());
}

int YahtzeePacket::WriteByte(char byte)
{
    *wt_cursor_ = byte;
    ++wt_cursor_;
    return 1;
}

int YahtzeePacket::WriteWord(short word)
{
    *(short*)wt_cursor_ = word;
    wt_cursor_ += sizeof(short);
    return sizeof(short);
}

int YahtzeePacket::WriteBool(bool boolean)
{
    *wt_cursor_ = (boolean ? 1 : 0);
    ++wt_cursor_;
    return 1;
}





YahtzeePacketBroadcastGameAvailable::YahtzeePacketBroadcastGameAvailable(const Network::DataBlock& db, const Network::IpAddress& from_address)
: YahtzeePacket(db, from_address)
{
    ReadString(player_name_);
    char byte;
    ReadByte(byte);
    num_players_ = int(byte);
}

YahtzeePacketBroadcastGameAvailable::YahtzeePacketBroadcastGameAvailable(const std::string& player_name, int num_players)
{
    InitWrite(1 + int(player_name.size()) + 1);
    SetType(PT_BROADCAST_GAME_AVAILABLE);
    WriteString(player_name);
    WriteByte(char(num_players));
}





YahtzeePacketJoinRequest::YahtzeePacketJoinRequest(const Network::DataBlock& db, const Network::IpAddress& from_address)
: YahtzeePacket(db, from_address)
{
    ReadString(player_name_);
}

YahtzeePacketJoinRequest::YahtzeePacketJoinRequest(const std::string& player_name)
{
    InitWrite(1 + int(player_name.size()));
    SetType(PT_JOIN_REQUEST);
    WriteString(player_name);
}





YahtzeePacketJoinAccepted::YahtzeePacketJoinAccepted(const Network::DataBlock& db, const Network::IpAddress& from_address)
: YahtzeePacket(db, from_address)
{
    char byte;
    ReadByte(byte);
    your_player_index_ = int(byte);
    ReadByte(byte);
    int num_players = int(byte);
    for(int i = 0; i < num_players; ++i)
    {
        std::string name;
        ReadString(name);
        player_names_.push_back(name);
    }
}

YahtzeePacketJoinAccepted::YahtzeePacketJoinAccepted(const std::vector<std::string>& player_names, int your_player_index)
{
    std::vector<std::string>::const_iterator itor;
    std::size_t total = 0;
    for(itor = player_names.begin(); itor != player_names.end(); ++itor)
    {
        total += itor->size() + 1;
    }
    InitWrite(2 + int(total));
    SetType(PT_JOIN_ACCEPTED);
    WriteByte(char(your_player_index));
    WriteByte(char(player_names.size()));
    for(itor = player_names.begin(); itor != player_names.end(); ++itor)
    {
        WriteString(*itor);
    }
}





YahtzeePacketJoinDeclined::YahtzeePacketJoinDeclined(const Network::DataBlock& db, const Network::IpAddress& from_address)
: YahtzeePacket(db, from_address)
{
    ReadString(reason_);
}

YahtzeePacketJoinDeclined::YahtzeePacketJoinDeclined(const std::string& reason)
{
    InitWrite(1 + int(reason.size()));
    SetType(PT_JOIN_DECLINED);
    WriteString(reason);
}





YahtzeePacketThrowDice::YahtzeePacketThrowDice(const Network::DataBlock& db, const Network::IpAddress& from_address)
: YahtzeePacket(db, from_address)
{
    char byte;
    for(int i = 0; i < 5; i++)
    {
        ReadByte(byte);
        values_[i] = int(byte);
    }
}

YahtzeePacketThrowDice::YahtzeePacketThrowDice(int values[5])
{
    InitWrite(5);
    SetType(PT_THROW_DICE);
    WriteByte(char(values[0]));
    WriteByte(char(values[1]));
    WriteByte(char(values[2]));
    WriteByte(char(values[3]));
    WriteByte(char(values[4]));
}





YahtzeePacketHoldDice::YahtzeePacketHoldDice(const Network::DataBlock& db, const Network::IpAddress& from_address)
: YahtzeePacket(db, from_address)
{
    for(int i = 0; i < 5; i++)
    {
        ReadBool(holds_[i]);
    }
}

YahtzeePacketHoldDice::YahtzeePacketHoldDice(bool holds[5])
{
    InitWrite(5);
    SetType(PT_HOLD_DICE);
    WriteBool(holds[0]);
    WriteBool(holds[1]);
    WriteBool(holds[2]);
    WriteBool(holds[3]);
    WriteBool(holds[4]);
}





YahtzeePacketScoreTurn::YahtzeePacketScoreTurn(const Network::DataBlock& db, const Network::IpAddress& from_address)
: YahtzeePacket(db, from_address)
{
    short word;
    ReadWord(word);
    score_ = int(word);
    char byte;
    ReadByte(byte);
    score_category_ = (YahtzeeNetworkHandler::ScoreCategory)byte;
}

YahtzeePacketScoreTurn::YahtzeePacketScoreTurn(int score, YahtzeeNetworkHandler::ScoreCategory score_category)
{
    InitWrite(3);
    SetType(PT_SCORE_TURN);
    WriteWord(short(score));
    WriteByte(char(score_category));
}





YahtzeePacketMessage::YahtzeePacketMessage(const Network::DataBlock& db, const Network::IpAddress& from_address)
: YahtzeePacket(db, from_address)
{
    ReadString(message_);
}

YahtzeePacketMessage::YahtzeePacketMessage(const std::string& message)
{
    InitWrite(1 + int(message.size()));
    SetType(PT_MESSAGE);
    WriteString(message);
}




YahtzeePacketEndGame::YahtzeePacketEndGame(const Network::DataBlock& db, const Network::IpAddress& from_address)
: YahtzeePacket(db, from_address)
{
    char byte;
    ReadByte(byte);
    which_player_ = int(byte);
    ReadString(reason_);
}

YahtzeePacketEndGame::YahtzeePacketEndGame(int which_player, const std::string& reason)
{
    InitWrite(2 + int(reason.size()));
    SetType(PT_END_GAME);
    WriteByte(char(which_player));
    WriteString(reason);
}





YahtzeePacketActivateApp::YahtzeePacketActivateApp(const Network::DataBlock& db, const Network::IpAddress& from_address)
: YahtzeePacket(db, from_address)
{
    char byte;
    ReadByte(byte);
    which_player_ = int(byte);
    ReadBool(activated_);
}

YahtzeePacketActivateApp::YahtzeePacketActivateApp(int which_player, bool activated)
{
    InitWrite(2);
    SetType(PT_ACTIVATE_APP);
    WriteByte(char(which_player));
    WriteBool(activated);
}





YahtzeePacketEnterSettings::YahtzeePacketEnterSettings(const Network::DataBlock& db, const Network::IpAddress& from_address)
: YahtzeePacket(db, from_address)
{
    char byte;
    ReadByte(byte);
    which_player_ = int(byte);
    ReadBool(in_settings_);
}

YahtzeePacketEnterSettings::YahtzeePacketEnterSettings(int which_player, bool in_settings)
{
    InitWrite(2);
    SetType(PT_ENTER_SETTINGS);
    WriteByte(char(which_player));
    WriteBool(in_settings);
}





YahtzeePacketPlayerJoined::YahtzeePacketPlayerJoined(const Network::DataBlock& db, const Network::IpAddress& from_address)
: YahtzeePacket(db, from_address)
{
    char byte;
    ReadByte(byte);
    which_player_ = int(byte);
    ReadString(player_name_);
}

YahtzeePacketPlayerJoined::YahtzeePacketPlayerJoined(int which_player, const std::string& player_name)
{
    InitWrite(2 + int(player_name.size()));
    SetType(PT_PLAYER_JOINED);
    WriteByte(char(which_player));
    WriteString(player_name);
}





YahtzeePacketPlayerLeft::YahtzeePacketPlayerLeft(const Network::DataBlock& db, const Network::IpAddress& from_address)
: YahtzeePacket(db, from_address)
{
    char byte;
    ReadByte(byte);
    which_player_ = int(byte);
}

YahtzeePacketPlayerLeft::YahtzeePacketPlayerLeft(int which_player)
{
    InitWrite(1);
    SetType(PT_PLAYER_LEFT);
    WriteByte(char(which_player));
}
