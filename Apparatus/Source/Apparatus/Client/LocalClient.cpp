#include "LocalClient.h"

#include "../Core/Logger.h"
#include "../Apparatus.h"
#include "../Components/TransformComponent.h"
#include "../Components/ModelComponent.h"
#include "../Components/CameraComponent.h"
#include "../Components/LightComponent.h"
#include "GenericHumanController.h"
#include "../Level.h"
#include "../Server/LocalServer.h"

LocalClient::LocalClient(Renderer* renderer) :
	debugMeshBufferSize(1024 * 1024 * 32), // 32MB per mesh for debug primitives
	renderer(renderer),
	activeController(nullptr),
	defaultController(nullptr)
{
}

void LocalClient::init()
{
	Apparatus& app = Apparatus::get();
	Window& window = app.getWindow();

	window.getEventHandler().addInputHandler(&inputHandler);

	defaultController = createController<GenericHumanController>("DefaultController", *this);

	if (std::unique_ptr<Controller>& controller = controllers[0])
	{
		setActiveController(controller.get());
	}

	glm::ivec2 windowSize = window.getWindowSize();
	viewport.setSize(windowSize);
	
	// Debug stuff
	Material* debugPrimitiveMaterial = Apparatus::getAssetManager().findAsset<Material>("Material_DebugPrimitive");
	debugPrimitiveMaterialInstance = debugPrimitiveMaterial->createMaterialInstance();
}

void LocalClient::update(float dt)
{
	inputHandler.update();

	// Render debug primitives
	composeDebugPrimitiveRenderData();
	composeEntityRenderData();
}

Viewport* LocalClient::getViewport()
{
	return &viewport;
}

Camera* LocalClient::getActiveCamera()
{
	if (activeEntity)
	{
		if (CameraComponent* cameraComponent = activeEntity->findComponent<CameraComponent>())
		{
			return &cameraComponent->getCamera();
		}
	}

	return nullptr;
}

InputHandler& LocalClient::getInputHandler()
{
	return inputHandler;
}

void LocalClient::setActiveController(Controller* controller)
{
	// Deactivate current
	if (activeController)
	{
		activeController->onDeactivate();
	}

	activeController = controller;

	// Activate new
	if (activeController)
	{
		inputHandler.clearAllBindings();
		activeController->setupInput();
		activeController->setControlEntity(getActiveEntity());
		activeController->onActivate();
	}

	onActiveControllerChanged();
}

Controller* LocalClient::getActiveController()
{
	return activeController;
}

Controller* LocalClient::getDefaultController()
{
	return defaultController;
}

void LocalClient::composeDebugPrimitiveRenderData()
{
	Apparatus& app = Apparatus::get();
	LocalServer* localServer = app.getServer<LocalServer>();

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

			Vertex vertex;

			vertex.position = debugPoint.position;
			vertex.color = primitiveData.color;
			vertices.push_back(vertex);

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

			Vertex vertex;

			vertex.position = debugLine.start;
			vertex.color = primitiveData.color;
			vertices.push_back(vertex);

			vertex.position = debugLine.end;
			vertex.color = primitiveData.color;
			vertices.push_back(vertex);

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
	if (!activeEntity)
	{
		return;
	}

	CameraComponent* cameraComponent = activeEntity->findComponent<CameraComponent>();

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

	const std::string meshName = "Mesh_" + assetNamePrefix + std::to_string(drawSize);

	// If no mesh cached, create a mesh asset for that purpose, and for it to be reused later
	if (!mesh)
	{
		// TODO: Take buffer size from somewhere else
		mesh = Apparatus::getAssetManager().createAsset<Mesh>(meshName, debugMeshBufferSize, debugMeshBufferSize);
		debugMeshCache[drawSize] = mesh;
	}
	else
	{
		const size_t vertexBufferSize = mesh->getVertexBufferSize();
		const size_t indexBufferSize = mesh->getIndexBufferSize();
		const size_t indexDataSize = indices.size() * sizeof(unsigned int);
		const size_t vertexDataSize = vertices.size() * sizeof(Vertex);

		if (indices.size())
		{
			if (indexDataSize >= indexDataSize && vertexBufferSize >= vertexDataSize)
			{
				mesh->setSubData(vertices, indices);
			}
			else
			{
				if (indexBufferSize < indexDataSize)
				{
					LOG("Buffer size exceeded for '" + meshName + "' mesh! Buffer size: " + std::to_string(indexBufferSize) + " Data size: " + std::to_string(indexDataSize), LogLevel::Error);
				}
				
				if (vertexBufferSize < vertexDataSize)
				{
					LOG("Buffer size exceeded for '" + meshName + "' mesh! Buffer size: " + std::to_string(vertexBufferSize) + " Data size: " + std::to_string(vertexDataSize), LogLevel::Error);
				}
			}
		}
		else
		{
			if (vertexBufferSize >= vertexDataSize)
			{
				mesh->setSubData(vertices);
			}
			else
			{
				LOG("Buffer size exceeded for '" + meshName + "' mesh! Buffer size: " + std::to_string(vertexBufferSize) + " Data size: " + std::to_string(vertexDataSize), LogLevel::Error);
			}
		}

		renderer->push(mesh, debugPrimitiveMaterialInstance.get(), &cameraComponent->getCamera(), glm::mat4(1.0f), renderMode, drawSize);
	}
}

void LocalClient::onActiveEntitySet()
{
	if (activeEntity)
	{
		if (CameraComponent* cameraComponent = activeEntity->findComponent<CameraComponent>())
		{
			if (renderer)
			{
				renderer->setActiveCamera(&cameraComponent->getCamera());
			}
		}

		if (activeController)
		{
			activeController->setControlEntity(getActiveEntity());
		}
	}
}

void LocalClient::composeEntityRenderData()
{
	if (!renderer)
	{
		return;
	}

	Apparatus& app = Apparatus::get();
	if (Level* level = app.getLevel())
	{
		for (Entity* entity : level->getAllEntities())
		{
			if (!entity)
			{
				continue;
			}

			for (ModelComponent* modelComponent : entity->getAllComponentsOfClass<ModelComponent>())
			{
				if (modelComponent->isVisible())
				{
					renderer->push(modelComponent->getModelInstance(), getActiveCamera(), modelComponent->getTransform());
				}
			}
		}
	}
}
