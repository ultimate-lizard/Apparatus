#include "GizmoComponent.h"

#include <cmath>

#include <Apparatus/Apparatus.h>
#include <Apparatus/Server/Entity.h>
#include <Apparatus/Components/TransformComponent.h>
#include <Apparatus/Components/ModelComponent.h>
#include <Apparatus/Rendering/Vector.h>
#include <Apparatus/Core/Window/Window.h>

#include "../Components/SelectableComponent.h"
#include "../Client/EditorLocalClient.h"
#include "../Server/EditorLocalServer.h"

#include "GizmoComponent/GizmoNames.h"
#include "GizmoComponent/GizmoState/GizmoTranslationState.h"

GizmoComponent::GizmoComponent() :
	Component("GizmoComponent"),
	interactionMode(InteractionMode::Translation),
	selectedGizmoModel(nullptr),
	pressed(false),
	gizmoVisible(false),
	selectedEntity(nullptr)
{
}

GizmoComponent::GizmoComponent(const std::string& componentName) :
	Component(componentName),
	interactionMode(InteractionMode::Translation),
	selectedGizmoModel(nullptr),
	pressed(false),
	gizmoVisible(false),
	selectedEntity(nullptr)
{
}

GizmoComponent::GizmoComponent(const GizmoComponent& other) :
	Component(other),
	interactionMode(other.interactionMode),
	selectedGizmoModel(other.selectedGizmoModel),
	pressed(other.pressed),
	gizmoVisible(other.pressed),
	selectedEntity(other.selectedEntity),
	snappingData(other.snappingData)
{
}

std::unique_ptr<Component> GizmoComponent::clone()
{
	return std::make_unique<GizmoComponent>(*this);
}

void GizmoComponent::init()
{
	Component::init();

	changeState<GizmoTranslationState>();

	setVisibility(false);
}

void GizmoComponent::update(float dt)
{
	Component::update(dt);

	Apparatus& app = Apparatus::get();

	if (LocalClient* localClient = app.getPrimaryLocalClient())
	{
		if (Camera* camera = localClient->getActiveCamera())
		{
			if (!owner)
			{
				return;
			}

			if (TransformComponent* ownerTransform = owner->findComponent<TransformComponent>())
			{
				float distanceToCamera = glm::distance(camera->getPosition(), ownerTransform->getWorldPosition()) * 0.075f;
				ownerTransform->setScale(glm::vec3(distanceToCamera));
			}
		}
	}

	if (gizmoState)
	{
		gizmoState->updateVisibility();
	}
}

void GizmoComponent::setSelectedGizmoModel(const std::string& gizmoModelName)
{
	if (!owner)
	{
		return;
	}

	if (ModelComponent* modelComponent = owner->findComponent<ModelComponent>(gizmoModelName))
	{
		selectedGizmoModel = modelComponent;
	}

	if (gizmoState)
	{
		gizmoState->setSelectedGizmoModelComponent(selectedGizmoModel);
	}
}

void GizmoComponent::setVisibility(bool enabled)
{
	gizmoVisible = enabled;

	if (gizmoState)
	{
		gizmoState->setGizmoVisibility(enabled);
	}
}

void GizmoComponent::attach(Entity* attachmentEntity)
{
	TransformComponent* gizmoTransform = owner->findComponent<TransformComponent>();
	if (!gizmoTransform)
	{
		return;
	}

	if (!attachmentEntity)
	{
		gizmoTransform->setParent(nullptr);
		gizmoTransform->setOrientation(glm::quat(glm::vec3(0.0f)));
		gizmoTransform->setPosition(glm::vec3(0.0f));

		return;
	}

	if (TransformComponent* selectedEntityTransform = attachmentEntity->findComponent<TransformComponent>())
	{
		snappingData.unsnappedRotator = selectedEntityTransform->getRotator();
		snappingData.unsnappedScale = selectedEntityTransform->getScale();

		if (owner)
		{
			if (TransformComponent* gizmoTransformComponent = owner->findComponent<TransformComponent>())
			{
				// Set local position of gizmo to 0 so it will not look displaced relative to the parent
				gizmoTransformComponent->setPosition(glm::vec3(0.0f));
				// Inherit position of the entity. Gizmo doesn't inherit rotation, though. It is set manually
				gizmoTransformComponent->setParent(selectedEntityTransform);

				// Gizmo must inherit entity parent's rotation manually, or reset its rotation, if no parent.
				// After that, gizmos and gimbals will face parent's direction but still have their own local rotations
				if (SceneNode* entitysParent = selectedEntityTransform->getParent())
				{
					// Gizmo rotation is now in the parent space
					gizmoTransformComponent->setOrientation(entitysParent->getWorldOrientation());
				}
				else
				{
					// Gizmo rotation is not in the global space
					gizmoTransformComponent->setOrientation(glm::quat(glm::vec3(0.0f)));
				}
			}

			// Pass the selected entity's LOCAL rotations to the gimbals
			if (ModelComponent* pitchModel = owner->findComponent<ModelComponent>(GizmoNames::Pitch))
			{
				pitchModel->setRotation(selectedEntityTransform->getRotationAngle(Euler::Pitch), Euler::Pitch);
			}

			if (ModelComponent* yawModel = owner->findComponent<ModelComponent>(GizmoNames::Yaw))
			{
				yawModel->setRotation(selectedEntityTransform->getRotationAngle(Euler::Yaw), Euler::Yaw);
			}

			if (ModelComponent* rollModel = owner->findComponent<ModelComponent>(GizmoNames::Roll))
			{
				rollModel->setRotation(selectedEntityTransform->getRotationAngle(Euler::Roll), Euler::Roll);
			}

			// Rotate scale
			for (ModelComponent* modelComponent : owner->getAllComponentsOfClass<ModelComponent>())
			{
				if (!modelComponent)
				{
					continue;
				}

				if (modelComponent->getComponentName().find(GizmoNames::Scale) != std::string::npos)
				{
					modelComponent->setRotation(selectedEntityTransform->getRotator());
				}
			}
		}
	}
}

void GizmoComponent::press(const glm::vec3& clickWorldPosition)
{
	this->pressed = true;

	if (gizmoState)
	{
		gizmoState->handleGizmoClick(clickWorldPosition);
	}
}

void GizmoComponent::release()
{
	if (gizmoState)
	{
		gizmoState->handleGizmoRelease();
	}

	this->pressed = false;
}

bool GizmoComponent::isPressed() const
{
	return pressed == true;
}

void GizmoComponent::handleCursorMovement(float inputX, float inputY)
{
	if (pressed)
	{
		Camera* camera = nullptr;

		Apparatus& app = Apparatus::get();
		if (LocalClient* localClient = app.getPrimaryLocalClient())
		{
			camera = localClient->getActiveCamera();
		}

		if (selectedEntity && gizmoState)
		{
			gizmoState->handleCursorMovement({ inputX, inputY }, camera, selectedEntity->findComponent<TransformComponent>());
		}
	}
}

void GizmoComponent::setSnapToGridEnabled(bool enabled)
{
	snappingData.snapToGrid = enabled;
}

void GizmoComponent::setSelectedEntity(Entity* selectedEntity)
{
	this->selectedEntity = selectedEntity;
}
