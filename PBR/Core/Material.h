#pragma once

#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <memory>
#include <map>
#include <vector>

class Texture;

class Material
{
public:
	Material(const std::string& vertShader, const std::string& fragShader, const std::vector<std::string>& textureNames);
	virtual ~Material() = default;

	virtual void Draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const;
	virtual void DrawUI() = 0;

	void SetVec2(const std::string& name, const glm::vec2& value) const;
	void SetVec3(const std::string& name, const glm::vec3& value) const;
	void SetVec4(const std::string& name, const glm::vec4& value) const;
	void SetFloat(const std::string& name, const float value) const;
	void SetMat4(const std::string& name, const glm::mat4& value) const;
	void SetBool(const std::string& name, const bool value) const;
	void SetTexture(const std::string& name, const std::shared_ptr<Texture>& texture);

protected:
	glm::ivec2 GetTextureSize(const std::string& name) const;
	GLuint GetTextureID(const std::string& name) const;

	void InitializeTextures();
	void AttachTextures() const;


	// UI
	bool DrawTexturePannel(const std::string& name, const glm::vec2& size);

private:
	Material() = default;
	bool CheckCurrentShader() const;

protected:
	GLuint shader;

	std::map<std::string, std::shared_ptr<Texture>> textures;
	std::vector<std::string> textureNames;
	std::vector<glm::vec4> defaultColors;
};