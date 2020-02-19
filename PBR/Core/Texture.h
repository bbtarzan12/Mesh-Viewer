#pragma once
#include <GL/glew.h>
#include <string>
#include <memory>
#include <glm/glm.hpp>

class Texture
{
public:
	Texture(const std::string& filePath, const bool sRGB = false);
	Texture(const GLenum& internalformat, const GLenum& dataType, const glm::ivec2& size, const bool sRGB = false);
	virtual ~Texture();

	const GLuint& GetID() const;
	const glm::ivec2& GetSize() const;
	const GLenum& GetType() const;
	const bool& GetsRGB() const;

protected:
	Texture(const glm::ivec2& size, const GLenum& type, const bool& sRGB = false);
	Texture() = default;

protected:
	GLuint id;
	glm::ivec2 size;
	GLenum type;
	bool sRGB;
};