#pragma once

#include "HumanControllerBase.h"

class GenericHumanController : public HumanControllerBase
{
public:
	GenericHumanController(const std::string& controllerName, LocalClient& localClient);

	virtual void onActivate() override;
	virtual void onDeactivate() override;
	virtual void setupInput() override;

protected:
	void moveZ(float rate);
	void moveX(float rate);

	void lookX(float value);
	void lookY(float value);
};
