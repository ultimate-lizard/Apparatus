#include "Apparatus.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Core/Level.h"
#include "Server/LocalServer.h"
#include "Client/LocalClient.h"
#include "Core/EntityRegistry.h"
#include "Core/AssetManager/AssetManager.h"
#include "Core/AssetManager/Importer/ModelImporter.h"

// TODO: These are probably temporary!
#include "Rendering/Model.h"
#include "Rendering/Shader.h"
#include "Rendering/Material.h"
#include "Components/TransformComponent.h"
#include "Components/MovementComponent.h"
#include "Components/CameraComponent.h"
#include "Components/ModelComponent.h"

Apparatus* Apparatus::apparatus;
std::vector<std::unique_ptr<EngineSystem>> Apparatus::engineSystems;
Window* Apparatus::globalWindowRef;

Apparatus::Apparatus(const std::string& gameTitle) :
	gameTitle(gameTitle),
	running(false)
{
	apparatus = this;
}

Apparatus& Apparatus::get()
{
	return *apparatus;
}

int Apparatus::launch(int argc, char** argv)
{
	// TODO: Handle args

	if (running)
	{
		return -1;
	}

	return initEngineInternal();
}

void Apparatus::quit()
{
	running = false;
}

std::vector<Client*> Apparatus::getClients()
{
	std::vector<Client*> result;

	for (std::unique_ptr<Client>& client : clients)
	{
		if (client)
		{
			result.push_back(client.get());
		}
	}

	return result;
}

LocalClient* Apparatus::getPrimaryLocalClient()
{
	for (std::unique_ptr<Client>& client : clients)
	{
		if (!client)
		{
			continue;
		}

		if (LocalClient* castedClient = dynamic_cast<LocalClient*>(client.get()))
		{
			return castedClient;
		}
	}

	return nullptr;
}

Level* Apparatus::getLevel()
{
	return level.get();
}

Window& Apparatus::getWindow()
{
	return *globalWindowRef;
}

void Apparatus::init()
{
 	window.getEventHandler()._bindQuitEvent(std::bind(&Apparatus::quit, this));
	
	// TODO: Load level sequence
	level = std::make_unique<Level>();
	if (level)
	{
		level->init();
	}
}

void Apparatus::createEngineSystems()
{
	createEngineSystem<EntityRegistry>();
	createEngineSystem<AssetManager>();
	createEngineSystem<EventDispatcher>();
	createEngineSystem<Renderer>(window);
	createEngineSystem<SpriteRenderer>();
}

void Apparatus::initEngineSystems()
{
	for (std::unique_ptr<EngineSystem>& system : engineSystems)
	{
		if (system)
		{
			system->init();
		}
	}
}

void Apparatus::uninitEngineSystems()
{
	for (std::unique_ptr<EngineSystem>& system : engineSystems)
	{
		if (system)
		{
			system->uninit();
		}
	}
}

int Apparatus::initEngineInternal()
{
	Logger::open("../Logs/");

	SDL_SetMainReady();
	if (SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Unable to initialize SDL!", nullptr);
		return 1;
	}

	if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Unable to initialize SDL Image!", nullptr);
		return 1;
	}

	if (!window.create(gameTitle))
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Unable to initialize Window!", nullptr);
		return 1;
	}

	globalWindowRef = &window;

	createEngineSystems();
	initEngineSystems();

	// SDL_GL_SetSwapInterval(1);

	_createAssets();
	_createEntityTemplates();

	init();

	if (server)
	{
		server->init();
	}

	for (std::unique_ptr<Client>& client : clients)
	{
		if (client)
		{
			client->init();
		}
	}

	if (server)
	{
		server->start();
	}

	for (std::unique_ptr<Client>& client : clients)
	{
		if (client)
		{
			client->onGameStart();
		}
	}

	// TODO: This is a default offline connection sequence. Find a better way and place to do it
	if (LocalClient* localClient = getPrimaryLocalClient())
	{
		if (LocalServer* localServer = getServer<LocalServer>())
		{
			ConnectionInfo info;
			info.client = localClient;
			localServer->connect(info);
		}
	}

	startGameLoop();

	return 0;
}

