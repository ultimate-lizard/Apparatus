#include "SelectableComponent.h"

#include <Apparatus/Server/Entity.h>
#include <Apparatus/Components/TransformComponent.h>
#include <Apparatus/Components/ModelComponent.h>
#include <Apparatus/Util/BoundingBox.h>

SelectableComponent::SelectableComponent(Entity* owner) :
	Component(owner),
	selected(false),
	cachedPosition(0.0f),
	boxVisible(true)
{
	assignDefaultObjectName();
}

void SelectableComponent::init()
{
	regenerateVisualBoundingBox();
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

	if (auto transform = owner->findComponent<TransformComponent>())
	{
		if (transform->getWorldPosition() != cachedPosition)
		{
			glm::vec3 delta = transform->getWorldPosition() - cachedPosition;
			cachedPosition = transform->getWorldPosition();
			box.position += delta;
		}
	}
}

void SelectableComponent::regenerateVisualBoundingBox()
{
	if (!owner)
	{
		return;
	}

	auto transformComponent = owner->findComponent<TransformComponent>();
	if (!transformComponent)
	{
		return;
	}

	cachedPosition = transformComponent->getWorldPosition();

	std::vector<ModelComponent*> modelComponents = owner->getAllComponentsOfClass<ModelComponent>();
	if (modelComponents.size() > 1)
	{
		std::vector<Model*> models;
		for (ModelComponent* modelComponent : modelComponents)
		{
			if (modelComponent)
			{
				if (Model* model = modelComponent->getModel())
				{
					models.push_back(model);
				}
			}
		}

		box = generateAABB(models, transformComponent->getWorldPosition(), transformComponent->getWorldOrientation());
	}
	else if (!modelComponents.empty())
	{
		ModelComponent* modelComponent = modelComponents[0];
		box = generateAABB(modelComponent->getModel(), modelComponent->getWorldPosition(), modelComponent->getWorldOrientation());
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

void SelectableComponent::assignDefaultObjectName()
{
	if (owner)
	{
		setObjectName(owner->getObjectName() + "_SelectableComponent");
	}
}
