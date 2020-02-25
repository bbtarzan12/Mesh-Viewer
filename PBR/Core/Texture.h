#pragma once
#include <GL/glew.h>
#include <string>
#include <memory>
#include <glm/glm.hpp>

class Texture
{
public:
	Texture(const std::string& filePath, const GLenum internalFormat = GL_RGB, const GLenum format = GL_BGR, const GLenum type = GL_UNSIGNED_BYTE, const bool generateMipMap = false);
	Texture(const glm::ivec2& size, const GLenum internalFormat = GL_RGB, const GLenum format = GL_BGR, const GLenum type = GL_UNSIGNED_BYTE, const bool generateMipMap = false);
	virtual ~Texture();

	const GLuint& GetID() const;
	const glm::ivec2& GetSize() const;
	const GLenum& GetTarget() const;
	const bool& GetsRGB() const;

protected:
	Texture(const glm::ivec2& size, const GLenum& target, const bool& sRGB = false);
	Texture() = default;

protected:
	GLuint id;
	glm::ivec2 size;
	GLenum target;
	bool sRGB;
};