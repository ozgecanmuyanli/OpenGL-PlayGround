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
#define GRID_SIZE_WIDTH    (256)
#define GRID_SIZE_HEIGHT   (256)
#define GRID_SCALE_SIZE    (7.0F)
#define SCREEN_CLEAR_RED   (111.0F / 255.0F)
#define SCREEN_CLEAR_GREEN (190.0F / 255.0F)
#define SCREEN_CLEAR_BLUE  (255.0F / 255.0F)
#define NEAR               (0.1F)
#define FAR                (100.0F)
#define OBJECTCOUNT        (600)

Window mainWindow;
Camera camera;
Shader* modelShader;
Shader* lineShader;
Shader* gridShader;
Shader* cloudShader;
Shader* screenSpaceQuadShader;
Shader* cubeShader;
Shader* grassShader;
Shader* windowShader;
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
glm::mat4 randomModelMatrix[OBJECTCOUNT];
std::vector<float> gridVertices;
std::vector<GLuint> gridIndices;
unsigned int textureFBO;
float grassWindSpeed;
float floatDragValuePosition[] = {0.0f, 0.0f, 0.0f};
float floatDragValueSpeed = 1;
float floatDragValueScale = 1;
float colorOfModel[] = { 0.3f, 0.0f, 0.3f };

GLfloat squareVertices[] = { //for a grass texture
	-0.5f, -0.5f, 0.2f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.2f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.2f, 1.0f, 1.0f,
	-0.5f, 0.5f, 0.2f, 0.0f, 1.0f
};
GLuint squareIndices[] = {
	0, 1, 2,
	2, 3, 0
};

GLfloat windowVertices[] = {
	-0.5f, -0.5f, 0.2f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.2f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.2f, 1.0f, 1.0f,
	-0.5f, 0.5f, 0.2f, 0.0f, 1.0f
};
GLuint windowIndices[] = {
	0, 1, 2,
	2, 3, 0
};


