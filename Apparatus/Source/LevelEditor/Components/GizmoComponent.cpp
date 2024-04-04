#include "GizmoComponent.h"

#include <Apparatus/Apparatus.h>
#include <Apparatus/Server/Entity.h>
#include <Apparatus/Server/LocalServer.h>
#include <Apparatus/Components/TransformComponent.h>
#include <Apparatus/Components/ModelComponent.h>
#include <Apparatus/Util/CollisionDetection.h>

#include "SelectableComponent.h"
#include "../Client/EditorLocalClient.h"

GizmoComponent::GizmoComponent(Entity* owner) :
	Component(owner),
	editorLocalClient(nullptr),
	interactionMode(InteractionMode::Translation),
	selectedGizmoModel(nullptr),
	pressed(false),
	visible(false),
	clickAngle(0.0f),
	lastCursorPosition(0),
	clickPosition(0.0f),
	gizmoSelectPositionOrigin(0.0f),
	gizmoClickOffset(0.0f)
{
	assignDefaultObjectName();
}

GizmoComponent::GizmoComponent(Entity* owner, const std::string& name) :
	Component(owner, name),
	editorLocalClient(nullptr),
	interactionMode(InteractionMode::Translation),
	selectedGizmoModel(nullptr),
	pressed(false),
	visible(false),
	clickAngle(0.0f),
	lastCursorPosition(0),
	clickPosition(0.0f),
	gizmoSelectPositionOrigin(0.0f),
	gizmoClickOffset(0.0f)
{
}

void GizmoComponent::init()
{
	Component::init();

	updateVisibility();
}

void GizmoComponent::setEditorLocalClient(EditorLocalClient* editorLocalClient)
{
	this->editorLocalClient = editorLocalClient;
}

void GizmoComponent::setSelectedGizmoModel(ModelComponent* modelComponent)
{
	this->selectedGizmoModel = modelComponent;
}

InteractionMode GizmoComponent::getInteractionMode() const
{
	return interactionMode;
}

void GizmoComponent::setInteractionMode(InteractionMode interactionMode)
{
	this->interactionMode = interactionMode;

	if (visible)
	{
		updateVisibility();
	}
}

void GizmoComponent::setVisibility(bool enabled)
{
	this->visible = enabled;

	updateVisibility();
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

	TransformComponent* selectedEntityTransform = attachmentEntity->findComponent<TransformComponent>();
	if (!selectedEntityTransform)
	{
		return;
	}

	// POSITION -------------------------------------------------------
		// Set local position of gizmo to 0 so it will not look displaced relative to the parent
	gizmoTransform->setPosition(glm::vec3(0.0f));
	// Inherit position of the entity. Gizmo doesn't inherit rotation, though. It is set manually
	gizmoTransform->setParent(selectedEntityTransform);

	// Gizmo must inherit entity parent's rotation manually, or reset its rotation, if no parent.
	// After that, gizmos and gimbals will face parent's direction but still have their own local rotations
	if (SceneNode* entitysParent = selectedEntityTransform->getParent())
	{
		// Gizmo rotation is now in the parent space
		gizmoTransform->setOrientation(entitysParent->getWorldOrientation());
	}
	else
	{
		// Gizmo rotation is not in the global space
		gizmoTransform->setOrientation(glm::quat(glm::vec3(0.0f)));
	}

	// ROTATION -------------------------------------------------------
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
}

