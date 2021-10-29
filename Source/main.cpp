#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "stb_image.h"

//VARIABLES
GLFWwindow* window;
Shader* myShader;
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 800.0f / 2.0f;
float lastY = 600.0f / 2.0f;
float fov = 45.0f;

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame


#define OBJECTCOUNT 10
glm::mat4 modelMatrix[OBJECTCOUNT];
glm::vec3 objectColors[OBJECTCOUNT];
glm::mat4 backgroundModelMatrix;

GLfloat cubeVertices[] =
{
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

	-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
};

//FUNCTIONS
void MainLoop();
int OpenWindow(int iWidth, int iHeight);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void ChangeObjectColor(Mesh* object, glm::vec3 color);
float GetRandom(float min, float max);
void CreateRandomModelMatrix();
void CreateRandomColors();
void processInput(GLFWwindow* window);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPos += cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPos -= cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed: y ranges bottom to top
	lastX = xpos;
	lastY = ypos;
	
	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset;
	if (fov < 1.0f)
	{
		fov = 1.0f;
	}
	if (fov > 45.0f)
	{
		fov = 45.0f;
	}
}

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
	return ((float)rand() / ((float)RAND_MAX)) * (max-min) + min;
}

void ChangeObjectColor(Mesh* object, glm::vec3 color)
{
	int objectColorLocation = glGetUniformLocation(myShader->ID, "objectColor");
	glUniform3f(objectColorLocation, color.x, color.y, color.z);
}

int main()
{
	srand(time(0));
	OpenWindow(SCR_WIDTH, SCR_HEIGHT);
	glClearColor(0.1, 0.4, 0.1, 1.0);
	CreateRandomModelMatrix();
	CreateRandomColors();
	MainLoop();
}

void MainLoop()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	myShader = new Shader("../../Shaders/myShader.vs", "../../Shaders/myShader.fs");
	float lightAngle = 0.0f;
	
	Mesh* cubeObject = new Mesh();
	cubeObject->CreateMesh(cubeVertices, 0, 180, 0);

	Texture* texture = new Texture();
	texture->LoadTexture("../../Textures/wood.png");

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		myShader->use();
		
		glm::mat4 projectionMatrix = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

		const float radius = 10.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		glm::mat4 viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));

		texture->ActivateTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(myShader->ID, "texture1"), 0);
		cubeObject->RenderMesh();

		//lightAngle = lightAngle + 0.05f;
		//glUniform1f(glGetUniformLocation(myShader->ID, "lightAngle"), lightAngle);

		// 1- send each model matrix, 2- set a color, 3- draw
		for (unsigned int i = 0; i <= OBJECTCOUNT; i++)
		{
			glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix[i]));
			cubeObject->RenderMesh();
		}
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	delete cubeObject;
	glfwDestroyWindow(window);
	glfwTerminate();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

int OpenWindow(int iWidth, int iHeight)
{
	int ret = glfwInit();
	if (!ret)
	{
		std::cout << "glfwInit failed\n";
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	
	if (!(window = glfwCreateWindow(iWidth, iHeight, "Window 1", NULL, NULL)))
	{
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);

	int gladSucces = 0;
	gladSucces = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	if (!gladSucces)
	{
		std::cout << "Failed to initialize GLAD\n";
		return -1;
	}
	return 1;
}