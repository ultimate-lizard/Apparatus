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
#include "../Util/Primitive.h"
#include "InputHandler.h"
#include "Controller.h"

class Entity;
class LocalServer;
class GenericHumanController;

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

	LocalServer* getLocalServer();

	virtual Viewport* getViewport() override;
	virtual Camera* getActiveCamera() override;

	template <class ControllerType, typename ... Args>
	ControllerType* createController(Args&& ... args);

	template <class ControllerType>
	ControllerType* findController(const std::string& name);

	Controller* getDefaultController();

	void setActiveEntity(Entity* entity);
	Entity* getActiveEntity();

	InputHandler& getInputHandler();

	void setActiveController(Controller* controller);
	Controller* getActiveController();

	virtual void onActiveControllerChanged() {};

protected:
	virtual void assignDefaultObjectName() override;

	// DEBUG STUFF
	void composeDebugPrimitiveData();
	void renderDebugPrimitives(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, std::map<float, Mesh*>& debugMeshCache, const std::string& assetNamePrefix, RenderMode renderMode, float drawSize);

	std::map<float, Mesh*> cachedDebugPointMeshes;
	std::map<float, Mesh*> cachedDebugLineMeshes;
	std::map<float, Mesh*> cachedDebugBoxMeshes;

	std::unique_ptr<MaterialInstance> debugPrimitiveMaterialInstance;

	size_t debugMeshBufferSize;
	//////////////

	void renderEntities();

	LocalServer* localServer;

private:
	// All possible controllers that the client would need should be stored here
	std::vector<std::unique_ptr<Controller>> controllers;

	Controller* activeController;

	GenericHumanController* defaultController;

	Entity* activeEntity;

	InputHandler inputHandler;

	Viewport viewport;
};

template<class ControllerType, typename ... Args>
inline ControllerType* LocalClient::createController(Args&& ... args)
{
	controllers.push_back(std::make_unique<ControllerType>(std::forward<Args>(args)...));
	ControllerType* newController = dynamic_cast<ControllerType*>(controllers.back().get());

	return newController;
}

template <class ControllerType>
inline ControllerType* LocalClient::findController(const std::string& name)
{
	auto iter = std::find_if(controllers.begin(), controllers.end(), [&name](const std::unique_ptr<Controller>& controller) {
		return controller.get() != nullptr && dynamic_cast<ControllerType*>(controller.get()) != nullptr && controller.get()->getObjectName() == name;
	});

	// TODO: TOO MANY CASTS. BAD. FIX IT
	if (iter != controllers.end())
	{
		return dynamic_cast<ControllerType*>(iter->get());
	}

	return nullptr;
}
