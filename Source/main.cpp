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

GLfloat vertices_triangle[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f
};
GLuint indices_triangle[] = {
	0, 1, 2
};

//FUNCTIONS
void MainLoop();
int OpenWindow(int iWidth, int iHeight);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main()
{
	OpenWindow(SCR_WIDTH, SCR_HEIGHT);
	glClearColor(0.3, 0.2, 0.5, 1.0);
	MainLoop();
}

void MainLoop()
{
	myShader = new Shader("../../Shaders/myShader.vs", "../../Shaders/myShader.fs");

	Mesh *triangle_object = new Mesh();
	triangle_object->CreateMesh(vertices_triangle, indices_triangle, 9, 3);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		myShader->use();

		triangle_object->RenderMesh(); // draw

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	delete triangle_object;
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