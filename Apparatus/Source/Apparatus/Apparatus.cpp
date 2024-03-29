#include "Apparatus.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Server/LocalServer.h"
#include "Client/LocalClient.h"

// TODO: This is temp
#include "Rendering/Model.h"
#include "Rendering/Mesh.h"
#include "Rendering/Shader.h"
#include "Rendering/Material.h"
#include "Core/ModelImporter.h"

Apparatus::Apparatus(const std::string& gameTitle) :
	gameTitle(gameTitle),
	window(nullptr),
	running(false),
	inputReader(this)
{
}

Apparatus::~Apparatus()
{
	Logger::close();
}

int Apparatus::launch(int argc, char** argv)
{
	// TODO: Handle args

	return initEngineInternal();
}

void Apparatus::quit()
{
	running = false;
}

WindowEventHandler& Apparatus::getInputReader()
{
	return inputReader;
}

Renderer* Apparatus::getRenderer()
{
	return renderer.get();
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

void Apparatus::onWindowResized()
{
	for (std::unique_ptr<Client>& client : clients)
	{
		if (client)
		{
			if (Viewport* viewport = client->getViewport())
			{
				viewport->setSize(getWindowSize());
			}

			if (Camera* camera = client->getActiveCamera())
			{
				glm::ivec2 windowSize = getWindowSize();
				camera->setAspect(windowSize.x / static_cast<float>(windowSize.y));
			}
		}
	}
}

glm::ivec2 Apparatus::getWindowSize() const
{
	int width = 0;
	int height = 0;
	SDL_GetWindowSize(window, &width, &height);

	return { width, height };
}

glm::ivec2 Apparatus::getMouseCursorPosition() const
{
	int x = 0;
	int y = 0;

	SDL_GetMouseState(&x, &y);

	return glm::ivec2(x, y);
}

AssetManager& Apparatus::getResourceManager()
{
	return assetManager;
}

void Apparatus::setCursorVisibleEnabled(bool enabled)
{
	SDL_SetRelativeMouseMode(enabled ? SDL_FALSE : SDL_TRUE);
}

bool Apparatus::isCursorVisible() const
{
	SDL_bool result = SDL_GetRelativeMouseMode();
	return result == SDL_FALSE;
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

	window = SDL_CreateWindow(
		gameTitle.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		800,
		600,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);
	if (!window)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Unable to initialize SDL window!", nullptr);
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

	initAssets();

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
			client->start();
		}
	}

	startGameLoop();

	return 0;
}

void Apparatus::initAssets()
{
	assetManager.init();

	if (Shader* debugPrimitiveShader = assetManager.createAsset<Shader>("Shader_DebugPrimitive", "../Shaders/DebugPrimitive.vert", "../Shaders/DebugPrimitive.frag"))
	{
		if (Material* debugPrimitiveMaterial = assetManager.createAsset<Material>("Material_DebugPrimitive"))
		{
			debugPrimitiveMaterial->setShader(debugPrimitiveShader);
		}
	}
	
	Shader* modelShader = assetManager.createAsset<Shader>("Shader_DefaultModel", "../Shaders/Model.vert", "../Shaders/Model.frag");
	ModelImporter* importer = assetManager.getImporter<ModelImporter>();

	if (modelShader && importer)
	{
		assetManager.createAsset(std::move(importer->import("Model_Scene", modelShader, "../Models/scene.fbx")));
		// assetManager.createAsset(std::move(importer->import("Model_Cube", modelShader, "../Models/cube.fbx")));
		assetManager.createAsset(std::move(importer->import("Model_Makarov", modelShader, "../Models/makarov.fbx")));
		// assetManager.createAsset(std::move(importer->import("Model_Entity", modelShader, "../Models/entity.fbx")));
		// assetManager.createAsset(std::move(importer->import("Model_Gizmo", modelShader, "C:/Users/mykha/Desktop/test/Gizmo.fbx")));
		auto gizmoModels = importer->importMultiple(modelShader, "../Models/Gizmo.fbx");
		for (auto& model : gizmoModels)
		{
			if (model)
			{
				assetManager.createAsset(std::move(model));
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
		if (!isCursorVisible())
		{
			SDL_WarpMouseInWindow(window, 800 / 2, 600 / 2);
		}
		
		lastFrame = currentFrame;
		currentFrame = SDL_GetPerformanceCounter();

		float dt = (currentFrame - lastFrame) / static_cast<float>(SDL_GetPerformanceFrequency());
		accumulator += dt;

		inputReader.handleEvents();

		// Update logic
		while (accumulator >= fixedDt)
		{
			if (server)
			{
				server->update(fixedDt);
			}

			accumulator -= fixedDt;
		}

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
			SDL_GL_SwapWindow(window);
		}

		// TODO: Shutdown
	}
}
