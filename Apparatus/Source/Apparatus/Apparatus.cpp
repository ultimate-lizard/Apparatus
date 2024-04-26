#include "Apparatus.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Level.h"
#include "Core/ModelImporter.h"
#include "Server/LocalServer.h"
#include "Client/LocalClient.h"

// TODO: These are probably temporary!
#include "Rendering/Model.h"
#include "Rendering/Mesh.h"
#include "Rendering/Shader.h"
#include "Rendering/Material.h"
#include "Components/TransformComponent.h"
#include "Components/MovementComponent.h"
#include "Components/CameraComponent.h"
#include "Components/ModelComponent.h"

Apparatus* Apparatus::apparatus;
Apparatus::ApparatusGlobalInstance Apparatus::globalState;

Apparatus::Apparatus(const std::string& gameTitle) :
	gameTitle(gameTitle),
	running(false)
{
	apparatus = this;

	globalState.entityRegistry = &entityRegistry;
	globalState.assetManager = &assetManager;
	globalState.eventDispatcher = &eventDispatcher;
	globalState.window = &window;
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

EntityRegistry& Apparatus::getEntityRegistry()
{
	return *globalState.entityRegistry;
}

AssetManager& Apparatus::getAssetManager()
{
	return *globalState.assetManager;
}

EventDispatcher& Apparatus::getEventDispatcher()
{
	return *globalState.eventDispatcher;
}

Window& Apparatus::getWindow()
{
	return *globalState.window;
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

Renderer* Apparatus::getRenderer()
{
	return renderer.get();
}

int Apparatus::initEngineInternal()
{
	Logger::open("../Logs/");

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

	renderer = std::make_unique<Renderer>(window);
	if (renderer)
	{
		renderer->init();
	}
	else
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Unable to initialize Renderer!", nullptr);
		return 1;
	}

	// SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "0");

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
	if (Shader* debugPrimitiveShader = assetManager.createAsset<Shader>("Shader_DebugPrimitive", "../Shaders/DebugPrimitive.vert", "../Shaders/DebugPrimitive.frag"))
	{
		if (Material* debugPrimitiveMaterial = assetManager.createAsset<Material>("Material_DebugPrimitive"))
		{
			debugPrimitiveMaterial->setShader(debugPrimitiveShader);
		}
	}

	if (Shader* solidColorShader = assetManager.createAsset<Shader>("Shader_SolidColorNoLight", "../Shaders/SolidColorNoLight.vert", "../Shaders/SolidColorNoLight.frag"))
	{
		if (ModelImporter* importer = assetManager.getImporter<ModelImporter>())
		{
			assetManager.createAsset(std::move(importer->import("Model_DirectionalLight", solidColorShader, "../Models/DirectionalLight.fbx")));
			assetManager.createAsset(std::move(importer->import("Model_PointLight", solidColorShader, "../Models/PointLight.fbx")));
			assetManager.createAsset(std::move(importer->import("Model_SpotLight", solidColorShader, "../Models/SpotLight.fbx")));
			assetManager.createAsset(std::move(importer->import("Model_Error", solidColorShader, "../Models/Error.fbx")));

			std::list<std::unique_ptr<Model>> gizmoModels = importer->importMultiple(solidColorShader, "../Models/Gizmo.fbx");
			for (std::unique_ptr<Model>& model : gizmoModels)
			{
				if (model)
				{
					assetManager.createAsset(std::move(model));
				}
			}
		}
	}
	
	if (Shader* modelShader = assetManager.createAsset<Shader>("Shader_DefaultModel", "../Shaders/Model.vert", "../Shaders/Model.frag"))
	{
		modelShader->createUniformBufferObject(4096, "LightUniformBuffer", 0);

		if (ModelImporter* importer = assetManager.getImporter<ModelImporter>())
		{
			//assetManager.createAsset(std::move(importer->import("Model_Scene", modelShader, "../Models/scene.fbx")));
			assetManager.createAsset(std::move(importer->import("Model_NewScene", modelShader, "../Models/NewScene.fbx")));
			assetManager.createAsset(std::move(importer->import("Model_Dragon", modelShader, "../Models/Dragon.fbx")));
			assetManager.createAsset(std::move(importer->import("Model_Makarov", modelShader, "../Models/makarov.fbx")));

			std::list<std::unique_ptr<Model>> dungeonModels = importer->importMultiple(modelShader, "../Models/Dungeon.fbx");
			for (std::unique_ptr<Model>& model : dungeonModels)
			{
				if (model)
				{
					assetManager.createAsset(std::move(model));
				}
			}
		}
	}
}

void Apparatus::_createEntityTemplates()
{
	if (Entity* playerEntity = entityRegistry.createEntityTemplate("Player"))
	{
		if (auto transformComponent = entityRegistry.createComponent<TransformComponent>(playerEntity))
		{
			if (auto cameraComponent = entityRegistry.createComponent<CameraComponent>(playerEntity))
			{
				cameraComponent->setParent(transformComponent);
				cameraComponent->getCamera().setPerspective(90.0f, 800.0f / 600.0f, 0.01f, 1000.0f);
			}
		}

		auto movementComponent = entityRegistry.createComponent<MovementComponent>(playerEntity);
	}

	if (Entity* modelEntity = entityRegistry.createEntityTemplate("ModelEntity"))
	{
		auto transformComponent = entityRegistry.createComponent<TransformComponent>(modelEntity);
		if (auto modelComponent = entityRegistry.createComponent<ModelComponent>(modelEntity))
		{
			modelComponent->setModel(assetManager.findAsset<Model>("Model_Error"));
			modelComponent->setParent(transformComponent);
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
		if (renderer)
		{
			renderer->render();

			window.swap();
		}

		// TODO: Shutdown
	}

	Logger::close();
}

void Apparatus::onWindowResized()
{
	// TODO: Renderer->GetActiveCameras->setAspect
	// TODO: Renderer->GetViewports->setSize


}
