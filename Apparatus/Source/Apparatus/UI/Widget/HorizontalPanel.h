#pragma once

#include "Widget.h"

class HorizontalPanel : public Widget
{
public:
	virtual void addChild(Widget* child) override;

	virtual glm::ivec2 getGlobalSize() override;
	virtual glm::ivec2 getSize() { return getGlobalSize(); }
	virtual bool refresh() override;

protected:
	std::vector<std::unique_ptr<Widget>> childContainers;
};
