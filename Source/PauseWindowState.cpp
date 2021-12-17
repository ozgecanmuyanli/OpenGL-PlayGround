# include "PauseWindowState.h"

PauseWindowState::PauseWindowState()
{
   stateType = PAUSE_WINDOW;
}

void PauseWindowState::Initialise()
{
   resumeButton = new Button(glm::vec3(0.0f, 100.0f, 0.0f), glm::vec3(254.0f / 255.0f, 158.0f / 255.0f, 132.0f / 255.0f));
   backToMainManuButton = new Button(glm::vec3(0.0f, -100.0f, 0.0f), glm::vec3(179.0f / 255.0f, 57.0f / 255.0f, 81.0f / 255.0f));

   clickEffectTexture = new Texture();
   clickEffectTexture->LoadTexture("../../Textures/clickEffect.png");

   resumeButtonTexture = new Texture();
   resumeButtonTexture->LoadTexture("../../Textures/resume.png");
   resumeButton->SetButtonTextures(resumeButtonTexture, clickEffectTexture);

   backToMainManuButtonTexture = new Texture();
   backToMainManuButtonTexture->LoadTexture("../../Textures/backToMainMenu.png");
   backToMainManuButton->SetButtonTextures(backToMainManuButtonTexture, clickEffectTexture);
}

StateType PauseWindowState::UpdateState(Window mainWindow)
{
   backToMainManuButton->CheckIsMouseInside((float)mainWindow.getCursorPosX(), (float)mainWindow.getCursorPosY());
   backToMainManuButton->HandleMouseInput(mainWindow.getWindow());

   resumeButton->CheckIsMouseInside((float)mainWindow.getCursorPosX(), (float)mainWindow.getCursorPosY());
   resumeButton->HandleMouseInput(mainWindow.getWindow());

   if (backToMainManuButton->IsButtonClicked())
   {
      return MAIN_WINDOW;
   }
   if (resumeButton->IsButtonClicked())
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