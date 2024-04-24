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

SelectableComponent::SelectableComponent(const SelectableComponent& other) :
	Component(other),
	selected(other.selected),
	cachedPosition(other.cachedPosition),
	boxVisible(other.boxVisible)
{
}

std::unique_ptr<Component> SelectableComponent::clone()
{
	return std::make_unique<SelectableComponent>(*this);
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
