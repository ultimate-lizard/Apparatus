#include "LocalClient.h"

#include "../Core/Logger.h"
#include "../Apparatus.h"
#include "HumanPlayerController.h"
#include "EditorController.h"
#include "../Server/LocalServer.h"
#include "../Components/TransformComponent.h"
#include "../Components/ModelComponent.h"
#include "../Components/CameraComponent.h"

LocalClient::LocalClient(Apparatus* apparatus) :
	Client(apparatus),
	localServer(nullptr),
	controlEntity(nullptr),
	activeController(nullptr),
	debugModeEnabled(false)
{
}

void LocalClient::init()
{
	if (!apparatus)
	{
		return;
	}

	WindowEventHandler& inputReader = apparatus->getInputReader();
	inputReader.addInputHandler(&inputHandler);

	// TODO: How do we determine which controller to use?
	auto humanController = std::make_unique<HumanPlayerController>(this);
	auto debugController = std::make_unique<EditorController>(this);
	controllers.push_back(std::move(humanController));
	controllers.push_back(std::move(debugController));

	localServer = apparatus->getServer<LocalServer>();
	if (!localServer)
	{
		return;
	}

	std::vector<std::unique_ptr<Entity>>& entities = localServer->getAllEntities();
	if (entities.size() > 0)
	{
		controlEntity = entities[0].get();
	}

	for (std::unique_ptr<Controller>& controller : controllers)
	{
		controller->init();
	}

	if (std::unique_ptr<Controller>& controller = controllers[0])
	{
		setActiveController(controller.get());
		activeController->setControlEntity(controlEntity);
	}

	glm::ivec2 windowSize = apparatus->getWindowSize();
	viewport.setSize(windowSize);
	
	// Debug stuff
	Material* debugPrimitiveMaterial = apparatus->getResourceManager().findResource<Material>("Material_DebugPrimitive");
	debugPrimitiveMaterialInstance = debugPrimitiveMaterial->createMaterialInstance();
}

void LocalClient::update(float dt)
{
	Renderer* renderer = nullptr;

	if (apparatus)
	{
		renderer = apparatus->getRenderer();
	}

	if (!renderer || !controlEntity || !localServer)
	{
		return;
	}

	inputHandler.update();

	// Render entities
	for (const std::unique_ptr<Entity>& entity : localServer->getAllEntities())
	{
		if (!entity)
		{
			continue;
		}

		if (entity.get() != controlEntity)
		{
			TransformComponent* transformComponent = entity->findComponent<TransformComponent>();

			if (CameraComponent* cameraComponent = controlEntity->findComponent<CameraComponent>())
			{
				// Debug models
				if (ModelInstance* debugModelInstance = transformComponent->getDebugModelInstance())
				{
					renderer->push(debugModelInstance, &cameraComponent->getCamera(), transformComponent->getTransform());
				}

				std::vector<ModelComponent*> modelComponents = entity->getAllComponentsOfClass<ModelComponent>();
				for (ModelComponent* modelComponent : modelComponents)
				{
					// TODO: Frustum check. Probably need to put all the visible to the client entities into a separate
					// vector inside the client
					renderer->push(modelComponent->getModelInstance(), &cameraComponent->getCamera(), modelComponent->getTransform());
				}
			}
		}
	}

	// Render debug primitives
	composeDebugPrimitiveData();
}

Viewport* LocalClient::getViewport()
{
	return &viewport;
}

Camera* LocalClient::getActiveCamera()
{
	if (!controlEntity)
	{
		return nullptr;
	}

	if (CameraComponent* cameraComponent = controlEntity->findComponent<CameraComponent>())
	{
		return &cameraComponent->getCamera();
	}

	return nullptr;
}

void LocalClient::setControlledEntity(Entity* entity)
{
	controlEntity = entity;
}

InputHandler& LocalClient::getInputHandler()
{
	return inputHandler;
}

bool LocalClient::isDebugModeEnabled() const
{
	return false;
}

