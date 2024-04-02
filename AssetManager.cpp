#include "AssetManager.h"

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


