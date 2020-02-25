#pragma once

#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <memory>
#include <map>
#include <vector>

class Texture;
namespace ImGui { class FileBrowser; }

class Material
{
public:
	Material(const std::string& vertShader, const std::string& fragShader, const std::vector<std::string>& textureNames, bool bVisible = true);
	virtual ~Material() = default;

	virtual void Use() const;
	virtual void Draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const;
	virtual void DrawUI();

	virtual void SetCaptureTexture(const std::shared_ptr<Texture>& texture);
	virtual void Capture(glm::ivec2& size, const int mip, const int maxMip) const;

	void SetShader(const std::string& vertShader, const std::string& fragShader);
	void SetInt(const std::string& name, const int value) const;
	void SetVec2(const std::string& name, const glm::vec2& value) const;
	void SetVec3(const std::string& name, const glm::vec3& value) const;
	void SetVec4(const std::string& name, const glm::vec4& value) const;
	void SetFloat(const std::string& name, const float value) const;
	void SetMat4(const std::string& name, const glm::mat4& value) const;
	void SetBool(const std::string& name, const bool value) const;

	void SetDefaultColor(const std::string& name, const glm::vec4& value);
	void SetTexture(const std::string& name, const std::shared_ptr<Texture>& texture);

	bool Visible() const;

	glm::ivec2 GetTextureSize(const std::string& name) const;
	GLuint GetTextureID(const std::string& name) const;

protected:

	void InitializeTextures();
	void AttachTextures() const;


	// UI
	bool DrawTexturePannel(const std::string& name, const glm::vec2& size);

private:
	Material() = default;
	bool CheckCurrentShader() const;

protected:
	GLuint shader = 0;

	std::map<std::string, std::shared_ptr<Texture>> textures;
	std::vector<std::string> textureNames;
	std::vector<glm::vec4> defaultColors;

private:

	//UI
	std::shared_ptr<ImGui::FileBrowser> fileDialog;
	bool bVisible;
};