#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include <string>
#include <vector>
#include "Texture.h"
using namespace std;

#define MAX_BONE_INFLUENCE 4

struct Vertex {
   glm::vec3 Position;
   glm::vec3 Normal;
   glm::vec2 TexCoords;
   glm::vec3 Tangent;
   glm::vec3 Bitangent;
};


class Mesh {
public:
   // mesh data
   vector<Vertex> vertices;
   vector<unsigned int> indices;
   vector<Texture> textures;
   //unsigned int VAO;

   // constructor
   Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
   void Draw(Shader& shader);

private:
   // render data
   unsigned int VAO, VBO, EBO;
   void setupMesh();
};

#endif
