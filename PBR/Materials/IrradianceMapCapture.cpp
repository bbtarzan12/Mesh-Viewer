#include "IrradianceMapCapture.h"

IrradianceMapCapture::IrradianceMapCapture()
	:Material
	(
		"Shaders/captureIrradianceMap_vert.glsl",
		"Shaders/captureIrradianceMap_frag.glsl",
		{"cubeMapTexture"},
		false
	)
{

}

void IrradianceMapCapture::Draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const
{
	Material::Draw(model, view, projection);
	SetMat4("P", projection);
	SetMat4("V", view);
}

void IrradianceMapCapture::SetCaptureTexture(const std::shared_ptr<Texture>& texture)
{
	SetTexture("cubeMapTexture", texture);
}
