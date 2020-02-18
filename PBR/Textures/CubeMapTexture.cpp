#include "CubeMapTexture.h"
#include "../Util/TextureLoader.h"


CubeMapTexture::CubeMapTexture(const std::vector<std::string>& facesPath, const bool sRGB)
	:Texture("", sRGB)
{
	TextureLoader::Load(facesPath, id, size, type, sRGB);
}
