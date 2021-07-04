#ifndef INCLUDED_YAHTZEENETWORKHANDLER
#define INCLUDED_YAHTZEENETWORKHANDLER

#include "../Network/eventhandler.h"
#include "../Utility/logcontext.h"
#include <string>
#include <vector>

namespace Core
{
class Engine;
}

class YahtzeePacket;
class YahtzeePacketAck;
class YahtzeePacketBroadcastGameAvailable;
class YahtzeePacketJoinRequest;
class YahtzeePacketJoinAccepted;
class YahtzeePacketJoinDeclined;
class YahtzeePacketGameStarting;
class YahtzeePacketThrowDice;
class YahtzeePacketThrowDone;
class YahtzeePacketHoldDice;
class YahtzeePacketScoreTurn;
class YahtzeePacketEndTurn;
class YahtzeePacketMessage;
class YahtzeePacketEndGame;
class YahtzeePacketActivateApp;
class YahtzeePacketEnterSettings;
class YahtzeePacketYahtzeeDone;
class YahtzeePacketGameOver;
class YahtzeePacketPlayerJoined;
class YahtzeePacketPlayerLeft;

class YahtzeeNetworkHandler
    : public Network::EventHandler
    , public Utility::LogContext
{
public:
    YahtzeeNetworkHandler(const std::string& log_context, Core::Engine* engine);
    ~YahtzeeNetworkHandler();

    enum ScoreCategory
    {
        SC_ONES,
        SC_TWOS,
        SC_THREES,
        SC_FOURS,
        SC_FIVES,
        SC_SIXES,
        SC_THREE_OF_A_KIND,
        SC_FOUR_OF_A_KIND,
        SC_FULL_HOUSE,
        SC_SMALL_STRAIGHT,
        SC_LARGE_STRAIGHT,
        SC_YAHTZEE,
        SC_CHANCE
    };

    void PropagateToOtherClients(const YahtzeePacket& yp);

    // The sending of yahtzee specific messages
    void BroadcastGameAvailable(const Network::IpAddresses& broadcast_addresses, const std::string& player_name, int num_players);
    void SendAck(const YahtzeePacket& yp);
    void SendNetworkEventJoinRequest(const Network::IpAddress& server_address, const std::string& player_name);
    void SendNetworkEventJoinRequest(const Network::IpAddress& server_address, const std::string& player_name, Network::SocketPtr sock);
    void SendNetworkEventJoinAccepted(const Network::IpAddress& client_address, const std::vector<std::string>& player_names, int your_player_index);
    void SendNetworkEventJoinDeclined(const Network::IpAddress& client_address, const std::string& reason);
    void BroadcastNetworkEventGameStarting();
    void BroadcastNetworkEventThrowDice(int dice_values[5]);
    void BroadcastNetworkEventThrowDone();
    void BroadcastNetworkEventHoldDice(bool dice_holds[5]);
    void BroadcastNetworkEventScoreTurn(int score, ScoreCategory score_category);
    void BroadcastNetworkEventEndTurn();
    void BroadcastNetworkEventMessage(const std::string& message);
    void BroadcastNetworkEventEndGame(int which_player, const std::string& reason);
    void BroadcastNetworkEventActivateApp(int which_player, bool activated);
    void BroadcastNetworkEventEnterSettings(int which_player, bool in_settings);
    void BroadcastNetworkEventYahtzeeDone();
    void BroadcastNetworkEventGameOver();
    void BroadcastNetworkEventPlayerJoined(int which_player, const std::string& player_name);
    void BroadcastNetworkEventPlayerLeft(int which_player);

    // Implementation of Network::EventHandler
    void OnDataBlockReceived(Network::SocketPtr sock, const Network::DataBlock& db, const Network::IpAddress& from_address);

    // These may be optionally overridden in sub classes
    virtual void OnNetworkBroadcastGameAvailable(Network::SocketPtr socket, const YahtzeePacketBroadcastGameAvailable& yp);
    virtual void OnNetworkEventJoinRequest(Network::SocketPtr sock, const YahtzeePacketJoinRequest& yp);
    virtual void OnNetworkEventJoinAccepted(Network::SocketPtr sock, const YahtzeePacketJoinAccepted& yp);
    virtual void OnNetworkEventJoinDeclined(Network::SocketPtr sock, const YahtzeePacketJoinDeclined& yp);
    virtual void OnNetworkEventGameStarting(Network::SocketPtr sock, const YahtzeePacketGameStarting& yp);
    virtual void OnNetworkEventThrowDice(Network::SocketPtr sock, const YahtzeePacketThrowDice& yp);
    virtual void OnNetworkEventThrowDone(Network::SocketPtr sock, const YahtzeePacketThrowDone& yp);
    virtual void OnNetworkEventHoldDice(Network::SocketPtr sock, const YahtzeePacketHoldDice& yp);
    virtual void OnNetworkEventScoreTurn(Network::SocketPtr sock, const YahtzeePacketScoreTurn& yp);
    virtual void OnNetworkEventEndTurn(Network::SocketPtr sock, const YahtzeePacketEndTurn& yp);
    virtual void OnNetworkEventMessage(Network::SocketPtr sock, const YahtzeePacketMessage& yp);
    virtual void OnNetworkEventEndGame(Network::SocketPtr sock, const YahtzeePacketEndGame& yp);
    virtual void OnNetworkEventActivateApp(Network::SocketPtr sock, const YahtzeePacketActivateApp& yp);
    virtual void OnNetworkEventEnterSettings(Network::SocketPtr sock, const YahtzeePacketEnterSettings& yp);
    virtual void OnNetworkEventYahtzeeDone(Network::SocketPtr sock, const YahtzeePacketYahtzeeDone& yp);
    virtual void OnNetworkEventGameOver(Network::SocketPtr sock, const YahtzeePacketGameOver& yp);
    virtual void OnNetworkEventPlayerJoined(Network::SocketPtr sock, const YahtzeePacketPlayerJoined& yp);
    virtual void OnNetworkEventPlayerLeft(Network::SocketPtr sock, const YahtzeePacketPlayerLeft& yp);

private:
    std::string ToString(ScoreCategory score_category);

private:
    Core::Engine* engine_;
};

#endif  // INCLUDED_YAHTZEENETWORKHANDLER
