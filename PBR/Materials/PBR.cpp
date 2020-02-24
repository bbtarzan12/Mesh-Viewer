#include "PBR.h"
#include <imgui.h>
#include <imgui_custom.h>

PBR::PBR()	:	
	uvScale(1, 1),
	uvOffset(0, 0),
	d(D::OrenNayer),
	ndf(NDF::TrowbridgeReitz),
	g(G::SchlickGGX),
	f(F::SchlickApproximation),
	Material("Shaders/pbr_vert.glsl", "Shaders/pbr_frag.glsl", 
		{
			"irradianceMapTexture",
			"preFilterSpecularMapTexture",
			"brdfSpecularMapTexture",
			"albedoTexture",
			"normalTexture",
			"metallicTexture",
			"roughnessTexture",
			"aoTexture"
		})
{

}

void PBR::Draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const
{
	Material::Draw(model, view, projection);
	glDepthFunc(GL_LESS);

	SetInt("dIndex", static_cast<int>(d));
	SetInt("ndfIndex", static_cast<int>(ndf));
	SetInt("gIndex", static_cast<int>(g));
	SetInt("fIndex", static_cast<int>(f));
	SetVec2("uvOffset", uvOffset);
	SetVec2("uvScale", uvScale);
	SetMat4("M", model);
	SetMat4("V", view);
	SetMat4("P", projection);
}

void PBR::DrawUI()
{
	ImGui::Text("UV Offset");
	ImGui::SameLine();
	ImGui::DragFloat2("##UV Offset", &uvOffset[0], 0.1f);

	ImGui::Text("UV Scale");
	ImGui::SameLine();
	ImGui::DragFloat2("##UV Scale", &uvScale[0], 0.1f);

	ImGui::Text("Diffuse");
	ImGui::SameLine();
	ImGui::Combo("##Diffuse", reinterpret_cast<int*>(&d), dStrings);

	ImGui::Text("NDF");
	ImGui::SameLine();
	ImGui::Combo("##NDF", reinterpret_cast<int*>(&ndf), ndfStrings);

	ImGui::Text("G");
	ImGui::SameLine();
	ImGui::Combo("##G", reinterpret_cast<int*>(&g), gStrings);

	ImGui::Text("F");
	ImGui::SameLine();
	ImGui::Combo("##F", reinterpret_cast<int*>(&f), fStrings);

	if (!DrawTexturePannel("albedoTexture", { 300, 60 }))
	{
		ImGui::ColorEdit4("##AlbedoColor", &defaultColors[3].r);
		ImGui::EndChild();
	}

	if (!DrawTexturePannel("normalTexture", { 300, 60 }))
	{
		ImGui::TextDisabled("Use Mesh Normal");
		ImGui::EndChild();
	}

	if (!DrawTexturePannel("metallicTexture", { 300, 60 }))
	{
		ImGui::Text("Metallic");
		ImGui::SameLine();
		ImGui::SliderFloat("##Metallic", &defaultColors[5].r, 0, 1);
		ImGui::EndChild();
	}

	if (!DrawTexturePannel("roughnessTexture", { 300, 60 }))
	{
		ImGui::Text("Roughness");
		ImGui::SameLine();
		ImGui::SliderFloat("##Roughness", &defaultColors[6].r, 0, 1);
		ImGui::EndChild();
	}

	if (!DrawTexturePannel("aoTexture", { 300, 60 }))
	{
		ImGui::Text("AO");
		ImGui::SameLine();
		ImGui::SliderFloat("##AO", &defaultColors[7].r, 0, 1);
		ImGui::EndChild();
	}
}

const std::vector<std::string> PBR::ndfStrings{ "BlinnPhong", "Beckmann", "TrowbridgeReitz" };
const std::vector<std::string> PBR::fStrings{ "SchlickApproximation" };
const std::vector<std::string> PBR::gStrings{ "SchlickGGX", "GGX" };
const std::vector<std::string> PBR::dStrings{ "Lambertian", "OrenNayer" };
