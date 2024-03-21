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

	return init();
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

ResourceManager& Apparatus::getResourceManager()
{
	return resourceManager;
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

int Apparatus::init()
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

	initResources();

	// Create server
	createServer<LocalServer>(this);

	// Create local client by default unless specified otherwise by program args
	createClient<LocalClient>(this);

	// Init all
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

	startGameLoop();

	return 0;
}

void Apparatus::initResources()
{
	resourceManager.init();

	if (Shader* debugPrimitiveShader = resourceManager.createResource<Shader>("DebugPrimitive", "../Shaders/DebugPrimitive.vert", "../Shaders/DebugPrimitive.frag"))
	{
		if (auto debugPrimitiveMaterial = resourceManager.createResource<Material>("DebugPrimitive"))
		{
			debugPrimitiveMaterial->setShader(debugPrimitiveShader);
		}
	}
	
	Shader* modelShader = resourceManager.createResource<Shader>("DefaultModelShader", "../Shaders/Model.vert", "../Shaders/Model.frag");
	ModelImporter* importer = resourceManager.getImporter<ModelImporter>();
	resourceManager.createResource(std::move(importer->import("Scene", modelShader, "C:\\Users\\mykha\\Desktop\\test\\scene.fbx")));
	resourceManager.createResource(std::move(importer->import("Cube", modelShader, "C:\\Users\\mykha\\Desktop\\test\\cube.fbx")));
	resourceManager.createResource(std::move(importer->import("Tree", modelShader, "C:\\Users\\mykha\\Desktop\\test\\tree.fbx")));
	resourceManager.createResource(std::move(importer->import("Entity", modelShader, "C:\\Users\\mykha\\Desktop\\test\\entity.fbx")));
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
