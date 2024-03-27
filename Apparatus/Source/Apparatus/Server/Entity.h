#pragma once

#include <string>
#include <vector>
#include <memory>

#include "../Components/Component.h"
#include "../Core/NameProvider.h"
#include "../Core/LifecycleInterface.h"

class Entity : public NameProvider, public LifecycleInterface
{
public:
	Entity(const std::string& name);
	virtual ~Entity() = default;

	virtual void init() override;
	virtual void start() override {};
	virtual void update(float dt) override {};

	void addComponent(Component* component);

	template <class ComponentType>
	ComponentType* findComponent(const std::string& id)
	{
		for (Component* component : components)
		{
			if (component->getObjectName() == id)
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
	
protected:
	virtual void assignDefaultObjectName() override;

private:
	std::vector<Component*> components;
};
