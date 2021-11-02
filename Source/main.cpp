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

//VARIABLES
Window mainWindow;
Camera camera;
Shader* myShader;
Shader* lineShader;
const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;
float fov = 45.0f;
float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

#define OBJECTCOUNT 10
glm::mat4 modelMatrix[OBJECTCOUNT];
glm::vec3 objectColors[OBJECTCOUNT];

GLfloat cubeVertices[] =
{ //vertices				//textures
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,

	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

	-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,

	0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,

	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f
};

// FUNCTIONS
void MainLoop();
void ChangeObjectColor(Mesh* object, glm::vec3 color);
float GetRandom(float min, float max);
void CreateRandomModelMatrix();
void CreateRandomColors();

void main()
{
	srand(time(0));
	mainWindow = Window(SCR_WIDTH, SCR_HEIGHT);
	mainWindow.Initialise();

	glClearColor(0.1, 0.4, 0.1, 1.0);
	CreateRandomModelMatrix();
	CreateRandomColors();
	MainLoop();
}

void MainLoop()
{
	//variables
	float lightAngle = 0.0f;
	unsigned int lineVAO;
	glGenVertexArrays(1, &lineVAO);

	myShader = new Shader("../../Shaders/myShader.vs", "../../Shaders/myShader.fs");
	lineShader = new Shader("../../Shaders/lineShader.vs", "../../Shaders/lineShader.fs");
	
	Mesh* cubeObject = new Mesh();
	cubeObject->CreateMesh(cubeVertices, 0, 288, 0);

	Texture* texture = new Texture();
	texture->LoadTexture("../../Textures/wood.png");

	camera = Camera(glm::vec3(0.5f, 0.5f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);

	//PROJECTION
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(fov),
		(GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(),
		0.1f, 100.0f);

	while (!mainWindow.getShouldClose())
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// DRAW LINE 
		lineShader->use();

		glUniformMatrix4fv(glGetUniformLocation(lineShader->ID, "projectionMatrix"),
			1, GL_FALSE, glm::value_ptr(projectionMatrix));
		glUniformMatrix4fv(glGetUniformLocation(lineShader->ID, "viewMatrix"),
			1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		glBindVertexArray(lineVAO);
		glDrawArrays(GL_LINES, 0, 6);
		glBindVertexArray(0);
		// END DRAW LINE
		
		// MAIN PROGRAM
		myShader->use();

		glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "projectionMatrix"),
			1, GL_FALSE, glm::value_ptr(projectionMatrix));
		glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "viewMatrix"),
			1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));


		texture->ActivateTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(myShader->ID, "texture1"), 0);

		//lightAngle = lightAngle + 0.05f;
		//glUniform1f(glGetUniformLocation(myShader->ID, "lightAngle"), lightAngle);

		// 1- send each model matrix, 2- set a color, 3- draw
		for (unsigned int i = 0; i <= OBJECTCOUNT; i++)
		{
			glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "modelMatrix"),
				1, GL_FALSE, glm::value_ptr(modelMatrix[i]));
			cubeObject->RenderMesh();
		}
		mainWindow.swapBuffers();
	}
	delete cubeObject, texture;
}


// FUNCTION IMPLEMENTATIONS
void CreateRandomModelMatrix()
{
	float scaleRatio;
	for (unsigned int i = 0; i < OBJECTCOUNT; i++)
	{
		modelMatrix[i] = glm::mat4(1.0f);
		modelMatrix[i] = glm::translate(modelMatrix[i], glm::vec3(GetRandom(-1.0f, 5.0f), GetRandom(-1.0f, 5.0f), GetRandom(-1.0f, 5.0f)));
		modelMatrix[i] = glm::rotate(modelMatrix[i], glm::radians(GetRandom(30.0f, 120.0f)), glm::vec3(0.0f, 0.0f, 1.0f));
		scaleRatio = GetRandom(0.2f, 0.7f);
		modelMatrix[i] = glm::scale(modelMatrix[i], glm::vec3(scaleRatio, scaleRatio, scaleRatio));
	}
}

void CreateRandomColors()
{
	for (unsigned int i = 0; i < OBJECTCOUNT; i++)
	{
		objectColors[i] = glm::vec3(GetRandom(0.0f, 1.0f), GetRandom(0.0f, 1.0f), GetRandom(0.0f, 1.0f));
	}
}

float GetRandom(float min, float max)
{
	return ((float)rand() / ((float)RAND_MAX)) * (max - min) + min;
}

void ChangeObjectColor(Mesh* object, glm::vec3 color)
{
	int objectColorLocation = glGetUniformLocation(myShader->ID, "objectColor");
	glUniform3f(objectColorLocation, color.x, color.y, color.z);
}