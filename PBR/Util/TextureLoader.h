#pragma once

#include <vector>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>


struct FIBITMAP;

class TextureLoader
{
public:
	static void Load(const std::string& filePath, GLuint& id, glm::ivec2& size, GLenum& type, const bool sRGB);
	static void Load(const std::vector<std::string>& facesPath, GLuint& id, glm::ivec2& size, GLenum& type, const bool sRGB);
	static void Create(GLenum& type, const GLenum& internalformat, const GLenum& dataType, GLuint& id, const glm::ivec2& size, const bool generateMipMap);
	static void CreateCubeMap(GLenum& type, const GLenum& internalformat, const GLenum& dataType, GLuint& id, const glm::ivec2& size, const bool generateMipMap);

private:
	static FIBITMAP* LoadImage(const std::string& filePath, glm::ivec2& size);
	static void UnloadImage(FIBITMAP* image);
};