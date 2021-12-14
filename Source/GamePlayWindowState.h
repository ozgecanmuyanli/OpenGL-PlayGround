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


class GamePlayWindowState : public State
{
public:
   GamePlayWindowState();

   void Initialise();
   StateType UpdateState(Window mainWindow);
   void RenderState();


private:
   StateType returnValue;
};
