#pragma once

#include <map>
#include <memory>
#include <vector>

#include "Resource.h"
#include "../Rendering/Shader.h"
#include "Logger.h"
#include "Importer.h"

class ResourceManager
{
public:
	ResourceManager() = default;
	~ResourceManager() = default;

	ResourceManager(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) = delete;
	void operator=(const ResourceManager&) = delete;

	void init();
	void initResources();

	template <class ResourceType, typename ... Args>
	ResourceType* createResource(Args&& ... args);

	template <class ResourceType, typename ... Args>
	ResourceType* createResource(std::unique_ptr<ResourceType> resource);

	template <class ImporterType>
	ImporterType* getImporter();

	template <class ResourceType>
	ResourceType* findResource(const std::string& name);

private:
	std::map<std::string, std::unique_ptr<Resource>> resourceMap;
	std::vector<std::unique_ptr<Importer>> importers;
};

template <class ResourceType, typename ... Args>
inline ResourceType* ResourceManager::createResource(Args&& ... args)
{
	if (std::unique_ptr<Resource> newResource = std::make_unique<ResourceType>(std::forward<Args>(args)...))
	{
		newResource->resourceManager = this;

		auto searchIter = resourceMap.find(newResource->getResourceName());
		if (searchIter != resourceMap.end())
		{
			LOG("Tried to create a resource '" + newResource->getResourceName() + "' but the name is already occupied. Skipping...", LogLevel::Error);
			return nullptr;
		}

		auto iter = resourceMap.emplace(newResource->getResourceName(), std::move(newResource));
		if (ResourceType* createdResource = static_cast<ResourceType*>(iter.first->second.get()))
		{
			LOG("Initializing resource " + createdResource->getResourceName(), LogLevel::Info);

			createdResource->init();
			return createdResource;
		}
	}

	return nullptr;
}

template<class ResourceType, typename ...Args>
inline ResourceType* ResourceManager::createResource(std::unique_ptr<ResourceType> resource)
{
	if (resource)
	{
		ResourceType* resourcePtr = resource.get();
		resource->resourceManager = this;
		auto iter = resourceMap.emplace(resource->getResourceName(), std::move(resource));
		if (iter.second)
		{
			LOG("Initializing resource " + resourcePtr->getResourceName(), LogLevel::Info);

			resourcePtr->init();
			return resourcePtr;
		}
	}

	return nullptr;
}

template<class ImporterType>
inline ImporterType* ResourceManager::getImporter()
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
inline ResourceType* ResourceManager::findResource(const std::string& name)
{
	auto iter = resourceMap.find(name);
	if (iter != resourceMap.end())
	{
		return dynamic_cast<ResourceType*>(iter->second.get());
	}

	LOG("Unable to find requested resource '" + name + "'", LogLevel::Error);
	return nullptr;
}
