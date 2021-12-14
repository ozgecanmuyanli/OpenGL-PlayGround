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
   keys = mainWindow.getsKeys();
   if (keys[GLFW_KEY_ESCAPE])
   {
      return PAUSE_WINDOW;
   }

   return stateType;
}

void GamePlayWindowState::RenderState()
{
}