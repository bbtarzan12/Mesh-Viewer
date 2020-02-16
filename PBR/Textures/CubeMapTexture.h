#pragma once

#include "../Core/Texture.h"
#include <vector>

class CubeMapTexture : public Texture
{
public:
	CubeMapTexture(const std::vector<std::string>& facesPath);
	virtual ~CubeMapTexture() = default;
};