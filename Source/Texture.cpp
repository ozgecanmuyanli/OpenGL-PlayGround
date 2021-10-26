# include "Texture.h"

Texture::Texture()
{
	int texture_width = 0;
	int texture_height = 0;
	int nrChannels = 0;
}

void Texture::LoadTexture(char *texture_path)
{
	unsigned char* texture_data = stbi_load(texture_path, &texture_width, &texture_height, &nrChannels, 0);

	//stbi_set_flip_vertically_on_load(true);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(texture_data);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::ActivateTexture(GLenum texture_unit)
{
	glActiveTexture(texture_unit); // activate texture unit first
	glBindTexture(GL_TEXTURE_2D, this->texture);
}
