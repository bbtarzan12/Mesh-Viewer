#include "Material.h"

void Material::Draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const
{
	glUseProgram(shader);
}

void Material::SetVec3(const std::string& name, const glm::vec3& value)
{
	glUseProgram(shader);
	GLuint id = glGetUniformLocation(shader, name.c_str());

	if (id == -1)
		return;

	glUniform3fv(id, 1, &value[0]);
	glUseProgram(0);
}
