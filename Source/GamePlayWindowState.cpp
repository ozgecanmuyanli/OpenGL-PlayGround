# include "GamePlayWindowState.h"

void GamePlayWindowState::setStateCamera(Camera* pCamera)
{
   this->camera = pCamera;
}

void GamePlayWindowState::drawGUI()
{
   if (ImGui::Begin("Light Properties"))
   {
      ImGui::DragFloat("Light Direction x", &lightDir.x, 0.5f, -FLT_MAX, +FLT_MAX, "%.3f");
      ImGui::DragFloat("Light Direction y", &lightDir.y, 0.5f, -FLT_MAX, +FLT_MAX, "%.3f");
      ImGui::DragFloat("Light Direction z", &lightDir.z, 0.5f, -FLT_MAX, +FLT_MAX, "%.3f");
   }
   ImGui::End();
}

GamePlayWindowState::GamePlayWindowState()
{
   mapShader = new Shader("../../Shaders/mapShader.vs", "../../Shaders/mapShader.fs");
   cubeShader = new Shader("../../Shaders/cubeShader.vs", "../../Shaders/cubeShader.fs");
   axisShader = new Shader("../../Shaders/axisShader.vs", "../../Shaders/axisShader.fs");
   modelShader = new Shader("../../Shaders/modelShader.vs", "../../Shaders/modelShader.fs");
   simpleDepthShader = new Shader("../../Shaders/simpleDepthShader.vs", "../../Shaders/simpleDepthShader.fs");
   debugDepthMap = new Shader("../../Shaders/debugShadowDepthMap.vs", "../../Shaders/debugShadowDepthMap.fs");
   debugOrthoOfLight = new Shader("../../Shaders/debugOrthoOfLight.vs", "../../Shaders/debugOrthoOfLight.fs");

   modelMatrix = glm::mat4(1.0f);
   viewMatrix = glm::mat4(1.0f);
   cubeModelMatrix = glm::mat4(1.0f); //ground
   lightDir = glm::vec3(10.0f);

   stateType = GAME_PLAY_WINDOW;
}

void GamePlayWindowState::Initialise()
{
   cubeModel = Model("../../Models/cube/cube1x1.obj");
   sponzaModel = Model("../../Models/sponza/sponza.obj");
   backpackModel = Model("../../Models/backpack/backpack.obj");

   mapTexture = new Texture();
   mapTexture->LoadTextureCPU("../../Textures/map.bmp");
   mapTexture->SetMapTexture();

   wallTexture = new Texture();
   wallTexture->LoadTextureGPU("../../Textures/newbrickwall.png");
   groundTexture = new Texture();
   groundTexture->LoadTextureGPU("../../Textures/sponza_floor_a_diff.png");

   normalMap = new Texture();
   normalMap->LoadTextureGPU("../../Textures/brickwall_normal.jpg");

   near_plane = 0.001f;
   far_plane = 1.0f;
   ortho_left = -0.5f;
   ortho_right = 0.5f;
   ortho_bottom = -0.5f;
   ortho_top = 0.5f;

   CreateDepthMapForShadow();
}

