# include "PauseWindowState.h"

PauseWindowState::PauseWindowState()
{
   stateType = PAUSE_WINDOW;
}

void PauseWindowState::Initialise()
{

}

StateType PauseWindowState::UpdateState(Window mainWindow)
{

   return stateType;
}

void PauseWindowState::RenderState()
{
}