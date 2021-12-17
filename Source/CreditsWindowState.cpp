# include "CreditsWindowState.h"

CreditsWindowState::CreditsWindowState()
{
   stateType = CREDITS_WINDOW;
}

void CreditsWindowState::Initialise()
{
   backButton = new Button(glm::vec3(200.0f, -400.0f, 0.0f), glm::vec3(PINK_R, PINK_G, PINK_B));
   creditsHeaderButton = new Button(glm::vec3(0.0f, 400.0f, 0.0f), glm::vec3(ORANGE_R, ORANGE_G, ORANGE_B));
   creditsWindowButton = new Button(glm::vec3(0.0f), glm::vec3(YELLOW_R, YELLOW_G, YELLOW_B), glm::vec3(2.0f, 12.0f, 1.0f));

   clickEffectTexture = new Texture();
   clickEffectTexture->LoadTexture("../../Textures/clickEffect.png");

   backButtonTexture = new Texture();
   backButtonTexture->LoadTexture("../../Textures/backButton.png");
   backButton->SetButtonTextures(backButtonTexture, clickEffectTexture);

   creditsHeaderButtonTexture = new Texture();
   creditsHeaderButtonTexture->LoadTexture("../../Textures/creditsButtonDefault.png");
   creditsHeaderButton->SetButtonTextures(creditsHeaderButtonTexture, creditsHeaderButtonTexture);

   creditsWindowButtonTexture = new Texture();
   creditsWindowButtonTexture->LoadTexture("../../Textures/creditsWindow.png");
   creditsWindowButton->SetButtonTextures(creditsWindowButtonTexture, creditsWindowButtonTexture);
}

StateType CreditsWindowState::UpdateState(Window mainWindow)
{
   backButton->CheckIsMouseInside((float)mainWindow.getCursorPosX(), (float)mainWindow.getCursorPosY());
   backButton->HandleMouseInput(mainWindow.getWindow());

   if (backButton->IsButtonClicked())
   {
      return MAIN_WINDOW;
   }

   return stateType;
}

void CreditsWindowState::RenderState()
{
   backButton->DrawButton();
   creditsHeaderButton->DrawButton();
   creditsWindowButton->DrawButton();
}