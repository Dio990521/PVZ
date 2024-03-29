#include "AssetManager.h"
template<typename T> 
T* AssetManager::Load(const string& id, const string& path)
{
	if (assets.find(id) != assets.end())
	{
		std::cerr << "Asset with ID " << id << " already exists." << std::endl;
		return nullptr;
	}

	T* asset = new T();
	if (asset->Load(path))
	{
		assets[id] = asset;
		return asset;
	}

	delete asset;
	return nullptr;
}

void AssetManager::Release(const string& id)
{
	auto it = assets.find(id);
	if (it != assets.end())
	{
		it->second->Release();
		delete it->second;
		assets.erase(id);
	}
	else
	{
		std::cerr << "Asset with ID " << id << " not found." << std::endl;
	}
}

template<typename T> 
T* AssetManager::Get(const string& id)
{
	auto it = assets.find(id);
	if (it != assets.end())
	{
		return dynamic_cast<T*>(it->second);
	}

	std::cerr << "Asset with ID " << id << " not found." << std::endl;
	return nullptr;
}