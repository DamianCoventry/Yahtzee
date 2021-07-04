#include "stdafx.h"
#include "state.h"
#include "engine.h"

using namespace Core;

State::State(Engine* engine)
: engine_(engine)
{
}

State::~State()
{
}

void State::SetCurrentMenuState(StatePtr state)
{
    engine_->SetCurrentMenuState(state);
}

void State::TransitionMenuState(StatePtr state)
{
    engine_->TransitionMenuState(state);
}

void State::SetCurrentGameState(StatePtr state)
{
    engine_->SetCurrentGameState(state);
}

void State::TransitionGameState(StatePtr state)
{
    engine_->TransitionGameState(state);
}
