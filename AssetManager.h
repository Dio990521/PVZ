#pragma once
#include "State.h"
#include "Asset.h"
class AssetManager
{
private:
	std::unordered_map<string, Asset*> assets;

public:
	
	template<typename T> T* Load(const string& id, const string& path);
	void Release(const string& id);
	template<typename T> T* Get(const string& id);
};

