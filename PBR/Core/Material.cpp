#include "Material.h"
#include "Texture.h"
#include <imgui.h>
#include <ImGuiFileDialog.h>

void Material::Draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const
{
	glUseProgram(shader);
}

void Material::SetVec3(const std::string& name, const glm::vec3& value)
{
	glUseProgram(shader);
	GLuint id = glGetUniformLocation(shader, name.c_str());

	if (id == -1)
		return;

	glUniform3fv(id, 1, &value[0]);
	glUseProgram(0);
}

void Material::SetTexture(const std::string& name, const std::shared_ptr<Texture>& texture)
{
	if (texture == nullptr)
		return;

	textures[name] = texture;
}


void Material::SetFloat(const std::string& name, const float power)
{
	glUseProgram(shader);
	GLuint id = glGetUniformLocation(shader, name.c_str());

	if (id == -1)
		return;

	glUniform1f(id, power);
	glUseProgram(0);
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
		return -1;

	return textures.at(name)->GetID();
}

void Material::DrawTexturePannel(const std::string& name)
{
	GLuint id = GetTextureID(name);
	if (id != -1)
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
	}
	else
	{
		ImGui::BeginChild(name.c_str(), { 150, 35 }, true);
		ImGui::Text(name.c_str());

		ImGui::SameLine();
		if (ImGui::Button("Load"))
		{
			ImGuiFileDialog::Instance()->OpenDialog(name, ("Load " + name + " Texture").c_str(), ".png\0", ".");
		}
		ImGui::EndChild();

		if (ImGuiFileDialog::Instance()->FileDialog(name))
		{
			if (ImGuiFileDialog::Instance()->IsOk)
			{
				std::string filePathName = ImGuiFileDialog::Instance()->GetFilepathName();

				SetTexture(name, Texture::CreateTexture(filePathName));
			}
			ImGuiFileDialog::Instance()->CloseDialog(name);
		}
	}
}

