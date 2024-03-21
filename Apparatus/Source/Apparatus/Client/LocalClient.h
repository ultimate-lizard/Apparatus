#pragma once

#include <memory>
#include <string>
#include <map>
#include <vector>

#include "Client.h"
#include "../Rendering/Camera.h"
#include "../Rendering/Renderer.h"
#include "../Rendering/Mesh.h"
#include "../Rendering/Material.h"
#include "../Common/Primitives.h"
#include "InputHandler.h"
#include "Controller.h"

class Entity;
class LocalServer;

class LocalClient : public Client
{
public:
	LocalClient(Apparatus* apparatus);
	virtual ~LocalClient() = default;

	LocalClient(const LocalClient&) = delete;
	LocalClient(LocalClient&&) = delete;
	void operator=(const LocalClient&) = delete;

	virtual void init() override;
	virtual void update(float dt) override;

	virtual Viewport* getViewport() override;
	virtual Camera* getActiveCamera() override;

	void setControlledEntity(Entity* entity);

	InputHandler& getInputHandler();

	bool isDebugModeEnabled() const;
	void setDebugModeEnabled(bool enabled);

	void setActiveController(Controller* controller);

protected:
	void composeDebugPrimitiveData();
	void renderDebugPrimitives(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, std::map<float, Mesh*>& debugMeshCache, const std::string& assetNamePrefix, RenderMode renderMode, float drawSize);

	std::map<float, Mesh*> cachedDebugPointMeshes;
	std::map<float, Mesh*> cachedDebugLineMeshes;
	std::map<float, Mesh*> cachedDebugBoxMeshes;

	std::unique_ptr<MaterialInstance> debugPrimitiveMaterialInstance;

	LocalServer* localServer;

	Entity* controlEntity;
	InputHandler inputHandler;

	std::vector<std::unique_ptr<Controller>> controllers;
	Controller* activeController;

	Viewport viewport;

	bool debugModeEnabled;
};
