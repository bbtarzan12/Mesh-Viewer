#pragma once

#include <map>
#include <string>
#include <memory>

class MaterialManager : public SingletonManager<MaterialManager>
{
	friend class SingletonManager<MaterialManager>;
	friend class Material;

public:
	template<typename T>
	std::shared_ptr<T> CreateMaterial(const std::string& name);

	std::shared_ptr<Material> GetMaterial(const std::string& name);

private:
	MaterialManager() = default;
	virtual ~MaterialManager() = default;

private:
	std::map<std::string, std::shared_ptr<Material>> materials;

};

template<typename T>
std::shared_ptr<T> MaterialManager::CreateMaterial(const std::string& name)
{
	std::shared_ptr<T> material = std::make_shared<T>();
	materials.insert(std::make_pair(name, material));

	return material;
}

std::shared_ptr<Material> MaterialManager::GetMaterial(const std::string& name)
{
	assert(materials.find(name) != materials.end());
	return materials[name];
}
