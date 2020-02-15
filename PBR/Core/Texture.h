#pragma once
#include <GL/glew.h>
#include <string>
#include <memory>
#include <glm/glm.hpp>

class Texture
{
public:
	static std::shared_ptr<Texture> CreateTexture(const std::string& filePath);

	Texture(const std::string& filePath);
	~Texture();

	const GLuint& GetID() const;
	const glm::ivec2& GetSize() const;

private:
	GLuint id;
	glm::ivec2 size;
};