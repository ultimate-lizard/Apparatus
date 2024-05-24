#pragma once

#include <unordered_map>
#include <typeindex>
#include <functional>

#include "../Server/Entity.h"
#include "EngineSystem/EngineSystem.h"

class Component;

class EntityRegistry : public EngineSystem
{
public:
	EntityRegistry();

	virtual void init() override {}
	virtual void uninit() override {}

	Entity* createEntityTemplate(const std::string& templateName);
	Entity* createEntityFromTemplate(const std::string& templateName);

	const std::map<std::string, std::unique_ptr<Entity>>& getEntityTemplateMap();

	Entity* clone(Entity* entity);
	Component* clone(Component* component);

	template <class ComponentType, typename ... Args>
	ComponentType* createComponent(Entity* entity, Args&& ... args);

	const std::vector<std::unique_ptr<Entity>>& getAllEntities(const std::string& name);

private:
	std::map<std::string, std::unique_ptr<Entity>> entityTemplates;
	std::map<std::string, std::vector<std::unique_ptr<Entity>>> entityMap;

	std::unordered_map<std::type_index, std::vector<std::unique_ptr<Component>>> componentMap;
};

template<class ComponentType, typename ...Args>
inline ComponentType* EntityRegistry::createComponent(Entity* entity, Args&& ... args)
{
	std::unique_ptr<ComponentType> newComponentPtr = std::make_unique<ComponentType>(std::forward<Args>(args)...);
	ComponentType* component = newComponentPtr.get();

	componentMap[typeid(ComponentType)].push_back(std::move(newComponentPtr));

	if (entity->findComponent(component->getComponentName()) != nullptr)
	{
		// TODO: This is a temporary solution
		throw std::exception();
	}
	
	entity->addComponent(component);

	return component;
}
