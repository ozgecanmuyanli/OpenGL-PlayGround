# include "Texture.h"

Texture::Texture()
{
	int texture_width = 0;
	int texture_height = 0;
	int nrChannels = 0;
}

Texture::Texture(const char* path, const std::string& directory)
{
	this->path = path;
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	glGenTextures(1, &textureID);

	int width, height, nrComponent;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponent, 0);
	if (data)
	{
		GLenum format;
		if (nrComponent == 1)
		{
			format = GL_RED;
		}
		else if (nrComponent == 3)
		{
			format = GL_RGB;
		}
		else if (nrComponent == 4)
		{
			format = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture load is failed at path: " << path << std::endl;
		stbi_image_free(data);
	}
}

void Texture::LoadTextureCPU(char* texture_path)
{
	texture_data = stbi_load(texture_path, &texture_width, &texture_height, &nrChannels, 0);

	//stbi_set_flip_vertically_on_load(true);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
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
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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
	glBindTexture(GL_TEXTURE_2D, this->textureID);
}

void Texture::UnbindTexture()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::SetMapTexture()
{
	unsigned char* pixelOffset;
	float red, green, blue;
	float translateX, translateZ;
	
	for (int i = 0; i < texture_width; i++)
	{
		for (int j = 0; j < texture_height; j++)
		{
			pixelOffset = texture_data + (j * texture_width + i) * 3;
			red = pixelOffset[0]; //wall
			green = pixelOffset[1]; //tree
			blue = pixelOffset[2]; //water

			// Pixels(i, j) are mapped according to texture coord
			translateX = (2.0f * i/ texture_width) + (1.0f / texture_width) - 1.0f;
			translateZ = -((2.0f * j/ texture_height) + (1.0f / texture_height) - 1.0f);

			if (red == 0 && green == 0 && blue == 0)
			{
				rockEntityTranslateValues.push_back(glm::vec2(translateX, translateZ));
			}
			else if (red > 0 && green == 0 && blue == 0)
			{
				wallEntityTranslateValues.push_back(glm::vec2(translateX, translateZ));
			}
			else if (red == 0 && green > 0 && blue == 0)
			{
				treeEntityTranslateValues.push_back(glm::vec2(translateX, translateZ));
			}
			else if (red == 0 && green == 0 && blue > 0)
			{
				waterEntityTranslateValues.push_back(glm::vec2(translateX, translateZ));
			}
			else if (red > 0 && green > 0 && blue > 0)
			{
				goldenEntityTranslateValues.push_back(glm::vec2(translateX, translateZ));
			}
		}
	}
	stbi_image_free(texture_data);
}
