#include "Light.h"
#include "Material.h"
#include <string>

Light::Light(const glm::vec3& position, const glm::vec3& color, const float power)
	:position(position), color(color), power(power)
{

}

void Light::Draw(const std::shared_ptr<Material> material, int index /*= 0*/) const
{
	std::string indexString = std::to_string(index);
	material->SetVec3("lights[" + indexString + "].position", position);
	material->SetVec3("lights[" + indexString + "].color", color);
	material->SetFloat("lights[" + indexString + "].power", power);
}
