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
   void DrawWall();
   void DrawGrid();
   void GenerateGrids(unsigned int gridWidth, unsigned int gridHeight);
   void setStateCamera(Camera* pCamera);

private:
   Camera* camera;
   Shader* gameShader;
   Texture* mapTexture;
   Mesh* gridObject;
   bool* keys;

   std::vector<float> gridVertices;
   std::vector<GLuint> gridIndices;

   glm::mat4 modelMatrix, projectionMatrix, viewMatrix;
   
};
