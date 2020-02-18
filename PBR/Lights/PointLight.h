#pragma once

#include "../Core/Light.h"

class PointLight : public Light
{

public:
	PointLight(const glm::vec3& position, const glm::vec3& color, const float power);
	virtual ~PointLight() = default;

	virtual void Draw(const std::shared_ptr<Material> material, int index = 0) const override;

};