#pragma once

#include <Apparatus/Components/Component.h>
#include <Apparatus/Util/Primitive.h>

class SelectableComponent : public Component
{
public:
	SelectableComponent();
	SelectableComponent(const std::string& componentName);

	virtual std::unique_ptr<Component> clone() override;

	virtual void init() override;
	virtual void update(float dt) override;

	void setSelected(bool selected);
	bool isSelected() const;

	Box getVisualBoundingBox() const;

	void updateVisualBoundingBoxPosition();

	void setBoxVisible(bool visible);
	bool isBoxVisible() const;

protected:
	bool selected;
	glm::vec3 cachedPosition;
	Box box;
	bool boxVisible;
};
