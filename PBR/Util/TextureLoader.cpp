#include "TextureLoader.h"
#include <cassert>

#include <FreeImage/FreeImage.h>

GLuint TextureLoader::Load(const std::string& filePath)
{
	FIBITMAP* temp = FreeImage_Load(FIF_PNG, filePath.c_str());
	assert(temp);

	FIBITMAP* image = FreeImage_ConvertTo24Bits(temp);
	assert(image);

	FreeImage_Unload(temp);

	unsigned int width = FreeImage_GetWidth(image);
	unsigned int height = FreeImage_GetHeight(image);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, image->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	FreeImage_Unload(image);

	return textureID;
}
