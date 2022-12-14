#pragma once
#include "stdio.h"
#include<iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Button.h"
#include "Texture.h"
#include "State.h"
#include "enum.h"


class MainWindowState : public State
{
public:
   MainWindowState();

   void Initialise();
   StateType UpdateState(Window mainWindow, GLfloat deltaTime);
   void RenderState();


private:
   Button* buttonGameName;
   Button* buttonPlay;
   Button* buttonCredits;
   Button* buttonQuit;

   Texture* clickEffectTexture;
   Texture* playButtonTexture;
   Texture* creditsButtonTexture;
   Texture* quitButtonTexture;
   Texture* gameButtonTexture;
};
