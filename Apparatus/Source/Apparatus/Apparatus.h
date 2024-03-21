#pragma once

#include <string>
#include <memory>
#include <vector>

#include "Rendering/Renderer.h"
#include "Server/Server.h"
#include "Client/Client.h"
#include "Core/Logger.h"
#include "Core/WindowEventHandler.h"
#include "Core/ResourceManager.h"

struct SDL_Window;
class LocalClient;

class Apparatus
{
public:
	Apparatus(const std::string& gameTitle);
	virtual ~Apparatus();

	int launch(int argc, char** argv);
	void quit();

	template <class ServerType, typename ... Args>
	void createServer(Args&& ... args);

	template <class ClientType, typename ... Args>
	void createClient(Args&& ... args);

	WindowEventHandler& getInputReader();
	Renderer* getRenderer();

	template <class ServerType>
	ServerType* getServer();

	LocalClient* getPrimaryLocalClient();
	
	void onWindowResized();

	glm::ivec2 getWindowSize() const;

	ResourceManager& getResourceManager();

	void setCursorVisibleEnabled(bool enabled);
	bool isCursorVisible() const;

protected:
	virtual int init();

private:
	void initResources();
	void startGameLoop();

	std::string gameTitle;
	SDL_Window* window;

	std::unique_ptr<Renderer> renderer;

	bool running;

	std::unique_ptr<Server> server;
	std::vector<std::unique_ptr<Client>> clients;

	WindowEventHandler inputReader;
	ResourceManager resourceManager;
};

template <typename ServerType, typename ... Args>
inline void Apparatus::createServer(Args&& ... args)
{
	std::unique_ptr<ServerType> newServer = std::make_unique<ServerType>(std::forward<Args>(args) ...);
	server = std::move(newServer);
}

template <typename ClientType, typename ... Args>
inline void Apparatus::createClient(Args&& ... args)
{	
	std::unique_ptr<ClientType> newClient = std::make_unique<ClientType>(std::forward<Args>(args) ...);
	clients.push_back(std::move(newClient));
}

template <class ServerType>
inline ServerType* Apparatus::getServer()
{
	return dynamic_cast<ServerType*>(server.get());
}
