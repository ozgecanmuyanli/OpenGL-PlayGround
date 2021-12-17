#pragma once
//#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "stb_image.h"
#include "stdio.h"
#include<iostream>

class Texture
{
public:
   Texture();

   void LoadTexture(char *texture_path);
   void ActivateTexture(GLenum texture_unit);
   void BindTexture();
   void UnbindTexture();

private:
   unsigned int texture;
   int texture_width, texture_height, nrChannels;
};
