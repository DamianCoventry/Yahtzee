#include "stdafx.h"
#include "yahtzeenetworkhandler.h"
#include "../Core/engine.h"
#include "../Utility/logmacros.h"
#include "../Utility/strutil.h"
#include "../Network/ipaddress.h"
#include "../Network/nic.h"
#include "yahtzeepacket.h"
#include <sstream>

#define DebugPrint(text_)                       \
{                                               \
    std::ostringstream oss__;                   \
    oss__ << text_ << "\n";                     \
    OutputDebugString(oss__.str().c_str());     \
    LOG(oss__.str());                           \
}

YahtzeeNetworkHandler::YahtzeeNetworkHandler(const std::string& log_context, Core::Engine* engine)
: LogContext(log_context)
, EventHandler(engine->GetSocketList(), engine->GetConnectionList())
, engine_(engine)
{
    engine->AddNetEventHandler(this);
}

YahtzeeNetworkHandler::~YahtzeeNetworkHandler()
{
    engine_->RemoveNetEventHandler(this);
}

void YahtzeeNetworkHandler::BroadcastGameAvailable(const Network::IpAddresses& broadcast_addresses, const std::string& player_name, int num_players)
{
    YahtzeePacketBroadcastGameAvailable packet(player_name, num_players);

    Network::IpAddresses::const_iterator ip;
    for(ip = broadcast_addresses.begin(); ip != broadcast_addresses.end(); ++ip)
    {
        SendOobDataBlock(*ip, packet);
    }
}

void YahtzeeNetworkHandler::SendAck(const YahtzeePacket& yp)
{
    // Send an ACK to the connection that sent this packet.
    YahtzeePacketAck ack;
    ack.SetSeqNumber(yp.GetSeqNumber());

    Network::ConnectionPtr connection = GetConnection(yp.GetFromAddress());
    if(connection)
    {
        connection->Send(ack);
    }
}

void YahtzeeNetworkHandler::SendNetworkEventJoinRequest(const Network::IpAddress& server_address, const std::string& player_name)
{
    DebugPrint("SendNetworkEventJoinRequest() [" << player_name << "]");
    YahtzeePacketJoinRequest packet(player_name);
    SendOobDataBlock(server_address, packet);
}

void YahtzeeNetworkHandler::SendNetworkEventJoinRequest(const Network::IpAddress& server_address, const std::string& player_name, Network::SocketPtr sock)
{
    DebugPrint("SendNetworkEventJoinRequest() [" << player_name << "]");
    YahtzeePacketJoinRequest packet(player_name);
    SendOobDataBlock(server_address, packet, sock);
}

void YahtzeeNetworkHandler::SendNetworkEventJoinAccepted(const Network::IpAddress& client_address, const std::vector<std::string>& player_names, int your_player_index)
{
    DebugPrint("SendNetworkEventJoinAccepted() [" << Utility::ToString(player_names) << "][" << your_player_index << "]");
    YahtzeePacketJoinAccepted packet(player_names, your_player_index);
    SendOobDataBlock(client_address, packet);
}

void YahtzeeNetworkHandler::SendNetworkEventJoinDeclined(const Network::IpAddress& client_address, const std::string& reason)
{
    DebugPrint("SendNetworkEventJoinDeclined() [" << reason << "]");
    YahtzeePacketJoinDeclined packet(reason);
    SendOobDataBlock(client_address, packet);
}

void YahtzeeNetworkHandler::BroadcastNetworkEventGameStarting()
{
    DebugPrint("BroadcastNetworkEventGameStarting()");
    YahtzeePacketGameStarting packet;
    SendToAllConnections(packet);
}

void YahtzeeNetworkHandler::BroadcastNetworkEventThrowDice(int dice_values[5])
{
    DebugPrint("BroadcastNetworkEventThrowDice() [" << dice_values[0] << ", " << dice_values[1] << ", " << dice_values[2] << ", " << dice_values[3] << ", " << dice_values[4] << "]");
    YahtzeePacketThrowDice packet(dice_values);
    SendToAllConnections(packet);
}

