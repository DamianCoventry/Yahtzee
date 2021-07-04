#ifndef INCLUDED_CORE_STATE
#define INCLUDED_CORE_STATE

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "timeouteventhandler.h"
#include "../Graphics/sprite.h"

namespace Graphics
{

class Device;
typedef boost::shared_ptr<Device> DevicePtr;

}       // namespace Graphics

namespace Core
{

class Engine;

class State;
typedef boost::shared_ptr<State> StatePtr;

class State :
    public boost::enable_shared_from_this<State>,
    public Graphics::SpriteEventHandler,
    public TimeoutEventHandler
{
public:
    State(Engine* engine);
    virtual ~State();

    void SetCurrentMenuState(StatePtr state);
    void SetCurrentGameState(StatePtr state);

    void TransitionMenuState(StatePtr state);
    void TransitionGameState(StatePtr state);

    virtual void SetupLists() = 0;

    virtual void OnBegin() {}
    virtual void OnEnd() {}
    virtual void DoFrameLogic(float time_delta) = 0;
    virtual void DrawFrame(float time_delta) = 0;

    virtual void OnWindowActivated() {}
    virtual void OnWindowDeactivated() {}

protected:
    Engine* GetEngine() const { return engine_; }

private:
    Engine* engine_;
};

}       // namespace Core

#endif  // INCLUDED_CORE_STATE
