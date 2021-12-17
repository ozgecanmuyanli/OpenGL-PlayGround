#pragma once
#include "stdio.h"
#include<iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Button.h"
#include "State.h"
#include "enum.h"


class PauseWindowState : public State
{
public:
   PauseWindowState();

   void Initialise();
   StateType UpdateState(Window mainWindow);
   void RenderState();


private:
   Button* resumeButton;
   Button* backToMainManuButton;
   Texture* resumeButtonTexture;
   Texture* backToMainManuButtonTexture;
   Texture* clickEffectTexture;
};
