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
Model* model;
const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;
float fov = 45.0f;
float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
float walkingX = 0.0f;
float walkingZ = 0.0f;
float timeInSin = 0.0f;
glm::mat4 modelMatrix;
glm::mat4 modelMatrixWithoutScale;

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


// FUNCTIONS
void MainLoop();
float GetRandom(float min, float max);
void GenerateGrids(unsigned int gridWidth, unsigned int gridHeight);
void DrawGrid(Mesh* gridObject, float scaleSize, glm::mat4 modelMatrix, glm::mat4 projectionMatrix);
void DrawModel(Model* model, float walkingX, float walkingZ);
void DrawAxis(glm::mat4 projectionMatrix);

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

	// LOAD MODEL
	model = new Model("../../Models/cube.obj");

	// GRIDS
	GenerateGrids(GRID_SIZE_WIDTH, GRID_SIZE_HEIGHT);
	Mesh* gridObject = new Mesh();
	gridObject->CreateMesh(&gridVertices[0], &gridIndices[0], gridVertices.size(), gridIndices.size());

	// TEXTURES
	//Texture* textureTerrain = new Texture();
	//textureTerrain->LoadTexture("../../Textures/E022N42.bmp");

	Texture* textureBunny = new Texture();
	textureBunny->LoadTexture("../../Textures/wood.png");

	Texture* textureNoise = new Texture();
	textureNoise->LoadTexture("../../Textures/noise_cloud.png");

	// CAMERA
	camera = Camera(glm::vec3(0.5f, 6.5f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.1f);

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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		modelMatrix = glm::mat4(1.0f);
		modelMatrixWithoutScale = glm::mat4(1.0f);

		textureNoise->ActivateTexture(GL_TEXTURE0);
		DrawGrid(gridObject, GRID_SCALE_SIZE, modelMatrix, projectionMatrix);
		DrawAxis(projectionMatrix);

		
		// MAIN PROGRAM
		modelShader->use();

		textureBunny->ActivateTexture(GL_TEXTURE0);
		modelShader->setInt("textureModel", 0);

		modelShader->setMat4("projectionMatrix", projectionMatrix);
		modelShader->setMat4("viewMatrix", camera.calculateViewMatrix());
		modelShader->setVec3("viewPos", camera.getCameraPosition()); //send cam pos for specular light

		
		timeInSin = sin(glfwGetTime() * 5.0f);
		if (timeInSin <= 0)
		{
			timeInSin = 0;
		}

		// MODEL MOVEMENT
		modelMatrix = glm::mat4(1.0f);

		if (keys[GLFW_KEY_SPACE]) //jump
		{
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, timeInSin, 0.0f));
		}
		if (keys[GLFW_KEY_RIGHT])
		{
			walkingX += 0.5 * deltaTime;
		}
		if (keys[GLFW_KEY_LEFT])
		{
			walkingX -= 0.5 * deltaTime;
		}
		if (keys[GLFW_KEY_DOWN])
		{
			walkingZ += 0.5 * deltaTime;
		}
		if (keys[GLFW_KEY_UP])
		{
			walkingZ -= 0.5 * deltaTime;
		}

		DrawModel(model, walkingX, walkingZ);


		// TO EDIT SHADERS AT RUN TIME
		if (keys[GLFW_KEY_F] && keys[GLFW_KEY_LEFT_CONTROL])
		{
			modelShader = new Shader("../../Shaders/modelShader.vs", "../../Shaders/modelShader.fs");
			lineShader = new Shader("../../Shaders/lineShader.vs", "../../Shaders/lineShader.fs");
			gridShader = new Shader("../../Shaders/gridShader.vs", "../../Shaders/gridShader.fs");
		}

		mainWindow.swapBuffers();
	}
	delete textureBunny;
}

// FUNCTION IMPLEMENTATIONS
float GetRandom(float min, float max)
{
	return ((float)rand() / ((float)RAND_MAX)) * (max - min) + min;
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

void DrawGrid(Mesh* gridObject, float scaleSize, glm::mat4 modelMatrix, glm::mat4 projectionMatrix)
{
	gridShader->setInt("textureNoise", 0);
	gridShader->use();
	gridShader->setFloat("time", (float)glfwGetTime());
	
	modelMatrix = glm::mat4(1.0f);
	// GRID SCALE
	modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleSize, 1.0f, scaleSize));
	gridShader->setMat4("modelMatrix", modelMatrix);
	gridShader->setMat4("projectionMatrix", projectionMatrix);
	gridShader->setMat4("viewMatrix", camera.calculateViewMatrix());

	gridObject->RenderMesh();
}

void DrawModel(Model* model, float walkingX, float walkingZ)
{
	float modelScaleSize = 0.25f;

	modelMatrix = glm::translate(modelMatrix, glm::vec3(walkingX, 0.0f, walkingZ));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(modelScaleSize, modelScaleSize, modelScaleSize));

	modelShader->setFloat("modelScaleSize", modelScaleSize);
	modelShader->setFloat("gridScaleSize", GRID_SCALE_SIZE);
	modelShader->setMat4("modelMatrix", modelMatrix);
	model->Draw(*modelShader);
}

void DrawAxis(glm::mat4 projectionMatrix)
{
	unsigned int lineVAO;
	glGenVertexArrays(1, &lineVAO);

	lineShader->use();
	lineShader->setMat4("projectionMatrix", projectionMatrix);
	lineShader->setMat4("viewMatrix", camera.calculateViewMatrix());

	glBindVertexArray(lineVAO);
	glDrawArrays(GL_LINES, 0, 6);
	glBindVertexArray(0);
}