#pragma once

#include <algorithm>

#include <glm/glm.hpp>

class Vector
{
public:
	static int findLongestAxis(const glm::vec3& vector)
	{
		const glm::vec3 checkedVector = glm::abs(vector);

		const float max = std::max({ checkedVector.x, checkedVector.y, checkedVector.z });

		int longestAxis = 0;

		for (int i = 0; i < 3; ++i)
		{
			if (checkedVector[i] == max)
			{
				longestAxis = i;
			}
		}

		return longestAxis;
	}

private:

};
