#include "Texture.h"
#include "../Util/TextureLoader.h"


Texture::Texture(const std::string& filePath, const bool sRGB)
	:sRGB(sRGB)
{
	if (filePath.empty())
		return;

	TextureLoader::Load(filePath, id, size, type, sRGB);
}

Texture::Texture(const GLenum& internalformat, const GLenum& dataType, const glm::ivec2& size, const bool mipMap)
	:sRGB(internalformat == GL_SRGB), size(size)
{
	TextureLoader::Create(type, internalformat, dataType, id, size, mipMap);
}

Texture::Texture(const glm::ivec2& size, const GLenum& type, const bool& sRGB)
	:size(size), type(type), sRGB(sRGB)
{

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
