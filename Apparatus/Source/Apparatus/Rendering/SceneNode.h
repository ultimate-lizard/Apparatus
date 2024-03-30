#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "../Rendering/Rotator.h"

class SceneNode
{
public:
	SceneNode();

	virtual void calculateTransform();

	void setPosition(const glm::vec3& position);
	void setRotation(const Rotator& rotator);
	void setRotation(float degrees, Euler angle);
	
	void setScale(const glm::vec3& scale);

	void offsetPosition(const glm::vec3& delta);

	const glm::vec3& getPosition() const;
	const Rotator& getRotator() const;
	float getRotationAngle(Euler angle) const;
	const glm::vec3& getScale() const;

	glm::vec3 getDerivedPosition() const;
	glm::quat getDerivedOrientation() const;

	void rotate(float degrees, Euler angle);

	glm::mat4 getTransform() const;

	void setParent(SceneNode* parent);
	SceneNode* getParent();
	void detachChild(SceneNode* child);

	void setInheritRotation(bool inheritRotation);

	glm::vec3 getForward() const;
	glm::vec3 getUp() const;
	glm::vec3 getRight() const;

	glm::vec3 getLocalForward() const;
	glm::vec3 getLocalUp() const;
	glm::vec3 getLocalRight() const;

protected:
	glm::mat4 transform;

	glm::vec3 position;
	Rotator rotator;
	glm::vec3 scale;

	SceneNode* parent;
	std::vector<SceneNode*> children;

	bool inheritRotation;
};
