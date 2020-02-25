#include "TextureManager.h"
#include <imgui.h>
#include "../Core/Texture.h"

void TextureManager::ToggleTexturesWindow()
{
	bEnableTexturesWindow = !bEnableTexturesWindow;
}

void TextureManager::RenderTexturesWindow(double deltaTime)
{
	if (!bEnableTexturesWindow)
		return;

	if (ImGui::Begin("Textures", &bEnableTexturesWindow, ImGuiWindowFlags_AlwaysAutoResize))
	{
		if (textures.empty())
		{
			ImGui::TextDisabled("textures.empty()");
			ImGui::End();
			return;
		}

		int index = 0;
		std::string selectedName = "";
		for (auto& pair : textures)
		{
			if (index % 4 != 0)
			{
				ImGui::SameLine();
			}

			ImGui::BeginGroup();
			const auto& name = pair.first;
			const auto& texture = pair.second;
			ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(texture->GetID())), { 128, 128 }, ImVec2(0, 1), ImVec2(1, 0));

			ImVec2 pos = ImGui::GetCursorPos();
			pos.x += 85;
			pos.y -= 25;
			ImGui::SetCursorPos(pos);
			if (bTextureSelectMode)
			{
				if (ImGui::Button(("Select##" + name).c_str()))
				{
					selectedName = name;
				}
			}
			else
			{
				if (ImGui::Button(("Unload##" + name).c_str()))
				{
					selectedName = name;
				}
			}
			ImGui::EndGroup();

			index++;
		}

		if (!selectedName.empty())
		{
			if (bTextureSelectMode)
			{
				if (callback)
				{
					callback(selectedName);
					bTextureSelectMode = false;
					bEnableTexturesWindow = false;
				}
			}
			else
			{
				textures.erase(selectedName);
			}
		}


		ImGui::End();
	}
	else
	{
		bTextureSelectMode = false;
	}

}

void TextureManager::OpenTexturesWindowForLoad(const std::function<void(const std::string&)>& func)
{
	callback = func;
	bEnableTexturesWindow = true;
	bTextureSelectMode = true;
}
