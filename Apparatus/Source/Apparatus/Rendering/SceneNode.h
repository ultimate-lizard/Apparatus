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

	/*
		!!! Since I couldn't manage to find a way to properly convert quaternions to euler angles,
		I made a separate 'orientation' variable that stores a local quaternion, that
		evaluates as orientation * rotator (pitch, yaw, roll) during the consrtuction of the transformation matrix
	*/
	void setRotation(const Rotator& rotator);

	/*
		!!! Since I couldn't manage to find a way to properly convert quaternions to euler angles,
		I made a separate 'orientation' variable that stores a local quaternion, that
		evaluates as orientation * rotator (pitch, yaw, roll) during the consrtuction of the transformation matrix
	*/
	void setRotation(float degrees, Euler angle);

	/*
		!!! Since I couldn't manage to find a way to properly convert quaternions to euler angles,
		I made a separate 'orientation' variable that stores a local quaternion, that
		evaluates as orientation * rotator (pitch, yaw, roll) during the consrtuction of the transformation matrix
	*/
	void setOrientation(const glm::quat& quat);
	
	void setScale(const glm::vec3& scale);

	void offsetPosition(const glm::vec3& delta);

	const glm::vec3& getPosition() const;
	const Rotator& getRotator() const;
	float getRotationAngle(Euler angle) const;
	const glm::vec3& getScale() const;

	glm::vec3 getWorldPosition() const;
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

	SceneNode* getRoot();

protected:
	glm::mat4 transform;

	glm::vec3 position;

	/*
		!!! Since I couldn't manage to find a way to properly convert quaternions to euler angles,
		I made a separate 'orientation' variable that stores a local quaternion, that
		evaluates as orientation * rotator (pitch, yaw, roll) during the consrtuction of the transformation matrix
	*/
	Rotator rotator;
	glm::quat orientation;

	glm::vec3 scale;

	SceneNode* parent;
	std::vector<SceneNode*> children;

	bool inheritRotation;
};
