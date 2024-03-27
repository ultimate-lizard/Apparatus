#pragma once

#include <Apparatus/Components/Component.h>
#include <Apparatus/Util/Primitive.h>

class SelectableComponent : public Component
{
public:
	SelectableComponent(Entity* owner);

	virtual void init() override;
	virtual void update(float dt) override;

	void setSelected(bool selected);
	bool isSelected() const;

	Box getVisualBoundingBox() const;

	void updateVisualBoundingBoxPosition();
	void regenerateVisualBoundingBox();

	void setBoxVisible(bool visible);
	bool isBoxVisible() const;

protected:
	virtual void assignDefaultObjectName() override;

	bool selected;
	glm::vec3 cachedPosition;
	Box box;
	bool boxVisible;
};
