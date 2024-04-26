#include "AssetManager.h"

#include "ModelImporter.h"
#include "TextureImporter.h"

AssetManager::AssetManager()
{
	importers.push_back(std::make_unique<ModelImporter>());
	importers.back().get()->assetManager = this;
	importers.push_back(std::make_unique<TextureImporter>());
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

void AssetManager::removeAsset(const std::string& name)
{
	auto searchIter = assetMap.find(name);
	if (searchIter != assetMap.end())
	{
		assetMap.erase(searchIter);
		LOG("Successfully removed '" + name + "'", LogLevel::Info);
	}
}
