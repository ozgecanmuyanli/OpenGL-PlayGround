#pragma once
//#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "stb_image.h"
#include "stdio.h"
#include <vector>
#include<iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Texture
{
public:
   Texture();

   void LoadTextureGPU(char *texture_path);
   void LoadTextureCPU(char* texture_path);
   void ActivateTexture(GLenum texture_unit);
   void BindTexture();
   void UnbindTexture();
   void SetMapTexture();
   int GetTextureWidth() { return texture_width; }
   int GetTextureHeight() { return texture_height; }

   std::vector<glm::vec2> wallEntityTranslateValues;
   std::vector<glm::vec2> treeEntityTranslateValues;
   std::vector<glm::vec2> waterEntityTranslateValues;
   std::vector<glm::vec2> rockEntityTranslateValues;
   std::vector<glm::vec2> goldenEntityTranslateValues;
   std::vector<GLfloat> gridVertices;

private:
   unsigned char* texture_data;
   unsigned int texture;
   unsigned char* pixelOffset;
   int texture_width, texture_height, nrChannels;
   
};
