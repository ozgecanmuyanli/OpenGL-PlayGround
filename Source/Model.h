#include <string>
#include <vector>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec2 textCoord;
	glm::vec3 normal;

	Vertex() {}
	Vertex(glm::vec3 position, glm::vec2 textCoord, glm::vec3 normal)
	{
		this->position = position;
		this->textCoord = textCoord;
		this->normal = normal;
	}
};

struct
{
	GLuint id;
	std::string type;
};

class ModelMesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	ModelMesh() { }
	ModelMesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
	{
		this->vertices = vertices;
		this->indices = indices;

		setupMesh();
	}
	void Draw(Shader shader)
	{
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		glBindVertexArray(0);
	}

private:
	unsigned int VAO, VBO, EBO;
	void setupMesh()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		//glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);*/

		/* vertex positions */
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
		/* vertex texture coords */
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, textCoord));
		/* vertex normals */
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));

		glBindVertexArray(0);
	}
};

class Model
{
public:
	glm::mat4 model;
	bool isModelValid;
	Model(std::string path)
	{
		model = glm::mat4(1.0);
		this->isModelValid = this->loadOBJModel(path);
	}

	void Draw(Shader shader)
	{
		for (int i = 0; i < meshes.size(); i++)
		{
			meshes[i].Draw(shader);

			//model = glm::rotate(model, (float)glm::radians(135.0f), glm::vec3(0.0, 1.0, 0.0));
			//shader.setFloat("timeSin", (((float)glm::sin(glfwGetTime()) + 1.0f) / 2.0f) / 8);
			//model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
			shader.setMat4("model", model);
		}
	}

private:
	std::vector<ModelMesh> meshes;
	bool loadOBJModel(std::string path)
	{
		if (path.substr(path.size() - 4, 4) != ".obj")
			return false;

		std::ifstream file(path);
		if (!file.is_open())
			return false;

		std::string line;
		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> textCoords;
		std::vector<glm::vec3> normals;
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		unsigned int ind = 0;
		while (std::getline(file, line))
		{
			std::string type = line.substr(0, 2);
			/* Position of vertex*/
			if (type == "v ")
			{
				std::istringstream v(line.substr(2));
				double x, y, z;
				v >> x >> y >> z;
				positions.push_back(glm::vec3(x, y, z));
			}
			/* Texture coordinate of vertex */
			else if (type == "vt")
			{
				std::istringstream vt(line.substr(3));
				double x, y, z;
				vt >> x >> y >> z;
				textCoords.push_back(glm::vec2(x, y));
			}
			/* Normal of surface */
			else if (type == "vn")
			{
				std::istringstream vn(line.substr(2));
				double x, y, z;
				vn >> x >> y >> z;
				normals.push_back(glm::vec3(x, y, z));
			}
			/* Face of surface */
			else if (type == "f ")
			{
				const char* chh = line.c_str();
				int vi, vj, vk;
				int vti, vtj, vtk;
				int vni, vnj, vnk;
				int format_1 = sscanf(chh, "f %i//%i  %i//%i %i//%i", &vi, &vni, &vj, &vnj, &vk, &vnk);
				int format_2 = sscanf(chh, "f %i/%i/%i  %i/%i/%i %i/%i/%i", &vi, &vti, &vni, &vj, &vtj, &vnj, &vk, &vtk, &vnk);
				//if (format_1)
				//{
				//	vertices.push_back(Vertex(positions[vi - 1], glm::vec2(1.0f, 1.0f), normals[vni - 1]));
				//	vertices.push_back(Vertex(positions[vj - 1], glm::vec2(1.0f, 1.0f), normals[vnj - 1]));
				//	vertices.push_back(Vertex(positions[vk - 1], glm::vec2(1.0f, 1.0f), normals[vnk - 1]));
				//}
				if (format_2)
				{
					vertices.push_back(Vertex(positions[vi - 1], textCoords[vti - 1], normals[vni - 1]));
					vertices.push_back(Vertex(positions[vj - 1], textCoords[vtj - 1], normals[vnj - 1]));
					vertices.push_back(Vertex(positions[vk - 1], textCoords[vtk - 1], normals[vnk - 1]));
				}
				else
				{
					std::cout << "Different format !" << std::endl;
					return false;
				}
			}
		}
		file.close();

		positions.clear();
		textCoords.clear();
		normals.clear();

		ModelMesh mesh = ModelMesh(vertices, indices);
		meshes.push_back(mesh);

		vertices.clear();
		indices.clear();

		return true;
	}
};


