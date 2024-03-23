#include "SelectableComponent.h"

#include <Apparatus/Server/Entity.h>
#include <Apparatus/Components/TransformComponent.h>
#include <Apparatus/Components/ModelComponent.h>
#include <Apparatus/Util/BoundingBox.h>

SelectableComponent::SelectableComponent(Entity* owner) :
	Component(owner),
	selected(false),
	cachedPosition(0.0f)
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
		if (transform->getDerivedPosition() != cachedPosition)
		{
			glm::vec3 delta = transform->getDerivedPosition() - cachedPosition;
			cachedPosition = transform->getDerivedPosition();
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

	cachedPosition = transformComponent->getDerivedPosition();

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

		box = generateAABB(models, transformComponent->getDerivedPosition(), transformComponent->getDerivedOrientation());
	}
	else if (!modelComponents.empty())
	{
		ModelComponent* modelComponent = modelComponents[0];
		box = generateAABB(modelComponent->getModel(), modelComponent->getDerivedPosition(), modelComponent->getDerivedOrientation());
	}
}

void SelectableComponent::assignDefaultObjectName()
{
	if (owner)
	{
		setObjectName(owner->getObjectName() + "_SelectableComponent");
	}
}
