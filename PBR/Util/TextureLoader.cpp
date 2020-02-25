#include "TextureLoader.h"
#include <cassert>

#include <FreeImage/FreeImage.h>

void TextureLoader::Load(const std::string& filePath, GLuint& id, glm::ivec2& size, GLenum& target, const GLenum internalFormat, const GLenum format, const GLenum type, const bool generateMipMap)
{
	FIBITMAP* image = LoadImage(filePath, size);


	target = GL_TEXTURE_2D;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.x, size.y, 0, format, type, FreeImage_GetBits(image));

	if (generateMipMap)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	UnloadImage(image);
}

void TextureLoader::Load(const std::vector<std::string>& facesPath, GLuint& id, glm::ivec2& size, GLenum& target, const GLenum internalFormat, const GLenum format, const GLenum type, const bool generateMipMap)
{
	target = GL_TEXTURE_CUBE_MAP;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	if (generateMipMap)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	for (int i = 0; i < facesPath.size(); i++)
	{
		FIBITMAP* image = LoadImage(facesPath[i], size);
		FreeImage_FlipHorizontal(image);
		FreeImage_FlipVertical(image);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, size.x, size.y, 0, format, type, FreeImage_GetBits(image));
		UnloadImage(image);
	}
}

void TextureLoader::Create(GLuint& id, const glm::ivec2& size, GLenum& target, const GLenum internalFormat, const GLenum format, const GLenum type, const bool generateMipMap)
{
	target = GL_TEXTURE_2D;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.x, size.y, 0, format, type, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	if (generateMipMap)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
}

void TextureLoader::CreateCubeMap(GLuint& id, const glm::ivec2& size, GLenum& target, const GLenum internalFormat, const GLenum format, const GLenum type, const bool generateMipMap)
{
	target = GL_TEXTURE_CUBE_MAP;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (generateMipMap)
	{
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	for (int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, size.x, size.y, 0, format, type, nullptr);
		if (generateMipMap)
		{
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		}
	}
}

FIBITMAP* TextureLoader::LoadImage(const std::string& filePath, glm::ivec2& size)
{
	FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(filePath.c_str());

	FIBITMAP* temp = FreeImage_Load(fif, filePath.c_str());
	assert(temp);

	FIBITMAP* image = FreeImage_ConvertTo24Bits(temp);
	assert(image);

	size.x = FreeImage_GetWidth(image);
	size.y = FreeImage_GetHeight(image);

	UnloadImage(temp);

	return image;
}

void TextureLoader::UnloadImage(FIBITMAP* image)
{
	FreeImage_Unload(image);
}
