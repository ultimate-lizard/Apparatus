#pragma once

#include "../Core/LifecycleInterface.h"
#include "../Core/NameProvider.h"
#include "../Rendering/Viewport.h"

class Apparatus;
class Camera;

class Client : public LifecycleInterface, public NameProvider
{
public:
	Client(Apparatus* apparatus);
	virtual ~Client() = default;

	Client(const Client&) = delete;
	Client(Client&&) = delete;
	void operator=(const Client&) = delete;

	virtual void init() = 0;
	virtual void start() {}
	virtual void update(float dt) = 0;

	virtual void quit();

	virtual Viewport* getViewport() = 0;
	virtual Camera* getActiveCamera() = 0;

	Apparatus* getApparatus();

protected:
	Apparatus* apparatus;
	bool quitting;
};
