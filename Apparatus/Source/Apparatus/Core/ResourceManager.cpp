#include "ResourceManager.h"

#include "ModelImporter.h"

void ResourceManager::init()
{
	importers.push_back(std::make_unique<ModelImporter>());
	importers.back().get()->resourceManager = this;
}

void ResourceManager::initResources()
{
	for (auto iter = resourceMap.begin(); iter != resourceMap.end(); ++iter)
	{
		if (iter->second)
		{
			iter->second->init();
		}
	}
}
