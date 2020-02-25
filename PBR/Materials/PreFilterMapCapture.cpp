#include "PreFilterMapCapture.h"


PreFilterMapCapture::PreFilterMapCapture()
	:Material
	(
		"Shaders/capturePreFilterSpecularMap_vert.glsl",
		"Shaders/capturePreFilterSpecularMap_frag.glsl",
		{ "cubeMapTexture" },
		false
	)
{

}

void PreFilterMapCapture::Draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const
{
	Material::Draw(model, view, projection);
	SetMat4("P", projection);
	SetMat4("V", view);
}

void PreFilterMapCapture::SetCaptureTexture(const std::shared_ptr<Texture>& texture)
{
	SetTexture("cubeMapTexture", texture);
}

void PreFilterMapCapture::Capture(glm::ivec2& size, const int mip, const int maxMip) const
{
	size.x *= std::pow(0.5f, mip);
	size.y *= std::pow(0.5f, mip);
	float roughness = mip / (float)(maxMip - 1);
	SetFloat("roughness", roughness);
}