void GizmoComponent::press(const glm::vec3& clickWorldPosition)
{
	this->pressed = true;

	if (!selectedGizmoModel || !editorLocalClient)
	{
		return;
	}

	LocalServer* localServer = editorLocalClient->getLocalServer();
	if (!localServer)
	{
		return;
	}

	Apparatus* apparatus = editorLocalClient->getApparatus();
	if (!apparatus)
	{
		return;
	}

	Camera* camera = editorLocalClient->getActiveCamera();
	if (!camera)
	{
		return;
	}

	if (getInteractionMode() == InteractionMode::Translation)
	{
		gizmoClickOffset = clickWorldPosition - selectedGizmoModel->getWorldPosition();
		gizmoSelectPositionOrigin = selectedGizmoModel->getWorldPosition();
	}
	else if (getInteractionMode() == InteractionMode::Rotation)
	{
		lastCursorPosition = apparatus->getMouseCursorPosition();
		apparatus->setCursorVisibleEnabled(false);

		glm::vec3 rayDirection = glm::normalize(localServer->getCursorToWorldRay(camera->getView(), camera->getProjection()));
		glm::vec3 rayOrigin = camera->getWorldPosition();
		glm::vec3 gizmoOrigin = selectedGizmoModel->getPosition();
		glm::vec3 gizmoUp = getLocalUpForGimbal(selectedGizmoModel->getObjectName());
		glm::vec3 camRight = camera->getRight();

		// Convert to gimbal's local space
		if (SceneNode* parent = selectedGizmoModel->getParent())
		{
			rayDirection = glm::inverse(parent->getTransform()) * glm::vec4(rayDirection, 0.0f);
			rayOrigin = glm::inverse(parent->getTransform()) * glm::vec4(rayOrigin, 1.0f);
			camRight = glm::inverse(parent->getTransform()) * glm::vec4(camRight, 1.0f);
		}

		clickPosition = rayVsPlane(gizmoOrigin, gizmoUp, rayOrigin, rayDirection);

		glm::vec3 clickToGimbalProjection = clickPosition - gizmoOrigin;
		const glm::vec2 clickToGimbalProjection2d = projectOnPlane(clickToGimbalProjection, gizmoUp);
		clickAngle = Rotator::normalizeAngle(glm::degrees(glm::atan(clickToGimbalProjection2d.x, clickToGimbalProjection2d.y)));
	}
}

void GizmoComponent::release()
{
	if (getInteractionMode() == InteractionMode::Rotation)
	{
		if (pressed)
		{
			if (Apparatus* apparatus = editorLocalClient->getApparatus())
			{
				apparatus->setCursorVisibleEnabled(true);
			}
		}
	}

	this->pressed = false;
}

bool GizmoComponent::isPressed() const
{
	return pressed == true;
}

void GizmoComponent::handleCursorMovement(float inputX, float inputY)
{
	if (!pressed)
	{
		return;
	}

	switch (getInteractionMode())
	{
	case InteractionMode::Translation:
		handleTranslation();
		break;

	case InteractionMode::Rotation:
		handleRotation();
		break;
	}
}

void GizmoComponent::assignDefaultObjectName()
{
	setObjectName("Gizmo");
}

void GizmoComponent::updateVisibility()
{
	for (ModelComponent* elementModelComponent : owner->getAllComponentsOfClass<ModelComponent>())
	{
		if (!elementModelComponent)
		{
			continue;
		}

		if (interactionMode == InteractionMode::Translation)
		{
			elementModelComponent->setVisibility((elementModelComponent->getObjectName().find(GizmoNames::Gizmo) != std::string::npos) && visible);
		}
		else if (interactionMode == InteractionMode::Rotation)
		{
			std::string modelName = elementModelComponent->getObjectName();
			elementModelComponent->setVisibility((modelName == GizmoNames::Pitch || modelName == GizmoNames::Yaw || modelName == GizmoNames::Roll) && visible);
		}
	}
}

