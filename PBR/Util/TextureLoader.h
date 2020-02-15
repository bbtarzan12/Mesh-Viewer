#pragma once

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

class TextureLoader
{
public:
	static void Load(const std::string& filePath, GLuint& id, glm::ivec2& size);
};