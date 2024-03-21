#pragma once

#include "HumanControllerBase.h"

class InputHandler;

class EditorController : public HumanControllerBase
{
public:
	EditorController(LocalClient* localClient);

	virtual void onActivate() override;
	
protected:
	virtual void setupInput() override;

	void toggleDebugMode();
	void select();

	void moveZ(float value);
	void moveX(float value);
	void rotateRoll(float value);

	void selectEntity(Entity* entity);

	Entity* selectedEntity;
};
