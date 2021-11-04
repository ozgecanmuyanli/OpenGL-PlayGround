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
Shader* myShader;
Shader* lineShader;
Model* model;
const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;
float fov = 45.0f;
float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
glm::mat4 modelMatrix;

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
void DrawGrid(Mesh* gridObject, float scaleSize);
void DrawModel(Model* model);
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
	myShader = new Shader("../../Shaders/myShader.vs", "../../Shaders/myShader.fs");
	lineShader = new Shader("../../Shaders/lineShader.vs", "../../Shaders/lineShader.fs");

	// LOAD MODEL
	//model = new Model("C:/OpenGL-Playground/Models/bunny.obj");

	// GRIDS
	GenerateGrids(GRID_SIZE_WIDTH, GRID_SIZE_HEIGHT);
	Mesh* gridObject = new Mesh();
	gridObject->CreateMesh(&gridVertices[0], &gridIndices[0], gridVertices.size(), gridIndices.size());

	// TEXTURES
	Texture* texture = new Texture();
	texture->LoadTexture("../../Textures/E022N42.bmp");

	// CAMERA
	camera = Camera(glm::vec3(0.5f, 0.5f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.1f);

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

		DrawAxis(projectionMatrix);
		
		// MAIN PROGRAM
		myShader->use();

		texture->ActivateTexture(GL_TEXTURE0);
		myShader->setInt("texture1", 0);

		myShader->setMat4("projectionMatrix", projectionMatrix);
		myShader->setMat4("viewMatrix", camera.calculateViewMatrix());
		myShader->setMat4("modelMatrix", modelMatrix);
		myShader->setVec3("viewPos", camera.getCameraPosition()); //send cam pos for specular light

		DrawGrid(gridObject, GRID_SCALE_SIZE);

		//DrawModel(model);

		// TO EDIT SHADERS AT RUN TIME
		if (keys[GLFW_KEY_F] && keys[GLFW_KEY_LEFT_CONTROL])
		{
			myShader = new Shader("../../Shaders/myShader.vs", "../../Shaders/myShader.fs");
			lineShader = new Shader("../../Shaders/lineShader.vs", "../../Shaders/lineShader.fs");
		}

		mainWindow.swapBuffers();
	}
	delete texture;
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

void DrawGrid(Mesh* gridObject, float scaleSize)
{
	// GRID SCALE
	modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleSize, 1.0f, scaleSize));
	myShader->setMat4("modelMatrix", modelMatrix);
	gridObject->RenderMesh();
}

void DrawModel(Model* model)
{
	model->Draw(*myShader);
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