void YahtzeeNetworkHandler::BroadcastNetworkEventThrowDone()
{
    DebugPrint("BroadcastNetworkEventThrowDone()");
    YahtzeePacketThrowDone packet;
    SendToAllConnections(packet);
}

void YahtzeeNetworkHandler::BroadcastNetworkEventHoldDice(bool dice_holds[5])
{
    DebugPrint("BroadcastNetworkEventHoldDice() [" << dice_holds[0] << ", " << dice_holds[1] << ", " << dice_holds[2] << ", " << dice_holds[3] << ", " << dice_holds[4] << "]");
    YahtzeePacketHoldDice packet(dice_holds);
    SendToAllConnections(packet);
}

void YahtzeeNetworkHandler::BroadcastNetworkEventScoreTurn(int score, ScoreCategory score_category)
{
    DebugPrint("BroadcastNetworkEventScoreTurn() [" << score << "][" << ToString(score_category) << "]");
    YahtzeePacketScoreTurn packet(score, score_category);
    SendToAllConnections(packet);
}

void YahtzeeNetworkHandler::BroadcastNetworkEventEndTurn()
{
    DebugPrint("BroadcastNetworkEventEndTurn()");
    YahtzeePacketEndTurn packet;
    SendToAllConnections(packet);
}

void YahtzeeNetworkHandler::BroadcastNetworkEventMessage(const std::string& message)
{
    DebugPrint("BroadcastNetworkEventMessage() [" << message << "]");
    YahtzeePacketMessage packet(message);
    SendToAllConnections(packet);
}

void YahtzeeNetworkHandler::BroadcastNetworkEventEndGame(int which_player, const std::string& reason)
{
    DebugPrint("BroadcastNetworkEventEndGame() [" << reason << "]");
    YahtzeePacketEndGame packet(which_player, reason);
    SendToAllConnections(packet);
}

void YahtzeeNetworkHandler::BroadcastNetworkEventActivateApp(int which_player, bool activated)
{
    DebugPrint("BroadcastNetworkEventActivateApp() [" << which_player << "][" << activated << "]");
    YahtzeePacketActivateApp packet(which_player, activated);
    SendToAllConnections(packet);
}

void YahtzeeNetworkHandler::BroadcastNetworkEventEnterSettings(int which_player, bool in_settings)
{
    DebugPrint("BroadcastNetworkEventEnterSettings() [" << which_player << "][" << in_settings << "]");
    YahtzeePacketEnterSettings packet(which_player, in_settings);
    SendToAllConnections(packet);
}

void YahtzeeNetworkHandler::BroadcastNetworkEventYahtzeeDone()
{
    DebugPrint("BroadcastNetworkEventYahtzeeDone()");
    YahtzeePacketYahtzeeDone packet;
    SendToAllConnections(packet);
}

void YahtzeeNetworkHandler::BroadcastNetworkEventGameOver()
{
    DebugPrint("BroadcastNetworkEventGameOver()");
    YahtzeePacketGameOver packet;
    SendToAllConnections(packet);
}

void YahtzeeNetworkHandler::BroadcastNetworkEventPlayerJoined(int which_player, const std::string& player_name)
{
    DebugPrint("BroadcastNetworkEventPlayerJoined() [" << which_player << "][" << player_name << "]");
    YahtzeePacketPlayerJoined packet(which_player, player_name);
    SendToAllConnections(packet);
}

void YahtzeeNetworkHandler::BroadcastNetworkEventPlayerLeft(int which_player)
{
    DebugPrint("BroadcastNetworkEventPlayerLeft() [" << which_player << "]");
    YahtzeePacketPlayerLeft packet(which_player);
    SendToAllConnections(packet);
}

