#pragma once
#include "../Core/Material.h"

class Phong : public Material
{
public:
	Phong();

	virtual void Draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const override;

	void SetDiffuseTexture(const GLuint texture);
	void SetNormalTexture(const GLuint texture);
	void SetSpecularTexture(const GLuint texture);

private:
	GLuint modelMatrix, viewMatrix, projectionMatrix;
	GLuint diffuseTexture, normalTexture, specularTexture;
	GLuint diffuseID, normalID, specularID;

};