#include "CubeMapCapture.h"

CubeMapCapture::CubeMapCapture()
	:Material
	(
		"Shaders/cubeMapCapture_vert.glsl",
		"Shaders/cubeMapCapture_frag.glsl",
		{"cubeMapTexture"}
	)
{

}

void CubeMapCapture::Draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const
{
	Material::Draw(model, view, projection);
	SetMat4("P", projection);
	SetMat4("V", view);
}

void CubeMapCapture::DrawUI()
{

}