void YahtzeeNetworkHandler::OnDataBlockReceived(Network::SocketPtr sock, const Network::DataBlock& db, const Network::IpAddress& from_address)
{
    YahtzeePacket packet(db, from_address);
    switch(packet.GetType())
    {
    case YahtzeePacket::PT_BROADCAST_GAME_AVAILABLE:
        {
            YahtzeePacketBroadcastGameAvailable yp(db, from_address);
            DebugPrint("Received: PT_BROADCAST_GAME_AVAILABLE from [" << yp.GetFromAddress().ToLongString() << "], [" << yp.GetPlayerName() << "][" << yp.GetNumPlayers() << "]");
            OnNetworkBroadcastGameAvailable(sock, yp);
            break;
        }
    case YahtzeePacket::PT_JOIN_REQUEST:
        {
            YahtzeePacketJoinRequest yp(db, from_address);
            DebugPrint("Received: PT_JOIN_REQUEST from [" << yp.GetFromAddress().ToLongString() << "], [" << yp.GetPlayerName() << "]");
            OnNetworkEventJoinRequest(sock, yp);
            break;
        }
    case YahtzeePacket::PT_JOIN_ACCEPTED:
        {
            YahtzeePacketJoinAccepted yp(db, from_address);
            DebugPrint("Received: PT_JOIN_ACCEPTED from [" << yp.GetFromAddress().ToLongString() << "], [" << Utility::ToString(yp.GetPlayerNames()) << "][" << yp.GetYourPlayerIndex() << "]");
            OnNetworkEventJoinAccepted(sock, yp);
            break;
        }
    case YahtzeePacket::PT_JOIN_DECLINED:
        {
            YahtzeePacketJoinDeclined yp(db, from_address);
            DebugPrint("Received: PT_JOIN_DECLINED from [" << yp.GetFromAddress().ToLongString() << "], [" << yp.GetReason() << "]");
            OnNetworkEventJoinDeclined(sock, yp);
            break;
        }
    case YahtzeePacket::PT_GAME_STARTING:
        {
            YahtzeePacketGameStarting yp(db, from_address);
            DebugPrint("Received: PT_GAME_STARTING from [" << yp.GetFromAddress().ToLongString() << "]");
            OnNetworkEventGameStarting(sock, yp);
            break;
        }
    case YahtzeePacket::PT_THROW_DICE:
        {
            YahtzeePacketThrowDice yp(db, from_address);
            DebugPrint("Received: PT_THROW_DICE from [" << yp.GetFromAddress().ToLongString() << "], [" << yp.GetDiceValue(0) << "][" << yp.GetDiceValue(1) << "][" << yp.GetDiceValue(2) << "][" << yp.GetDiceValue(3) << "][" << yp.GetDiceValue(4) << "]");
            OnNetworkEventThrowDice(sock, yp);
            PropagateToOtherClients(yp);
            break;
        }
    case YahtzeePacket::PT_THROW_DONE:
        {
            YahtzeePacketThrowDone yp(db, from_address);
            DebugPrint("Received: PT_THROW_DONE from [" << yp.GetFromAddress().ToLongString() << "]");
            OnNetworkEventThrowDone(sock, yp);
            PropagateToOtherClients(yp);
            break;
        }
    case YahtzeePacket::PT_HOLD_DICE:
        {
            YahtzeePacketHoldDice yp(db, from_address);
            DebugPrint("Received: PT_HOLD_DICE from [" << yp.GetFromAddress().ToLongString() << "], [" << yp.GetDiceHold(0) << "][" << yp.GetDiceHold(1) << "][" << yp.GetDiceHold(2) << "][" << yp.GetDiceHold(3) << "][" << yp.GetDiceHold(4) << "]");
            OnNetworkEventHoldDice(sock, yp);
            PropagateToOtherClients(yp);
            break;
        }
    case YahtzeePacket::PT_SCORE_TURN:
        {
            YahtzeePacketScoreTurn yp(db, from_address);
            DebugPrint("Received: PT_SCORE_TURN from [" << yp.GetFromAddress().ToLongString() << "], [" << yp.GetScore() << "][" << ToString(yp.GetScoreCategory()) << "]");
            OnNetworkEventScoreTurn(sock, yp);
            PropagateToOtherClients(yp);
            break;
        }
    case YahtzeePacket::PT_END_TURN:
        {
            YahtzeePacketEndTurn yp(db, from_address);
            DebugPrint("Received: PT_END_TURN from [" << yp.GetFromAddress().ToLongString() << "]");
            OnNetworkEventEndTurn(sock, yp);
            PropagateToOtherClients(yp);
            break;
        }
    case YahtzeePacket::PT_MESSAGE:
        {
            YahtzeePacketMessage yp(db, from_address);
            DebugPrint("Received: PT_MESSAGE from [" << yp.GetFromAddress().ToLongString() << "], [" << yp.GetMessage() << "]");
            OnNetworkEventMessage(sock, yp);
            PropagateToOtherClients(yp);
            break;
        }
    case YahtzeePacket::PT_END_GAME:
        {
            YahtzeePacketEndGame yp(db, from_address);
            DebugPrint("Received: PT_END_GAME from [" << yp.GetFromAddress().ToLongString() << "], [" << yp.GetWhichPlayer() << "][" << yp.GetReason() << "]");
            OnNetworkEventEndGame(sock, yp);
            PropagateToOtherClients(yp);
            break;
        }
    case YahtzeePacket::PT_ACTIVATE_APP:
        {
            YahtzeePacketActivateApp yp(db, from_address);
            DebugPrint("Received: PT_ACTIVATE_APP from [" << yp.GetFromAddress().ToLongString() << "], [" << yp.GetWhichPlayer() << "][" << yp.GetActivated() << "]");
            OnNetworkEventActivateApp(sock, yp);
            PropagateToOtherClients(yp);
            break;
        }
    case YahtzeePacket::PT_ENTER_SETTINGS:
        {
            YahtzeePacketEnterSettings yp(db, from_address);
            DebugPrint("Received: PT_ENTER_SETTINGS from [" << yp.GetFromAddress().ToLongString() << "], [" << yp.GetWhichPlayer() << "][" << yp.GetInSettings() << "]");
            OnNetworkEventEnterSettings(sock, yp);
            PropagateToOtherClients(yp);
            break;
        }
    case YahtzeePacket::PT_YAHTZEE_DONE:
        {
            YahtzeePacketYahtzeeDone yp(db, from_address);
            DebugPrint("Received: PT_YAHTZEE_DONE from [" << yp.GetFromAddress().ToLongString() << "]");
            OnNetworkEventYahtzeeDone(sock, yp);
            PropagateToOtherClients(yp);
            break;
        }
    case YahtzeePacket::PT_GAME_OVER:
        {
            YahtzeePacketGameOver yp(db, from_address);
            DebugPrint("Received: PT_GAME_OVER from [" << yp.GetFromAddress().ToLongString() << "]");
            OnNetworkEventGameOver(sock, yp);
            PropagateToOtherClients(yp);
            break;
        }
    case YahtzeePacket::PT_PLAYER_JOINED:
        {
            YahtzeePacketPlayerJoined yp(db, from_address);
            DebugPrint("Received: PT_PLAYER_JOINED from [" << yp.GetFromAddress().ToLongString() << "], [" << yp.GetWhichPlayer() << "][" << yp.GetPlayerName() << "]");
            OnNetworkEventPlayerJoined(sock, yp);
            PropagateToOtherClients(yp);
            break;
        }
    case YahtzeePacket::PT_PLAYER_LEFT:
        {
            YahtzeePacketPlayerLeft yp(db, from_address);
            DebugPrint("Received: PT_PLAYER_LEFT from [" << yp.GetFromAddress().ToLongString() << "], [" << yp.GetWhichPlayer() << "]");
            OnNetworkEventPlayerLeft(sock, yp);
            PropagateToOtherClients(yp);
            break;
        }
    }
}

