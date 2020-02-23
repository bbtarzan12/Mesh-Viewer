#pragma once
#include <vector>
#include <string>

namespace ImGui
{
	bool Combo(const char* label, int* currIndex, const std::vector<std::string>& values);

	bool ListBox(const char* label, int* currIndex, const std::vector<std::string>& values);

}