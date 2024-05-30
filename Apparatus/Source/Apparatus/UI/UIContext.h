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
#include "../Core/Input/InputHandler.h"

class Material;
class InputHandler;
class Button;
class Texture;

class UIContext
{
public:
	UIContext(InputHandler* inputHandler);

	void init();

	void renderContext(SpriteRenderer* renderer);

	void refreshWidgetTree();
	void invalidateTree();

	template <class WidgetType>
	WidgetType* createWidget(const std::string& name);

	template <class WidgetType>
	Material* findMaterialForWidget();

	// Returns whether the input was captured
	bool handleKeyInput(InputKey key, KeyEventType type);
	void handleAxisInput(InputAxis axis, float value);

	Button* createNinePatchButton(const std::string& name, const std::string& idleTextureName, const std::string& hoverTextureName, const std::string& pressTextureName, unsigned int border, const std::string& label = "", unsigned int fontSize = 18);

private:
	void onWindowResize(std::shared_ptr<WindowResizeEvent> event);

	std::vector<std::unique_ptr<Widget>> spawnedWidgets;
	std::map<std::type_index, Material*> widgetMaterialMap;

	InputHandler* inputHandler;
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

	// refreshWidgetTree();
	
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
