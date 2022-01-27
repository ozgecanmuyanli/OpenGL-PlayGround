# include "GamePlayWindowState.h"

void GamePlayWindowState::setStateCamera(Camera* pCamera)
{
   this->camera = pCamera;
}

GamePlayWindowState::GamePlayWindowState()
{
   mapShader = new Shader("../../Shaders/mapShader.vs", "../../Shaders/mapShader.fs");
   cubeShader = new Shader("../../Shaders/cubeShader.vs", "../../Shaders/cubeShader.fs");
   axisShader = new Shader("../../Shaders/axisShader.vs", "../../Shaders/axisShader.fs");
   modelShader = new Shader("../../Shaders/modelShader.vs", "../../Shaders/modelShader.fs");

   modelMatrix = glm::mat4(1.0f);
   viewMatrix = glm::mat4(1.0f);
   cubeModelMatrix = glm::mat4(1.0f);

   stateType = GAME_PLAY_WINDOW;
}

void GamePlayWindowState::Initialise()
{
   cubeModel = Model("../../Models/cube1x1.obj");
   sponzaModel = Model("../../Models/sponza/sponza.obj");
   backpackModel = Model("../../Models/backpack/backpack.obj");

   mapTexture = new Texture();
   mapTexture->LoadTextureCPU("../../Textures/map.bmp");
   mapTexture->SetMapTexture();

   wallTexture = new Texture();
   wallTexture->LoadTextureGPU("../../Textures/brickwall.jpg");
   groundTexture = new Texture();
   groundTexture->LoadTextureGPU("../../Textures/sponza_floor_a_diff.png");

   normalMap = new Texture();
   normalMap->LoadTextureGPU("../../Textures/brickwall_normal.jpg");
}

StateType GamePlayWindowState::UpdateState(Window mainWindow, GLfloat deltaTime)
{
   glClearColor(SKY_CLEAR_RED, SKY_CLEAR_GREEN, SKY_CLEAR_BLUE, 1.0f);
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
   //DrawMap();
   DrawAxis();
   DrawEntity(wallTexture, mapTexture->waterEntityTranslateValues);
   DrawEntity(wallTexture, mapTexture->wallEntityTranslateValues);

   modelShader->use();
   modelShader->setMat4("projection", projectionMatrix);
   modelShader->setMat4("view", viewMatrix);
   modelMatrix = glm::mat4(1.0f);
  // modelMatrix = glm::scale(modelMatrix, glm::vec3(0.002f));
   modelShader->setMat4("model", modelMatrix);
   modelShader->setVec3("viewPos", this->camera->getCameraPosition());
   modelShader->setVec3("lightPos", glm::vec3(0.0f, 0.5f, 0.5f));
   //sponzaModel.Draw(*modelShader);
   backpackModel.Draw(*modelShader);
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
      //cubeModel.Draw(*cubeShader);
   }

   cubeShader->setVec3("lightPos", 0.271438479, 0.199558944, 0.912041306);
   cubeShader->setVec3("viewPos", this->camera->getCameraPosition());
   normalMap->ActivateTexture(GL_TEXTURE1);
   normalMap->BindTexture();
   cubeShader->setInt("normalMap", 1);
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
   mapShader->setVec3("lightPos", 0.271438479, 0.199558944, 0.912041306);
   mapShader->setVec3("viewPos", this->camera->getCameraPosition());

   glDrawArrays(GL_TRIANGLES, 0, 6);
   glBindVertexArray(0);
}

void GamePlayWindowState::DrawAxis()
{
   axisShader->use();
   axisShader->setMat4("projectionMatrix", projectionMatrix);
   axisShader->setMat4("viewMatrix", viewMatrix);

   static GLuint axisVAO;
   if (!axisVAO)
   {
      glGenVertexArrays(1, &axisVAO);
   }
   glBindVertexArray(axisVAO);
   glDrawArrays(GL_LINES, 0, 6);
   glBindVertexArray(0);
}
