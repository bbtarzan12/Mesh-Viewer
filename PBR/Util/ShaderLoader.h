#pragma once

#include <string>
#include <GL/glew.h>

class ShaderLoader
{
public:
	static GLuint Load(const std::string& vertexFilePath, const std::string& fragmentFilePath);

private:
	static std::string GetShaderCode(const std::string& path, const std::string& includeIdentifier = "#include ");
	static void getFilePath(const std::string& filePath, std::string& pathWithoutFileName);
};