# include "CreditsWindowState.h"

CreditsWindowState::CreditsWindowState()
{
   std::cout << " CreditsWindowState constructor" << std::endl;
}

void CreditsWindowState::Initialise()
{

}

StateType CreditsWindowState::UpdateState()
{
   returnValue = CREDITS_WINDOW;
   std::cout << " CreditsWindowState class UpdateState function" << std::endl;

   // if backButton clicked returnValue=MAINWINDOW

   return returnValue;
}

void CreditsWindowState::RenderState()
{
   std::cout << " CreditsWindowState class UpdateState function" << std::endl;
}