#include "SceneNode.h"

SceneNode::SceneNode() :
	transform(1.0f),
	position(0.0f),
	orientation(glm::vec3(0.0f)),
	scale(1.0f),
	parent(nullptr),
	inheritRotation(true),
	inheritScale(true),
	dirty(true)
{

}

SceneNode::SceneNode(const SceneNode& other) :
	transform(other.transform),
	position(other.position),
	rotator(other.rotator),
	orientation(other.orientation),
	scale(other.scale),
	parent(other.parent),
	children(other.children),
	inheritRotation(other.inheritRotation),
	inheritScale(other.inheritScale),
	dirty(other.dirty)
{

}

void SceneNode::calculateTransform()
{
	if (!dirty)
	{
		return;
	}

	glm::vec3 derivedPosition = getWorldPosition();
	glm::quat derivedRotation = inheritRotation ? getWorldOrientation() : (orientation * rotator.asQuat());
	glm::vec3 derivedScale = getWorldScale();

	glm::mat4 newTransform = glm::translate(glm::mat4(1.0f), derivedPosition);
	newTransform *= glm::mat4(derivedRotation);
	newTransform = glm::scale(newTransform, derivedScale);

	transform = newTransform;

	for (SceneNode* child : children)
	{
		child->dirty = true;
		child->calculateTransform();
	}

	dirty = false;
}

void SceneNode::setPosition(const glm::vec3& position)
{
	this->position = position;
	dirty = true;
}

void SceneNode::setRotation(const Rotator& rotator)
{
	this->rotator = rotator;
	dirty = true;
}

void SceneNode::setRotation(float degrees, Euler angle)
{
	rotator.set(degrees, angle);
	dirty = true;
}

void SceneNode::setOrientation(const glm::quat& quat)
{
	this->orientation = quat;
	dirty = true;
}

void SceneNode::setScale(const glm::vec3& scale)
{
	this->scale = scale;
	dirty = true;
}

void SceneNode::offsetPosition(const glm::vec3& delta)
{
	this->position += delta;
	dirty = true;
}

void SceneNode::offsetScale(const glm::vec3& delta)
{
	this->scale += delta;
	dirty = true;
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
		glm::vec3 derivedPosition = position;
		derivedPosition = parent->getWorldScale() * derivedPosition;
		derivedPosition = parent->getWorldOrientation() * derivedPosition;
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

glm::vec3 SceneNode::getWorldScale() const
{
	if (parent && inheritScale)
	{
		return parent->getWorldScale() * scale;
	}

	return scale;
}

void SceneNode::rotate(float degrees, Euler angle)
{
	rotator.rotate(degrees, angle);
	dirty = true;
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
	this->parent->children.push_back(this);
	this->parent->dirty = true;

	dirty = true;
}

SceneNode* SceneNode::getParent() const
{
	return parent;
}

std::vector<SceneNode*> SceneNode::getChildren()
{
	return children;
}

void SceneNode::detachChild(SceneNode* child)
{
	auto iter = std::find(children.begin(), children.end(), child);
	if (iter != children.end())
	{
		children.erase(iter);
	}

	if (child)
	{
		child->dirty = true;
	}
}

void SceneNode::setInheritRotation(bool inheritRotation)
{
	this->inheritRotation = inheritRotation;
	dirty = true;
}

void SceneNode::setInheritScale(bool inheritScale)
{
	this->inheritScale = inheritScale;
	dirty = true;
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

bool SceneNode::isDirty() const
{
	return dirty;
}
