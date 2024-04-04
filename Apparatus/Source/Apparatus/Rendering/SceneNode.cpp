#include "SceneNode.h"

SceneNode::SceneNode() :
	transform(1.0f),
	position(0.0f),
	orientation(glm::vec3(0.0f)),
	scale(1.0f),
	parent(nullptr),
	inheritRotation(true)
{

}

void SceneNode::calculateTransform()
{
	glm::vec3 derivedPosition = getWorldPosition();
	glm::quat derivedRotation = inheritRotation ? getWorldOrientation() : (orientation * rotator.asQuat());

	glm::mat4 newTransform = glm::translate(glm::mat4(1.0f), derivedPosition);
	newTransform *= glm::mat4(derivedRotation);
	newTransform = glm::scale(newTransform, scale);

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

void SceneNode::setOrientation(const glm::quat& quat)
{
	this->orientation = quat;
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

glm::vec3 SceneNode::getWorldPosition() const
{
	if (parent)
	{
		glm::vec3 derivedPosition = parent->getWorldOrientation() * position;
		derivedPosition += parent->getWorldPosition();

		return derivedPosition;
	}

	return position;
}

glm::quat SceneNode::getWorldOrientation() const
{
	if (parent && inheritRotation)
	{
		return glm::normalize(parent->getWorldOrientation() * rotator.asQuat());
	}

	return orientation * rotator.asQuat();
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
	if (this->parent)
	{
		this->parent->detachChild(this);
		this->parent = parent;
	}

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

void SceneNode::detachChild(SceneNode* child)
{
	auto iter = std::find(children.begin(), children.end(), child);
	if (iter != children.end())
	{
		children.erase(iter);
	}
}

void SceneNode::setInheritRotation(bool inheritRotation)
{
	this->inheritRotation = inheritRotation;
}

glm::vec3 SceneNode::getForward() const
{
	return getWorldOrientation() * glm::vec3(0.0f, 0.0f, 1.0f);
}

glm::vec3 SceneNode::getUp() const
{
	return getWorldOrientation() * glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::vec3 SceneNode::getRight() const
{
	return getWorldOrientation() * glm::vec3(-1.0f, 0.0f, 0.0f);
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
