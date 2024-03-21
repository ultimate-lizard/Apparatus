#include "SceneNode.h"

SceneNode::SceneNode() :
	transform(1.0f),
	position(0.0f),
	scale(1.0f),
	parent(nullptr)
{

}

void SceneNode::calculateTransform()
{
	glm::vec3 derivedPosition = getDerivedPosition();
	glm::quat derivedRotation = getDerivedOrientation();
	// newTransform = glm::scale(newTransform, scale);

	glm::mat4 newTransform = glm::translate(glm::mat4(1.0f), derivedPosition);
	newTransform *= glm::mat4(derivedRotation);

	//if (parent)
	//{
	//	transform = parent->transform * localTransform;
	//}
	//else
	//{
	//	transform = localTransform;
	//}

	transform = newTransform;

	for (SceneNode* child : children)
	{
		child->calculateTransform();
	}
}

void SceneNode::setPosition(const glm::vec3& position)
{
	this->position = position;
}

void SceneNode::setRotation(const Rotator& rotator)
{
	this->rotator = rotator;
}

void SceneNode::setRotation(float degrees, Euler angle)
{
	rotator.set(degrees, angle);
}

void SceneNode::setScale(const glm::vec3& scale)
{
	this->scale = scale;
}

void SceneNode::offsetPosition(const glm::vec3& delta)
{
	this->position += delta;
}

const glm::vec3& SceneNode::getPosition() const
{
	return position;
}

const Rotator& SceneNode::getRotator() const
{
	return rotator;
}

float SceneNode::getRotationAngle(Euler angle) const
{
	return rotator.get(angle);
}

const glm::vec3& SceneNode::getScale() const
{
	return scale;
}

glm::vec3 SceneNode::getDerivedPosition() const
{
	if (parent)
	{
		glm::vec3 derivedPosition = parent->getDerivedOrientation() * position;
		derivedPosition += parent->getDerivedPosition();

		return derivedPosition;
	}

	return position;
}

glm::quat SceneNode::getDerivedOrientation() const
{
	if (parent)
	{
		return glm::normalize(parent->getDerivedOrientation() * rotator.asQuat());
	}

	return rotator.asQuat();
}

void SceneNode::rotate(float degrees, Euler angle)
{
	rotator.rotate(degrees, angle);
}

glm::mat4 SceneNode::getTransform() const
{
	return transform;
}

void SceneNode::setParent(SceneNode* parent)
{
	if (!parent)
	{
		return;
	}

	this->parent = parent;
	parent->children.push_back(this);
}

SceneNode* SceneNode::getParent()
{
	return parent;
}

glm::vec3 SceneNode::getForward() const
{
	return getDerivedOrientation() * glm::vec3(0.0f, 0.0f, 1.0f);
}

glm::vec3 SceneNode::getUp() const
{
	return getDerivedOrientation() * glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::vec3 SceneNode::getRight() const
{
	return getDerivedOrientation() * glm::vec3(-1.0f, 0.0f, 0.0f);
}

glm::vec3 SceneNode::getLocalForward() const
{
	return getRotator().asQuat() * glm::vec3(0.0f, 0.0f, 1.0f);
}

glm::vec3 SceneNode::getLocalUp() const
{
	return getRotator().asQuat() * glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::vec3 SceneNode::getLocalRight() const
{
	return getRotator().asQuat() * glm::vec3(-1.0f, 0.0f, 0.0f);
}
