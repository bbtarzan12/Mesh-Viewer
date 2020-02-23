#include "CubeMap.h"
#include <imgui.h>

CubeMap::CubeMap()
	:Material
	(
		"Shaders/cubeMap_vert.glsl",
		"Shaders/cubeMap_frag.glsl",
		{"cubeMapTexture"},
		false
	)
{

}

void CubeMap::Draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const
{
	glm::mat4 newView = glm::mat4(glm::mat3(view));

	Material::Draw(model, newView, projection);
	glDepthFunc(GL_LEQUAL);

	SetMat4("V", newView);
	SetMat4("P", projection);
}

void CubeMap::DrawUI()
{

}
