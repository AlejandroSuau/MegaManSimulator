#include "Screen.h"
#include "StateMachineMngr.h"

// TODO: set the initial state here
Screen::Screen()
{
}

void Screen::setState(StateMachineMngr& newState)
{
    currentState->exit(this);  // do stuff before we change state
    currentState = &newState;  // change state
    currentState->enter(this); // do stuff after we change state
}

void Screen::toggle()
{
    // Delegate the task of determining the next state to the current state!
    currentState->toggle(this);
}