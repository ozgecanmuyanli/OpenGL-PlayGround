#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "stb_image.h"

#include "Window.h"
#include "shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Camera.h"
#include "Model.h"

//VARIABLES
Window mainWindow;
Camera camera;
Shader* modelShader;
Shader* lineShader;
Shader* gridShader;
Shader* cloudShader;
Shader* screenSpaceQuadShader;
Shader* cubeShader;
Model* model;
Model* cubeRoom;
const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;
float fov = 45.0f;
float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
float walkingX = 0.0f;
float walkingZ = 0.0f;
float timeInSin = 0.0f;
glm::mat4 modelMatrix;
glm::mat4 miniProjectionMatrix;
glm::mat4 cubeModelMatrix;

#define GRID_SIZE_WIDTH    (256)
#define GRID_SIZE_HEIGHT   (256)
#define GRID_SCALE_SIZE    (7.0F)
#define SCREEN_CLEAR_RED   (111.0F / 255.0F)
#define SCREEN_CLEAR_GREEN (190.0F / 255.0F)
#define SCREEN_CLEAR_BLUE  (255.0F / 255.0F)
#define NEAR               (0.1F)
#define FAR                (100.0F)

std::vector<float> gridVertices;
std::vector<GLuint> gridIndices;
unsigned int textureFBO;

