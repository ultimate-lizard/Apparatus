#pragma once

#include "Component.h"
#include "../Common/Primitives.h"

class SelectableComponent : public Component
{
public:
	SelectableComponent(const std::string& id);

	virtual void init() override;
	virtual void update(float dt) override;

	void setSelected(bool selected);
	bool isSelected() const;

	Box getBoundingBox() const;

protected:
	bool selected;
};
