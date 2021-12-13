# include "MainWindowState.h"

MainWindowState::MainWindowState()
{
   std::cout << " MainWindowState constructor" << std::endl;
   stateType = MAIN_WINDOW;
}

void MainWindowState::Initialise()
{

}

StateType MainWindowState::UpdateState()
{
   std::cout << " MainWindowState class UpdateState function" << std::endl;

   //if credits button is clicked
   //return CREDITS_WINDOW

   return stateType;
}

void MainWindowState::RenderState()
{
   std::cout << " MainWindowState class UpdateState function" << std::endl;
}