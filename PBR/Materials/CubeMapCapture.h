#pragma once
#include "../Core/Material.h"

class CubeMapCapture : public Material
{
public:
	CubeMapCapture();

	virtual void Draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const override;
};