void GizmoComponent::handleTranslation()
{
	if (!selectedGizmoModel || !editorLocalClient)
	{
		return;
	}

	LocalServer* localServer = editorLocalClient->getLocalServer();
	if (!localServer)
	{
		return;
	}

	Camera* camera = editorLocalClient->getActiveCamera();
	if (!camera)
	{
		return;
	}

	Entity* selectedEntity = editorLocalClient->getSelectedEntity();
	if (!selectedEntity)
	{
		return;
	}

	TransformComponent* selectedEntityTransform = selectedEntity->findComponent<TransformComponent>();
	if (!selectedEntityTransform)
	{
		return;
	}

	// Convert all required vectors into gizmo's local space for the easier creation of the gizmo axis normals
	glm::vec3 rayOriginLocal = camera->getWorldPosition();
	glm::vec3 rayDirectionLocal = glm::normalize(localServer->getCursorToWorldRay(camera->getView(), camera->getProjection()));
	glm::vec3 gizmoOriginLocal = selectedEntityTransform->getWorldPosition();

	glm::vec3 gizmoSelectPositionOriginLocal = gizmoSelectPositionOrigin;
	// No need to change offsets to local space. Only rotate
	glm::vec3 gizmoClickOffsetLocal = gizmoClickOffset;

	// Convert to gizmo's local space (for easier way of getting the proper axis plane normal)
	if (SceneNode* parent = selectedEntityTransform->getParent())
	{
		rayOriginLocal = glm::inverse(parent->getTransform()) * glm::vec4(rayOriginLocal, 1.0f);
		rayDirectionLocal = glm::inverse(parent->getTransform()) * glm::vec4(rayDirectionLocal, 0.0f);
		gizmoOriginLocal = glm::inverse(parent->getTransform()) * glm::vec4(gizmoOriginLocal, 1.0f);

		gizmoSelectPositionOriginLocal = glm::inverse(parent->getTransform()) * glm::vec4(gizmoSelectPositionOriginLocal, 1.0f);

		// No need to convert to local coordiante system since this is just an offset. But need to rotate it
		gizmoClickOffsetLocal = glm::inverse(parent->getWorldOrientation()) * gizmoClickOffsetLocal;
	}

	glm::vec3 axisPlaneNormal = glm::normalize(rayOriginLocal - gizmoSelectPositionOriginLocal);

	const std::string selectedGizmoName = selectedGizmoModel->getObjectName();
	if (selectedGizmoName == GizmoNames::GizmoX)
	{
		axisPlaneNormal.x = 0.0f;
	}
	else if (selectedGizmoName == GizmoNames::GizmoXY)
	{
		axisPlaneNormal.y = 0.0f;
		axisPlaneNormal.x = 0.0f;
	}
	else if (selectedGizmoName == GizmoNames::GizmoXZ)
	{
		axisPlaneNormal.z = 0.0f;
		axisPlaneNormal.x = 0.0f;
	}
	else if (selectedGizmoName == GizmoNames::GizmoY)
	{
		axisPlaneNormal.y = 0.0f;
	}
	else if (selectedGizmoName == GizmoNames::GizmoZY)
	{
		axisPlaneNormal.y = 0.0f;
		axisPlaneNormal.z = 0.0f;
	}
	else if (selectedGizmoName == GizmoNames::GizmoZ)
	{
		axisPlaneNormal.z = 0.0f;
	}

	glm::vec3 newPosition = rayVsPlane(gizmoSelectPositionOriginLocal, axisPlaneNormal, rayOriginLocal, rayDirectionLocal);
	if (glm::all(glm::isnan(newPosition)))
	{
		return;
	}

	newPosition -= gizmoClickOffsetLocal;

	if (selectedGizmoName == GizmoNames::GizmoX)
	{
		newPosition.y = gizmoOriginLocal.y;
		newPosition.z = gizmoOriginLocal.z;
	}
	else if (selectedGizmoName == GizmoNames::GizmoXY)
	{
		newPosition.z = gizmoOriginLocal.z;
	}
	else if (selectedGizmoName == GizmoNames::GizmoXZ)
	{
		newPosition.y = gizmoOriginLocal.y;
	}
	else if (selectedGizmoName == GizmoNames::GizmoY)
	{
		newPosition.x = gizmoOriginLocal.x;
		newPosition.z = gizmoOriginLocal.z;
	}
	else if (selectedGizmoName == GizmoNames::GizmoZY)
	{
		newPosition.x = gizmoOriginLocal.x;
	}
	else if (selectedGizmoName == GizmoNames::GizmoZ)
	{
		newPosition.x = gizmoOriginLocal.x;
		newPosition.y = gizmoOriginLocal.y;
	}

	selectedEntityTransform->setPosition(newPosition);
}

