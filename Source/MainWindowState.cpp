# include "MainWindowState.h"

MainWindowState::MainWindowState()
{
   stateType = MAIN_WINDOW;
}

void MainWindowState::Initialise()
{
   buttonGameName = new Button(glm::vec3(0.0f, 300.0f, 0.0f), glm::vec3(254.0f / 255.0f, 158.0f / 255.0f, 132.0f / 255.0f), glm::vec3(3.0f, 1.0f, 1.0f));
   buttonPlay = new Button(glm::vec3(0.0f, 100.0f, 0.0f), glm::vec3(125.0f / 255.0f, 206.0f / 255.0f, 130.0f / 255.0f));
   buttonCredits = new Button(glm::vec3(0.0f, -100.0f, 0.0f), glm::vec3(232.0f / 255.0f, 226.0f / 255.0f, 136.0f / 255.0f));
   buttonQuit = new Button(glm::vec3(0.0f, -300.0f, 0.0f), glm::vec3(179.0f / 255.0f, 57.0f / 255.0f, 81.0f / 255.0f));

	gameButtonDefaultTexture = new Texture();
	gameButtonDefaultTexture->LoadTexture("../../Textures/creditsButtonDefault.png");
	buttonGameName->SetButtonTextures(gameButtonDefaultTexture);
	//gameButtonClickedTexture = new Texture();
	//gameButtonClickedTexture->LoadTexture("../../Textures/gameButtonClicked.png");

	playButtonDefaultTexture = new Texture();
	playButtonDefaultTexture->LoadTexture("../../Textures/playButtonDefault.png");
	buttonPlay->SetButtonTextures(playButtonDefaultTexture);
	//playButtonClickedTexture = new Texture();
	//playButtonClickedTexture->LoadTexture("../../Textures/playButtonClicked.png");

	creditsButtonDefaultTexture = new Texture();
	creditsButtonDefaultTexture->LoadTexture("../../Textures/creditsButtonDefault.png");
	buttonCredits->SetButtonTextures(creditsButtonDefaultTexture);
	//creditsButtonClickedTexture = new Texture();
	//creditsButtonClickedTexture->LoadTexture("../../Textures/creditsButtonClicked.png");

	quitButtonDefaultTexture = new Texture();
	quitButtonDefaultTexture->LoadTexture("../../Textures/quitButtonDefault.png");
	buttonQuit->SetButtonTextures(quitButtonDefaultTexture);
	//quitButtonClickedTexture = new Texture();
	//quitButtonClickedTexture->LoadTexture("../../Textures/quitButtonClicked.png");	
}

StateType MainWindowState::UpdateState(Window mainWindow)
{
	if (buttonPlay->ClickButton((float)mainWindow.getCursorPosX(), (float)mainWindow.getCursorPosY()) && mainWindow.getButtonClickInfo())
	{
		return GAME_PLAY_WINDOW;
	}
	if (buttonCredits->ClickButton((float)mainWindow.getCursorPosX(), (float)mainWindow.getCursorPosY()) && mainWindow.getButtonClickInfo())
	{
		return CREDITS_WINDOW;
	}
	if (buttonQuit->ClickButton((float)mainWindow.getCursorPosX(), (float)mainWindow.getCursorPosY()) && mainWindow.getButtonClickInfo())
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