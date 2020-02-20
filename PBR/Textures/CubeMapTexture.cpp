#include "CubeMapTexture.h"
#include "../Util/TextureLoader.h"


CubeMapTexture::CubeMapTexture(const std::vector<std::string>& facesPath, const bool sRGB)
	:Texture(size, type, sRGB)
{
	TextureLoader::Load(facesPath, id, size, type, sRGB);
}

CubeMapTexture::CubeMapTexture(const GLenum& internalformat, const GLenum& dataType, const glm::ivec2& size, const bool mipMap)
	:Texture(size, type, internalformat == GL_SRGB)
{
	TextureLoader::CreateCubeMap(type, internalformat, dataType, id, size, mipMap);
}
