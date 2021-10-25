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
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
Shader* myShader;
#define OBJECTCOUNT 10
glm::mat4 modelMatrix[OBJECTCOUNT];
glm::vec3 objectColors[OBJECTCOUNT];
glm::mat4 backgroundModelMatrix;


GLfloat vertices_square[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.5f, 0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f
};
GLuint indices_square[] = {
	0, 1, 2,
	2, 3, 0
};

GLfloat vertices_background[] = {
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f
};
GLuint indices_background[] = {
	0, 1, 2,
	2, 3, 0
};

//FUNCTIONS
void MainLoop();
int OpenWindow(int iWidth, int iHeight);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void ChangeObjectColor(Mesh* object, glm::vec3 color);
float GetRandom(float min, float max);
void CreateRandomModelMatrix();
void CreateRandomColors();


void CreateRandomModelMatrix()
{
	float scaleRatio;
	for (unsigned int i = 0; i < OBJECTCOUNT; i++)
	{
		modelMatrix[i] = glm::mat4(1.0f);
		modelMatrix[i] = glm::translate(modelMatrix[i], glm::vec3(GetRandom(-1.0f, 1.0f), GetRandom(-1.0f, 1.0f), 0.0f));
		modelMatrix[i] = glm::rotate(modelMatrix[i], glm::radians(GetRandom(30.0f, 120.0f)), glm::vec3(0.0f, 0.0f, 1.0f));
		scaleRatio = GetRandom(0.2f, 0.5f);
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
	glClearColor(0.3, 0.2, 0.5, 1.0);

	CreateRandomModelMatrix();
	CreateRandomColors();
	MainLoop();
}

void MainLoop()
{
	myShader = new Shader("../../Shaders/myShader.vs", "../../Shaders/myShader.fs");
	float lightAngle = 0.0f;

	Mesh *square_object = new Mesh();
	square_object->CreateMesh(vertices_square, indices_square, 
		sizeof(vertices_square)/sizeof(GLfloat), sizeof(indices_square)/sizeof(GLfloat));

	Mesh* backgroung_object = new Mesh();
	backgroung_object->CreateMesh(vertices_background, indices_background,
		sizeof(vertices_background) / sizeof(GLfloat), sizeof(indices_background) / sizeof(GLfloat));

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		myShader->use();

		backgroundModelMatrix = glm::mat4(1.0f);
		glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(backgroundModelMatrix));
		ChangeObjectColor(backgroung_object, glm::vec3(1.0f, 1.0f, 1.0f));
		backgroung_object->RenderMesh();

		lightAngle = lightAngle + 0.05f;
		glUniform1f(glGetUniformLocation(myShader->ID, "lightAngle"), lightAngle);

		// 1- send each model matrix, 2- set a color, 3- draw
		for (unsigned int i = 0; i <= OBJECTCOUNT; i++)
		{
			glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix[i]));
			ChangeObjectColor(square_object, objectColors[i]);
			square_object->RenderMesh();
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	delete square_object;
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

	int gladSucces = 0;
	gladSucces = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	if (!gladSucces)
	{
		std::cout << "Failed to initialize GLAD\n";
		return -1;
	}
	return 1;
}