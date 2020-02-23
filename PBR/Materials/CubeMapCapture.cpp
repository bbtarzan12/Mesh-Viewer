#include "CubeMapCapture.h"

CubeMapCapture::CubeMapCapture()
	:Material
	(
		"Shaders/captureIrradianceMap_vert.glsl",
		"Shaders/captureIrradianceMap_frag.glsl",
		{"cubeMapTexture"},
		false
	)
{

}

void CubeMapCapture::Draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const
{
	Material::Draw(model, view, projection);
	SetMat4("P", projection);
	SetMat4("V", view);
}