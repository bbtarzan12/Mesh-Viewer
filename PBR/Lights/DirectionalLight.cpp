#include "DirectionalLight.h"
#include "../Core/Material.h"

DirectionalLight::DirectionalLight(const glm::vec3& direction, const glm::vec3& color, const float power)
	:direction(direction), color(color), power(power)
{

}

void DirectionalLight::Draw(const std::shared_ptr<Material> material, int index /*= 0*/) const
{
	material->SetVec3("directionalLight.direction", direction);
	material->SetVec3("directionalLight.color", color);
	material->SetFloat("directionalLight.power", power);
}
