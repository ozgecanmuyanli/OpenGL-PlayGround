#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "shader.h"
#include "Mesh.h"
#include "stb_image.h"
#include <filesystem>

//VARIABLES
GLFWwindow* window;
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
Shader* myShader;

GLfloat vertices0[] = {
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f
};
unsigned int indices0[] = {
	0, 1, 2,
	1, 2, 3,
	2, 3, 0,
	3, 0, 1
};

GLfloat vertices1[] = {
	-0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
	0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
   0.5f, 0.5f, 0.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.0f, 1.0f, 0.0f
};
unsigned int indices1[] = {
	0, 1, 2,
	1, 2, 3,
	2, 3, 0,
	3, 0, 1
};

GLfloat vertices2[] = {
	0.7f, 0.5f, 0.0f,
	0.9f, 0.5f, 0.0f,
	0.8f, 0.9f, 0.0f
};
unsigned int indices2[] = {
	0, 1, 2,
	1, 2, 0,
	2, 0, 1
};

GLfloat vertices3[] = {
	-0.9f, 0.5f, 0.0f,
	-0.7f, 0.5f, 0.0f,
	-0.8f, 0.9f, 0.0f
};
unsigned int indices3[] = {
	0, 1, 2,
	1, 2, 0,
	2, 0, 1
};

GLfloat vertices4[] = {
	-0.9f, -0.5f, 0.0f,
	-0.7f, -0.5f, 0.0f,
	-0.8f, -0.1f, 0.0f
};
unsigned int indices4[] = {
	0, 1, 2,
	1, 2, 0,
	2, 0, 1
};

GLfloat vertices5[] = {
	0.7f, -0.5f, 0.0f,
	0.9f, -0.5f, 0.0f,
	0.8f, -0.1f, 0.0f
};
unsigned int indices5[] = {
	0, 1, 2,
	1, 2, 0,
	2, 0, 1
};

GLfloat square_vertices[] = {
	-0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
	0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
	0.5f, 0.5f, 0.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.0f, 1.0f, 0.0f
};
unsigned int square_indices[] = {
	0, 1, 2,
	1, 2, 3,
	2, 3, 0,
	3, 0, 1
};

glm::mat4 modelMatrix = glm::mat4(1.0f);

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

void lightApplication(Mesh *object0, Mesh* object1, Mesh* object2, Mesh* object3, Mesh* object4, Mesh* object5)
{
	//ChangeObjectColor(object0, glm::vec3(0.0f, 1.0f, 0.0f));
	//object0->RenderMesh();

	//ChangeObjectColor(object1, glm::vec3(1.0f, 0.0f, 1.0f));
	object1->RenderMesh();

	//ChangeObjectColor(object2, glm::vec3(0.0f, 1.0f, 0.0f));
	//object2->RenderMesh();

	//ChangeObjectColor(object3, glm::vec3(1.0f, 0.0f, 0.0f));
	//object3->RenderMesh();

	//ChangeObjectColor(object4, glm::vec3(0.0f, 0.0f, 1.0f));
	//object4->RenderMesh();

	//ChangeObjectColor(object5, glm::vec3(1.0f, 1.0f, 0.0f));
	//object5->RenderMesh();
}

int main()
{
	OpenWindow(SCR_WIDTH, SCR_HEIGHT);
	glClearColor(0.3, 0.2, 0.5, 1.0);
	MainLoop();
}

int CreateTexture()
{
	//stbi_set_flip_vertically_on_load(true);
	int texture_width, texture_height, nrChannels;
	unsigned char* texture_data = stbi_load("../../Textures/awesomeface.png", &texture_width, &texture_height, &nrChannels, 0);
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_width, texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(texture_data);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}
void ActivateTexture(unsigned int texture, GLenum texture_unit)
{
	glActiveTexture(texture_unit); // activate texture unit first
	glBindTexture(GL_TEXTURE_2D, texture);
}

void MainLoop()
{
	myShader = new Shader("../../Shaders/myShader.vs", "../../Shaders/myShader.fs");
	float angleOfLightPos = 0.0f;

	Mesh* object0 = new Mesh();
	object0->CreateMesh(vertices0, indices0, 12, 12);
	
	Mesh *object1 = new Mesh();
	object1->CreateMesh(vertices1, indices1, 20, 12);	

	Mesh *object2 = new Mesh();
	object2->CreateMesh(vertices2, indices2, 9, 9);

	Mesh* object3 = new Mesh();
	object3->CreateMesh(vertices3, indices3, 9, 9);

	Mesh* object4 = new Mesh();
	object4->CreateMesh(vertices4, indices4, 9, 9);

	Mesh* object5 = new Mesh();
	object5->CreateMesh(vertices5, indices5, 9, 9);

	Mesh* squareObject = new Mesh();
	squareObject->CreateMesh(square_vertices, square_indices, 20, 12);

	unsigned int texture = CreateTexture();


	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		myShader->use();

		// changes light position along positive z axis
		//float timeValue = sin(glfwGetTime() * 2.0) * 5.0f;
		//int timeUniformLocation = glGetUniformLocation(myShader->ID, "time");
		//glUniform1f(timeUniformLocation, timeValue);

		// changes the light position along a circle (0,0,0. r=0.5)
		angleOfLightPos = angleOfLightPos + 0.05f; // it increases each frame
		int angleLocation = glGetUniformLocation(myShader->ID, "angle");
		glUniform1f(angleLocation, angleOfLightPos);


		ActivateTexture(texture, GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(myShader->ID, "texture1"), 0);
		
		modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::rotate(modelMatrix, glm::radians((float)glfwGetTime() * 10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		int modelMatrixLoc = glGetUniformLocation(myShader->ID, "modelMatrix");
		glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	
		lightApplication(object0, object1, object2, object3, object4, object5);


		//squareObject->RenderMesh();


		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	delete object0, object1, object2, object3, object4, object5;
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