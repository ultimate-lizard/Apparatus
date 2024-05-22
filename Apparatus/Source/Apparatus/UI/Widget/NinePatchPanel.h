#pragma once

#include "ImagePanel.h"

class NinePatchPanel : public ImagePanel
{
public:
	NinePatchPanel();

	virtual void init() override;
	virtual void refresh() override;

	void setBorder(Side side, int border);
	int getBorder(Side side) const;

private:
	std::array<int, 4> borders;
};
