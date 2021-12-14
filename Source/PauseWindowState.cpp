# include "PauseWindowState.h"

PauseWindowState::PauseWindowState()
{
   stateType = PAUSE_WINDOW;
}

void PauseWindowState::Initialise()
{
   resumeButton = new Button(glm::vec3(0.0f, 100.0f, 0.0f), glm::vec3(254.0f / 255.0f, 158.0f / 255.0f, 132.0f / 255.0f));
   backToMainManuButton = new Button(glm::vec3(0.0f, -100.0f, 0.0f), glm::vec3(179.0f / 255.0f, 57.0f / 255.0f, 81.0f / 255.0f));

   resumeButtonTexture = new Texture();
   resumeButtonTexture->LoadTexture("../../Textures/resume.png");
   resumeButton->SetButtonTextures(resumeButtonTexture);

   backToMainManuButtonTexture = new Texture();
   backToMainManuButtonTexture->LoadTexture("../../Textures/backToMainMenu.png");
   backToMainManuButton->SetButtonTextures(backToMainManuButtonTexture);
}

StateType PauseWindowState::UpdateState(Window mainWindow)
{
   if (backToMainManuButton->ClickButton((float)mainWindow.getCursorPosX(), (float)mainWindow.getCursorPosY()) && mainWindow.getButtonClickInfo())
   {
      return MAIN_WINDOW;
   }
   if (resumeButton->ClickButton((float)mainWindow.getCursorPosX(), (float)mainWindow.getCursorPosY()) && mainWindow.getButtonClickInfo())
   {
      return GAME_PLAY_WINDOW;
   }
   return stateType;
}

void PauseWindowState::RenderState()
{
   resumeButton->DrawButton();
   backToMainManuButton->DrawButton();
}