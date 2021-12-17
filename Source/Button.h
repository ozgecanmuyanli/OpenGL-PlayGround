#pragma once
#include "stdio.h"
#include<iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"
#include "shader.h"
#include "Texture.h"
#include "Window.h"

class Button
{
public:
   Button(glm::vec3 buttonPosition = glm::vec3(0.0f), glm::vec3 buttonColor = glm::vec3(1.0f), glm::vec3 buttonScale = glm::vec3(1.0f));

   void InitialiseButton();
   void DrawButton();
   void CheckIsMouseInside(GLfloat xCursorPos, GLfloat yCursorPos);
   void CreateBoundingBox();
   void SetButtonTextures(Texture* textureRelease, Texture* texturePress);

   void HandleMouseInput(GLFWwindow* mainWindow);
   bool IsButtonClicked() { return isButtonClicked; }

private:
   Shader* buttonShader;
   Texture* buttonTextureRelease;
   Texture* buttonTexturePress;
   unsigned int buttonWidth, buttonHeight;
   unsigned int numOfIndices, numOfVertices;
   unsigned int buttonVAO, buttonIBO, buttonVBO;
   glm::vec3 buttonPosition, buttonScale, buttonColor;
   glm::mat4 projection, model;
   glm::vec4 boundingBoxLeftBottom, boundingBoxRightTop;

   bool isButtonPressed = false;
   bool isButtonClicked = false;
   bool outPressed = false;
   //bool inPressed = false;
   bool inside, outside;
};