void YahtzeeNetworkHandler::OnNetworkBroadcastGameAvailable(Network::SocketPtr socket, const YahtzeePacketBroadcastGameAvailable& yp)
{
    LOG("Received the network message [PT_BROADCAST_GAME_AVAILABLE] from [" << yp.GetFromAddress().ToLongString() << "], but it's not applicable in the current state. player_name=[" << yp.GetPlayerName() << "], num_players=[" << yp.GetNumPlayers() << "]");
}

void YahtzeeNetworkHandler::OnNetworkEventJoinRequest(Network::SocketPtr sock, const YahtzeePacketJoinRequest& yp)
{
    LOG("Received the network message [PT_JOIN_REQUEST] from [" << yp.GetFromAddress().ToLongString() << "], but it's not applicable in the current state. player_name=[" << yp.GetPlayerName() << "]");
}

void YahtzeeNetworkHandler::OnNetworkEventJoinAccepted(Network::SocketPtr sock, const YahtzeePacketJoinAccepted& yp)
{
    LOG("Received the network message [PT_JOIN_ACCEPTED] from [" << yp.GetFromAddress().ToLongString() << "], but it's not applicable in the current state. your_player_index=[" << Utility::ToString(yp.GetPlayerNames()) << "]. your_player_index=[" << yp.GetYourPlayerIndex() << "]");
}

