#include "Phong.h"
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

Phong::Phong()
	:uvScale(1, 1),
	uvOffset(0, 0),
	shininess(30),
	useBlinnPhong(false),
	Material
	(
		"Shaders/phong_vert.glsl", "Shaders/phong_frag.glsl",
		{"cubeMapTexture", "diffuseTexture", "normalTexture", "specularTexture"}
	)
{

}

void Phong::Draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const
{
	Material::Draw(model, view, projection);
	glDepthFunc(GL_LESS);

	SetInt("useBlinnPhong", useBlinnPhong);
	SetInt("shininess", shininess);
	SetVec2("uvOffset", uvOffset);
	SetVec2("uvScale", uvScale);
	SetMat4("M", model);
	SetMat4("V", view);
	SetMat4("P", projection);
}

void Phong::DrawUI()
{
	ImGui::Text("Use Blinn-Phong");
	ImGui::SameLine();
	ImGui::Checkbox("##Use Blinn-Phong", &useBlinnPhong);

	ImGui::Text("UV Offset");
	ImGui::SameLine();
	ImGui::DragFloat2("##UV Offset", &uvOffset[0], 0.1f);

	ImGui::Text("UV Scale");
	ImGui::SameLine();
	ImGui::DragFloat2("##UV Scale", &uvScale[0], 0.1f);

	ImGui::Text("Shininess");
	ImGui::SameLine();
	ImGui::DragInt("##Shininess", &shininess, 1.0f, 1, 100);

	if (!DrawTexturePannel("diffuseTexture", {300, 300}))
	{
		ImGui::ColorPicker4("##Color", glm::value_ptr(defaultColors[1]));

		ImGui::EndChild();
	}
	if (!DrawTexturePannel("normalTexture", { 300, 60 }))
	{
		ImGui::TextDisabled("Use Mesh Normal");
		ImGui::EndChild();
	}
	
	if (!DrawTexturePannel("specularTexture", {300, 100}))
	{
		float power = defaultColors[3].r;
		ImGui::Text("Power");
		ImGui::SameLine();
		if (ImGui::SliderFloat("##Power", &power, 0, 1))
		{
			defaultColors[3].r = power;
			defaultColors[3].g = power;
			defaultColors[3].b = power;
		}
		ImGui::EndChild();
	}
}