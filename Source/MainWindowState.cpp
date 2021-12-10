# include "MainWindowState.h"

MainWindowState::MainWindowState()
{
   std::cout << " MainWindowState constructor" << std::endl;
}

void MainWindowState::Initialise()
{

}

StateType MainWindowState::UpdateState()
{
   returnValue = MAIN_WINDOW;
   std::cout << " MainWindowState class UpdateState function" << std::endl;

   //if credits button is clicked
   //returnValue = CREDITS_WINDOW
   return returnValue;
}

void MainWindowState::RenderState()
{
   std::cout << " MainWindowState class UpdateState function" << std::endl;
}