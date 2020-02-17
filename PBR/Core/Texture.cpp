#include "Texture.h"
#include "../Util/TextureLoader.h"


Texture::Texture(const std::string& filePath)
{
	TextureLoader::Load(filePath, id, size, type);
}

Texture::~Texture()
{
	glDeleteTextures(1, &id);
}

const GLuint& Texture::GetID() const
{
	return id;
}

const glm::ivec2& Texture::GetSize() const
{
	return size;
}

const GLenum& Texture::GetType() const
{
	return type;
}