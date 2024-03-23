#pragma once

#include <Apparatus/Client/HumanControllerBase.h>

class InputHandler;

class EditorController : public HumanControllerBase
{
public:
	EditorController(LocalClient* localClient);

	virtual void onActivate() override;
	virtual void onDeactivate() override {};
	
protected:
	virtual void assignDefaultObjectName() override;

	virtual void setupInput() override;

	void select();
	void deselect();

	void moveZ(float value);
	void moveX(float value);
	void rotateRoll(float value);

	void selectEntity(Entity* entity);

	Entity* selectedEntity;
};
