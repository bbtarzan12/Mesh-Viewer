#pragma once

#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <memory>
#include <map>

class Texture;

class Material
{
public:
	Material() = default;
	virtual ~Material() = default;

	virtual void Draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const;
	virtual void DrawUI() = 0;

	void SetVec3(const std::string& name, const glm::vec3& value);
	void SetTexture(const std::string& name, const std::shared_ptr<Texture>& texture);
	void SetFloat(const std::string& name, const float power);

protected:
	glm::ivec2 GetTextureSize(const std::string& name) const;
	GLuint GetTextureID(const std::string& name) const;

	void DrawTexturePannel(const std::string& name);

protected:
	GLuint shader;

	std::map<std::string, std::shared_ptr<Texture>> textures;
};