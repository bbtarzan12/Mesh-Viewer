#include "Mesh.h"
#include "../Util/MeshLoader.h"
#include "Material.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <imgui.h>
#include <glm/gtx/quaternion.hpp>

Mesh::Mesh(const std::string& filePath)
	: position(0, 0, 0), rotation(1, 0, 0, 0), scale(1, 1, 1)
{
	glGenVertexArrays(1, &vertexArray);
	glGenBuffers(1, &vertexPosition);
	glGenBuffers(1, &vertexNormal);
	glGenBuffers(1, &vertexUV);
	glGenBuffers(1, &vertexTangent);
	glGenBuffers(1, &indexArray);

	if (!filePath.empty())
	{
		Load(filePath);
	}
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vertexArray);
	glDeleteBuffers(1, &vertexPosition);
	glDeleteBuffers(1, &vertexNormal);
	glDeleteBuffers(1, &vertexUV);
	glDeleteBuffers(1, &vertexTangent);
	glDeleteBuffers(1, &indexArray);
}

void Mesh::Draw(const glm::mat4& view, const glm::mat4& projection) const
{
	assert(material);

	glm::mat4 model = glm::translate(position) * glm::toMat4(rotation) * glm::scale(scale);

	material->Draw(model, view, projection);

	glBindVertexArray(vertexArray);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexPosition);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vertexNormal);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, vertexUV);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, vertexTangent);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexArray);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
	
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);

	glBindVertexArray(0);
	glUseProgram(0);
}

void Mesh::DrawUI()
{
	ImGui::Text("Position");
	ImGui::SameLine();
	if (ImGui::DragFloat3("##Position", &position[0], 0.1f))
	{

	}

	glm::vec3 euler = glm::degrees(glm::eulerAngles(rotation));
	ImGui::Text("Rotation");
	ImGui::SameLine();
	if (ImGui::DragFloat3("##Rotation", &euler[0], 0.1f))
	{
		rotation = glm::quat(glm::radians(euler));
	}

	ImGui::Text("Scale");
	ImGui::SameLine();
	if (ImGui::DragFloat3("##Scale", &scale[0], 0.1f))
	{
		
	}
}

void Mesh::SetMaterial(const std::shared_ptr<Material> newMaterial)
{
	material = newMaterial;
}

const std::string& Mesh::GetName() const
{
	return name;
}

std::shared_ptr<Material> Mesh::GetMaterial()
{
	return material;
}

void Mesh::Load(const std::string& filePath)
{
	MeshLoader::Load(filePath, name, vertices, normals, uvs, tangents, indices);

	glBindVertexArray(vertexArray);
	{
		glBindBuffer(GL_ARRAY_BUFFER, vertexPosition);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, vertexNormal);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, vertexUV);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), uvs.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, vertexTangent);
		glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(glm::vec3), tangents.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexArray);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	}
	glBindVertexArray(0);
}
