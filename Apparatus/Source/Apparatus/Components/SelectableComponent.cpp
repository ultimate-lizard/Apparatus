#include "SelectableComponent.h"

#include "../Server/Entity.h"
#include "TransformComponent.h"
#include "ModelComponent.h"
#include "../Common/BoundingBox.h"

SelectableComponent::SelectableComponent(const std::string& id) :
	Component(id),
	selected(false)
{
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

Box SelectableComponent::getBoundingBox() const
{
	if (owner)
	{
		if (auto modelComponent = owner->findComponent<ModelComponent>())
		{
			return generateAABB(modelComponent->getModel(), modelComponent->getDerivedPosition(), modelComponent->getDerivedOrientation());
		}

		if (auto transform = owner->findComponent<TransformComponent>())
		{
			return generateAABB(transform->getDebugModel(), transform->getDerivedPosition(), transform->getDerivedOrientation());
		}
	}

	return {};
}