void YahtzeeNetworkHandler::OnNetworkEventJoinDeclined(Network::SocketPtr sock, const YahtzeePacketJoinDeclined& yp)
{
    LOG("Received the network message [PT_JOIN_DECLINED] from [" << yp.GetFromAddress().ToLongString() << "], but it's not applicable in the current state. reason=[" << yp.GetReason() << "]");
}

void YahtzeeNetworkHandler::OnNetworkEventGameStarting(Network::SocketPtr sock, const YahtzeePacketGameStarting& yp)
{
    LOG("Received the network message [PT_GAME_STARTING] from [" << yp.GetFromAddress().ToLongString() << "], but it's not applicable in the current state.");
}

void YahtzeeNetworkHandler::OnNetworkEventThrowDice(Network::SocketPtr sock, const YahtzeePacketThrowDice& yp)
{
    LOG("Received the network message [PT_THROW_DICE] from [" << yp.GetFromAddress().ToLongString() << "], but it's not applicable in the current state. dice_values=["
       << yp.GetDiceValue(0) << ", " << yp.GetDiceValue(1) << ", " << yp.GetDiceValue(2) << ", "
       << yp.GetDiceValue(3) << ", " << yp.GetDiceValue(4) << ")");
}

void YahtzeeNetworkHandler::OnNetworkEventThrowDone(Network::SocketPtr sock, const YahtzeePacketThrowDone& yp)
{
    LOG("Received the network message [PT_THROW_DONE] from [" << yp.GetFromAddress().ToLongString() << "], but it's not applicable in the current state.");
}

void YahtzeeNetworkHandler::OnNetworkEventHoldDice(Network::SocketPtr sock, const YahtzeePacketHoldDice& yp)
{
    LOG("Received the network message [PT_HOLD_DICE] from [" << yp.GetFromAddress().ToLongString() << "], but it's not applicable in the current state.. dice_holds=["
       << (yp.GetDiceHold(0) ? "True" : "False") << ", " << (yp.GetDiceHold(1) ? "True" : "False") << ", "
       << (yp.GetDiceHold(2) ? "True" : "False") << ", " << (yp.GetDiceHold(3) ? "True" : "False") << ", "
       << (yp.GetDiceHold(4) ? "True" : "False") << ")");
}

void YahtzeeNetworkHandler::OnNetworkEventScoreTurn(Network::SocketPtr sock, const YahtzeePacketScoreTurn& yp)
{
    LOG("Received the network message [PT_SCORE_TURN] from [" << yp.GetFromAddress().ToLongString() << "], but it's not applicable in the current state. score=[" << yp.GetScore() << "], score_category=[" << ToString(yp.GetScoreCategory()) << "]");
}

void YahtzeeNetworkHandler::OnNetworkEventEndTurn(Network::SocketPtr sock, const YahtzeePacketEndTurn& yp)
{
    LOG("Received the network message [PT_END_TURN] from [" << yp.GetFromAddress().ToLongString() << "], but it's not applicable in the current state.");
}

void YahtzeeNetworkHandler::OnNetworkEventMessage(Network::SocketPtr sock, const YahtzeePacketMessage& yp)
{
    LOG("Received the network message [PT_MESSAGE] from [" << yp.GetFromAddress().ToLongString() << "], but it's not applicable in the current state. message=[" << yp.GetMessage() << "]");
}

