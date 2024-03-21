#pragma once

#include <string>
#include <vector>
#include <memory>

#include "../Components/Component.h"

class Entity
{
public:
	Entity(const std::string& id);
	~Entity() = default;

	void setId(const std::string& id);
	std::string getId() const;

	void addComponent(Component* component);

	template <class ComponentType>
	ComponentType* findComponent(const std::string& id)
	{
		for (Component* component : components)
		{
			if (component->getId() == id)
			{
				if (ComponentType* foundComponent = dynamic_cast<ComponentType*>(component))
				{
					return foundComponent;
				}
			}
		}

		return nullptr;
	}

	template <class ComponentType>
	ComponentType* findComponent()
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

	template <class ComponentType>
	std::vector<ComponentType*> getAllComponentsOfClass()
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
	
private:
	std::string id;
	std::vector<Component*> components;
};
