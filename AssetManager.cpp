#include "AssetManager.h"
//template<typename T> 
//T* AssetManager::Load(const string& id, const string& path)


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

//template<typename T> 
//T* AssetManager::Get(const string& id)
