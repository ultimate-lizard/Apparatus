#include "EntityRegistry.h"

#include "../Rendering/SceneNode.h"

EntityRegistry::EntityRegistry()
{

}

Entity* EntityRegistry::createEntityTemplate(const std::string& templateName)
{
	auto insertionIter = entityTemplates.emplace(templateName, std::make_unique<Entity>(templateName));

	const bool emplacedSuccessfully = insertionIter.second;
	if (emplacedSuccessfully)
	{
		auto mapIter = insertionIter.first;

		if (Entity* insertedEntity = mapIter->second.get())
		{
			return insertedEntity;
		}
	}
	
	return nullptr;
}

Entity* EntityRegistry::createEntityFromTemplate(const std::string& templateName)
{
	auto searchIter = entityTemplates.find(templateName);
	
	if (searchIter != entityTemplates.end())
	{
		if (Entity* entity = searchIter->second.get())
		{
			return clone(entity);
		}
	}
	
	return nullptr;
}

const std::map<std::string, std::unique_ptr<Entity>>& EntityRegistry::getEntityTemplateMap()
{
	return entityTemplates;
}

Entity* EntityRegistry::clone(Entity* entity)
{
	if (entity)
	{
		// Entity exists. Let's clone it
		std::unique_ptr<Entity> newEntityPtr = std::make_unique<Entity>(*entity);
		newEntityPtr->components.clear();
		//newEntityPtr->setEntityName(newEntityPtr->getEntityName() + "_Clone");

		if (newEntityPtr)
		{
			for (Component* component : entity->getComponents())
			{
				if (!component)
				{
					continue;
				}

				if (SceneNode* componentAsNode = dynamic_cast<SceneNode*>(component))
				{
					Component* newComponent = nullptr;
					if (Component* foundComponent = newEntityPtr->findComponent(component->getComponentName()))
					{
						newComponent = foundComponent;
					}
					else
					{
						newComponent = clone(component);
						newEntityPtr->addComponent(newComponent);

						// Clear children
						if (SceneNode* n = dynamic_cast<SceneNode*>(newComponent))
						{
							for (SceneNode* child : n->getChildren())
							{
								n->detachChild(child);
							}
						}
					}

					for (SceneNode* child : componentAsNode->getChildren())
					{
						if (Component* childAsComponent = dynamic_cast<Component*>(child))
						{
							if (childAsComponent->getOwner() != entity)
							{
								continue;
							}

							if (Component* newChildComponent = clone(childAsComponent))
							{
								newEntityPtr->addComponent(newChildComponent);

								if (SceneNode* newChildAsNode = dynamic_cast<SceneNode*>(newChildComponent))
								{
									//// Clear children
									//for (SceneNode* child : newChildAsNode->getChildren())
									//{
									//	newChildAsNode->detachChild(child);
									//}

									if (SceneNode* newComponentAsNode = dynamic_cast<SceneNode*>(newComponent))
									{
										newChildAsNode->setParent(newComponentAsNode);
									}
								}
							}
						}
					}
				}
				else
				{
					// No need to clone hierarchy, it's not a SceneNode
					Component* newComponent = clone(component);
					newEntityPtr->addComponent(newComponent);
				}
			}

			Entity* newEntity = newEntityPtr.get();

			auto searchIter = entityMap.find(newEntityPtr->getEntityName());
			if (searchIter != entityMap.end())
			{
				newEntityPtr->setEntityName(newEntityPtr->getEntityName() + "_Copy");
			}

			entityMap[newEntityPtr->getEntityName()].push_back(std::move(newEntityPtr));

			return newEntity;
		}
	}

	return nullptr;
}

const std::vector<std::unique_ptr<Entity>>& EntityRegistry::getAllEntities(const std::string& name)
{
	return entityMap[name];
}

Component* EntityRegistry::clone(Component* component)
{
	std::unique_ptr<Component> clonedComponentPtr = component->clone();

	Component* clonedComponent = clonedComponentPtr.get();

	componentMap[typeid(component)].push_back(std::move(clonedComponentPtr));

	return clonedComponent;
}
