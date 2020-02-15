#pragma once

#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext/quaternion_float.hpp>

class Material;

class Mesh
{

public:
	Mesh(const std::string& filePath);
	~Mesh();

	void Draw(const glm::mat4& view, const glm::mat4& projection) const;
	void DrawUI();

	void SetMaterial(const std::shared_ptr<Material> newMaterial);
	const std::string& GetName() const;
	std::shared_ptr<Material> GetMaterial();

private:
	void Load(const std::string& filePath);

private:
	std::string name;

	std::vector<glm::vec3> vertices, normals, tangents;
	std::vector<unsigned int> indices;
	std::vector<glm::vec2> uvs;

	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;

	GLuint vertexArray, indexArray, vertexPosition, vertexNormal, vertexUV, vertexTangent;
	std::shared_ptr<Material> material;
};