#pragma once

#include <array>

#include "Widget.h"

class BoxModelPanel : public Widget
{
public:
	BoxModelPanel();

	virtual glm::ivec2 getGlobalPosition() const override;
	virtual glm::ivec2 getGlobalSize() const override;

	void setSize(const glm::ivec2& size);
	const glm::ivec2& getSize() const;

	void setMargin(Side side, int margin);
	int getMargin(Side side) const;

protected:
	glm::ivec2 size;
	std::array<int, 4> margins;
};
