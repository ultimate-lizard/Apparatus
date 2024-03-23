#include "EditorController.h"

#include <Apparatus/Apparatus.h>
#include <Apparatus/Client/LocalClient.h>
#include <Apparatus/Components/CameraComponent.h>
#include <Apparatus/Components/TransformComponent.h>
#include <Apparatus/Components/ModelComponent.h>
#include <Apparatus/Components/MovementComponent.h>
#include <Apparatus/Server/Entity.h>
#include <Apparatus/Server/LocalServer.h>
#include <Apparatus/Util/DebugPrimitive.h>

#include "../Components/SelectableComponent.h"

EditorController::EditorController(LocalClient* localClient) :
	HumanControllerBase(localClient),
	selectedEntity(nullptr)
{
	assignDefaultObjectName();
}

void EditorController::assignDefaultObjectName()
{
	setObjectName("EditorController");
}

void EditorController::onActivate()
{
	HumanControllerBase::onActivate();
	
	if (localClient)
	{
		if (Apparatus* apparatus = localClient->getApparatus())
		{
			apparatus->setCursorVisibleEnabled(true);
		}
	}
}

void EditorController::setupInput()
{
	if (inputHandler)
	{
		inputHandler->bindKeyAction("Fire", KeyEventType::Press, std::bind(&EditorController::select, this));
		inputHandler->bindKeyAction("AltFire", KeyEventType::Press, std::bind(&EditorController::deselect, this));
		inputHandler->bindKeyAction("OpenMainMenu", KeyEventType::Press, std::bind(&EditorController::deselect, this));

		inputHandler->bindAxisAction("MoveZ", std::bind(&EditorController::moveZ, this, std::placeholders::_1));
		inputHandler->bindAxisAction("MoveX", std::bind(&EditorController::moveX, this, std::placeholders::_1));

		inputHandler->bindAxisAction("RotateRoll", std::bind(&EditorController::rotateRoll, this, std::placeholders::_1));
	}
}

void EditorController::select()
{
	if (!localClient || !controlledEntity)
	{
		return;
	}

	if (Apparatus* apparatus = localClient->getApparatus())
	{
		if (LocalServer* localServer = apparatus->getServer<LocalServer>())
		{
			if (CameraComponent* cameraComponent = controlledEntity->findComponent<CameraComponent>())
			{
				Camera& camera = cameraComponent->getCamera();

				glm::vec3 rayWorld = localServer->getCursorToWorldRay(camera.getView(), camera.getProjection());
				glm::vec3 start = camera.getDerivedPosition();

				std::vector<RayTraceResult> traceResult = localServer->traceRay(start, rayWorld, 50.0f, DetectionType::Visibility);
				if (traceResult.size())
				{
					selectEntity(traceResult[0].entity);
				}
				else
				{
					selectEntity(nullptr);
				}
			}
		}
	}
}

void EditorController::deselect()
{
	selectEntity(nullptr);
}

void EditorController::moveZ(float value)
{
	if (selectedEntity)
	{
		if (auto transformComponent = selectedEntity->findComponent<TransformComponent>())
		{
			glm::vec3 forward = transformComponent->getLocalForward();
			glm::vec3 offset = forward * value * 0.0025f;
			transformComponent->offsetPosition(offset);

			LocalServer::drawDebugLine({ transformComponent->getDerivedPosition(), transformComponent->getDerivedPosition() + (forward * 100000.0f) }, { 1.0f, 0.0f, 1.0f, 1.0f }, 5.0f);
			std::string str = std::to_string(transformComponent->getPosition().x) + " " + std::to_string(transformComponent->getPosition().z);
		}
	}
}

void EditorController::moveX(float value)
{
	if (selectedEntity)
	{
		if (auto transformComponent = selectedEntity->findComponent<TransformComponent>())
		{
			glm::vec3 right = transformComponent->getLocalRight();
			glm::vec3 offset = right * value * 0.0025f;
			transformComponent->offsetPosition(offset);

			LocalServer::drawDebugLine({ transformComponent->getDerivedPosition(), transformComponent->getDerivedPosition() + (offset * 100000.0f) }, { 1.0f, 0.0f, 1.0f, 1.0f }, 5.0f);
			std::string str = std::to_string(transformComponent->getPosition().x) + " " + std::to_string(transformComponent->getPosition().z);
		}
	}
}

void EditorController::rotateRoll(float value)
{
	if (selectedEntity)
	{
		if (auto transformComponent = selectedEntity->findComponent<TransformComponent>())
		{
			transformComponent->rotate(value * 0.025f, Euler::Yaw);
		}
	}
}

// TODO: Refactor
void EditorController::selectEntity(Entity* entity)
{
	if (entity == selectedEntity)
	{
		return;
	}

	if (selectedEntity)
	{
		std::vector<ModelComponent*> modelComponents = selectedEntity->getAllComponentsOfClass<ModelComponent>();
		
		for (ModelComponent* modelComponent : modelComponents)
		{
			if (!modelComponent)
			{
				continue;
			}

			if (ModelInstance* modelInstance = modelComponent->getModelInstance())
			{
				if (Model* model = modelInstance->getModel())
				{
					const size_t materialsNum = model->getMaterials().size();
					for (size_t i = 0; i < materialsNum; ++i)
					{
						if (MaterialInstance* materialInstance = modelInstance->getMaterialInstance(i))
						{
							MaterialParameters& params = materialInstance->getMaterialParameters();

							params.setVec4("selectionColor", { 1.0f, 0.0f, 0.0f, 1.0f });
							params.setBool("selected", false);
						}
					}
				}
			}
		}

		if (auto selectableComponent = selectedEntity->findComponent<SelectableComponent>())
		{
			selectableComponent->setSelected(false);
		}
	}

	selectedEntity = entity;

	if (selectedEntity)
	{
		std::vector<ModelComponent*> modelComponents = selectedEntity->getAllComponentsOfClass<ModelComponent>();

		for (ModelComponent* modelComponent : modelComponents)
		{
			if (!modelComponent)
			{
				continue;
			}

			if (ModelInstance* modelInstance = modelComponent->getModelInstance())
			{
				if (Model* model = modelInstance->getModel())
				{
					const size_t materialsNum = model->getMaterials().size();
					for (size_t i = 0; i < materialsNum; ++i)
					{
						if (MaterialInstance* materialInstance = modelInstance->getMaterialInstance(i))
						{
							MaterialParameters& params = materialInstance->getMaterialParameters();

							params.setVec4("selectionColor", { 1.0f, 0.0f, 0.0f, 1.0f });
							params.setBool("selected", true);
						}
					}
				}
			}
		}

		if (auto selectableComponent = selectedEntity->findComponent<SelectableComponent>())
		{
			selectableComponent->setSelected(true);
		}
	}
}
