#include "Material.h"
#include "Texture.h"
#include "../Util/ShaderLoader.h"
#include <imgui.h>
#include <imfilebrowser.h>
#include "../Managers/TextureManager.h"

Material::Material(const std::string& vertShader, const std::string& fragShader, const std::vector<std::string>& textureNames, bool bVisible)
	:textureNames(textureNames), bVisible(bVisible)
{
	SetShader(vertShader, fragShader);
	InitializeTextures();
}

void Material::Use() const
{
	assert(shader != 0);

	glUseProgram(shader);
	AttachTextures();
}

void Material::Draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const
{

}

void Material::DrawUI()
{

}

void Material::SetCaptureTexture(const std::shared_ptr<Texture>& texture)
{

}

void Material::SetShader(const std::string& vertShader, const std::string& fragShader)
{
	shader = ShaderLoader::Load(vertShader, fragShader);
}

void Material::SetInt(const std::string& name, const int value) const
{
	glUseProgram(shader);
	GLuint id = glGetUniformLocation(shader, name.c_str());

	if (id == -1)
		return;

	glUniform1i(id, value);

	if (!CheckCurrentShader())
		glUseProgram(0);
}

void Material::SetVec2(const std::string& name, const glm::vec2& value) const
{
	glUseProgram(shader);
	GLuint id = glGetUniformLocation(shader, name.c_str());

	if (id == -1)
		return;

	glUniform2fv(id, 1, &value[0]);

	if (!CheckCurrentShader())
		glUseProgram(0);
}

void Material::SetVec3(const std::string& name, const glm::vec3& value) const
{
	glUseProgram(shader);
	GLuint id = glGetUniformLocation(shader, name.c_str());

	if (id == -1)
		return;

	glUniform3fv(id, 1, &value[0]);

	if (!CheckCurrentShader())
		glUseProgram(0);
}

void Material::SetVec4(const std::string& name, const glm::vec4& value) const
{
	glUseProgram(shader);
	GLuint id = glGetUniformLocation(shader, name.c_str());

	if (id == -1)
		return;

	glUniform4fv(id, 1, &value[0]);

	if (!CheckCurrentShader())
		glUseProgram(0);
}

void Material::SetFloat(const std::string& name, const float value) const
{
	glUseProgram(shader);
	GLuint id = glGetUniformLocation(shader, name.c_str());

	if (id == -1)
		return;

	glUniform1f(id, value);

	if(!CheckCurrentShader())
		glUseProgram(0);
}

void Material::SetMat4(const std::string& name, const glm::mat4& value) const
{
	glUseProgram(shader);
	GLuint id = glGetUniformLocation(shader, name.c_str());

	if (id == -1)
		return;

	glUniformMatrix4fv(id, 1, GL_FALSE, &value[0][0]);

	if (!CheckCurrentShader())
		glUseProgram(0);
}

void Material::SetBool(const std::string& name, const bool value) const
{
	glUseProgram(shader);
	GLuint id = glGetUniformLocation(shader, name.c_str());

	if (id == -1)
		return;

	glUniform1i(id, value);

	if (!CheckCurrentShader())
		glUseProgram(0);
}

void Material::SetDefaultColor(const std::string& name, const glm::vec4& value)
{
	const auto& iter = std::find(textureNames.begin(), textureNames.end(), name);
	if (iter == textureNames.end())
		return;

	int index = std::distance(textureNames.begin(), iter);

	defaultColors[index] = value;
}

void Material::SetTexture(const std::string& name, const std::shared_ptr<Texture>& texture)
{
	textures[name] = texture;
}

bool Material::Visible() const
{
	return bVisible;
}

GLuint Material::GetTextureID(const std::string& name) const
{
	if (textures.find(name) == textures.end())
		return 0;

	std::shared_ptr<Texture> texture = textures.at(name).lock();

	if (texture)
	{
		return texture->GetID();
	}
	else
	{
		textures.erase(name);
		return 0;
	}
}

GLenum Material::GetTarget(const std::string& name) const
{
	if (textures.find(name) == textures.end())
		return 0;

	std::shared_ptr<Texture> texture = textures.at(name).lock();

	if (texture)
	{
		return texture->GetTarget();
	}
	else
	{
		textures.erase(name);
		return 0;
	}
}

void Material::InitializeTextures()
{
	glUseProgram(shader);

	for (int i = 0; i < textureNames.size(); i++)
	{
		GLuint uniformID = glGetUniformLocation(shader, (textureNames[i] + ".texture").c_str());
		glUniform1i(uniformID, i);
		defaultColors.emplace_back(1.0f, 1.0f, 1.0f, 1.0f);
	}

	glUseProgram(0);
}

void Material::AttachTextures() const
{
	for (int i = 0; i < textureNames.size(); i++)
	{
		GLuint textureID = GetTextureID(textureNames[i]);
		glActiveTexture(GL_TEXTURE0 + i);

		if (textureID == 0)
		{
			glBindTexture(GL_TEXTURE_2D, textureID);
			SetBool(textureNames[i] + ".use", false);
		}
		else
		{
			glBindTexture(GetTarget(textureNames[i]), textureID);
			SetBool(textureNames[i] + ".use", true);
		}

		SetVec4(textureNames[i] + ".defaultColor", defaultColors[i]);
	}
}

void Material::Capture(glm::ivec2& size, const int mip, const int maxMip) const
{
	
}

bool Material::DrawTexturePannel(const std::string& name, const glm::vec2& size)
{
	GLuint id = GetTextureID(name);
	if (id != 0)
	{
		ImGui::BeginChild(name.c_str(), { 150, 170 }, true);
		ImGui::Text(name.c_str());
		ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(id)), { 128, 128 }, ImVec2(0, 1), ImVec2(1, 0));

		ImVec2 pos = ImGui::GetCursorPos();
		pos.x += ImGui::GetWindowWidth() - 60;
		pos.y -= 15;
		ImGui::SetCursorPos(pos);
		if (ImGui::Button("Clear"))
		{
			textures.erase(name);
		}
		ImGui::EndChild();
		return true;
	}
	else
	{
		ImGui::BeginChild(name.c_str(), { size.x, size.y }, true);
		ImGui::Text(name.c_str());

		ImGui::SameLine();
		if (ImGui::Button("Load"))
		{
			TextureManager::Instance().OpenTexturesWindowForLoad
			(
				[=](const std::string& selectedTextureName)
			{
				SetTexture(name, TextureManager::Instance().GetTexture<Texture>(selectedTextureName));
			}
			);
		}
		return false;
	}
}

bool Material::CheckCurrentShader() const
{
	GLint id;
	glGetIntegerv(GL_CURRENT_PROGRAM, &id);
	return shader == id;
}

