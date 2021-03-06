#include "imgui_custom.h"
#include <imgui/imgui.h>

static auto vector_getter = [](void* vec, int idx, const char** out_text)
{
	auto& vector = *static_cast<std::vector<std::string>*>(vec);
	if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
	*out_text = vector.at(idx).c_str();
	return true;
};

bool ImGui::Combo(const char* label, int* currIndex, const std::vector<std::string>& values)
{
	if (values.empty()) { return false; }
	std::vector<std::string> localCopies(values);
	return Combo(label, currIndex, vector_getter,
		static_cast<void*>(&localCopies), values.size());
}

bool ImGui::ListBox(const char* label, int* currIndex, const std::vector<std::string>& values)
{
	if (values.empty()) { return false; }
	std::vector<std::string> localCopies(values);
	return ListBox(label, currIndex, vector_getter,
		static_cast<void*>(&localCopies), values.size());
}
