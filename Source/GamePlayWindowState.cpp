# include "GamePlayWindowState.h"

GamePlayWindowState::GamePlayWindowState()
{
   std::cout << " GamePlayWindowState constructor" << std::endl;
}

void GamePlayWindowState::Initialise()
{

}

StateType GamePlayWindowState::UpdateState()
{
   returnValue = GAME_PLAY_WINDOW;
   std::cout << " GamePlayWindowState class UpdateState function" << std::endl;

   return returnValue;
}

void GamePlayWindowState::RenderState()
{
   std::cout << " GamePlayWindowState class UpdateState function" << std::endl;
}