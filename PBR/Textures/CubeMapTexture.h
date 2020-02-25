#pragma once

#include "../Core/Texture.h"
#include <vector>

class CubeMapTexture : public Texture
{
public:
	CubeMapTexture(const std::vector<std::string>& facesPath, const GLenum internalFormat = GL_RGB, const GLenum format = GL_BGR, const GLenum type = GL_UNSIGNED_BYTE, const bool generateMipMap = false);
	CubeMapTexture(const glm::ivec2& size, const GLenum internalFormat = GL_RGB, const GLenum format = GL_BGR, const GLenum type = GL_UNSIGNED_BYTE, const bool generateMipMap = false);
	virtual ~CubeMapTexture() = default;
};