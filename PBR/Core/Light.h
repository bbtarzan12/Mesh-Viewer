#pragma once

#include <GL/glew.h>
#include <memory>

class Material;

class Light
{
public:
	Light() = default;
	virtual ~Light() = default;

	virtual void Draw(const std::shared_ptr<Material> material, int index = 0) const = 0;
};