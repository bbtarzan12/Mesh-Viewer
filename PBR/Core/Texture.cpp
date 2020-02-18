#include "Texture.h"
#include "../Util/TextureLoader.h"


Texture::Texture(const std::string& filePath, const bool sRGB)
	:sRGB(sRGB)
{
	if (filePath.empty())
		return;

	TextureLoader::Load(filePath, id, size, type, sRGB);
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

const bool& Texture::GetsRGB() const
{
	return sRGB;
}
