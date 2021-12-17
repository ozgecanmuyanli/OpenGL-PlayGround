# include "CreditsWindowState.h"

CreditsWindowState::CreditsWindowState()
{
   stateType = CREDITS_WINDOW;
}

void CreditsWindowState::Initialise()
{
   backButton = new Button(glm::vec3(200.0f, -400.0f, 0.0f), glm::vec3(179.0f / 255.0f, 57.0f / 255.0f, 81.0f / 255.0f));
   creditsHeaderButton = new Button(glm::vec3(0.0f, 400.0f, 0.0f), glm::vec3(254.0f / 255.0f, 158.0f / 255.0f, 132.0f / 255.0f));
   creditsWindowButton = new Button(glm::vec3(0.0f), glm::vec3(247.0f / 255.0f, 231.0f / 255.0f, 190.0f / 255.0f), glm::vec3(2.0f, 12.0f, 1.0f));

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