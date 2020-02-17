#include "Material.h"
#include "Texture.h"
#include "../Util/ShaderLoader.h"
#include <imgui.h>
#include <ImGuiFileDialog.h>

Material::Material(const std::string& vertShader, const std::string& fragShader, const std::vector<std::string>& textureNames)
	:textureNames(textureNames)
{
	shader = ShaderLoader::Load(vertShader, fragShader);
	InitializeTextures();
}

void Material::Draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const
{
	glUseProgram(shader);
	AttachTextures();
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

void Material::SetTexture(const std::string& name, const std::shared_ptr<Texture>& texture)
{
	if (texture == nullptr)
		return;

	textures[name] = texture;
}

glm::ivec2 Material::GetTextureSize(const std::string& name) const
{
	if (textures.find(name) == textures.end())
		return {0, 0};

	return textures.at(name)->GetSize();
}

GLuint Material::GetTextureID(const std::string& name) const
{
	if (textures.find(name) == textures.end())
		return 0;

	return textures.at(name)->GetID();
}
#include <iostream>
void Material::InitializeTextures()
{
	glUseProgram(shader);

	for (int i = 0; i < textureNames.size(); i++)
	{
		GLuint textureID = glGetUniformLocation(shader, (textureNames[i] + ".texture").c_str());
		glUniform1i(textureID, i);
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
			glBindTexture(textures.at(textureNames[i])->GetType(), textureID);
			SetBool(textureNames[i] + ".use", true);
		}

		SetVec4(textureNames[i] + ".defaultColor", defaultColors[i]);
	}
}

bool Material::DrawTexturePannel(const std::string& name, const glm::vec2& size)
{
	GLuint id = GetTextureID(name);
	if (id != 0)
	{
		ImGui::BeginChild(name.c_str(), { 150, 170 }, true);
		ImGui::Text(name.c_str());
		ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(id)), { 128, 128 });

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
			ImGuiFileDialog::Instance()->OpenDialog(name, ("Load " + name + " Texture").c_str(), ".png\0", ".");
		}

		if (ImGuiFileDialog::Instance()->FileDialog(name))
		{
			if (ImGuiFileDialog::Instance()->IsOk)
			{
				std::string filePathName = ImGuiFileDialog::Instance()->GetFilepathName();

				SetTexture(name, std::make_shared<Texture>(filePathName));
			}
			ImGuiFileDialog::Instance()->CloseDialog(name);
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