void LocalClient::setDebugModeEnabled(bool enabled)
{
	debugModeEnabled = enabled;
	if (enabled)
	{
		setActiveController(controllers[1].get());
	}
	else
	{
		setActiveController(controllers[0].get());
	}

	if (activeController)
	{
		activeController->setControlEntity(controlEntity);
	}
}

void LocalClient::setActiveController(Controller* controller)
{
	activeController = controller;
	if (activeController)
	{
		inputHandler.clearAllBindings();
		activeController->onActivate();
		activeController->setupInput();
	}
}

void LocalClient::composeDebugPrimitiveData()
{
	if (!localServer)
	{
		return;
	}

	const DebugPrimitives& debugPrimitives = localServer->getDebugPrimitives();

	for (auto debugPointMapIter : debugPrimitives.debugPoints)
	{
		float drawSize = debugPointMapIter.first;

		const std::vector<std::pair<Point, DebugPrimitiveData>>& debugPoints = debugPointMapIter.second;

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		for (unsigned int i = 0; i < debugPoints.size(); ++i)
		{
			const Point& debugPoint = debugPoints[i].first;
			const DebugPrimitiveData& primitiveData = debugPoints[i].second;

			vertices.push_back({ debugPoint.position, {}, primitiveData.color });

			indices.push_back(0 + i * primitiveData.vertexCount);
		}

		renderDebugPrimitives(vertices, indices, cachedDebugPointMeshes, "DebugPoints_", RenderMode::Points, drawSize);
	}

	for (auto debugLineMapIter : debugPrimitives.debugLines)
	{
		float drawSize = debugLineMapIter.first;
		const std::vector<std::pair<Line, DebugPrimitiveData>>& debugLines = debugLineMapIter.second;

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		for (unsigned int i = 0; i < debugLines.size(); ++i)
		{
			const Line& debugLine = debugLines[i].first;
			const DebugPrimitiveData& primitiveData = debugLines[i].second;

			vertices.push_back({ debugLine.start, {}, primitiveData.color });
			vertices.push_back({ debugLine.end, {}, primitiveData.color });

			indices.push_back(0 + i * primitiveData.vertexCount);
			indices.push_back(1 + i * primitiveData.vertexCount);
		}

		renderDebugPrimitives(vertices, indices, cachedDebugLineMeshes, "DebugLines_", RenderMode::Lines, drawSize);
	}

	for (auto debugBoxMapIter : debugPrimitives.debugBoxes)
	{
		float drawSize = debugBoxMapIter.first;
		const std::vector<std::pair<Box, DebugPrimitiveData>>& debugBoxes = debugBoxMapIter.second;

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		for (unsigned int i = 0; i < debugBoxes.size(); ++i)
		{
			const Box& box = debugBoxes[i].first;
			const DebugPrimitiveData& primitiveData = debugBoxes[i].second;

			Vertex vertex;
			vertex.color = primitiveData.color;

			vertex.position.x = -box.size.x / 2.0f + box.position.x;
			vertex.position.y = box.size.y / 2.0f + box.position.y;
			vertex.position.z = box.size.z / 2.0f + box.position.z;
			vertices.push_back(vertex);

			vertex.position.x = box.size.x / 2.0f + box.position.x;
			vertex.position.y = box.size.y / 2.0f + box.position.y;
			vertex.position.z = box.size.z / 2.0f + box.position.z;
			vertices.push_back(vertex);

			vertex.position.x = -box.size.x / 2.0f + box.position.x;
			vertex.position.y = -box.size.y / 2.0f + box.position.y;
			vertex.position.z = box.size.z / 2.0f + box.position.z;
			vertices.push_back(vertex);

			vertex.position.x = box.size.x / 2.0f + box.position.x;
			vertex.position.y = -box.size.y / 2.0f + box.position.y;
			vertex.position.z = box.size.z / 2.0f + box.position.z;
			vertices.push_back(vertex);

			vertex.position.x = -box.size.x / 2.0f + box.position.x;
			vertex.position.y = box.size.y / 2.0f + box.position.y;
			vertex.position.z = -box.size.z / 2.0f + box.position.z;
			vertices.push_back(vertex);

			vertex.position.x = box.size.x / 2.0f + box.position.x;
			vertex.position.y = box.size.y / 2.0f + box.position.y;
			vertex.position.z = -box.size.z / 2.0f + box.position.z;
			vertices.push_back(vertex);

			vertex.position.x = -box.size.x / 2.0f + box.position.x;
			vertex.position.y = -box.size.y / 2.0f + box.position.y;
			vertex.position.z = -box.size.z / 2.0f + box.position.z;
			vertices.push_back(vertex);

			vertex.position.x = box.size.x / 2.0f + box.position.x;
			vertex.position.y = -box.size.y / 2.0f + box.position.y;
			vertex.position.z = -box.size.z / 2.0f + box.position.z;
			vertices.push_back(vertex);

			indices.push_back(0 + (primitiveData.vertexCount * i));
			indices.push_back(1 + (primitiveData.vertexCount * i));
			indices.push_back(0 + (primitiveData.vertexCount * i));
			indices.push_back(2 + (primitiveData.vertexCount * i));
			indices.push_back(2 + (primitiveData.vertexCount * i));
			indices.push_back(3 + (primitiveData.vertexCount * i));
			indices.push_back(1 + (primitiveData.vertexCount * i));
			indices.push_back(3 + (primitiveData.vertexCount * i));
			indices.push_back(0 + (primitiveData.vertexCount * i));
			indices.push_back(4 + (primitiveData.vertexCount * i));
			indices.push_back(2 + (primitiveData.vertexCount * i));
			indices.push_back(6 + (primitiveData.vertexCount * i));
			indices.push_back(1 + (primitiveData.vertexCount * i));
			indices.push_back(5 + (primitiveData.vertexCount * i));
			indices.push_back(3 + (primitiveData.vertexCount * i));
			indices.push_back(7 + (primitiveData.vertexCount * i));
			indices.push_back(4 + (primitiveData.vertexCount * i));
			indices.push_back(5 + (primitiveData.vertexCount * i));
			indices.push_back(4 + (primitiveData.vertexCount * i));
			indices.push_back(6 + (primitiveData.vertexCount * i));
			indices.push_back(6 + (primitiveData.vertexCount * i));
			indices.push_back(7 + (primitiveData.vertexCount * i));
			indices.push_back(5 + (primitiveData.vertexCount * i));
			indices.push_back(7 + (primitiveData.vertexCount * i));
		}

		renderDebugPrimitives(vertices, indices, cachedDebugBoxMeshes, "DebugBoxes_", RenderMode::Lines, drawSize);
	}
}

