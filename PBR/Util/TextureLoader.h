#pragma once

#include <string>
#include <GL/glew.h>

class TextureLoader
{
public:
	static GLuint Load(const std::string& filePath);
};