//#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "stb_image.h"

class Texture
{
public:
   Texture();

   int LoadTexture(char *texture_path);
   void ActivateTexture(GLenum texture_unit);

private:
   unsigned int texture;
   int texture_width, texture_height, nrChannels;
};
