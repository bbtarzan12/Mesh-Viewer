#pragma once

#include <vector>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>


struct FIBITMAP;

class TextureLoader
{
public:
	static void Load(const std::string& filePath, GLuint& id, glm::ivec2& size);
	static void Load(const std::vector<std::string>& facesPath, GLuint& id);

private:
	static FIBITMAP* LoadImage(const std::string& filePath, glm::ivec2& size);
	static void UnloadImage(FIBITMAP* image);
};