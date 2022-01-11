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
#include "Mesh.h"
#include "Model.h"
#include "Camera.h"

class GamePlayWindowState : public State
{
public:
   GamePlayWindowState();

   void Initialise();
   StateType UpdateState(Window mainWindow, GLfloat deltaTime);
   void RenderState();
   void DrawMap();
   void DrawEntity(Texture* texture, std::vector<glm::vec2> entity);
   void setStateCamera(Camera* pCamera);

private:
   Camera* camera;
   Shader* mapShader;
   Shader* cubeShader;
   Texture* mapTexture;
   Texture* groundTexture;
   Texture* woodTexture;
   Texture* wallTexture;
   Model* cubeModel;
   bool* keys;

   glm::mat4 modelMatrix, projectionMatrix, viewMatrix;
   glm::mat4 cubeModelMatrix;
};
