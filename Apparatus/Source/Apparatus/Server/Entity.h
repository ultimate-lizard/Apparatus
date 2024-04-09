#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>

#include "../Components/Component.h"
#include "../Core/NameProvider.h"
#include "../Core/LifecycleInterface.h"

class SceneNode;

class Entity
{
public:
	Entity(const std::string& name);
	Entity(const Entity& other);
	virtual ~Entity() = default;

	Entity(Entity&&) = delete;

	virtual void init();
	virtual void onSpawn();
	virtual void update(float dt);

	// virtual std::unique_ptr<Entity> clone();

	void setEntityName(const std::string& entityName);
	std::string getEntityName() const;

	std::vector<Component*> getComponents();

	//template <class ComponentType, typename ... Args>
	//ComponentType* createComponent(Args&& ... args);

	//Component* addComponent(std::unique_ptr<Component> component);

	void addComponent(Component* component);

	Component* findComponent(const std::string& name);

	template <class ComponentType>
	ComponentType* findComponent(const std::string& name);

	template <class ComponentType>
	ComponentType* findComponent();

	template <class ComponentType>
	const ComponentType* findComponent() const
	{
		for (const std::unique_ptr<Component>& componentPtr : components)
		{
			if (const Component* component = componentPtr.get())
			{
				if (const ComponentType* foundComponent = dynamic_cast<const ComponentType*>(component))
				{
					return foundComponent;
				}
			}
		}

		return nullptr;
	}

	template <class ComponentType>
	std::vector<ComponentType*> getAllComponentsOfClass();

	template <class ComponentType>
	std::vector<const ComponentType*> getAllComponentsOfClass() const;

protected:
	std::vector<SceneNode*> getAllRootSceneNodeComponents();

	std::string entityName;

	std::vector<Component*> components;

	std::map<std::string, size_t> componentNameMap {};

	friend class EntityRegistry;
};

template<class ComponentType>
inline ComponentType* Entity::findComponent(const std::string& name)
{
	for (Component* component : components)
	{
		if (component->getComponentName() == name)
		{
			if (ComponentType* foundComponent = dynamic_cast<ComponentType*>(component))
			{
				return foundComponent;
			}
		}
	}

	return nullptr;
}

template<class ComponentType>
inline ComponentType* Entity::findComponent()
{
	for (Component* component : components)
	{
		if (ComponentType* foundComponent = dynamic_cast<ComponentType*>(component))
		{
			return foundComponent;
		}
	}

	return nullptr;
}

template<class ComponentType>
inline std::vector<ComponentType*> Entity::getAllComponentsOfClass()
{
	std::vector<ComponentType*> result;

	for (Component* component : components)
	{
		if (ComponentType* foundComponent = dynamic_cast<ComponentType*>(component))
		{
			result.push_back(foundComponent);
		}
	}

	return result;
}

template<class ComponentType>
inline std::vector<const ComponentType*> Entity::getAllComponentsOfClass() const
{
	std::vector<const ComponentType*> result;

	for (Component* component : components)
	{
		if (const ComponentType* foundComponent = dynamic_cast<ComponentType*>(component))
		{
			result.push_back(foundComponent);
		}
	}

	return result;
}

//template<class ComponentType>
//inline std::vector<ComponentType*> LocalServer::getAllComponentsOfClass()
//{
//	std::vector<ComponentType*> result;
//	for (std::unique_ptr<Component>& component : components)
//	{
//		if (ComponentType* foundComponent = dynamic_cast<ComponentType*>(component.get()))
//		{
//			if (Entity* owner = foundComponent->getOwner())
//			{
//				if (!owner->isTemplate())
//				{
//					result.push_back(foundComponent);
//				}
//			}
//		}
//	}
//
//	return result;
//}

//template<class ComponentType, typename ...Args>
//inline ComponentType* Entity::createComponent(Args&& ... args)
//{
//	components.push_back(std::make_unique<ComponentType>(std::forward<Args>(args)...));
//
//	ComponentType* component = dynamic_cast<ComponentType*>(components.back().get());
//
//	const std::string name = component->getComponentName();
//
//	size_t& namesFound = componentNameMap[name];
//	if (namesFound != 0)
//	{
//		component->setComponentName(name + "_" + std::to_string(namesFound));
//		namesFound++;
//	}
//
//	return component;
//}