void Apparatus::_createAssets()
{
	if (AssetManager* assetManager = findEngineSystem<AssetManager>())
	{
		if (Shader* debugPrimitiveShader = assetManager->createAsset<Shader>("Shader_DebugPrimitive", "../Shaders/DebugPrimitive.vert", "../Shaders/DebugPrimitive.frag"))
		{
			if (Material* debugPrimitiveMaterial = assetManager->createAsset<Material>("Material_DebugPrimitive"))
			{
				debugPrimitiveMaterial->setShader(debugPrimitiveShader);
			}
		}

		if (Shader* solidColorShader = assetManager->createAsset<Shader>("Shader_SolidColorNoLight", "../Shaders/SolidColorNoLight.vert", "../Shaders/SolidColorNoLight.frag"))
		{
			if (ModelImporter* importer = assetManager->getImporter<ModelImporter>())
			{
				assetManager->createAsset("Model_DirectionalLight", std::move(importer->import(solidColorShader, "../Models/DirectionalLight.fbx")));
				assetManager->createAsset("Model_PointLight", std::move(importer->import(solidColorShader, "../Models/PointLight.fbx")));
				assetManager->createAsset("Model_SpotLight", std::move(importer->import(solidColorShader, "../Models/SpotLight.fbx")));
				assetManager->createAsset("Model_Error", std::move(importer->import(solidColorShader, "../Models/Error.fbx")));

				std::list<std::unique_ptr<Model>> gizmoModels = importer->importMultiple(solidColorShader, "../Models/Gizmo.fbx");
				for (std::unique_ptr<Model>& model : gizmoModels)
				{
					if (model)
					{
						// TODO: Do something about this!
						std::string modelName = model->getAssetName();
						assetManager->createAsset(modelName, std::move(model));
					}
				}
			}
		}

		if (Shader* modelShader = assetManager->createAsset<Shader>("Shader_DefaultModel", "../Shaders/Model.vert", "../Shaders/Model.frag"))
		{
			modelShader->createUniformBufferObject(4096, "LightUniformBuffer", 0);

			if (ModelImporter* importer = assetManager->getImporter<ModelImporter>())
			{
				//assetManager.createAsset(std::move(importer->import("Model_Scene", modelShader, "../Models/scene.fbx")));
				assetManager->createAsset("Model_NewScene", std::move(importer->import(modelShader, "../Models/NewScene.fbx")));
				assetManager->createAsset("Model_Makarov", std::move(importer->import(modelShader, "../Models/makarov.fbx")));

				std::list<std::unique_ptr<Model>> dungeonModels = importer->importMultiple(modelShader, "../Models/Dungeon.fbx");
				for (std::unique_ptr<Model>& model : dungeonModels)
				{
					if (model)
					{
						std::string modelName = model->getAssetName();
						assetManager->createAsset(modelName, std::move(model));
					}
				}
			}
		}
	}
}

void Apparatus::_createEntityTemplates()
{
	if (EntityRegistry* entityRegistry = findEngineSystem<EntityRegistry>())
	{
		if (Entity* playerEntity = entityRegistry->createEntityTemplate("Player"))
		{
			if (auto transformComponent = entityRegistry->createComponent<TransformComponent>(playerEntity))
			{
				if (auto cameraComponent = entityRegistry->createComponent<CameraComponent>(playerEntity))
				{
					cameraComponent->setParent(transformComponent);
					cameraComponent->getCamera().setPerspective(90.0f, 800.0f / 600.0f, 0.01f, 1000.0f);
				}
			}

			auto movementComponent = entityRegistry->createComponent<MovementComponent>(playerEntity);
		}

		if (Entity* modelEntity = entityRegistry->createEntityTemplate("ModelEntity"))
		{
			auto transformComponent = entityRegistry->createComponent<TransformComponent>(modelEntity);
			if (auto modelComponent = entityRegistry->createComponent<ModelComponent>(modelEntity))
			{
				if (AssetManager* assetManager = findEngineSystem<AssetManager>())
				{
					modelComponent->setModel(assetManager->findAsset<Model>("Model_Error"));
					modelComponent->setParent(transformComponent);
				}
			}
		}
	}
}

void Apparatus::startGameLoop()
{
	running = true;

	unsigned long long currentFrame = SDL_GetPerformanceCounter();
	unsigned long long lastFrame = 0;

	// TODO: Should be somewhere else + from args
	const unsigned int tickRate = 170;

	const float fixedDt = 1.0f / static_cast<float>(tickRate);
	
	float accumulator = 0.0f;

	while (running)
	{
		if (window.isWrapCursorEnabled())
		{
			// TODO: Toggle wrap
			const glm::ivec2 windowSize = window.getWindowSize();
			window.setCursorPosition({ windowSize.x / 2, windowSize.y / 2 });
		}
		
		lastFrame = currentFrame;
		currentFrame = SDL_GetPerformanceCounter();

		window.handleEvents();

		// Update logic
		while (accumulator == 0 || accumulator >= fixedDt)
		{
			if (server)
			{
				server->update(fixedDt);
			}

			accumulator -= fixedDt;
		}

		float dt = (currentFrame - lastFrame) / static_cast<float>(SDL_GetPerformanceFrequency());
		accumulator += dt;

		// Update clients
		for (std::unique_ptr<Client>& client : clients)
		{
			if (client)
			{
				client->update(dt);
			}
		}

		// Renderer
		if (Renderer* renderer = findEngineSystem<Renderer>())
		{
			glClear(GL_COLOR_BUFFER_BIT);

			renderer->render();
		}

		if (SpriteRenderer* spriteRenderer = findEngineSystem<SpriteRenderer>())
		{
			spriteRenderer->render();
		}

		window.swap();

		// TODO: Shutdown
	}

	uninitEngineSystems();

	Logger::close();
}
