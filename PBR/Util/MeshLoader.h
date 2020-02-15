#pragma once

#include <vector>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

class MeshLoader
{
public:
	static void Load(const std::string& filePath, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& tangents, std::vector<unsigned int>& indices);
};