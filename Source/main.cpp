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
#define SCREEN_CLEAR_RED   (0.0F / 255.0F)
#define SCREEN_CLEAR_GREEN (159.0F / 255.0F)
#define SCREEN_CLEAR_BLUE  (146.0F / 255.0F)
#define SCR_WIDTH          (1600)
#define SCR_HEIGHT         (900)

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
Window* cWindow;
Camera* camera;

State* currentState;
StateType returnedState;
MainWindowState* mainWindowState;
CreditsWindowState* creditsWindowState;
GamePlayWindowState* gamePlayWindowState;
PauseWindowState* pauseWindowState;


// FUNCTIONS
void MainLoop();
void Init();

void main()
{
   cWindow = new Window(SCR_WIDTH, SCR_HEIGHT);
   cWindow->Initialise();
   glClearColor(SCREEN_CLEAR_RED, SCREEN_CLEAR_GREEN, SCREEN_CLEAR_BLUE, 1.0f);

   Init();
   MainLoop();
}

void Init()
{
   camera = new Camera(glm::vec3(0.0f, 0.1f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 0.005f, 0.1f);

   mainWindowState = new MainWindowState();
   creditsWindowState = new CreditsWindowState();
   gamePlayWindowState = new GamePlayWindowState();
   pauseWindowState = new PauseWindowState();

   cWindow->setWindowCamera(camera);
   mainWindowState->Initialise();
   creditsWindowState->Initialise();
   gamePlayWindowState->Initialise();
   gamePlayWindowState->setStateCamera(camera);
   pauseWindowState->Initialise();

   currentState = mainWindowState;
}

void MainLoop()
{
   while (!cWindow->getShouldClose())
   {
      // IMGUI
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();
      // END IMGUI

      float currentFrame = glfwGetTime();
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;
      glfwPollEvents();
      camera->keyControl(cWindow->getsKeys(), deltaTime);
      camera->ApplyHeadBob(deltaTime);
      bool* keys = cWindow->getsKeys();
      glClearColor(SCREEN_CLEAR_RED, SCREEN_CLEAR_GREEN, SCREEN_CLEAR_BLUE, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      returnedState = currentState->UpdateState(*cWindow, deltaTime);

      if (returnedState != currentState->stateType)
      {
         if (MAIN_WINDOW == returnedState)
         {
            currentState = mainWindowState;
         }
         else if (CREDITS_WINDOW == returnedState)
         {
            currentState = creditsWindowState;
         }
         else if (GAME_PLAY_WINDOW == returnedState)
         {
            currentState = gamePlayWindowState;
         }
         else if (PAUSE_WINDOW == returnedState)
         {
            currentState = pauseWindowState;
         }
      }

      currentState->RenderState();

      //cWindow->DrawGUI();
      camera->drawCameraGUI();
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

      cWindow->swapBuffers();
   }
}
