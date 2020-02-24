#pragma once
#include "../Core/Material.h"
#include <vector>
#include <string>

class PBR : public Material
{
	enum class NDF
	{
		BlinnPhong,
		Beckmann,
		TrowbridgeReitz
	};

	enum class F
	{
		SchlickApproximation
	};

	enum class G
	{
		SchlickGGX,
		GGX
	};

	enum class D
	{
		Lambertian,
		OrenNayer
	};

public:
	PBR();

	virtual void Draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const override;
	virtual void DrawUI() override;

private:
	glm::vec2 uvScale;
	glm::vec2 uvOffset;
	NDF ndf;
	F f;
	G g;
	D d;

	static const std::vector<std::string> ndfStrings;
	static const std::vector<std::string> fStrings;
	static const std::vector<std::string> gStrings;
	static const std::vector<std::string> dStrings;

};