#pragma once

#include "Panel.h"

class NinePatchPanel : public Panel
{
public:
	NinePatchPanel(Material* material, Texture* texture);

	void setBorder(Side side, int border);
	int getBorder(Side side) const;

private:
	virtual void refresh() override;

	std::array<int, 4> borders;
};
