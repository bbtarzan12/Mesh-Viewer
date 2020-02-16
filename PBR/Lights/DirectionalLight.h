#pragma once

#include "../Core/Light.h"
#include <glm/glm.hpp>

class DirectionalLight : public Light
{
public:
	DirectionalLight(const glm::vec3& direction, const glm::vec3& color, const float power);
	virtual ~DirectionalLight() = default;

	virtual void Draw(const std::shared_ptr<Material> material, int index = 0) const override;

private:
	glm::vec3 direction;
	glm::vec3 color;
	float power;
};