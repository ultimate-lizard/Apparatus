#pragma once

#include <Apparatus/Server/LocalServer.h>

class EditorLocalServer : public LocalServer
{
public:
	EditorLocalServer(Apparatus* apparatus);

	// virtual void init() override;
	virtual void update(float dt) override;

protected:
	virtual void assignDefaultObjectName() override;

	virtual void setupEntities() override;
};
