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
#include <assimp/Importer.hpp>

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
   void DrawAxis();
   void DrawGivenModel(Model givenModel);

   void CreateDepthMapForShadow();
   void DebugDepthMap();
   void SetView(glm::mat4 view);
   void SetProjection(glm::mat4 projection);
   void SetCubeConfiguration();
   void SetModelConfiguration();
   void drawGUI();
   void DrawOrthoOfLight();

private:
   Camera* camera;
   Shader* mapShader;
   Shader* cubeShader;
   Shader* axisShader;
   Shader* modelShader;
   Shader* simpleDepthShader;
   Shader* debugDepthMap;
   Shader* debugOrthoOfLight;
   Texture* mapTexture;
   Texture* groundTexture;
   Texture* wallTexture;
   Texture* normalMap;
   Model cubeModel;
   Model sponzaModel;
   Model backpackModel;
   Model orthoCubeModel;
   bool* keys;

   glm::mat4 modelMatrix, projectionMatrix, viewMatrix;
   glm::mat4 sceneProjectionMatrix, sceneViewMatrix;
   glm::mat4 cubeModelMatrix;

   const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
   unsigned int depthMap;
   unsigned int depthMapFBO;
   glm::mat4 lightSpaceMatrix;
   glm::vec3 lightDir;
   float near_plane, far_plane;
   float ortho_left, ortho_right, ortho_bottom, ortho_top;
};
