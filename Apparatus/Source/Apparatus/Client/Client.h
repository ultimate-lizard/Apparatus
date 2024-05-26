#pragma once

#include <vector>
#include <memory>

#include "../Rendering/Viewport.h"

class Camera;
class Entity;

class Client
{
public:
	Client();
	virtual ~Client() = default;

	Client(const Client&) = delete;
	Client(Client&&) = delete;
	void operator=(const Client&) = delete;

	// These should probably be replaced by the state. Like PlayingState->Start, MenuState->Start etc.
	virtual void init() = 0;
	virtual void onGameStart() = 0;
	virtual void update(float dt) = 0;
	virtual void stop();

	bool isStopping() const;

	virtual Camera* getActiveCamera() = 0;
	virtual Viewport* getViewport() = 0;

	void setActiveEntity(Entity* entity);
	Entity* getActiveEntity();

protected:
	virtual void onActiveEntitySet() = 0;

	bool stopping;
	Entity* activeEntity;
};
