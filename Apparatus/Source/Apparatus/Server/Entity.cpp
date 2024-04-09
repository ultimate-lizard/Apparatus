#include "Entity.h"

#include "../Core/Logger.h"
#include "../Rendering/SceneNode.h"

Entity::Entity(const std::string& name) :
	entityName(name)
{
}

Entity::Entity(const Entity& other) :
	entityName(other.entityName),
	components(other.components)
{

}


//
//Component* Entity::addComponent(std::unique_ptr<Component> component)
//{
//	const std::string name = component->getComponentName();
//
//	size_t& namesFound = componentNameMap[name];
//	if (namesFound != 0)
//	{
//		component->setComponentName(name + "_" + std::to_string(namesFound));
//		namesFound++;
//	}
//
//	components.push_back(std::move(component));
//	
//	std::unique_ptr<Component>& newComponent = components.back();
//
//	return newComponent.get();
//}

void Entity::addComponent(Component* component)
{
	component->setOwner(this);
	components.push_back(component);
}

Component* Entity::findComponent(const std::string& name)
{
	auto foundIter = std::find_if(components.begin(), components.end(), [name](Component* component) {
		return component && component->getComponentName() == name;
		});

	if (foundIter != components.end())
	{
		return *foundIter;
	}

	return nullptr;
}

void Entity::setEntityName(const std::string& entityName)
{
	this->entityName = entityName;
}

std::string Entity::getEntityName() const
{
	return entityName;
}

std::vector<Component*> Entity::getComponents()
{
	return components;
}

void Entity::init()
{
	LOG("Initialized entity '" + getEntityName() + "'", LogLevel::Info);

	for (Component* component : components)
	{
		if (component)
		{
			component->init();
		}
	}
}

void Entity::onSpawn()
{
	for (Component* component : components)
	{
		if (component)
		{
			component->onEntitySpawn();
		}
	}
}

void Entity::update(float dt)
{
	for (Component* component : components)
	{
		if (component)
		{
			component->update(dt);
		}
	}
}

std::vector<SceneNode*> Entity::getAllRootSceneNodeComponents()
{
	std::vector<SceneNode*> result;

	for (Component* component : components)
	{
		if (SceneNode* node = dynamic_cast<SceneNode*>(component))
		{
			if (!node->getParent())
			{
				result.push_back(node);
			}
		}
	}

	return result;
}
