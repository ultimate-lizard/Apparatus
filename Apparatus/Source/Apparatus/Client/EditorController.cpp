#include "EditorController.h"

#include "../Apparatus.h"
#include "../Core/Logger.h"
#include "../Server/LocalServer.h"
#include "../Components/CameraComponent.h"
#include "../Components/ModelComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/SelectableComponent.h"
#include "InputHandler.h"
#include "LocalClient.h"

EditorController::EditorController(LocalClient* localClient) :
	HumanControllerBase(localClient),
	selectedEntity(nullptr)
{
}

void EditorController::onActivate()
{
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
		inputHandler->bindKeyAction("ToggleDebugMode", KeyEventType::Press, std::bind(&EditorController::toggleDebugMode, this));
		inputHandler->bindKeyAction("Select", KeyEventType::Press, std::bind(&EditorController::select, this));
		inputHandler->bindAxisAction("MoveZ", std::bind(&EditorController::moveZ, this, std::placeholders::_1));
		inputHandler->bindAxisAction("MoveX", std::bind(&EditorController::moveX, this, std::placeholders::_1));
		inputHandler->bindAxisAction("RotateRoll", std::bind(&EditorController::rotateRoll, this, std::placeholders::_1));
	}
}

void EditorController::toggleDebugMode()
{
	if (localClient)
	{
		localClient->setDebugModeEnabled(false);
	}

	LOG("Free camera mode", LogLevel::Info);
}

void EditorController::select()
{
	if (localClient)
	{
		if (Apparatus* apparatus = localClient->getApparatus())
		{
			if (LocalServer* localServer = apparatus->getServer<LocalServer>())
			{
				if (CameraComponent* cameraComponent = controlledEntity->findComponent<CameraComponent>())
				{
					int x = 0;
					int y = 0;
					SDL_GetMouseState(&x, &y);


					glm::vec2 windowSize = localClient->getApparatus()->getWindowSize();

					float normalX = (2.0f * x) / windowSize.x - 1.0f;
					float normalY = 1.0f - (2.0f * y) / windowSize.y;
					float z = 1.0f;

					glm::vec4 rayClip(normalX, normalY, -1.0f, 1.0f);

					Camera& camera = cameraComponent->getCamera();
					glm::mat4 projection = camera.getProjection();

					glm::vec4 rayEye = glm::inverse(projection) * rayClip;
					rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

					glm::vec3 rayWorld = glm::normalize(glm::inverse(camera.getView()) * rayEye);

					glm::vec3 start = cameraComponent->getCamera().getDerivedPosition();
					std::vector<RayTraceResult> traceResult = localServer->traceRay(rayWorld, start, 50.0f);
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

			LocalServer::drawLine({ transformComponent->getDerivedPosition(), transformComponent->getDerivedPosition() + (forward * 100000.0f) }, { 1.0f, 0.0f, 1.0f, 1.0f }, 5.0f);
			std::string str = std::to_string(transformComponent->getPosition().x) + " " + std::to_string(transformComponent->getPosition().z);
			LOG(str, LogLevel::Trace);
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

			LocalServer::drawLine({ transformComponent->getDerivedPosition(), transformComponent->getDerivedPosition() + (offset * 100000.0f) }, { 1.0f, 0.0f, 1.0f, 1.0f }, 5.0f);
			std::string str = std::to_string(transformComponent->getPosition().x) + " " + std::to_string(transformComponent->getPosition().z);
			LOG(str, LogLevel::Trace);
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

void EditorController::selectEntity(Entity* entity)
{
	if (entity == selectedEntity)
	{
		return;
	}

	if (selectedEntity)
	{
		if (auto modelComponent = selectedEntity->findComponent<ModelComponent>())
		{
			if (ModelInstance* modelInstance = modelComponent->getModelInstance())
			{
				if (MaterialInstance* materialInstance = modelInstance->getMaterialInstance(0))
				{
					materialInstance->color = { 1.0f, 0.0f, 0.0f, 0.5f };
					materialInstance->selected = false;
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
		if (auto modelComponent = selectedEntity->findComponent<ModelComponent>())
		{
			if (ModelInstance* modelInstance = modelComponent->getModelInstance())
			{
				if (MaterialInstance* materialInstance = modelInstance->getMaterialInstance(0))
				{
					materialInstance->color = { 1.0f, 0.0f, 0.0f, 0.5f };
					materialInstance->selected = true;
				}
			}
		}

		if (auto selectableComponent = selectedEntity->findComponent<SelectableComponent>())
		{
			selectableComponent->setSelected(true);
		}
	}
}