// FUNCTIONS
void MainLoop();
float GetRandom(float min, float max);
void GenerateGrids(unsigned int gridWidth, unsigned int gridHeight);
void DrawAxis(glm::mat4 projectionMatrix);
void DrawModel(Model* model, Texture* texture, const std::string& textureUniformName, glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
void DrawCloud(Mesh* cloudObject, float scaleSize, glm::mat4 projectionMatrix);
void DrawGrid(Mesh* gridObject, float scaleSize, Texture* texture, const std::string& textureUniformName, glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
void DrawCubeRoom(Texture* texture, const std::string& textureUniformName, glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
void DrawScreenSpaceQuad();
void DrawSquare(GLfloat* vertices, GLuint* indices, unsigned int numOfVertices, unsigned int numOfIndices);
void DrawGrass(glm::mat4 projectionMatrix, glm::mat4 randomModelMatrix);
void DrawGlassWindow(glm::mat4 projectionMatrix);
void CreateRandomModelMatrix();
void drawGUI();


void main()
{
	srand(time(0));
	mainWindow = Window(SCR_WIDTH, SCR_HEIGHT);
	mainWindow.Initialise();
	glClearColor(SCREEN_CLEAR_RED, SCREEN_CLEAR_GREEN, SCREEN_CLEAR_BLUE, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //to see grids/vertices use GL_LINE instead of GL_FILL

	CreateRandomModelMatrix();
	MainLoop();
}

void MainLoop()
{
	modelShader = new Shader("../../Shaders/modelShader.vs", "../../Shaders/modelShader.fs");
	lineShader = new Shader("../../Shaders/lineShader.vs", "../../Shaders/lineShader.fs");
	gridShader = new Shader("../../Shaders/gridShader.vs", "../../Shaders/gridShader.fs");
	cloudShader = new Shader("../../Shaders/cloudShader.vs", "../../Shaders/cloudShader.fs");
	cubeShader = new Shader("../../Shaders/cubeShader.vs", "../../Shaders/cubeShader.fs");
	grassShader = new Shader("../../Shaders/grassShader.vs", "../../Shaders/grassShader.fs");
	windowShader = new Shader("../../Shaders/WindowShader.vs", "../../Shaders/WindowShader.fs");
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
	camera = Camera(glm::vec3(5.5f, 4.5f, 20.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 20.0f, 0.1f);

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
		DrawModel(model, textureTerrain, "textureTerrain", projectionMatrix, camera.calculateViewMatrix());

		//for (unsigned int i = 0; i <= OBJECTCOUNT; i++)
		//{
		//	DrawGrass(projectionMatrix, randomModelMatrix[i]);
		//}

		DrawCubeRoom(textureWood, "textureWood", projectionMatrix, camera.calculateViewMatrix());
		DrawGlassWindow(projectionMatrix);

		// TO EDIT SHADERS AT RUN TIME
		if (keys[GLFW_KEY_F] && keys[GLFW_KEY_LEFT_CONTROL])
		{
			modelShader = new Shader("../../Shaders/modelShader.vs", "../../Shaders/modelShader.fs");
			lineShader = new Shader("../../Shaders/lineShader.vs", "../../Shaders/lineShader.fs");
			gridShader = new Shader("../../Shaders/gridShader.vs", "../../Shaders/gridShader.fs");
			cloudShader = new Shader("../../Shaders/cloudShader.vs", "../../Shaders/cloudShader.fs");
			cubeShader = new Shader("../../Shaders/cubeShader.vs", "../../Shaders/cubeShader.fs");
			grassShader = new Shader("../../Shaders/grassShader.vs", "../../Shaders/grassShader.fs");
			windowShader = new Shader("../../Shaders/WindowShader.vs", "../../Shaders/WindowShader.fs");
			//screenSpaceQuadShader = new Shader("../../Shaders/screenSpaceQuadShader.vs", "../../Shaders/screenSpaceQuadShader.fs");
		}

		drawGUI();
		mainWindow.swapBuffers();
	}
}

// FUNCTION IMPLEMENTATIONS
void DrawCubeRoom(Texture* texture, const std::string& textureUniformName, glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
	cubeModelMatrix = glm::mat4(1.0f);
	cubeModelMatrix = glm::translate(cubeModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
	cubeModelMatrix = glm::scale(cubeModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));

	texture->ActivateTexture(GL_TEXTURE0);
	cubeShader->setInt(textureUniformName, 0);

	cubeShader->use();
	cubeShader->setMat4("modelMatrix", cubeModelMatrix);
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

	static bool isModelTextureLoaded = false;
	static Texture* textureModel;
	if (!isModelTextureLoaded)
	{
		textureModel = new Texture();
		textureModel->LoadTexture("../../Textures/awesomeface.png");
		isModelTextureLoaded = true;
	}

	modelShader->use();
	
	textureModel->ActivateTexture(GL_TEXTURE0);
	modelShader->setInt("textureModel", 0);

	modelShader->setMat4("projectionMatrix", projectionMatrix);
	modelShader->setMat4("viewMatrix", viewMatrix);
	modelShader->setVec3("viewPos", camera.getCameraPosition()); //send cam pos for specular light
	modelShader->setFloat("gridScaleSize", GRID_SCALE_SIZE);
	modelShader->setVec3("objectColor", colorOfModel);

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
	
	modelMatrix = glm::translate(modelMatrix, 
										  glm::vec3(floatDragValuePosition[0] + walkingX, 
											  floatDragValuePosition[1], 
											  floatDragValuePosition[2] + walkingZ));
	modelMatrix = glm::scale(modelMatrix, 
									glm::vec3(floatDragValueScale, 
										floatDragValueScale, 
										floatDragValueScale));
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

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureFBO);
	modelShader->setInt("quadTexture", 0);

	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void DrawGlassWindow(glm::mat4 projectionMatrix)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glm::mat4 modelMatrix = glm::mat4(1.0f);

	static bool isWindowTextureLoaded = false;
	static Texture* textureWindow;
	if (!isWindowTextureLoaded)
	{
		textureWindow = new Texture();
		textureWindow->LoadTexture("../../Textures/window.png");
		isWindowTextureLoaded = true;
	}

	windowShader->use();

	textureWindow->ActivateTexture(GL_TEXTURE0);
	windowShader->setInt("textureWindow", 0);


	modelMatrix = glm::translate(modelMatrix, glm::vec3(5.0f, 5.0f, 8.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(10.0f, 10.0f, 10.0f));
	windowShader->setMat4("modelMatrix", modelMatrix);
	windowShader->setMat4("projectionMatrix", projectionMatrix);
	windowShader->setMat4("viewMatrix", camera.calculateViewMatrix());
	DrawSquare(windowVertices, windowIndices, sizeof(windowVertices) / sizeof(GLfloat), sizeof(windowIndices) / sizeof(GLuint));
	glDisable(GL_BLEND);
}

void DrawGrass(glm::mat4 projectionMatrix, glm::mat4 randomModelMatrix)
{
	static bool isGrassTextureLoaded = false;
	static Texture* textureGrass;
	if (!isGrassTextureLoaded)
	{
		textureGrass = new Texture();
		textureGrass->LoadTexture("../../Textures/grass.png");
		isGrassTextureLoaded = true;
	}

	grassShader->use();

	textureGrass->ActivateTexture(GL_TEXTURE0);
	grassShader->setInt("textureGrass", 0);

	grassShader->setFloat("time", ((float)glfwGetTime() * floatDragValueSpeed));

	grassShader->setMat4("modelMatrix", randomModelMatrix);
	grassShader->setMat4("projectionMatrix", projectionMatrix);
	grassShader->setMat4("viewMatrix", camera.calculateViewMatrix());
	DrawSquare(squareVertices, squareIndices, sizeof(squareVertices) / sizeof(GLfloat), sizeof(squareIndices) / sizeof(GLuint));

	randomModelMatrix = glm::rotate(randomModelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	grassShader->setMat4("modelMatrix", randomModelMatrix);
	DrawSquare(squareVertices, squareIndices, sizeof(squareVertices) / sizeof(GLfloat), sizeof(squareIndices) / sizeof(GLuint));
}

void DrawSquare(GLfloat* vertices, GLuint* indices, unsigned int numOfVertices, unsigned int numOfIndices)
{
	static unsigned int squareVAO;
	static unsigned int squareIBO;
	static unsigned int squareVBO;
	if (!squareVAO)
	{
		glGenVertexArrays(1, &squareVAO);
		glGenBuffers(1, &squareIBO);
		glGenBuffers(1, &squareVBO);

		glBindVertexArray(squareVAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, squareIBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, squareVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0 * sizeof(float)));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

	}

	glBindVertexArray(squareVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void CreateRandomModelMatrix()
{
	float scaleRatio;
	for (unsigned int i = 0; i < OBJECTCOUNT; i++)
	{
		randomModelMatrix[i] = glm::mat4(1.0f);
		randomModelMatrix[i] = glm::translate(randomModelMatrix[i], glm::vec3(GetRandom(0.5f, 9.2f), 1.0f, GetRandom(0.5f, 9.2f)));
		randomModelMatrix[i] = glm::rotate(randomModelMatrix[i], glm::radians(GetRandom(0.0f, 90.0f)), glm::vec3(0.0f, 1.0f, 0.0f));
		scaleRatio = GetRandom(1.0f, 2.0f);
		randomModelMatrix[i] = glm::scale(randomModelMatrix[i], glm::vec3(scaleRatio, scaleRatio, scaleRatio));
	}
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

float GetRandom(float min, float max)
{
	return ((float)rand() / ((float)RAND_MAX)) * (max - min) + min;
}

void drawGUI()
{
	float x = 0;
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	//draw gu�

	if (ImGui::Begin("Object Properties"))
	{
		ImGui::DragFloat("Wind Speed", &floatDragValueSpeed, 0.01f, -FLT_MAX, +FLT_MAX, "%.3f");
		ImGui::DragFloat("Scale", &floatDragValueScale, 0.01f, -FLT_MAX, +FLT_MAX, "%.3f");
		ImGui::DragFloat3("Position", floatDragValuePosition, 0.01f, -FLT_MAX, +FLT_MAX, "%.3f");
		ImGui::ColorPicker3("ColorPicker", colorOfModel);
	}
	ImGui::End();


	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}