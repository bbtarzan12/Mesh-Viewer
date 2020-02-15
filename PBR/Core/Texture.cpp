#include "Texture.h"
#include "../Util/TextureLoader.h"


std::shared_ptr<Texture> Texture::CreateTexture(const std::string& filePath)
{
	return std::make_shared<Texture>(filePath);
}

Texture::Texture(const std::string& filePath)
{
	TextureLoader::Load(filePath, id, size);
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
