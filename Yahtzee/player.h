#ifndef INCLUDED_PLAYER
#define INCLUDED_PLAYER

#include <boost/shared_ptr.hpp>
#include <list>
#include <string>
#include "scorecard.h"

class Player
{
public:
    Player(const std::string& name);
    ~Player();

    void SetName(const std::string& name);
    void SetRemote(bool remote);

    const std::string& GetName() const  { return name_; }
    Scorecard& GetScorecard() const     { return (Scorecard&)scorecard_; }
    bool IsRemote() const               { return remote_; }
private:
    std::string name_;
    Scorecard scorecard_;
    bool remote_;
};

typedef boost::shared_ptr<Player> PlayerPtr;
typedef std::list<PlayerPtr> PlayerContainer;
typedef boost::shared_ptr<PlayerContainer> PlayerContainerPtr;

#endif