void GizmoComponent::handleRotation()
{
	if (!selectedGizmoModel)
	{
		return;
	}

	LocalServer* localServer = editorLocalClient->getLocalServer();
	if (!localServer)
	{
		return;
	}

	Camera* camera = editorLocalClient->getActiveCamera();
	if (!camera)
	{
		return;
	}

	Entity* selectedEntity = editorLocalClient->getSelectedEntity();
	if (!selectedEntity)
	{
		return;
	}

	TransformComponent* selectedEntityTransform = selectedEntity->findComponent<TransformComponent>();
	if (!selectedEntityTransform)
	{
		return;
	}

	if (SelectableComponent* selectableComponent = selectedEntity->findComponent<SelectableComponent>())
	{
		selectableComponent->setBoxVisible(false);
	}

	glm::vec3 rayDirection = glm::normalize(localServer->getCursorToWorldRay(camera->getView(), camera->getProjection()));
	glm::vec3 rayOrigin = camera->getWorldPosition();
	glm::vec3 gizmoOrigin = selectedGizmoModel->getPosition();
	glm::vec3 gizmoUp = getLocalUpForGimbal(selectedGizmoModel->getObjectName());

	if (SceneNode* parent = selectedGizmoModel->getParent())
	{
		rayDirection = glm::inverse(parent->getTransform()) * glm::vec4(rayDirection, 0.0f);
		rayOrigin = glm::inverse(parent->getTransform()) * glm::vec4(rayOrigin, 1.0f);
	}

	glm::vec3 dragPos = rayVsPlane(gizmoOrigin, gizmoUp, rayOrigin, rayDirection);
	glm::vec3 dragToGimbalProjection = dragPos - gizmoOrigin;
	const glm::vec2 dragToGimbalProjection2d = projectOnPlane(dragToGimbalProjection, gizmoUp);

	float dragAngle = Rotator::normalizeAngle(glm::degrees(glm::atan(dragToGimbalProjection2d.x, dragToGimbalProjection2d.y)));
	float deltaAngle = clickAngle - dragAngle;

	if (selectedGizmoModel->getObjectName() == GizmoNames::Yaw)
	{
		deltaAngle = -deltaAngle;
	}

	const Euler gimbalAngle = getEulerAngleOfGimbal(selectedGizmoModel->getObjectName());
	selectedEntityTransform->rotate(deltaAngle, gimbalAngle);
	selectedGizmoModel->rotate(deltaAngle, gimbalAngle);

	if (Apparatus* app = editorLocalClient->getApparatus())
	{
		app->setCursorPosition(lastCursorPosition);
	}
}

glm::vec3 GizmoComponent::getLocalUpForGimbal(const std::string& gimbalName) const
{
	if (gimbalName == GizmoNames::Pitch)
	{
		return selectedGizmoModel->getLocalRight();
	}
	else if (gimbalName == GizmoNames::Yaw)
	{
		return selectedGizmoModel->getLocalUp();
	}
	else if (gimbalName == GizmoNames::Roll)
	{
		return selectedGizmoModel->getLocalForward();
	}

	return glm::vec3(std::numeric_limits<float>::quiet_NaN());
}

Euler GizmoComponent::getEulerAngleOfGimbal(const std::string& gimbalName) const
{
	if (gimbalName == GizmoNames::Pitch)
	{
		return Euler::Pitch;
	}
	else if (gimbalName == GizmoNames::Yaw)
	{
		return Euler::Yaw;
	}
	else if (gimbalName == GizmoNames::Roll)
	{
		return Euler::Roll;
	}

	return Euler::Pitch;
}

glm::vec2 GizmoComponent::projectOnPlane(const glm::vec3& position, const glm::vec3& normal) const
{
	glm::vec2 result;

	glm::vec3 test = position;

	glm::vec3 planeNormal = glm::abs(normal);
	int upAxis = 0;

	float max = std::max({ planeNormal.x, planeNormal.y, planeNormal.z });
	for (int i = 0; i < 3; ++i)
	{
		if (planeNormal[i] == max)
		{
			upAxis = i;
		}
	}

	test[upAxis] = std::numeric_limits<float>::quiet_NaN();

	result.x = glm::isnan(test.x) ? test.y : test.x;
	result.y = (result.x == test.y) ? test.z : (glm::isnan(test.y) ? test.z : test.y);

	return result;
}
