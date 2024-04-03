#pragma once

#include <map>

#include <glm/gtc/quaternion.hpp>

enum Euler
{
	Pitch = 0,
	Yaw,
	Roll
};

class Rotator
{
public:
	Rotator() = default;
	Rotator(float pitch, float yaw, float roll);

	glm::quat asQuat() const;

	float get(Euler angle) const;

	void set(float degree, Euler angle);

	void rotate(float degree, Euler angle);

	void clamp(float min, float max, Euler angle);

	static float normalizeAngle(float degree);

private:
	float angles[3]{};
	glm::vec2 clamps[3]{};
};
