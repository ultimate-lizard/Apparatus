#pragma once

#include <memory>
#include <string>
#include <vector>
#include <typeindex>
#include <map>
#include <unordered_map>

#include "Panel/Panel.h"
#include "../Rendering/Camera.h"
#include "../Core/Logger.h"
#include "../Event/Event.h"

class Material;

class UIContext
{
public:
	UIContext();

	void init();

	template <class PanelType, typename ... Args>
	PanelType* createPanel(const std::string& name, Args&& ... args);

	void pushContextToRender(SpriteRenderer* renderer);

	void onWindowResize(std::shared_ptr<WindowResizeEvent> event);

	std::unordered_map<std::type_index, Material*> materialsMap;

private:
	std::vector<std::unique_ptr<Panel>> spawnedPanels;

	Panel* root;
};

template<class PanelType, typename ... Args>
inline PanelType* UIContext::createPanel(const std::string& name, Args&& ... args)
{
	// std::unique_ptr<PanelType> newPanelPtr = std::make_unique<PanelType>(std::forward<Args>(args)...);
	auto searchIter = materialsMap.find(typeid(PanelType));
	if (searchIter != materialsMap.end())
	{
		Material* newPanelMaterial = searchIter->second;

		std::unique_ptr<PanelType> newPanelPtr = std::make_unique<PanelType>(std::forward<Args>(args)...);
		if (PanelType* newPanel = newPanelPtr.get())
		{
			spawnedPanels.push_back(std::move(newPanelPtr));
			return newPanel;
		}
	}
	
	return nullptr;
}
