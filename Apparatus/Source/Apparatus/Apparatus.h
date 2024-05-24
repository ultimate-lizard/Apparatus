#pragma once

#include <string>
#include <memory>
#include <vector>

#include "Window.h"
#include "Level.h"
#include "WindowEventHandler.h"
#include "Core/EngineSystem/EngineSystem.h"
#include "Rendering/Renderer.h"
#include "Server/Server.h"
#include "Client/Client.h"
#include "Event/EventDispatcher.h"
#include "Rendering/Sprite/Sprite.h"
#include "Rendering/Sprite/SpriteRenderer.h"

class LocalClient;
class EntityRegistry;
class AssetManager;

class Apparatus
{
	struct ApparatusGlobalInstance
	{
		EntityRegistry* entityRegistry;
		AssetManager* assetManager;
		EventDispatcher* eventDispatcher;
		Window* window;
	};

public:
	Apparatus(const std::string& gameTitle);
	virtual ~Apparatus() = default;

	Apparatus(const Apparatus&) = delete;
	Apparatus(Apparatus&&) = delete;

	// Returns a static pointer to the engine
	template <class EngineType>
	static EngineType* getAs();

	// Returns a static pointer to the engine
	static Apparatus& get();

	int launch(int argc, char** argv);
	void quit();

	template <class ServerType>
	ServerType* getServer();

	std::vector<Client*> getClients();

	// TODO: Finalize the logic
	// At the moment the method returns the first available LocalClient
	LocalClient* getPrimaryLocalClient();

	Level* getLevel();

	static EntityRegistry& getEntityRegistry();
	static AssetManager& getAssetManager();
	static EventDispatcher& getEventDispatcher();
	static Window& getWindow();

protected:
	virtual void init();

	virtual void createEngineSystems();
	void initEngineSystems();
	void uninitEngineSystems();

	// TODO: The assets should probably be gathered from files
	void _createAssets();
	// TODO: The entity templates must be gathered from files
	virtual void _createEntityTemplates();

	// Must be called during init() to ensure initialization by the engine
	template <class SystemType, typename ... Args>
	SystemType* createEngineSystem(Args&& ... args);

	template <class SystemType>
	SystemType* findEngineSystem();

	// Replaces the current server with a new one. Doesn't do initialization automatically
	template <class ServerType, typename ... Args>
	ServerType* createServer(Args&& ... args);

	// Creates and adds a new client to the engine. Doesn't do initialization automatically
	template <class ClientType, typename ... Args>
	ClientType* createClient(Args&& ... args);

	Renderer* getRenderer();
	SpriteRenderer* getSpriteRenderer();

private:
	int initEngineInternal();
	void startGameLoop();

	static Apparatus* apparatus;

	std::string gameTitle;
	Window window;
	bool running;

	std::vector<std::unique_ptr<EngineSystem>> engineSystems;

	std::unique_ptr<Renderer> renderer;
	std::unique_ptr<SpriteRenderer> spriteRenderer;

	std::unique_ptr<Server> server;
	std::vector<std::unique_ptr<Client>> clients;

	std::unique_ptr<Level> level;

	static ApparatusGlobalInstance globalState;
};

template<class SystemType, typename ...Args>
inline SystemType* Apparatus::createEngineSystem(Args && ...args)
{
	auto newSystemPtr = std::make_unique<SystemType>(std::forward<Args>()...);
	SystemType* newSystem = newSystemPtr.get();
	engineSystems.push_back(std::move(newSystemPtr));

	return newSystem;
}

template<class SystemType>
inline SystemType* Apparatus::findEngineSystem()
{
	for (std::unique_ptr<EngineSystem>& system : engineSystems)
	{
		if (SystemType* castedSystem = dynamic_cast<SystemType*>(system.get()))
		{
			return castedSystem;
		}
	}

	return nullptr;
}

template <typename ServerType, typename ... Args>
inline ServerType* Apparatus::createServer(Args&& ... args)
{
	std::unique_ptr<ServerType> newServerPtr = std::make_unique<ServerType>(std::forward<Args>(args) ...);

	ServerType* newServer = newServerPtr.get();

	server = std::move(newServerPtr);

	return newServer;
}

template <typename ClientType, typename ... Args>
inline ClientType* Apparatus::createClient(Args&& ... args)
{	
	std::unique_ptr<ClientType> newClientPtr = std::make_unique<ClientType>(std::forward<Args>(args) ...);

	ClientType* newClient = newClientPtr.get();

	clients.push_back(std::move(newClientPtr));

	return newClient;
}

template<class EngineType>
inline EngineType* Apparatus::getAs()
{
	return dynamic_cast<EngineType*>(apparatus);
}

template <class ServerType>
inline ServerType* Apparatus::getServer()
{
	return dynamic_cast<ServerType*>(server.get());
}
