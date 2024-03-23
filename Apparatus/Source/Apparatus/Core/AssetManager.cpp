#include "AssetManager.h"

#include "ModelImporter.h"

void AssetManager::init()
{
	importers.push_back(std::make_unique<ModelImporter>());
	importers.back().get()->assetManager = this;
}

void AssetManager::initAssets()
{
	for (auto iter = assetMap.begin(); iter != assetMap.end(); ++iter)
	{
		if (iter->second)
		{
			iter->second->init();
		}
	}
}
