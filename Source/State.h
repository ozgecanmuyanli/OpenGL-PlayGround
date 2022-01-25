#pragma once
#include "stdio.h"
#include<iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Window.h"
#include "enum.h"

// COLORS of the game
#define ORANGE_R (254.0f / 255.0f)
#define ORANGE_G (158.0f / 255.0f)
#define ORANGE_B (132.0f / 255.0f)
#define YELLOW_R (247.0f / 255.0f)
#define YELLOW_G (231.0f / 255.0f)
#define YELLOW_B (190.0f / 255.0f)
#define PINK_R   (179.0f / 255.0f)
#define PINK_G   (57.0f / 255.0f)
#define PINK_B   (81.0f / 255.0f)
#define GREEN_R  (125.0f / 255.0f)
#define GREEN_G  (206.0f / 255.0f)
#define GREEN_B  (130.0f / 255.0f)

#define SKY_CLEAR_RED   (111.0F / 255.0F)
#define SKY_CLEAR_GREEN (190.0F / 255.0F)
#define SKY_CLEAR_BLUE  (255.0F / 255.0F)

#define FOV      (45.0F)
#define NEAR     (0.1F)
#define FAR      (100.0F)

class State
{
public:
   State();

   virtual StateType UpdateState(Window mainWindow, GLfloat deltaTime);
   virtual void RenderState();
   StateType stateType;

private:
   
};
