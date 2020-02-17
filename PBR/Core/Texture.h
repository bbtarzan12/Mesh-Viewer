#pragma once
#include <GL/glew.h>
#include <string>
#include <memory>
#include <glm/glm.hpp>

class Texture
{
public:
	Texture(const std::string& filePath);
	virtual ~Texture();

	const GLuint& GetID() const;
	const glm::ivec2& GetSize() const;
	const GLenum& GetType() const;

protected:
	Texture() = default;

protected:
	GLuint id;
	glm::ivec2 size;
	GLenum type;
};