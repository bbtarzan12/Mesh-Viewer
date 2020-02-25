#include "CubeMapTexture.h"
#include "../Util/TextureLoader.h"


CubeMapTexture::CubeMapTexture(const std::vector<std::string>& facesPath, const GLenum internalFormat, const GLenum format, const GLenum type, const bool generateMipMap)
	:Texture(size, target, internalFormat == GL_SRGB)
{
	TextureLoader::Load(facesPath, id, size, target, internalFormat, format, type, generateMipMap);
}

CubeMapTexture::CubeMapTexture(const glm::ivec2& size, const GLenum internalFormat, const GLenum format, const GLenum type, const bool generateMipMap)
	:Texture(size, target, internalFormat == GL_SRGB)
{
	TextureLoader::CreateCubeMap(id, size, target, internalFormat, format, type, generateMipMap);
}
