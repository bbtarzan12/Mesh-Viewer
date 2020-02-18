#pragma once

#include <GL/glew.h>
#include <memory>
#include <glm/glm.hpp>

class Material;

class Light
{
public:
	Light(const glm::vec3& position, const glm::vec3& color, const float power);
	virtual ~Light() = default;

	virtual void Draw(const std::shared_ptr<Material> material, int index = 0) const;

protected:
	glm::vec3 position;
	glm::vec3 color;
	float power;
};