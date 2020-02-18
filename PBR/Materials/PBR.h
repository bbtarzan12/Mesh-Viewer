#pragma once
#include "../Core/Material.h"

class PBR : public Material
{
public:
	PBR();

	virtual void Draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const override;
	virtual void DrawUI() override;

private:
	glm::vec2 uvScale;
	glm::vec2 uvOffset;

};