// FUNCTIONS
void MainLoop();
void GenerateGrids(unsigned int gridWidth, unsigned int gridHeight);
void DrawAxis(glm::mat4 projectionMatrix);
void DrawModel(Model* model, Texture* texture, const std::string& textureUniformName, glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
void DrawCloud(Mesh* cloudObject, float scaleSize, glm::mat4 projectionMatrix);
void DrawGrid(Mesh* gridObject, float scaleSize, Texture* texture, const std::string& textureUniformName, glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
void DrawCubeRoom(Texture* texture, const std::string& textureUniformName, glm::mat4 modelMatrix, glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
void DrawScreenSpaceQuad();

void main()
{
	srand(time(0));
	mainWindow = Window(SCR_WIDTH, SCR_HEIGHT);
	mainWindow.Initialise();
	glClearColor(SCREEN_CLEAR_RED, SCREEN_CLEAR_GREEN, SCREEN_CLEAR_BLUE, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //to see grids/vertices use GL_LINE instead of GL_FILL

	MainLoop();
}

void MainLoop()
{
	modelShader = new Shader("../../Shaders/modelShader.vs", "../../Shaders/modelShader.fs");
	lineShader = new Shader("../../Shaders/lineShader.vs", "../../Shaders/lineShader.fs");
	gridShader = new Shader("../../Shaders/gridShader.vs", "../../Shaders/gridShader.fs");
	cloudShader = new Shader("../../Shaders/cloudShader.vs", "../../Shaders/cloudShader.fs");
	cubeShader = new Shader("../../Shaders/cubeShader.vs", "../../Shaders/cubeShader.fs");
	//screenSpaceQuadShader = new Shader("../../Shaders/screenSpaceQuadShader.vs", "../../Shaders/screenSpaceQuadShader.fs");

	// LOAD MODEL
	model = new Model("../../Models/cube.obj");

	cubeRoom = new Model("../../Models/cube1x1.obj");

	// OBJECTS
	GenerateGrids(GRID_SIZE_WIDTH, GRID_SIZE_HEIGHT);
	Mesh* gridObject = new Mesh();
	gridObject->CreateMesh(&gridVertices[0], &gridIndices[0], gridVertices.size(), gridIndices.size());

	Mesh* cloudObject = new Mesh();
	cloudObject = gridObject;

	// TEXTURES
	Texture* textureTerrain = new Texture();
	textureTerrain->LoadTexture("../../Textures/E022N42.bmp");

	Texture* textureWood = new Texture();
	textureWood->LoadTexture("../../Textures/wood.png");


	// CAMERA
	camera = Camera(glm::vec3(3.5f, 2.5f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 20.0f, 0.1f);

	// PROJECTION
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(fov),
		(GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(), NEAR, FAR);


	while (!mainWindow.getShouldClose())
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		bool* keys = mainWindow.getsKeys();
		glClearColor(SCREEN_CLEAR_RED, SCREEN_CLEAR_GREEN, SCREEN_CLEAR_BLUE, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		DrawAxis(projectionMatrix);

		cubeModelMatrix = glm::mat4(1.0f);
		cubeModelMatrix = glm::translate(cubeModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
		cubeModelMatrix = glm::scale(cubeModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
		DrawCubeRoom(textureWood, "textureWood", cubeModelMatrix, projectionMatrix, camera.calculateViewMatrix());

		DrawModel(model, textureTerrain, "textureTerrain", projectionMatrix, camera.calculateViewMatrix());


		// TO EDIT SHADERS AT RUN TIME
		if (keys[GLFW_KEY_F] && keys[GLFW_KEY_LEFT_CONTROL])
		{
			modelShader = new Shader("../../Shaders/modelShader.vs", "../../Shaders/modelShader.fs");
			lineShader = new Shader("../../Shaders/lineShader.vs", "../../Shaders/lineShader.fs");
			gridShader = new Shader("../../Shaders/gridShader.vs", "../../Shaders/gridShader.fs");
			cloudShader = new Shader("../../Shaders/cloudShader.vs", "../../Shaders/cloudShader.fs");
			cubeShader = new Shader("../../Shaders/cubeShader.vs", "../../Shaders/cubeShader.fs");
			//screenSpaceQuadShader = new Shader("../../Shaders/screenSpaceQuadShader.vs", "../../Shaders/screenSpaceQuadShader.fs");
		}
		mainWindow.swapBuffers();
	}
}

// FUNCTION IMPLEMENTATIONS
void DrawCubeRoom(Texture* texture, const std::string& textureUniformName, glm::mat4 modelMatrix, glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
	texture->ActivateTexture(GL_TEXTURE0);
	cubeShader->setInt(textureUniformName, 0);

	cubeShader->use();
	cubeShader->setMat4("modelMatrix", modelMatrix);
	cubeShader->setMat4("projectionMatrix", projectionMatrix);
	cubeShader->setMat4("viewMatrix", viewMatrix);
	cubeShader->setVec3("viewPos", camera.getCameraPosition()); //send cam pos for specular light
	
	cubeRoom->Draw(*cubeShader);
}

void DrawGrid(Mesh* gridObject, float scaleSize, Texture* texture, const std::string& textureUniformName, glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	texture->ActivateTexture(GL_TEXTURE0);
	gridShader->setInt(textureUniformName, 0);

	gridShader->use();
	gridShader->setFloat("time", (float)glfwGetTime());
	
	// GRID SCALE
	modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleSize, 1.0f, scaleSize));
	gridShader->setMat4("modelMatrix", modelMatrix);
	gridShader->setMat4("projectionMatrix", projectionMatrix);
	gridShader->setMat4("viewMatrix", viewMatrix);

	gridObject->RenderMesh();
}

void DrawCloud(Mesh* cloudObject, float scaleSize, glm::mat4 projectionMatrix)
{
	glm::mat4 modelMatrix = glm::mat4(1.0f);

	static bool isNoiseTextureLoaded = false;
	static Texture* textureNoise;
	if (!isNoiseTextureLoaded)
	{
		textureNoise = new Texture();
		textureNoise->LoadTexture("../../Textures/noise_cloud.png");
		isNoiseTextureLoaded = true;
	}
	textureNoise->ActivateTexture(GL_TEXTURE3);
	cloudShader->setInt("textureNoise", 3);

	cloudShader->use();

	cloudShader->setFloat("time", (float)glfwGetTime());

	// GRID SCALE
	modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleSize, 1.0f, scaleSize));
	cloudShader->setMat4("modelMatrix", modelMatrix);
	cloudShader->setMat4("projectionMatrix", projectionMatrix);
	cloudShader->setMat4("viewMatrix", camera.calculateViewMatrix());

	cloudObject->RenderMesh();
}

void DrawModel(Model* model, Texture* texture, const std::string& textureUniformName, glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
	bool* keys = mainWindow.getsKeys();
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	float modelScaleSize = 0.25;

	static bool isModelTextureLoaded = false;
	static Texture* textureModel;
	if (!isModelTextureLoaded)
	{
		textureModel = new Texture();
		textureModel->LoadTexture("../../Textures/awesomeface.png");
		isModelTextureLoaded = true;
	}

	modelShader->use();
	
	textureModel->ActivateTexture(GL_TEXTURE0); // activate texture unit first
	modelShader->setInt("textureModel", 0);

	//texture->ActivateTexture(GL_TEXTURE2); //terrainTexture
	//modelShader->setInt(textureUniformName, 2);

	modelShader->setMat4("projectionMatrix", projectionMatrix);
	modelShader->setMat4("viewMatrix", viewMatrix);
	modelShader->setVec3("viewPos", camera.getCameraPosition()); //send cam pos for specular light
	modelShader->setFloat("modelScaleSize", modelScaleSize);
	modelShader->setFloat("gridScaleSize", GRID_SCALE_SIZE);

	float timeInSin = sin(glfwGetTime() * 5.0f);
	if (timeInSin <= 0)
	{
		timeInSin = 0;
	}
	// MODEL MOVEMENT
	if (keys[GLFW_KEY_SPACE]) //jump
	{
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, timeInSin, 0.0f));
	}
	if (keys[GLFW_KEY_RIGHT])
	{
		walkingX += 0.75 * deltaTime;
	}
	if (keys[GLFW_KEY_LEFT])
	{
		walkingX -= 0.75 * deltaTime;
	}
	if (keys[GLFW_KEY_DOWN])
	{
		walkingZ += 0.75 * deltaTime;
	}
	if (keys[GLFW_KEY_UP])
	{
		walkingZ -= 0.75 * deltaTime;
	}
	modelMatrix = glm::translate(modelMatrix, glm::vec3(walkingX, 0.0f, walkingZ));
	//modelMatrix = glm::scale(modelMatrix, glm::vec3(modelScaleSize, modelScaleSize, modelScaleSize));
	modelShader->setMat4("modelMatrix", modelMatrix);
	model->Draw(*modelShader);
}

void DrawAxis(glm::mat4 projectionMatrix)
{
	static unsigned int lineVAO;
	if (!lineVAO)
	{
		glGenVertexArrays(1, &lineVAO);
	}

	lineShader->use();
	lineShader->setMat4("projectionMatrix", projectionMatrix);
	lineShader->setMat4("viewMatrix", camera.calculateViewMatrix());

	glBindVertexArray(lineVAO);
	glDrawArrays(GL_LINES, 0, 6);
	glBindVertexArray(0);
}

void DrawScreenSpaceQuad()
{
	static unsigned int quadVAO;
	if (!quadVAO)
	{
		glGenVertexArrays(1, &quadVAO);
	}

	screenSpaceQuadShader->use();

	glActiveTexture(GL_TEXTURE0); // activate texture unit first
	glBindTexture(GL_TEXTURE_2D, textureFBO);
	modelShader->setInt("quadTexture", 0);

	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void GenerateGrids(unsigned int gridWidth, unsigned int gridHeight)
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
