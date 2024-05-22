#pragma once

#include <memory>
#include <string>
#include <vector>
#include <typeindex>
#include <map>
#include <unordered_map>

#include "Widget/Widget.h"
#include "../Rendering/Camera.h"
#include "../Core/Logger.h"
#include "../Event/Event.h"

class Material;

class UIContext
{
public:
	UIContext();

	void init();

	template <class WidgetType>
	WidgetType* createWidget(const std::string& name);

	void renderContext(SpriteRenderer* renderer);

	void onWindowResize(std::shared_ptr<WindowResizeEvent> event);

	template <class WidgetType>
	Material* findMaterialForWidget();

	void refreshWidgetTree();

private:
	std::vector<std::unique_ptr<Widget>> spawnedWidgets;
	std::map<std::type_index, Material*> widgetMaterialMap;
};

template<class WidgetType>
inline WidgetType* UIContext::createWidget(const std::string& name)
{
	auto widgetSearchIter = std::find_if(spawnedWidgets.cbegin(), spawnedWidgets.cend(), [name](const std::unique_ptr<Widget>& widget) {
		return widget && widget->getName() == name;
	});

	if (widgetSearchIter == spawnedWidgets.end())
	{
		auto newWidgetPtr = std::make_unique<WidgetType>();
		newWidgetPtr->setName(name);
		newWidgetPtr->init();

		WidgetType* spawnedWidget = newWidgetPtr.get();
		spawnedWidgets.push_back(std::move(newWidgetPtr));
		return spawnedWidget;
	}
	
	return nullptr;
}

template<class WidgetType>
inline Material* UIContext::findMaterialForWidget()
{
	auto materialSearchIter = widgetMaterialMap.find(typeid(WidgetType));
	if (materialSearchIter != widgetMaterialMap.cend())
	{
		Material* foundMaterial = materialSearchIter->second;
		return foundMaterial;
	}

	return nullptr;
}
