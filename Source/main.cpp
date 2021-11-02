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

GLfloat groundVertices[] =
{
	0.0f, -0.1f, 0.0f, 0.0f, 100.0f, 0.0f, 1.0f, 0.0f,
	0.0f, -0.1f, 999.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	999.0f, -0.1f, 999.0f, 100.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	999.0f, -0.1f, 0.0f, 100.0f, 100.0f, 0.0f, 1.0f, 0.0f
};
GLuint groundIndices[] =
{
	0, 3, 2,
	2, 1, 0
};

// FUNCTIONS
void MainLoop();
void ChangeObjectColor(Mesh* object, glm::vec3 color);
float GetRandom(float min, float max);
void CreateRandomColors();
void Move(glm::mat4 modelMatrix, float dx, float timInSin, float directionX, float directionY, float directionZ);

void main()
{
	srand(time(0));
	mainWindow = Window(SCR_WIDTH, SCR_HEIGHT);
	mainWindow.Initialise();
	glClearColor(0.1, 0.4, 0.1, 1.0);

	// LOAD MODEL
	model = new Model("C:/OpenGL-Playground/Models/bunny.obj");
	MainLoop();
}

void MainLoop()
{
	//variables
	float lightAngle = 0.0f;
	float dx = 0.0f;
	unsigned int lineVAO, groundVAO, groundVBO, groundIBO;
	glGenVertexArrays(1, &lineVAO);

	myShader = new Shader("../../Shaders/myShader.vs", "../../Shaders/myShader.fs");
	lineShader = new Shader("../../Shaders/lineShader.vs", "../../Shaders/lineShader.fs");

	Mesh* groundObject = new Mesh();
	groundObject->CreateMesh(groundVertices, groundIndices, 32, 6);

	Texture* texture = new Texture();
	texture->LoadTexture("../../Textures/wood.png");

	camera = Camera(glm::vec3(0.5f, 0.5f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.1f);

	// PROJECTION
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

		modelMatrix = glm::mat4(1.0f);
		
		texture->ActivateTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(myShader->ID, "texture1"), 0);

		glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "projectionMatrix"),
			1, GL_FALSE, glm::value_ptr(projectionMatrix));
		glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "viewMatrix"),
			1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "modelMatrix"),
			1, GL_FALSE, glm::value_ptr(modelMatrix));
		groundObject->RenderMesh();


		// MOVE
		float timeInSin = sin(glfwGetTime() * 5.0f);
		if (timeInSin < 0)
		{
			timeInSin = 0;
		}
		dx = dx + (timeInSin * deltaTime);
		Move(modelMatrix, dx, timeInSin, 0.0f, 1.0f, 0.0f);

		// DRAW MODEL
		model->Draw(*myShader);

		mainWindow.swapBuffers();
	}
	delete texture;
}

// FUNCTION IMPLEMENTATIONS
float GetRandom(float min, float max)
{
	return ((float)rand() / ((float)RAND_MAX)) * (max - min) + min;
}

void ChangeObjectColor(Mesh* object, glm::vec3 color)
{
	int objectColorLocation = glGetUniformLocation(myShader->ID, "objectColor");
	glUniform3f(objectColorLocation, color.x, color.y, color.z);
}

void Move(glm::mat4 modelMatrix, float dx, float timeInSin, float directionX, float directionY, float directionZ)
{
	modelMatrix = glm::translate(modelMatrix, glm::vec3(dx, timeInSin, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3(directionX, directionY, directionZ));
	glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "modelMatrix"),
		1, GL_FALSE, glm::value_ptr(modelMatrix));
}
