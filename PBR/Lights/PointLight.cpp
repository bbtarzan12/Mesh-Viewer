#include "PointLight.h"
#include "../Core/Material.h"


PointLight::PointLight(const glm::vec3& position, const glm::vec3& color, const float power)
	:position(position), color(color), power(power)
{

}

void PointLight::Draw(const std::shared_ptr<Material> material, int index) const
{
	std::string indexString = std::to_string(index);
	material->SetVec3("pointLights[" + indexString + "].position", position);
	material->SetVec3("pointLights[" + indexString + "].color", color);
	material->SetFloat("pointLights[" + indexString + "].power", power);
}