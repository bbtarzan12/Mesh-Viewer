#pragma once

#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>

class Material
{
public:
	Material() = default;
	virtual ~Material() = default;

	virtual void Draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const;

	void SetVec3(const std::string& name, const glm::vec3& value);

protected:
	GLuint shader;
};                                                              