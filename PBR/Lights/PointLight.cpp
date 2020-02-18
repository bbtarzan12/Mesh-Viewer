#include "PointLight.h"
#include "../Core/Material.h"


PointLight::PointLight(const glm::vec3& position, const glm::vec3& color, const float power)
	:Light(position, color, power)
{

}

void PointLight::Draw(const std::shared_ptr<Material> material, int index) const
{
	Light::Draw(material, index);

	std::string indexString = std::to_string(index);
	material->SetFloat("lights[" + indexString + "].isDirectional", false);
}