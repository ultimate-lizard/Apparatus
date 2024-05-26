#include "AssetManager.h"

#include "Importer/ModelImporter.h"
#include "Importer/TextureImporter.h"
#include "Importer/FontImporter.h"

void AssetManager::init()
{
	importers.push_back(std::make_unique<ModelImporter>());
	importers.back().get()->assetManager = this;
	importers.push_back(std::make_unique<TextureImporter>());
	importers.back().get()->assetManager = this;
	importers.push_back(std::make_unique<FontImporter>());
	importers.back().get()->assetManager = this;

	for (auto& importer : importers)
	{
		if (importer)
		{
			importer->init();
		}
	}
}

void AssetManager::uninit()
{
	for (auto& pair : assetMap)
	{
		if (std::unique_ptr<Asset>& assetPtr = pair.second)
		{
			assetPtr->uninit();
		}
	}

	for (auto& importer : importers)
	{
		if (importer)
		{
			importer->uninit();
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
