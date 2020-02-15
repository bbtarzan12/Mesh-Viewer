#pragma once

#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Material;

class Mesh
{

public:
	Mesh(const std::string& filePath);
	~Mesh();

	void Draw(const glm::mat4& view, const glm::mat4& projection);
	void SetMaterial(const std::shared_ptr<Material> newMaterial);
	std::shared_ptr<Material> GetMaterial();

private:
	void Load(const std::string& filePath);

public:
	std::vector<glm::vec3> vertices, normals, tangents;
	std::vector<unsigned int> indices;
	std::vector<glm::vec2> uvs;
	glm::mat4 model;
	GLuint vertexArray, indexArray, vertexPosition, vertexNormal, vertexUV, vertexTangent;
	std::shared_ptr<Material> material;
};