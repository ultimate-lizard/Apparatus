#include "GizmoComponent.h"

#include <cmath>

#include <Apparatus/Apparatus.h>
#include <Apparatus/Server/Entity.h>
#include <Apparatus/Server/LocalServer.h>
#include <Apparatus/Components/TransformComponent.h>
#include <Apparatus/Components/ModelComponent.h>
#include <Apparatus/Util/CollisionDetection.h>
#include <Apparatus/Rendering/Vector.h>
#include <Apparatus/Window.h>

#include "../Components/SelectableComponent.h"
#include "../Client/EditorLocalClient.h"
#include "../Server/EditorLocalServer.h"

GizmoComponent::GizmoComponent() :
	Component("GizmoComponent"),
	interactionMode(InteractionMode::Translation),
	selectedGizmoModel(nullptr),
	pressed(false),
	visible(false),
	rotationClickAngle(0.0f),
	rotationClickPosition(0.0f),
	gizmoSelectPositionOrigin(0.0f),
	gizmoClickPosition(0.0f),
	gizmoClickOffset(0.0f),
	lastCursorScreenPosition(0),
	clickCursorDevicePosition(0.0f),
	selectedEntity(nullptr)
{
}

GizmoComponent::GizmoComponent(const std::string& componentName) :
	Component(componentName),
	interactionMode(InteractionMode::Translation),
	selectedGizmoModel(nullptr),
	pressed(false),
	visible(false),
	rotationClickAngle(0.0f),
	rotationClickPosition(0.0f),
	gizmoSelectPositionOrigin(0.0f),
	gizmoClickPosition(0.0f),
	gizmoClickOffset(0.0f),
	lastCursorScreenPosition(0),
	clickCursorDevicePosition(0.0f),
	selectedEntity(nullptr)
{
}

