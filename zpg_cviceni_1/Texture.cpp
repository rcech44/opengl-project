#include "Texture.h"

void Texture::assignTexture(const char* path)
{
	switch (this->type)
	{
		case TextureType::Standard:
			glActiveTexture(33984);
			GLuint textureID = SOIL_load_OGL_texture(path, SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glBindTexture(GL_TEXTURE_2D, textureID);
			this->id = textureID;
			break;
	}
}

void Texture::assignTexture(const char* path1, const char* path2, const char* path3, const char* path4, const char* path5, const char* path6)
{
	switch (this->type)
	{
		case TextureType::CubeMap:
			glActiveTexture(33984);
			GLuint image = SOIL_load_OGL_cubemap(path1, path2, path3, path4, path5, path6, SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
			if (image == NULL) {
				exit(EXIT_FAILURE);
			}
			glBindTexture(GL_TEXTURE_2D, image);
			glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
			this->id = image;

			break;
	}
}

void Texture::setTextureType(int type)
{
	this->type = type;
}

int Texture::getID()
{
	return this->id;
}
