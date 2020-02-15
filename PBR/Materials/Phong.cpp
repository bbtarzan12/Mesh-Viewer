#include "Phong.h"
#include "../Util/ShaderLoader.h"

Phong::Phong()
{
	shader = ShaderLoader::Load("Shaders/phong_vert.glsl", "Shaders/phong_frag.glsl");
	modelMatrix = glGetUniformLocation(shader, "M");
	viewMatrix = glGetUniformLocation(shader, "V");
	projectionMatrix = glGetUniformLocation(shader, "P");
	diffuseID = glGetUniformLocation(shader, "diffuseTexture");
	normalID = glGetUniformLocation(shader, "normalTexture");
	specularID = glGetUniformLocation(shader, "specularTexture");
}

void Phong::Draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const
{
	Material::Draw(model, view, projection);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GetTextureID("diffuse"));
	glUniform1i(diffuseID, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, GetTextureID("normal"));
	glUniform1i(normalID, 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, GetTextureID("specular"));
	glUniform1i(specularID, 2);

	glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projectionMatrix, 1, GL_FALSE, &projection[0][0]);
}

void Phong::DrawUI()
{
	DrawTexturePannel("diffuse");
	DrawTexturePannel("normal");
	DrawTexturePannel("specular");
}
