#include "Phong.h"
#include "../Util/ShaderLoader.h"
#include <imgui.h>

Phong::Phong()
	:uvScale(1, 1), uvOffset(0, 0), IOR(1.5)
{
	shader = ShaderLoader::Load("Shaders/phong_vert.glsl", "Shaders/phong_frag.glsl");

	glUseProgram(shader);

	GLuint cubeMapTexture = glGetUniformLocation(shader, "cubeMaptexture");
	glUniform1i(cubeMapTexture, 0);

	GLuint diffuseTexture = glGetUniformLocation(shader, "diffuseTexture");
	glUniform1i(diffuseTexture, 1);

	GLuint normalTexture = glGetUniformLocation(shader, "normalTexture");
	glUniform1i(normalTexture, 2);

	GLuint specularTexture = glGetUniformLocation(shader, "specularTexture");
	glUniform1i(specularTexture, 3);
	glUseProgram(0);
}

void Phong::Draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const
{
	Material::Draw(model, view, projection);
	glDepthFunc(GL_LESS);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GetTextureID("cubeMapTexture"));

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, GetTextureID("diffuse"));

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, GetTextureID("normal"));

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, GetTextureID("specular"));

	SetVec2("uvOffset", uvOffset);
	SetVec2("uvScale", uvScale);
	SetFloat("IOR", IOR);
	SetMat4("M", model);
	SetMat4("V", view);
	SetMat4("P", projection);
}

void Phong::DrawUI()
{
	ImGui::Text("UV Offset");
	ImGui::SameLine();
	ImGui::DragFloat2("##UV Offset", &uvOffset[0], 0.1f);

	ImGui::Text("UV Scale");
	ImGui::SameLine();
	ImGui::DragFloat2("##UV Scale", &uvScale[0], 0.1f);

	ImGui::Text("IOR");
	ImGui::SameLine();
	ImGui::SliderFloat("##IOR", &IOR, 1.0f, 3.5f, "%.4f");

	DrawTexturePannel("diffuse");
	DrawTexturePannel("normal");
	DrawTexturePannel("specular");
}