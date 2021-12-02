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

class Button
{
public:
   Button(glm::vec3 buttonPosition = glm::vec3(0.0f), glm::vec3 buttonColor = glm::vec3(1.0f), glm::vec3 buttonScale = glm::vec3(1.0f));

   void InitialiseButton();
   void DrawButton();
   bool ClickButton(GLfloat xCursorPos, GLfloat yCursorPos);
   void CreateBoundingBox();
   void SetButtonTextures(Texture* buttonTtexture);

private:
   Shader* buttonShader;
   Texture* buttonTexture;
   unsigned int buttonWidth, buttonHeight;
   unsigned int numOfIndices, numOfVertices;
   unsigned int buttonVAO, buttonIBO, buttonVBO;
   glm::vec3 buttonPosition, buttonScale, buttonColor;
   glm::mat4 projection, model;
   bool isButtonClicked;
   glm::vec4 boundingBoxLeftBottom, boundingBoxRightTop;
};
