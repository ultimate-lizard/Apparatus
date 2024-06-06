#pragma once

#include "BoxModelPanel.h"

class HorizontalPanel : public Widget
{
public:
	virtual void addChild(Widget* child) override;

	virtual glm::ivec2 getGlobalSize() const override;
	virtual const glm::ivec2& getSize() const { return getGlobalSize(); }
	virtual void refresh() override;

protected:
	std::vector<std::unique_ptr<BoxModelPanel>> childContainers;
};