void LocalClient::renderDebugPrimitives(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, std::map<float, Mesh*>& debugMeshCache, const std::string& assetNamePrefix, RenderMode renderMode, float drawSize)
{
	if (!apparatus || !controlEntity)
	{
		return;
	}

	Renderer* renderer = apparatus->getRenderer();
	CameraComponent* cameraComponent = controlEntity->findComponent<CameraComponent>();

	if (!renderer || !cameraComponent)
	{
		return;
	}

	if (!vertices.size())
	{
		return;
	}

	// Look for corresponding to the draw size mesh
	Mesh* mesh = debugMeshCache[drawSize];

	// If no mesh cached, create a mesh asset for that purpose, and for it to be reused later
	if (!mesh)
	{
		ResourceManager& resourceManager = apparatus->getResourceManager();
		// TODO: Take buffer size from somewhere else
		mesh = resourceManager.createResource<Mesh>(assetNamePrefix + std::to_string(drawSize), 1024 * 1024);
		debugMeshCache[drawSize] = mesh;
	}
	else
	{
		if (indices.size())
		{
			mesh->setSubData(vertices, indices);
		}
		else
		{
			mesh->setSubData(vertices);
		}

		renderer->push(mesh, debugPrimitiveMaterialInstance.get(), &cameraComponent->getCamera(), glm::mat4(1.0f), renderMode, drawSize);
	}
}
