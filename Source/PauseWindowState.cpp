# include "PauseWindowState.h"

PauseWindowState::PauseWindowState()
{
   stateType = PAUSE_WINDOW;
}

void PauseWindowState::Initialise()
{
   resumeButton = new Button(glm::vec3(0.0f, 100.0f, 0.0f), glm::vec3(ORANGE_R, ORANGE_G, ORANGE_B));
   backToMainManuButton = new Button(glm::vec3(0.0f, -100.0f, 0.0f), glm::vec3(PINK_R, PINK_G, PINK_B));

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