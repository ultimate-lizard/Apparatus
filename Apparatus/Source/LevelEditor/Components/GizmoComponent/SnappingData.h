#pragma once

#include <Apparatus/Rendering/Rotator.h>

struct SnappingData
{
	bool snapToGrid = false;
	Rotator unsnappedRotator;
	glm::vec3 unsnappedScale{ 0.0f };
	float translationStep = 0.25f;
	float rotationStep = 10.0f;
	float scaleStep = 0.1f;
};
