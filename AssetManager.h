#pragma once
#include "State.h"
#include "Asset.h"
#include "AudioAsset.h"
#include "ImageAsset.h"
class AssetManager
{
private:
	std::unordered_map<string, Asset*> assets;

public:
	
	template<typename T> T* Load(const string& id, const string& path)
	{
		if (assets.find(id) != assets.end()) return nullptr;

		T* asset = new T();
		if (asset->Load(path))
		{
			assets[id] = asset;
			return asset;
		}

		delete asset;
		return nullptr;
	}

	void Release(const string& id);

	template<typename T> T* Get(const string& id)
	{
		{
			auto it = assets.find(id);
			if (it != assets.end())
			{
				return dynamic_cast<T*>(it->second);
			}

			std::cerr << "Asset with ID " << id << " not found." << std::endl;
			return nullptr;
		}
	}
};

