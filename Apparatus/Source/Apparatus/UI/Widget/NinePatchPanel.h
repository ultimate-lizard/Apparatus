#pragma once

#include "ImagePanel.h"

class NinePatchPanel : public ImagePanel
{
public:
	NinePatchPanel();

	virtual void init() override;
	virtual bool refresh() override;

	void setBorder(Side side, int border);
	void setBorder(int border);
	int getBorder(Side side) const;

private:
	std::array<int, 4> borders;
};
