# include "Button.h"

#define SCR_WIDTH     (1600.0F)
#define SCR_HEIGHT    (900.0F)
#define BUTTON_WIDTH  (400.0F)
#define BUTTON_HEIGHT (50.0F)

static GLfloat buttonVertices[] =
{  //POSITION												 //TEXT COORDS
	- BUTTON_WIDTH / 2, - BUTTON_HEIGHT / 2, 0.0f, 0.0f, 0.0f, //bottom left 0
	+ BUTTON_WIDTH / 2, - BUTTON_HEIGHT / 2, 0.0f, 1.0f, 0.0f, //buttom right 1
	+ BUTTON_WIDTH / 2, + BUTTON_HEIGHT / 2, 0.0f, 1.0f, 1.0f, //top right 2
	- BUTTON_WIDTH / 2, + BUTTON_HEIGHT / 2, 0.0f, 0.0f, 1.0f//top left 3
};
static GLuint buttonIndices[] =
{
	0, 1, 2,
	2, 3, 0
};

Button::Button(glm::vec3 buttonPosition, glm::vec3 buttonColor, glm::vec3 buttonScale)
{
	buttonShader = new Shader("../../Shaders/buttonShader.vs", "../../Shaders/buttonShader.fs");
	numOfIndices = 6;
	numOfVertices = 20;
	this->buttonPosition = buttonPosition;
	this->buttonScale = buttonScale;
	this->buttonColor = buttonColor;

	model = glm::mat4(1.0f);
	model = glm::translate(model, buttonPosition);
	model = glm::scale(model, buttonScale);
	projection = glm::ortho(-SCR_WIDTH / 2, SCR_WIDTH / 2, -SCR_HEIGHT / 2, SCR_HEIGHT / 2);

	InitialiseButton();
	CreateBoundingBox();
}

void Button::InitialiseButton()
{
	glGenVertexArrays(1, &buttonVAO);
	glGenBuffers(1, &buttonIBO);
	glGenBuffers(1, &buttonVBO);

	glBindVertexArray(buttonVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buttonIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(buttonIndices[0]) * numOfIndices, buttonIndices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, buttonVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(buttonVertices[0]) * numOfVertices, buttonVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void Button::SetButtonTextures(Texture* buttonTexture)
{
	this->buttonTexture = buttonTexture;
}

void Button::DrawButton()
{
	buttonShader->use();
	buttonShader->setMat4("projection", projection);
	buttonShader->setMat4("model", model);
	buttonShader->setVec3("color", buttonColor);

	buttonTexture->ActivateTexture(GL_TEXTURE0);
	buttonShader->setInt("buttonTexture", 0);
	
	glBindVertexArray(buttonVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Button::CreateBoundingBox()
{
	boundingBoxLeftBottom = model * glm::vec4(buttonVertices[0], buttonVertices[1], 1.0f, 1.0f);
	boundingBoxRightTop = model * glm::vec4(buttonVertices[10], buttonVertices[11], 1.0f, 1.0f);
}

bool Button::ClickButton(GLfloat xCursorPos, GLfloat yCursorPos)
{
	if (xCursorPos < boundingBoxRightTop.x && xCursorPos > boundingBoxLeftBottom.x &&
		yCursorPos < boundingBoxRightTop.y && yCursorPos > boundingBoxLeftBottom.y)
	{
		return true;
	}
	else { return false; }
}