void YahtzeeNetworkHandler::OnNetworkEventEndGame(Network::SocketPtr sock, const YahtzeePacketEndGame& yp)
{
    LOG("Received the network message [PT_END_GAME] from [" << yp.GetFromAddress().ToLongString() << "], but it's not applicable in the current state. which_player=[" << yp.GetWhichPlayer() << "], reason=[" << yp.GetReason() << "]");
}

void YahtzeeNetworkHandler::OnNetworkEventActivateApp(Network::SocketPtr sock, const YahtzeePacketActivateApp& yp)
{
    LOG("Received the network message [PT_ACTIVATE_APP] from [" << yp.GetFromAddress().ToLongString() << "], but it's not applicable in the current state. which_player=[" << yp.GetWhichPlayer() << "], activated=[" << (yp.GetActivated() ? "True" : "False") << "]");
}

void YahtzeeNetworkHandler::OnNetworkEventEnterSettings(Network::SocketPtr sock, const YahtzeePacketEnterSettings& yp)
{
    LOG("Received the network message [PT_ENTER_SETTINGS] from [" << yp.GetFromAddress().ToLongString() << "], but it's not applicable in the current state. which_player=[" << yp.GetWhichPlayer() << "], in_settings=[" << (yp.GetInSettings() ? "True" : "False") << "]");
}

void YahtzeeNetworkHandler::OnNetworkEventYahtzeeDone(Network::SocketPtr sock, const YahtzeePacketYahtzeeDone& yp)
{
    LOG("Received the network message [PT_ENTER_SETTINGS] from [" << yp.GetFromAddress().ToLongString() << "], but it's not applicable in the current state.");
}

void YahtzeeNetworkHandler::OnNetworkEventGameOver(Network::SocketPtr sock, const YahtzeePacketGameOver& yp)
{
    LOG("Received the network message [PT_GAME_OVER] from [" << yp.GetFromAddress().ToLongString() << "], but it's not applicable in the current state.");
}

void YahtzeeNetworkHandler::OnNetworkEventPlayerJoined(Network::SocketPtr sock, const YahtzeePacketPlayerJoined& yp)
{
    LOG("Received the network message [PT_PLAYER_JOINED] from [" << yp.GetFromAddress().ToLongString() << "], but it's not applicable in the current state. which_player=[" << yp.GetWhichPlayer() << "], player_name=[" << yp.GetPlayerName() << "]");
}

void YahtzeeNetworkHandler::OnNetworkEventPlayerLeft(Network::SocketPtr sock, const YahtzeePacketPlayerLeft& yp)
{
    LOG("Received the network message [PT_PLAYER_LEFT] from [" << yp.GetFromAddress().ToLongString() << "], but it's not applicable in the current state. which_player=[" << yp.GetWhichPlayer() << "]");
}

std::string YahtzeeNetworkHandler::ToString(ScoreCategory score_category)
{
    switch(score_category)
    {
    case SC_ONES: return "SC_ONES";
    case SC_TWOS: return "SC_TWOS";
    case SC_THREES: return "SC_THREES";
    case SC_FOURS: return "SC_FOURS";
    case SC_FIVES: return "SC_FIVES";
    case SC_SIXES: return "SC_SIXES";
    case SC_THREE_OF_A_KIND: return "SC_THREE_OF_A_KIND";
    case SC_FOUR_OF_A_KIND: return "SC_FOUR_OF_A_KIND";
    case SC_FULL_HOUSE: return "SC_FULL_HOUSE";
    case SC_SMALL_STRAIGHT: return "SC_SMALL_STRAIGHT";
    case SC_LARGE_STRAIGHT: return "SC_LARGE_STRAIGHT";
    case SC_YAHTZEE: return "SC_YAHTZEE";
    case SC_CHANCE: return "SC_CHANCE";
    }
    return "Unknown";
}

void YahtzeeNetworkHandler::PropagateToOtherClients(const YahtzeePacket& yp)
{
    SendToAllConnectionsExcept(yp, yp.GetFromAddress());
}
