#pragma once

#include "../Core/Texture.h"
#include <vector>

class CubeMapTexture : public Texture
{
public:
	CubeMapTexture(const std::vector<std::string>& facesPath, const bool sRGB = false);
	CubeMapTexture(const GLenum& internalformat, const GLenum& dataType, const glm::ivec2& size, const bool mipMap = false);
	virtual ~CubeMapTexture() = default;
};