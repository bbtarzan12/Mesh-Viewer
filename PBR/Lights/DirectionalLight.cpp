#include "DirectionalLight.h"
#include "../Core/Material.h"

DirectionalLight::DirectionalLight(const glm::vec3& direction, const glm::vec3& color, const float power)
	:Light(direction, color, power)
{

}

void DirectionalLight::Draw(const std::shared_ptr<Material> material, int index /*= 0*/) const
{
	Light::Draw(material, index);

	std::string indexString = std::to_string(index);
	material->SetFloat("lights[" + indexString + "].isDirectional", true);
}
