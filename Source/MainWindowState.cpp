# include "MainWindowState.h"

MainWindowState::MainWindowState()
{
   stateType = MAIN_WINDOW;
}

void MainWindowState::Initialise()
{
   buttonGameName = new Button(glm::vec3(0.0f, 300.0f, 0.0f), glm::vec3(ORANGE_R, ORANGE_G, ORANGE_B), glm::vec3(3.0f, 1.0f, 1.0f));
   buttonPlay = new Button(glm::vec3(0.0f, 100.0f, 0.0f), glm::vec3(GREEN_R, GREEN_G, GREEN_B));
   buttonCredits = new Button(glm::vec3(0.0f, -100.0f, 0.0f), glm::vec3(YELLOW_R, YELLOW_G, YELLOW_B));
   buttonQuit = new Button(glm::vec3(0.0f, -300.0f, 0.0f), glm::vec3(PINK_R, PINK_G, PINK_B));

	clickEffectTexture = new Texture();
	clickEffectTexture->LoadTexture("../../Textures/clickEffect.png");

	gameButtonTexture = new Texture();
	gameButtonTexture->LoadTexture("../../Textures/creditsButtonDefault.png");
	buttonGameName->SetButtonTextures(gameButtonTexture, gameButtonTexture);

	playButtonTexture = new Texture();
	playButtonTexture->LoadTexture("../../Textures/playButtonDefault.png");
	buttonPlay->SetButtonTextures(playButtonTexture, clickEffectTexture);

	creditsButtonTexture = new Texture();
	creditsButtonTexture->LoadTexture("../../Textures/creditsButtonDefault.png");
	buttonCredits->SetButtonTextures(creditsButtonTexture, clickEffectTexture);


	quitButtonTexture = new Texture();
	quitButtonTexture->LoadTexture("../../Textures/quitButtonDefault.png");
	buttonQuit->SetButtonTextures(quitButtonTexture, clickEffectTexture);
}

StateType MainWindowState::UpdateState(Window mainWindow)
{

	buttonPlay->CheckIsMouseInside((float)mainWindow.getCursorPosX(), (float)mainWindow.getCursorPosY());
	buttonPlay->HandleMouseInput(mainWindow.getWindow());
	
	buttonCredits->CheckIsMouseInside((float)mainWindow.getCursorPosX(), (float)mainWindow.getCursorPosY());
	buttonCredits->HandleMouseInput(mainWindow.getWindow());

	buttonQuit->CheckIsMouseInside((float)mainWindow.getCursorPosX(), (float)mainWindow.getCursorPosY());
	buttonQuit->HandleMouseInput(mainWindow.getWindow());

	if (buttonPlay->IsButtonClicked())
	{
		return GAME_PLAY_WINDOW;
	}
	if (buttonCredits->IsButtonClicked())
	{
		return CREDITS_WINDOW;
	}
	if (buttonQuit->IsButtonClicked())
	{
		glfwSetWindowShouldClose(mainWindow.getWindow(), GL_TRUE);
	}

   return stateType;
}

void MainWindowState::RenderState()
{
	buttonGameName->DrawButton();
	buttonPlay->DrawButton();
	buttonCredits->DrawButton();
	buttonQuit->DrawButton();
}