# include "GamePlayWindowState.h"

void GamePlayWindowState::setStateCamera(Camera* pCamera)
{
   this->camera = pCamera;
}

GamePlayWindowState::GamePlayWindowState()
{
   gameShader = new Shader("../../Shaders/gameShader.vs", "../../Shaders/gameShader.fs");
   modelMatrix = glm::mat4(1.0f);
   viewMatrix = glm::mat4(1.0f);

   stateType = GAME_PLAY_WINDOW;
}

void GamePlayWindowState::Initialise()
{
   mapTexture = new Texture();
   mapTexture->LoadTextureCPU("../../Textures/deneme.bmp");
   mapTexture->SetMapTexture();

   GenerateGrids(mapTexture->GetTextureWidth(), mapTexture->GetTextureHeight());
   gridObject = new Mesh();
   gridObject->CreateMesh(&gridVertices[0], &gridIndices[0], gridVertices.size(), gridIndices.size());
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
   DrawGrid();
}

void GamePlayWindowState::DrawWall()
{

}

void GamePlayWindowState::DrawGrid()
{
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   mapTexture->LoadTextureGPU("../../Textures/deneme.bmp");
   mapTexture->ActivateTexture(GL_TEXTURE0);
   mapTexture->BindTexture();

   gameShader->use();
   gameShader->setInt("mapTexture", 0);

   // GRID SCALE
   gameShader->setMat4("modelMatrix", modelMatrix);
   gameShader->setMat4("projectionMatrix", projectionMatrix);
   gameShader->setMat4("viewMatrix", viewMatrix);

   gridObject->RenderMesh();
}

void GamePlayWindowState::GenerateGrids(unsigned int gridWidth, unsigned int gridHeight)
{
   // generate vertices
   for (int i = 0; i < gridWidth; i++)
   {
      for (int j = 0; j < gridHeight; j++)
      {
         gridVertices.push_back((float)i / gridWidth);
         gridVertices.push_back(0.0f);
         gridVertices.push_back((float)j / gridHeight);
      }
   }

   // generate indices
   for (size_t i = 0; i < gridHeight * (gridWidth - 1); i = i + gridHeight)
   {
      for (size_t j = i; j < i + (gridHeight - 1); j++)
      {
         gridIndices.push_back(j);
         gridIndices.push_back(j + gridHeight);
         gridIndices.push_back(j + 1);
         gridIndices.push_back(j + 1);
         gridIndices.push_back(j + gridHeight);
         gridIndices.push_back(j + gridHeight + 1);
      }
   }
}
