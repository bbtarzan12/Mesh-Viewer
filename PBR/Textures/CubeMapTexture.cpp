#include "CubeMapTexture.h"
#include "../Util/TextureLoader.h"


CubeMapTexture::CubeMapTexture(const std::vector<std::string>& facesPath)
{
	TextureLoader::Load(facesPath, id, size, type);
}