StateType GamePlayWindowState::UpdateState(Window mainWindow, GLfloat deltaTime)
{
   // PROJECTION
   projectionMatrix = glm::perspective(glm::radians(FOV),
      (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(), NEAR, FAR);
   sceneProjectionMatrix = projectionMatrix;

   viewMatrix = this->camera->calculateViewMatrix();
   sceneViewMatrix = viewMatrix;

   keys = mainWindow.getsKeys();

   if (keys[GLFW_KEY_ESCAPE])
   {
      return PAUSE_WINDOW;
   }
   else if (keys[GLFW_KEY_J])
   {
      modelShader = new Shader("../../Shaders/modelShader.vs", "../../Shaders/modelShader.fs");
   }

   return stateType;
}

void GamePlayWindowState::DrawOrthoOfLight()
{
   modelMatrix = glm::mat4(1.0f);

   debugOrthoOfLight->use();

   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   debugOrthoOfLight->setMat4("projection", sceneProjectionMatrix);
   debugOrthoOfLight->setMat4("view", sceneViewMatrix);

   modelMatrix = glm::scale(modelMatrix, glm::vec3((ortho_right - ortho_left) / 2.0f, 
                                                   (ortho_top - ortho_bottom) / 2.0f, 
                                                   (far_plane - near_plane) / 2.0f));
   debugOrthoOfLight->setMat4("model", modelMatrix);

   cubeModel.Draw(*debugOrthoOfLight);

   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   modelMatrix = glm::mat4(1.0f);
}

void GamePlayWindowState::RenderState()
{
   drawGUI();
   
   // 1. first render to depth map
   glm::mat4 lightProjection = glm::ortho(ortho_left, 
                                          ortho_right, 
                                          ortho_bottom, 
                                          ortho_top,
                                          near_plane, 
                                          far_plane);
   glm::mat4 lightView = glm::lookAt(lightDir / 100.0f,
                                       glm::vec3(0.0f),
                                       glm::vec3(0.0f, 1.0f, 0.0f));
   SetView(lightView);
   SetProjection(lightProjection);

   lightSpaceMatrix = lightProjection * lightView;
   simpleDepthShader->use();
   simpleDepthShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

   glm::mat4 backpackModelMatrix = glm::mat4(1.0f);
   backpackModelMatrix = glm::translate(backpackModelMatrix, glm::vec3(0.0f, 0.0f, 0.0f)); //backpack
   backpackModelMatrix = glm::scale(backpackModelMatrix, glm::vec3(0.05f)); //backpack
   //backpackModelMatrix = glm::scale(backpackModelMatrix, glm::vec3(0.0002f)); //sponza
   simpleDepthShader->setMat4("model", backpackModelMatrix);

   // 1. render to DepthMap texture(FBO)
   glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
   glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
   glClear(GL_DEPTH_BUFFER_BIT);
   backpackModel.Draw(*simpleDepthShader);

   modelMatrix = glm::mat4(1.0f);
   modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -0.2f, -0.1f));
   modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 0.01f, 1.0f));
   simpleDepthShader->setMat4("model", modelMatrix);
   cubeModel.Draw(*simpleDepthShader);
   glBindFramebuffer(GL_FRAMEBUFFER, 0);

   //// 2. then render scene as normal with shadow mapping (using depth map)
   glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glActiveTexture(GL_TEXTURE6);
   glBindTexture(GL_TEXTURE_2D, depthMap);
   modelShader->use();
   modelShader->setInt("shadowMap", 6);
   SetProjection(sceneProjectionMatrix); //my view, not light
   SetView(sceneViewMatrix);
   modelShader->setMat4("projection", projectionMatrix);
   modelShader->setMat4("view", viewMatrix);
   modelShader->setMat4("model", backpackModelMatrix);
   modelShader->setVec3("viewPos", this->camera->getCameraPosition());
   modelShader->setVec3("lightPos", lightDir);
   modelShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
   backpackModel.Draw(*modelShader);
   modelShader->setMat4("model", modelMatrix);
   cubeModel.Draw(*modelShader);
   
   DrawOrthoOfLight();
   DebugDepthMap();
}

void GamePlayWindowState::SetView(glm::mat4 view)
{
   viewMatrix = view;
}

void GamePlayWindowState::SetProjection(glm::mat4 projection)
{
   projectionMatrix = projection;
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

   modelMatrix = glm::mat4(1.0f);
   mapShader->setMat4("modelMatrix", modelMatrix);
   mapShader->setMat4("projectionMatrix", projectionMatrix);
   mapShader->setMat4("viewMatrix", viewMatrix);
   mapShader->setVec3("lightPos", 0.0f, 0.52f, 0.5f);
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

// Writes depth values to a texture to use in shadow mapping
void GamePlayWindowState::CreateDepthMapForShadow()
{
   float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
   glGenFramebuffers(1, &depthMapFBO);

   //Generate dept buffer texture
   glGenTextures(1, &depthMap);
   glBindTexture(GL_TEXTURE_2D, depthMap);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH,
      SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
   glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

   //Attach depth buffer texture to framebuffer
   glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
      depthMap, 0);
   glDrawBuffer(GL_NONE); // to tell OpenGL we’re not going to render any color data
   glReadBuffer(GL_NONE); //
   glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GamePlayWindowState::DebugDepthMap()
{
   static unsigned int quadVAO;
   unsigned int quadVBO;

   if (!quadVAO)
   {
      float quadVertices[] = {
         -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
         -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f
      };
      glGenBuffers(1, &quadVBO);
      glGenVertexArrays(1, &quadVAO);
      glBindVertexArray(quadVAO);
      glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
   }

   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, depthMap);

   debugDepthMap->use();
   debugDepthMap->setInt("depthMap", 0);

   glViewport(0, 600, 300, 300);
   glBindVertexArray(quadVAO);
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
   glBindVertexArray(0);
   glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
}