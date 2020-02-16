#pragma once
#include "../Core/Material.h"

class CubeMap : public Material
{
public:
	CubeMap();

	virtual void Draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const override;
	virtual void DrawUI() override;
};
