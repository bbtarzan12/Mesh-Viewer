#include "CubeMap.h"
#include "../Util/ShaderLoader.h"
#include <imgui.h>

CubeMap::CubeMap()
{
	shader = ShaderLoader::Load("Shaders/cubeMap_vert.glsl", "Shaders/cubeMap_frag.glsl");
}

void CubeMap::Draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const
{
	glm::mat4 newView = glm::mat4(glm::mat3(view));

	Material::Draw(model, newView, projection);
	glDepthFunc(GL_LEQUAL);

	glBindTexture(GL_TEXTURE_CUBE_MAP, GetTextureID("cubeMapTexture"));

	SetMat4("V", newView);
	SetMat4("P", projection);
}

void CubeMap::DrawUI()
{

}