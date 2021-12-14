# include "GamePlayWindowState.h"

GamePlayWindowState::GamePlayWindowState()
{
   stateType = GAME_PLAY_WINDOW;
}

void GamePlayWindowState::Initialise()
{

}

StateType GamePlayWindowState::UpdateState(Window mainWindow)
{

   return stateType;
}

void GamePlayWindowState::RenderState()
{
}