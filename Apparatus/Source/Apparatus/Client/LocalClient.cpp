#include "LocalClient.h"

#include <sstream>

#include "GenericHumanController.h"
#include "../Apparatus.h"
#include "../Core/Level.h"
#include "../Core/Logger.h"
#include "../Core/AssetManager/Importer/TextureImporter.h"
#include "../Core/AssetManager/AssetManager.h"
#include "../Components/TransformComponent.h"
#include "../Components/ModelComponent.h"
#include "../Components/CameraComponent.h"
#include "../Components/LightComponent.h"
#include "../Server/LocalServer.h"
#include "../UI/Widget/NinePatchPanel.h"

LocalClient::LocalClient(Renderer* renderer, SpriteRenderer* spriteRenderer) :
	debugPrimitiveMaterial(nullptr),
	debugMeshBufferSize(1024 * 1024 * 32), // 32MB per mesh for debug primitives
	renderer(renderer),
	spriteRenderer(spriteRenderer),
	activeController(nullptr),
	defaultController(nullptr),
	uiContext(&inputHandler)
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
	if (AssetManager* assetManager = Apparatus::findEngineSystem<AssetManager>())
	{
		debugPrimitiveMaterial = assetManager->findAsset<Material>("Material_DebugPrimitive");
	}

	if (EventDispatcher* eventDispatcher = Apparatus::findEngineSystem<EventDispatcher>())
	{
		eventDispatcher->bind<LightComponentCreationEvent>(std::bind(&LocalClient::onLightCreation, this, std::placeholders::_1));
		eventDispatcher->bind<WindowResizeEvent>(std::bind(&LocalClient::onWindowResize, this, std::placeholders::_1));
	}

	// UI stuff
	uiContext.init();
	inputHandler.setUIContext(&uiContext);
	uiCamera.setOrthographic(0.0f, static_cast<float>(windowSize.x), 0.0f, static_cast<float>(windowSize.y), -1.0f, 1.0f);
}

void LocalClient::onLightCreation(std::shared_ptr<LightComponentCreationEvent> event)
{
	if (renderer)
	{
		if (event)
		{
			if (LightComponent* pointLightComponent = event->getLightComponent())
			{
				renderer->addLight(&pointLightComponent->getLight());
			}
		}
	}
}

void LocalClient::update(float dt)
{
	inputHandler.update();

	// Render debug primitives
	composeDebugPrimitiveRenderData();
	composeEntityRenderData();

	if (spriteRenderer)
	{
		spriteRenderer->setActiveCamera(&uiCamera);

		uiContext.refreshWidgetTree();
		uiContext.renderContext(spriteRenderer);
	}
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

void LocalClient::onWindowResize(std::shared_ptr<WindowResizeEvent> event)
{
	auto weakEvent = std::weak_ptr<WindowResizeEvent>(event);
	auto lockedEvent = weakEvent.lock();

	if (lockedEvent)
	{
		uiCamera.setOrthographic(0.0f, static_cast<float>(lockedEvent->getWindowSize().x), 0.0f, static_cast<float>(lockedEvent->getWindowSize().y), -1.0f, 1.0f);
	}
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

		std::vector<ModelVertex> vertices;
		std::vector<unsigned int> indices;

		for (unsigned int i = 0; i < debugPoints.size(); ++i)
		{
			const Point& debugPoint = debugPoints[i].first;
			const DebugPrimitiveData& primitiveData = debugPoints[i].second;

			ModelVertex vertex;

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

		std::vector<ModelVertex> vertices;
		std::vector<unsigned int> indices;

		for (unsigned int i = 0; i < debugLines.size(); ++i)
		{
			const Line& debugLine = debugLines[i].first;
			const DebugPrimitiveData& primitiveData = debugLines[i].second;

			ModelVertex vertex;

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

		std::vector<ModelVertex> vertices;
		std::vector<unsigned int> indices;

		for (unsigned int i = 0; i < debugBoxes.size(); ++i)
		{
			const Box& box = debugBoxes[i].first;
			const DebugPrimitiveData& primitiveData = debugBoxes[i].second;

			ModelVertex vertex;
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

void LocalClient::renderDebugPrimitives(const std::vector<ModelVertex>& vertices, const std::vector<unsigned int>& indices, std::map<float, Mesh*>& debugMeshCache, const std::string& assetNamePrefix, RenderMode renderMode, float drawSize)
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
		auto vao = std::make_unique<VertexArrayObject>();
		assert(vao);
		vao->setStride(sizeof(ModelVertex));
		vao->addAttribute(3);
		vao->addAttribute(2);
		vao->addAttribute(3);
		vao->addAttribute(4);

		// TODO: Take buffer size from somewhere else
		if (AssetManager* assetManager = Apparatus::findEngineSystem<AssetManager>())
		{
			mesh = assetManager->createAsset<Mesh>(meshName, std::move(vao), debugMeshBufferSize, debugMeshBufferSize);
		}

		debugMeshCache[drawSize] = mesh;
	}
	else
	{
		auto vertexBuffer = std::make_shared<VertexBuffer<ModelVertex>>();
		vertexBuffer->vertices = vertices;

		const size_t vertexBufferSize = mesh->getVertexBufferSize();
		const size_t indexBufferSize = mesh->getIndexBufferSize();
		const size_t indexDataSize = indices.size() * sizeof(unsigned int);
		const size_t vertexDataSize = vertices.size() * sizeof(ModelVertex);

		if (indices.size())
		{
			if (indexDataSize >= indexDataSize && vertexBufferSize >= vertexDataSize)
			{
				mesh->setSubData(vertexBuffer, indices);
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
				mesh->setSubData(vertexBuffer);
			}
			else
			{
				LOG("Buffer size exceeded for '" + meshName + "' mesh! Buffer size: " + std::to_string(vertexBufferSize) + " Data size: " + std::to_string(vertexDataSize), LogLevel::Error);
			}
		}

		renderer->push(mesh, debugPrimitiveMaterial, &cameraComponent->getCamera(), glm::mat4(1.0f), renderMode, drawSize);
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
					renderer->push(modelComponent->getModel(), getActiveCamera(), modelComponent->getTransform(), modelComponent->getDepthBufferLayer());
				}
			}
		}
	}
}
