#pragma once
#include "../Core/Material.h"

class PreFilterMapCapture : public Material
{
public:
	PreFilterMapCapture();

	virtual void Draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const override;

	virtual void SetCaptureTexture(const std::shared_ptr<Texture>& texture) override;
	virtual void Capture(glm::ivec2& size, const int mip, const int maxMip) const override;
};