GizmoComponent::GizmoComponent(const GizmoComponent& other) :
	Component(other),
	interactionMode(other.interactionMode),
	selectedGizmoModel(other.selectedGizmoModel),
	pressed(other.pressed),
	visible(other.visible),
	rotationClickAngle(other.rotationClickAngle),
	rotationClickPosition(other.rotationClickPosition),
	gizmoSelectPositionOrigin(other.gizmoSelectPositionOrigin),
	gizmoClickPosition(other.gizmoClickPosition),
	gizmoClickOffset(other.gizmoClickOffset),
	lastCursorScreenPosition(other.lastCursorScreenPosition),
	clickCursorDevicePosition(other.clickCursorDevicePosition),
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

	updateVisibility();
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
				float distanceToCamera = glm::distance(camera->getWorldPosition(), ownerTransform->getWorldPosition()) * 0.075f;
				ownerTransform->setScale(glm::vec3(distanceToCamera));
			}
		}
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

	snappingData.unsnappedRotator = selectedEntityTransform->getRotator();
	snappingData.unsnappedScale = selectedEntityTransform->getScale();

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

	if (!owner)
	{
		return;
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

void GizmoComponent::press(const glm::vec3& clickWorldPosition)
{
	Apparatus& app = Apparatus::get();
	Window& window = app.getWindow();

	if (EditorLocalServer* editor = app.getServer<EditorLocalServer>())
	{
		selectedEntity = editor->getSelectedEntity();
	}

	this->pressed = true;

	if (!selectedGizmoModel)
	{
		return;
	}

	if (getInteractionMode() == InteractionMode::Translation || getInteractionMode() == InteractionMode::Scale)
	{
		gizmoClickPosition = clickWorldPosition;
		gizmoClickOffset = (clickWorldPosition - selectedGizmoModel->getWorldPosition()) / selectedGizmoModel->getWorldScale();
		gizmoSelectPositionOrigin = selectedGizmoModel->getWorldPosition();
		clickCursorDevicePosition = getCursorToDevice();
	}
	else if (getInteractionMode() == InteractionMode::Rotation)
	{
		if (LocalClient* localClient = app.getPrimaryLocalClient())
		{
			if (Camera* camera = localClient->getActiveCamera())
			{
				glm::vec3 rayDirectionLocal = glm::normalize(getCursorToWorldRay(camera->getView(), camera->getProjection()));
				glm::vec3 rayOriginLocal = camera->getWorldPosition();
				glm::vec3 camRightLocal = camera->getRight();

				// Convert to gimbal's local space
				if (SceneNode* parent = selectedGizmoModel->getParent())
				{
					rayDirectionLocal = glm::inverse(parent->getTransform()) * glm::vec4(rayDirectionLocal, 0.0f);
					rayOriginLocal = glm::inverse(parent->getTransform()) * glm::vec4(rayOriginLocal, 1.0f);
					camRightLocal = glm::inverse(parent->getTransform()) * glm::vec4(camRightLocal, 1.0f);
				}

				const glm::vec3 gizmoOriginLocal = selectedGizmoModel->getPosition();
				const glm::vec3 gimbalUpLocal = getLocalUpForGimbal(selectedGizmoModel->getComponentName());

				rotationClickPosition = rayVsPlane(gizmoOriginLocal, gimbalUpLocal, rayOriginLocal, rayDirectionLocal);

				glm::vec3 clickToGimbalProjection = rotationClickPosition - gizmoOriginLocal;
				const glm::vec2 clickToGimbalProjection2d = projectOnPlane(clickToGimbalProjection, gimbalUpLocal);
				rotationClickAngle = Rotator::normalizeAngle(glm::degrees(glm::atan(clickToGimbalProjection2d.x, clickToGimbalProjection2d.y)));

				LOG(std::to_string(rotationClickAngle), LogLevel::Info);
			}
		}
	}

	if (getInteractionMode() == InteractionMode::Rotation || getInteractionMode() == InteractionMode::Scale)
	{
		lastCursorScreenPosition = window.getMouseCursorPosition();
		window.setCursorVisibleEnabled(false);

		if (EditorLocalServer* editorLocalServer = app.getServer<EditorLocalServer>())
		{
			editorLocalServer->setSelectionBoxVisibility(false);
		}
	}
}

void GizmoComponent::release()
{
	Apparatus& app = Apparatus::get();

	if (pressed)
	{
		Window& window = app.getWindow();

		if (getInteractionMode() == InteractionMode::Rotation)
		{
			window.setCursorVisibleEnabled(true);

			// Rotate the scale models
			if (selectedEntity && owner)
			{
				if (TransformComponent* selectedEntityTransform = selectedEntity->findComponent<TransformComponent>())
				{
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
		else if (getInteractionMode() == InteractionMode::Scale)
		{
			window.setCursorVisibleEnabled(true);
		}
	}

	if (EditorLocalServer* editorLocalServer = app.getServer<EditorLocalServer>())
	{
		editorLocalServer->regenerateSelectionBoundingBox();
		editorLocalServer->setSelectionBoxVisibility(true);
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
	
	Camera* camera = nullptr;
	Apparatus& app = Apparatus::get();
	Window& window = app.getWindow();
	if (LocalClient* localClient = app.getPrimaryLocalClient())
	{
		camera = localClient->getActiveCamera();
	}

	switch (getInteractionMode())
	{
	case InteractionMode::Translation:
		handleTranslation(window, camera);
		break;

	case InteractionMode::Rotation:
		handleRotation(window, camera);
		break;

	case InteractionMode::Scale:
		handleScale(window, camera);
		break;
	}
}

void GizmoComponent::setSnapToGridEnabled(bool enabled)
{
	snappingData.snapToGrid = enabled;
}

void GizmoComponent::updateVisibility()
{
	if (!owner)
	{
		return;
	}

	for (ModelComponent* elementModelComponent : owner->getAllComponentsOfClass<ModelComponent>())
	{
		if (!elementModelComponent)
		{
			continue;
		}

		const std::string modelName = elementModelComponent->getComponentName();
		if (interactionMode == InteractionMode::Translation)
		{
			elementModelComponent->setVisibility((modelName.find(GizmoNames::Gizmo) != std::string::npos) && visible);
		}
		else if (interactionMode == InteractionMode::Rotation)
		{
			elementModelComponent->setVisibility((modelName == GizmoNames::Pitch || modelName == GizmoNames::Yaw || modelName == GizmoNames::Roll) && visible);
		}
		else
		{
			elementModelComponent->setVisibility((modelName.find(GizmoNames::Scale) != std::string::npos) && visible);
		}
	}
}

void GizmoComponent::handleTranslation(Window& window, Camera* camera)
{
	if (!selectedGizmoModel)
	{
		return;
	}

	if (!camera)
	{
		return;
	}

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
	glm::vec3 rayDirectionLocal = glm::normalize(getCursorToWorldRay(camera->getView(), camera->getProjection()));
	glm::vec3 gizmoOriginLocal = selectedEntityTransform->getWorldPosition();
	glm::vec3 gizmoSelectPositionOriginLocal = gizmoSelectPositionOrigin;

	// No need to change offsets to local space. Only rotate
	glm::vec3 gizmoClickOffsetLocal = gizmoClickOffset * selectedGizmoModel->getWorldScale();

	// Convert to gizmo's local space (for easier way of getting the proper axis plane normal)
	if (SceneNode* parent = selectedEntityTransform->getParent())
	{
		rayOriginLocal = glm::inverse(parent->getTransform()) * glm::vec4(rayOriginLocal, 1.0f);
		rayDirectionLocal = glm::inverse(parent->getTransform()) * glm::vec4(rayDirectionLocal, 0.0f);
		gizmoOriginLocal = glm::inverse(parent->getTransform()) * glm::vec4(gizmoOriginLocal, 1.0f);
		gizmoSelectPositionOriginLocal = glm::inverse(parent->getTransform()) * glm::vec4(gizmoSelectPositionOriginLocal, 1.0f);

		// No need to convert to local coordiante system since this is just an offset. But need to rotate it
		gizmoClickOffsetLocal = glm::inverse(parent->getWorldOrientation()) * gizmoClickOffsetLocal / parent->getWorldScale();
	}

	glm::vec3 axisPlaneNormal = glm::normalize(rayOriginLocal - gizmoSelectPositionOriginLocal);

	const std::string selectedGizmoName = selectedGizmoModel->getComponentName();
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
	if (glm::any(glm::isnan(newPosition)))
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

	// Snapping
	if (snappingData.snapToGrid)
	{
		newPosition = glm::round(newPosition / snappingData.translationStep) * snappingData.translationStep;
	}

	selectedEntityTransform->setPosition(newPosition);
}

void GizmoComponent::handleRotation(Window& window, Camera* camera)
{
	if (!selectedGizmoModel || !camera || !selectedEntity)
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

	glm::vec3 rayDirection = glm::normalize(getCursorToWorldRay(camera->getView(), camera->getProjection()));
	glm::vec3 rayOrigin = camera->getWorldPosition();
	glm::vec3 gizmoOrigin = selectedGizmoModel->getPosition();
	glm::vec3 gizmoUp = getLocalUpForGimbal(selectedGizmoModel->getComponentName());

	if (SceneNode* parent = selectedGizmoModel->getParent())
	{
		rayDirection = glm::inverse(parent->getTransform()) * glm::vec4(rayDirection, 0.0f);
		rayOrigin = glm::inverse(parent->getTransform()) * glm::vec4(rayOrigin, 1.0f);
	}

	glm::vec3 dragPosition = rayVsPlane(gizmoOrigin, gizmoUp, rayOrigin, rayDirection);
	if (glm::any(glm::isnan(dragPosition)))
	{
		return;
	}

	glm::vec3 dragToGimbalProjection = dragPosition - gizmoOrigin;
	const glm::vec2 dragToGimbalProjection2d = projectOnPlane(dragToGimbalProjection, gizmoUp);

	float dragAngle = Rotator::normalizeAngle(glm::degrees(glm::atan(dragToGimbalProjection2d.x, dragToGimbalProjection2d.y)));

	float deltaAngle = rotationClickAngle - dragAngle;

	// We need to handle the case when comparing two numbers that stand to the left and right from 0 degrees
	if (glm::abs(deltaAngle) > 180.0f)
	{
		if (deltaAngle < 0.0f)
		{
			deltaAngle = 360.0f - glm::abs(deltaAngle);
		}
		else
		{
			deltaAngle = 360.0f - glm::abs(deltaAngle);
			deltaAngle = -deltaAngle;
		}
	}

	if (selectedGizmoModel->getComponentName() == GizmoNames::Yaw)
	{
		deltaAngle = -deltaAngle;
	}

	// Sensitivity
	deltaAngle *= 0.1f;

	// Snapping
	const Euler gimbalAngle = getEulerAngleOfGimbal(selectedGizmoModel->getComponentName());

	snappingData.unsnappedRotator.rotate(deltaAngle, gimbalAngle);

	if (snappingData.snapToGrid)
	{
		float angleSnapped = snappingData.unsnappedRotator.get(gimbalAngle);
		angleSnapped = glm::round(angleSnapped / snappingData.rotationStep) * snappingData.rotationStep;

		selectedEntityTransform->setRotation(angleSnapped, gimbalAngle);
		selectedGizmoModel->setRotation(angleSnapped, gimbalAngle);
	}
	else
	{
		selectedEntityTransform->rotate(deltaAngle, gimbalAngle);
		selectedGizmoModel->rotate(deltaAngle, gimbalAngle);
	}

	window.setCursorPosition(lastCursorScreenPosition);
}

void GizmoComponent::handleScale(Window& window, Camera* camera)
{
	if (!selectedGizmoModel || !camera || !selectedEntity)
	{
		return;
	}

	if (SelectableComponent* selectableComponent = selectedEntity->findComponent<SelectableComponent>())
	{
		selectableComponent->setBoxVisible(false);
	}

	TransformComponent* selectedEntityTransform = selectedEntity->findComponent<TransformComponent>();
	if (!selectedEntityTransform)
	{
		return;
	}

	glm::vec3 newScale(0.0f);
	if (selectedGizmoModel->getComponentName() == GizmoNames::ScaleAll)
	{
		glm::vec2 cursorDragPosition = getCursorToDevice();
		glm::vec2 cursorDragOffset = (cursorDragPosition - clickCursorDevicePosition) * 2.0f;

		newScale = snappingData.unsnappedScale * (cursorDragOffset.x + cursorDragOffset.y + 1.0f);
	}
	else
	{
		// Convert all required vectors into gizmo's local space for the easier creation of the gizmo axis normals
		glm::vec3 rayOrigin = camera->getWorldPosition();
		glm::vec3 rayDirection = glm::normalize(getCursorToWorldRay(camera->getView(), camera->getProjection()));

		glm::vec3 axisPlaneNormal = glm::normalize(rayOrigin - gizmoClickPosition);

		const std::string selectedGizmoName = selectedGizmoModel->getComponentName();

		glm::vec3 dragPositionLocal = rayVsPlane(gizmoClickPosition, axisPlaneNormal, rayOrigin, rayDirection);
		if (glm::any(glm::isnan(dragPositionLocal)))
		{
			return;
		}

		// Determine whether the scaling must be negated or not
		glm::vec3 clickOffset = gizmoClickPosition - gizmoSelectPositionOrigin;
		glm::vec3 scaleOffset = dragPositionLocal - gizmoClickPosition;

		const int longestAxis = Vector::findLongestAxis(scaleOffset);
		if (clickOffset[longestAxis] < 0.0f)
		{
			scaleOffset = -scaleOffset;
		}

		scaleOffset.x = scaleOffset[longestAxis];
		scaleOffset.y = scaleOffset[longestAxis];
		scaleOffset.z = scaleOffset[longestAxis];

		newScale = snappingData.unsnappedScale + scaleOffset;

		const glm::vec3 gizmoCurrentScale = snappingData.unsnappedScale;
		if (selectedGizmoName == GizmoNames::ScaleUp || selectedGizmoName == GizmoNames::ScaleDown)
		{
			newScale.x = gizmoCurrentScale.x;
			newScale.z = gizmoCurrentScale.z;
		}
		else if (selectedGizmoName == GizmoNames::ScaleRight || selectedGizmoName == GizmoNames::ScaleLeft)
		{
			newScale.y = gizmoCurrentScale.y;
			newScale.z = gizmoCurrentScale.z;
		}
		else if (selectedGizmoName == GizmoNames::ScaleFront || selectedGizmoName == GizmoNames::ScaleBack)
		{
			newScale.x = gizmoCurrentScale.x;
			newScale.y = gizmoCurrentScale.y;
		}
	}
	
	snappingData.unsnappedScale = newScale;

	if (snappingData.snapToGrid)
	{
		glm::vec3 snappedScale = glm::round(snappingData.unsnappedScale / snappingData.scaleStep) * snappingData.scaleStep;
		selectedEntityTransform->setScale(snappedScale);
	}
	else
	{
		selectedEntityTransform->setScale(newScale);
	}

	window.setCursorPosition(lastCursorScreenPosition);
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

	int upAxis = Vector::findLongestAxis(glm::abs(normal));

	test[upAxis] = std::numeric_limits<float>::quiet_NaN();

	result.x = glm::isnan(test.x) ? test.y : test.x;
	result.y = (result.x == test.y) ? test.z : (glm::isnan(test.y) ? test.z : test.y);

	return result;
}
