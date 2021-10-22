#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "shader.h"
#include "Mesh.h"
#include "stb_image.h"

//VARIABLES
GLFWwindow* window;
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
Shader* myShader;

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

//FUNCTIONS
void MainLoop();
int OpenWindow(int iWidth, int iHeight);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void ChangeObjectColor(Mesh* object, glm::vec3 color);

void ChangeObjectColor(Mesh* object, glm::vec3 color)
{
	int objectColorLocation = glGetUniformLocation(myShader->ID, "objectColor");
	glUniform3f(objectColorLocation, color.x, color.y, color.z);
}

int main()
{
	OpenWindow(SCR_WIDTH, SCR_HEIGHT);
	glClearColor(0.3, 0.2, 0.5, 1.0);
	MainLoop();
}

void MainLoop()
{
	myShader = new Shader("../../Shaders/myShader.vs", "../../Shaders/myShader.fs");

	Mesh * square_object = new Mesh();
	square_object->CreateMesh(vertices_square, indices_square, 
		sizeof(vertices_square)/sizeof(GLfloat), sizeof(indices_square)/sizeof(GLfloat));

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		myShader->use();


		ChangeObjectColor(square_object, glm::vec3(1.0f, 0.0f, 0.0f));
		square_object->RenderMesh(); // draw


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