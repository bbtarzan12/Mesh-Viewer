#include "Texture.h"
#include "../Util/TextureLoader.h"


Texture::Texture(const std::string& filePath, const GLenum internalFormat, const GLenum format, const GLenum type, const bool generateMipMap)
	:sRGB(internalFormat == GL_SRGB)
{
	if (filePath.empty())
		return;

	TextureLoader::Load(filePath, id, size, target, internalFormat, format, type, generateMipMap);
}

Texture::Texture(const glm::ivec2& size, const GLenum internalFormat, const GLenum format, const GLenum type, const bool generateMipMap)
	:sRGB(internalFormat == GL_SRGB), size(size)
{
	TextureLoader::Create(id, size, target, internalFormat, format, type, generateMipMap);
}

Texture::Texture(const glm::ivec2& size, const GLenum& target, const bool& sRGB)
	:size(size), target(target), sRGB(sRGB)
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

const GLenum& Texture::GetTarget() const
{
	return target;
}

const bool& Texture::GetsRGB() const
{
	return sRGB;
}
