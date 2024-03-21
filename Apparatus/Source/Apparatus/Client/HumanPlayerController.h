#pragma once

#include "HumanControllerBase.h"

class HumanPlayerController : public HumanControllerBase
{
public:
	HumanPlayerController(LocalClient* localClient);

	virtual void onActivate() override;
	virtual void setupInput() override;

	void moveZ(float rate);
	void moveX(float rate);

	void lookX(float value);
	void lookY(float value);

protected:
	void toggleDebugMode();
};
