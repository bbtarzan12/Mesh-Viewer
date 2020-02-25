#pragma once
#include "../Core/Material.h"

class IrradianceMapCapture : public Material
{
public:
	IrradianceMapCapture();

	virtual void Draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const override;

	virtual void SetCaptureTexture(const std::shared_ptr<Texture>& texture) override;
};
