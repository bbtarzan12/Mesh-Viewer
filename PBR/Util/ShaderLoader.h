#pragma once

#include <string>
#include <GL/glew.h>

class ShaderLoader
{
public:
	static GLuint Load(const std::string& vertexFilePath, const std::string& fragmentFilePath);
};