#pragma once

#include <array>

#include "Button.h"

class NinePatchButton : public Button
{
public:
	virtual void refresh() override;

	void setBorder(Side side, int border);
	void setBorder(int border);
	int getBorder(Side side) const;

protected:
	virtual void initMaterial() override;
	virtual void refreshSprite() override;

	std::array<int, 4> borders;
};
