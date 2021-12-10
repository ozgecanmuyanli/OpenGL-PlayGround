#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "stb_image.h"

#include "Window.h"
#include "shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Camera.h"
#include "Model.h"
#include "Button.h"
#include "State.h"
#include "MainWindowState.h"
#include "CreditsWindowState.h"
#include "GamePlayWindowState.h"
#include "PauseWindowState.h"
#include "enum.h"

//VARIABLES
#define SCREEN_CLEAR_RED   (82.0F / 255.0F)
#define SCREEN_CLEAR_GREEN (9.0F / 255.0F)
#define SCREEN_CLEAR_BLUE  (9.0F / 255.0F)
#define SCR_WIDTH          (1600)
#define SCR_HEIGHT         (900)

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
Window mainWindow;
Camera camera;
Button* buttonGameName;
Button* buttonPlay;
Button* buttonCredits;
Button* buttonQuit;
Texture* playButtonDefaultTexture;
Texture* creditsButtonDefaultTexture;
Texture* quitButtonDefaultTexture;
Texture* gameButtonDefaultTexture;
Texture* playButtonClickedTexture;
Texture* creditsButtonClickedTexture;
Texture* quitButtonClickedTexture;
Texture* gameButtonClickedTexture;

State* currentState;
MainWindowState* mainWindowState;
CreditsWindowState* creditsWindowState;
GamePlayWindowState* gamePlayWindowState;
PauseWindowState* pauseWindowState;


// FUNCTIONS
void MainLoop();
void Init();
void DrawMainMenu();

void main()
{
	mainWindow = Window(SCR_WIDTH, SCR_HEIGHT);
	mainWindow.Initialise();
	glClearColor(SCREEN_CLEAR_RED, SCREEN_CLEAR_GREEN, SCREEN_CLEAR_BLUE, 1.0f);

	Init();
	MainLoop();
}

void Init()
{
	buttonGameName = new Button(glm::vec3(0.0f, 300.0f, 0.0f), glm::vec3(46.0f / 255.0f, 42.0f / 255.0f, 42.0f / 255.0f), glm::vec3(3.0f, 1.0f, 1.0f));
	buttonPlay = new Button(glm::vec3(0.0f, 100.0f, 0.0f), glm::vec3(16.0f / 255.0f, 121.0f / 255.0f, 34.0f / 255.0f));
	buttonCredits = new Button(glm::vec3(0.0f, -100.0f, 0.0f), glm::vec3(230.0f / 255.0f, 230.0f / 255.0f, 70.0f / 255.0f));
	buttonQuit = new Button(glm::vec3(0.0f, -300.0f, 0.0f), glm::vec3(182.0f / 255.0f, 27.0f / 255.0f, 27.0f / 255.0f));

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

	mainWindowState = new MainWindowState();
	creditsWindowState = new CreditsWindowState();
	gamePlayWindowState = new GamePlayWindowState();
	pauseWindowState = new PauseWindowState();

	currentState = mainWindowState;
}

void MainLoop()
{
	while (!mainWindow.getShouldClose())
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		bool* keys = mainWindow.getsKeys();
		glClearColor(SCREEN_CLEAR_RED, SCREEN_CLEAR_GREEN, SCREEN_CLEAR_BLUE, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (MAIN_WINDOW == currentState->UpdateState())
		{
			std::cout << "mainwindow render operations here" << std::endl;
		}
		else if (CREDITS_WINDOW == currentState->UpdateState())
		{
			std::cout << "creditswindow render operations here" << std::endl;
		}
		else if (GAME_PLAY_WINDOW == currentState->UpdateState())
		{
			std::cout << "gameplaywindow render operations here" << std::endl;
		}
		else if (PAUSE_WINDOW == currentState->UpdateState())
		{
			std::cout << "pausewindow render operations here" << std::endl;
		}


		if (buttonGameName->ClickButton((float)mainWindow.getCursorPosX(), (float)mainWindow.getCursorPosY()) && mainWindow.getButtonClickInfo())
		{
		}

		if (buttonPlay->ClickButton((float)mainWindow.getCursorPosX(), (float)mainWindow.getCursorPosY()) && mainWindow.getButtonClickInfo())
		{
		}

		if (buttonCredits->ClickButton((float)mainWindow.getCursorPosX(), (float)mainWindow.getCursorPosY()) && mainWindow.getButtonClickInfo())
		{
		}

		if (buttonQuit->ClickButton((float)mainWindow.getCursorPosX(), (float)mainWindow.getCursorPosY()) && mainWindow.getButtonClickInfo())
		{
			glfwSetWindowShouldClose(mainWindow.getWindow(), GL_TRUE);
		}

		DrawMainMenu();

		mainWindow.swapBuffers();
	}
}

void DrawMainMenu()
{


	buttonGameName->DrawButton();
	buttonPlay->DrawButton();
	buttonCredits->DrawButton();
	buttonQuit->DrawButton();
}
