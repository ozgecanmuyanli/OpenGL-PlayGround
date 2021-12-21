# include "Texture.h"

Texture::Texture()
{
	int texture_width = 0;
	int texture_height = 0;
	int nrChannels = 0;
}

void Texture::LoadTextureCPU(char* texture_path)
{
	texture_data = stbi_load(texture_path, &texture_width, &texture_height, &nrChannels, 0);
}

void Texture::LoadTextureGPU(char *texture_path)
{
	texture_data = stbi_load(texture_path, &texture_width, &texture_height, &nrChannels, 0);
	int channel = 0;
	if (nrChannels == 4)
	{
		channel = GL_RGBA;
	}
	else if (nrChannels == 3)
	{
		channel = GL_RGB;
	}
	else if (nrChannels == 1)
	{
		channel = GL_RED;
	}

	stbi_set_flip_vertically_on_load(true);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, channel, texture_width, texture_height, 0, channel, GL_UNSIGNED_BYTE, texture_data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(texture_data);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::ActivateTexture(GLenum texture_unit)
{
	glActiveTexture(texture_unit); // activate texture unit first
}

void Texture::BindTexture()
{
	glBindTexture(GL_TEXTURE_2D, this->texture);
}

void Texture::UnbindTexture()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::SetMapTexture()
{
	unsigned char* pixelOffset;
	float red, green, blue;
	
	for (int i = 0; i < texture_width; i++)
	{
		for (int j = 0; j < texture_height; j++)
		{
			pixelOffset = texture_data + (j * texture_width + i) * 3;
			red = pixelOffset[0]; //wall
			green = pixelOffset[1]; //tree
			blue = pixelOffset[2]; //water

			if (red > 0 && green == 0 && blue == 0)
			{
				wallEntityTranslateValues.push_back(glm::vec2(i, j));
			}
			else if (red == 0 && green > 0 && blue == 0)
			{
				treeEntityTranslateValues.push_back(glm::vec2(i, j));
			}
			else if (red == 0 && green == 0 && blue > 0)
			{
				waterEntityTranslateValues.push_back(glm::vec2(i, j));
			}
			else if (red == 0 && green == 0 && blue == 0)
			{
				groundEntityTranslateValues.push_back(glm::vec2(i, j));
			}
			else if (red > 0 && green > 0 && blue > 0)
			{
				goldenEntityTranslateValues.push_back(glm::vec2(i, j));
			}
		}
	}
}


