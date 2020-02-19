#pragma once

#include <map>
#include <string>
#include <memory>
#include <GL/glew.h>
#include <cassert>
#include "../Core/SingletonManager.h"

class TextureManager : public SingletonManager<TextureManager>
{
	friend class SingletonManager<TextureManager>;
	friend class Texture;

public:
	template<typename T, typename... Types>
	std::shared_ptr<T> LoadTexture(const std::string& name, Types&&... args);

	template<typename T, typename... Types>
	std::shared_ptr<T> CreateTexture(const std::string& name, const GLenum& textureType, const GLenum& dataFormat, Types&&... args);

	template<typename T>
	std::shared_ptr<T> GetTexture(const std::string& name);

private:
	TextureManager() = default;
	virtual ~TextureManager() = default;

private:
	std::map<std::string, std::shared_ptr<Texture>> textures;
};

template<typename T, typename... Types>
std::shared_ptr<T> TextureManager::LoadTexture(const std::string& name, Types&&... args)
{
	std::shared_ptr<T> texture = std::make_shared<T>(std::forward<Types>(args)...);
	textures.insert(std::make_pair(name, texture));

	return texture;
}

template<typename T, typename... Types>
std::shared_ptr<T> TextureManager::CreateTexture(const std::string& name, const GLenum& textureType, const GLenum& dataFormat, Types&&... args)
{
	std::shared_ptr<T> texture = std::make_shared<T>(textureType, dataFormat, std::forward<Types>(args)...);
	textures.insert(std::make_pair(name, texture));

	return texture;
}

template<typename T>
std::shared_ptr<T> TextureManager::GetTexture(const std::string& name)
{
	assert(textures.find(name) != textures.end());
	return std::dynamic_pointer_cast<T>(textures[name]);
}