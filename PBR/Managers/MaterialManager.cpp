#include "MaterialManager.h"
#include <cassert>

std::shared_ptr<Material> MaterialManager::GetMaterial(const std::string& name)
{
	assert(materials.find(name) != materials.end());
	return materials[name];
}

int MaterialManager::NumMaterials() const
{
	return materials.size();
}

const std::map<std::string, std::shared_ptr<Material>>& MaterialManager::GetMaterials() const
{
	return materials;
}