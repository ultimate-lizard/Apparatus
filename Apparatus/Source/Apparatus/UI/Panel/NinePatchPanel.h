#pragma once

#include "Panel.h"

class NinePatchPanel : public Panel
{
public:
	NinePatchPanel();

	void setBorder(Side side, int border);
	int getBorder(Side side) const;

	virtual void refresh() override;

private:
	std::array<int, 4> borders;
};
