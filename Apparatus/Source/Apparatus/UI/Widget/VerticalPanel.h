#pragma once

#include "Widget.h"

class VerticalPanel : public Widget
{
public:
	VerticalPanel();
	virtual void addChild(Widget* child) override;

	virtual glm::ivec2 getGlobalSize() override;
	virtual glm::ivec2 getSize() { return getGlobalSize(); }

	virtual bool refresh() override;

	void setOffset(int offset);

protected:
	std::vector<std::unique_ptr<Widget>> childContainers;
	int offset;
};
