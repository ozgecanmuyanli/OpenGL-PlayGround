# include "GamePlayWindowState.h"

void GamePlayWindowState::setStateCamera(Camera* pCamera)
{
   this->camera = pCamera;
}

GamePlayWindowState::GamePlayWindowState()
{
   mapShader = new Shader("../../Shaders/mapShader.vs", "../../Shaders/mapShader.fs");
   cubeShader = new Shader("../../Shaders/cubeShader.vs", "../../Shaders/cubeShader.fs");

   modelMatrix = glm::mat4(1.0f);
   viewMatrix = glm::mat4(1.0f);
   cubeModelMatrix = glm::mat4(1.0f);

   stateType = GAME_PLAY_WINDOW;
}

void GamePlayWindowState::Initialise()
{
   cubeModel = new Model("../../Models/cube1x1.obj");

   mapTexture = new Texture();
   mapTexture->LoadTextureCPU("../../Textures/map.bmp");
   mapTexture->SetMapTexture();

   wallTexture = new Texture();
   wallTexture->LoadTextureGPU("../../Textures/wall.png");
   woodTexture = new Texture();
   woodTexture->LoadTextureGPU("../../Textures/wood.png");
   groundTexture = new Texture();
   groundTexture->LoadTextureGPU("../../Textures/sponza_floor_a_diff.png");
}

StateType GamePlayWindowState::UpdateState(Window mainWindow, GLfloat deltaTime)
{
   // PROJECTION
   projectionMatrix = glm::perspective(glm::radians(FOV),
      (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(), NEAR, FAR);

   viewMatrix = this->camera->calculateViewMatrix();
   keys = mainWindow.getsKeys();

   if (keys[GLFW_KEY_ESCAPE])
   {
      return PAUSE_WINDOW;
   }

   return stateType;
}

void GamePlayWindowState::RenderState()
{
   DrawMap();
   DrawEntity(wallTexture, mapTexture->waterEntityTranslateValues);
   DrawEntity(woodTexture, mapTexture->wallEntityTranslateValues);
}

void GamePlayWindowState::DrawEntity(Texture* entityTexture, std::vector<glm::vec2> entity)
{
   entityTexture->ActivateTexture(GL_TEXTURE0);
   entityTexture->BindTexture();

   cubeShader->use();
   cubeShader->setMat4("projection", projectionMatrix);
   cubeShader->setMat4("view", viewMatrix);
   cubeShader->setInt("entityTexture", 0);

   for (unsigned int i = 0; i < entity.size(); i++)
   {
      cubeModelMatrix = glm::mat4(1.0f);
      cubeModelMatrix = glm::translate(cubeModelMatrix, glm::vec3(entity[i].x,
                                                                  1.0f / mapTexture->GetTextureWidth(),
                                                                  entity[i].y));
      cubeModelMatrix = glm::scale(cubeModelMatrix, glm::vec3(1.0f / mapTexture->GetTextureWidth()));
      cubeShader->setMat4("model", cubeModelMatrix);
      cubeModel->Draw(*cubeShader);
   }
}

void GamePlayWindowState::DrawMap()
{
   static GLuint gridVAO;
   if (!gridVAO)
   {
      glGenVertexArrays(1, &gridVAO);
   }
   glBindVertexArray(gridVAO);

   groundTexture->ActivateTexture(GL_TEXTURE0);
   groundTexture->BindTexture();
   mapShader->use();

   // TO SEE ORIGINAL MAP TEXTURE
   //mapTexture->ActivateTexture(GL_TEXTURE0);
   //mapTexture->BindTexture();
   mapShader->setInt("mapTexture", 0);

   mapShader->setMat4("modelMatrix", modelMatrix);
   mapShader->setMat4("projectionMatrix", projectionMatrix);
   mapShader->setMat4("viewMatrix", viewMatrix);

   glDrawArrays(GL_TRIANGLES, 0, 6);
   glBindVertexArray(0);
}
