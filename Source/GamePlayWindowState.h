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

#define WALL_R     (255.0F)
#define WALL_G     (0.0F)
#define WALL_B     (0.0F)
#define WATER_R    (0.0F)
#define WATER_G    (0.0F)
#define WATER_B    (255.0F)

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
   glm::vec3 modelColor;
};
