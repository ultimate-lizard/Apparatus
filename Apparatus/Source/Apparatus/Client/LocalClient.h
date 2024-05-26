#pragma once


#include <string>
#include <map>
#include <vector>

#include "Client.h"
#include "Controller.h"
#include "../Core/Input/InputHandler.h"
#include "../UI/UIContext.h"
#include "../Util/Primitive.h"
#include "../Rendering/Renderer.h"

#include "../Rendering/Mesh.h"
#include "../Rendering/Material.h"

class Entity;
class LocalServer;
class GenericHumanController;
class Renderer;
class SpriteRenderer;

class LocalClient : public Client
{
public:
	LocalClient(Renderer* renderer, SpriteRenderer* spriteRenderer);
	virtual ~LocalClient() = default;

	LocalClient(const LocalClient&) = delete;
	LocalClient(LocalClient&&) = delete;
	void operator=(const LocalClient&) = delete;

	// These should probably be replaced by the state. Like PlayingState->Start, MenuState->Start etc.
	virtual void init() override;
	virtual void onGameStart() override {}
	virtual void update(float dt) override;

	// TODO: RenderTarget
	virtual Viewport* getViewport() override;
	virtual Camera* getActiveCamera() override;

	template <class ControllerType>
	ControllerType* findController(const std::string& name);

	void setActiveController(Controller* controller);
	Controller* getActiveController();
	Controller* getDefaultController();

	InputHandler& getInputHandler();

	void onWindowResize(std::shared_ptr<WindowResizeEvent> event);

protected:
	// DEBUG STUFF
	void composeDebugPrimitiveRenderData();
	void renderDebugPrimitives(const std::vector<ModelVertex>& vertices, const std::vector<unsigned int>& indices, std::map<float, Mesh*>& debugMeshCache, const std::string& assetNamePrefix, RenderMode renderMode, float drawSize);

	std::map<float, Mesh*> cachedDebugPointMeshes;
	std::map<float, Mesh*> cachedDebugLineMeshes;
	std::map<float, Mesh*> cachedDebugBoxMeshes;

	Material* debugPrimitiveMaterial;

	int debugMeshBufferSize;
	//////////////

	template <class ControllerType, typename ... Args>
	ControllerType* createController(Args&& ... args);

	virtual void onActiveEntitySet() override;

	virtual void onActiveControllerChanged() {}

	void composeEntityRenderData();

	void onLightCreation(std::shared_ptr<LightComponentCreationEvent> event);

	Renderer* renderer;
	SpriteRenderer* spriteRenderer;

	// All possible controllers that the client would need should be stored here
	std::vector<std::unique_ptr<Controller>> controllers;

	Controller* activeController;

	GenericHumanController* defaultController;

	InputHandler inputHandler;

	Viewport viewport;

	UIContext uiContext;
	Camera uiCamera;
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
		return controller.get() != nullptr && dynamic_cast<ControllerType*>(controller.get()) != nullptr && controller.get()->getControllerName() == name;
	});

	// TODO: TOO MANY CASTS. BAD. FIX IT
	if (iter != controllers.end())
	{
		return dynamic_cast<ControllerType*>(iter->get());
	}

	return nullptr;
}
