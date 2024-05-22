#pragma once

#include <map>
#include <memory>
#include <vector>

#include "Asset.h"
#include "Logger.h"
#include "Importer.h"
#include "../Rendering/Shader.h"

class AssetManager
{
public:
	AssetManager();
	~AssetManager() = default;

	AssetManager(const AssetManager&) = delete;
	AssetManager(AssetManager&&) = delete;
	void operator=(const AssetManager&) = delete;

	void initAssets();

	template <class ResourceType, typename ... Args>
	ResourceType* createAsset(const std::string& assetName, Args&& ... args);

	template <class ResourceType, typename ... Args>
	ResourceType* createAsset(const std::string& assetName, std::unique_ptr<ResourceType> asset);

	void removeAsset(const std::string& name);

	template <class ImporterType>
	ImporterType* getImporter();

	template <class ResourceType>
	ResourceType* findAsset(const std::string& name);

private:
	std::map<std::string, std::unique_ptr<Asset>> assetMap;
	std::vector<std::unique_ptr<Importer>> importers;
};

template <class ResourceType, typename ... Args>
inline ResourceType* AssetManager::createAsset(const std::string& assetName, Args&& ... args)
{
	if (std::unique_ptr<Asset> newResource = std::make_unique<ResourceType>(std::forward<Args>(args)...))
	{
		newResource->assetName = assetName;
		auto searchIter = assetMap.find(newResource->getAssetName());
		if (searchIter != assetMap.end())
		{
			LOG("Detected duplicate asset '" + newResource->getAssetName() + "'. Skipping...", LogLevel::Info);
			return findAsset<ResourceType>(newResource->getAssetName());
		}

		auto iter = assetMap.emplace(newResource->getAssetName(), std::move(newResource));
		if (ResourceType* createdResource = static_cast<ResourceType*>(iter.first->second.get()))
		{
			createdResource->init();
			LOG("Initialized asset '" + createdResource->getAssetName() + '\'', LogLevel::Info);
			return createdResource;
		}
	}

	return nullptr;
}

template<class ResourceType, typename ...Args>
inline ResourceType* AssetManager::createAsset(const std::string& assetName, std::unique_ptr<ResourceType> asset)
{
	if (asset)
	{
		std::string nameCached = assetName;
		asset->setAssetName(nameCached);

		auto searchIter = assetMap.find(asset->getAssetName());
		if (searchIter != assetMap.end())
		{
			LOG("Tried to register an asset that already exist! No changes were made", LogLevel::Error);
			return nullptr;
		}

		ResourceType* resourcePtr = asset.get();
		auto iter = assetMap.emplace(asset->getAssetName(), std::move(asset));
		if (iter.second)
		{
			LOG("Initialized asset '" + resourcePtr->getAssetName() + '\'', LogLevel::Info);

			resourcePtr->init();
			return resourcePtr;
		}
	}

	return nullptr;
}

template<class ImporterType>
inline ImporterType* AssetManager::getImporter()
{
	for (std::unique_ptr<Importer>& importer : importers)
	{
		if (ImporterType* castedImporter = dynamic_cast<ImporterType*>(importer.get()))
		{
			return castedImporter;
		}
	}

	return nullptr;
}

template<class ResourceType>
inline ResourceType* AssetManager::findAsset(const std::string& name)
{
	auto iter = assetMap.find(name);
	if (iter != assetMap.end())
	{
		return dynamic_cast<ResourceType*>(iter->second.get());
	}

	LOG("Unable to find requested asset '" + name + "'", LogLevel::Error);
	return nullptr;
}
