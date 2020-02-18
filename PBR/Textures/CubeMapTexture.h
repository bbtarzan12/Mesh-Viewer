#pragma once

#include "../Core/Texture.h"
#include <vector>

class CubeMapTexture : public Texture
{
public:
	CubeMapTexture(const std::vector<std::string>& facesPath, const bool sRGB = false);
	virtual ~CubeMapTexture() = default;
};