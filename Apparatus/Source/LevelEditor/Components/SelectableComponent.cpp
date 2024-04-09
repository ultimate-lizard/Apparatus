#include "SelectableComponent.h"

#include <Apparatus/Server/Entity.h>
#include <Apparatus/Components/TransformComponent.h>
#include <Apparatus/Components/ModelComponent.h>
#include <Apparatus/Util/BoundingBox.h>

SelectableComponent::SelectableComponent() :
	Component("SelectableComponent"),
	selected(false),
	cachedPosition(0.0f),
	boxVisible(true)
{

}

SelectableComponent::SelectableComponent(const std::string& componentName) :
	Component(componentName),
	selected(false),
	cachedPosition(0.0f),
	boxVisible(true)
{
}

std::unique_ptr<Component> SelectableComponent::clone()
{
	std::unique_ptr<SelectableComponent> newSelectableComponent = std::make_unique<SelectableComponent>(getComponentName());

	if (newSelectableComponent)
	{
		newSelectableComponent->selected = selected;
		newSelectableComponent->cachedPosition = cachedPosition;
		newSelectableComponent->box = box;
		newSelectableComponent->boxVisible = boxVisible;
	}

	return newSelectableComponent;
}

void SelectableComponent::init()
{

}

void SelectableComponent::update(float dt)
{

}

void SelectableComponent::setSelected(bool selected)
{
	this->selected = selected;
}

bool SelectableComponent::isSelected() const
{
	return selected;
}

Box SelectableComponent::getVisualBoundingBox() const
{
	return box;
}

void SelectableComponent::updateVisualBoundingBoxPosition()
{
	if (!owner)
	{
		return;
	}


}

void SelectableComponent::setBoxVisible(bool visible)
{
	boxVisible = visible;
}

bool SelectableComponent::isBoxVisible() const
{
	return boxVisible == true;
}
