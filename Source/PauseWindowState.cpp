# include "PauseWindowState.h"

PauseWindowState::PauseWindowState()
{
   std::cout << " PauseWindowState constructor" << std::endl;
}

void PauseWindowState::Initialise()
{

}

StateType PauseWindowState::UpdateState()
{
   returnValue = PAUSE_WINDOW;
   std::cout << " PauseWindowState class UpdateState function" << std::endl;

   return returnValue;
}

void PauseWindowState::RenderState()
{
   std::cout << " PauseWindowState class UpdateState function" << std::endl;
}