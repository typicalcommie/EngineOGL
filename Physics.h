#pragma once
#include "glm\common.hpp"

using namespace glm;

namespace Physics
{
	struct AABB //Axis Alligned Bounding Box
	{
		vec3 initialMin{ 0,0,0 };
		vec3 initialMax{ 0,0,0 };

		vec3 pointMin{ 0,0,0 };
		vec3 pointMax{ 0,0,0 };

		vec3 scale{};
		vec3 rotation{};

		void ParseModel(float* vertices, uint count, uint stride)
		{
			for (uint i = 0, size = count * stride; i < size; i += stride)
			{
				if (initialMax.x < vertices[i])
					initialMax.x = vertices[i];

				if (initialMax.y < vertices[i + 1])
					initialMax.y = vertices[i + 1];

				if (initialMax.z < vertices[i + 2])
					initialMax.z = vertices[i + 2];



				if (initialMin.x > vertices[i])
					initialMin.x = vertices[i];

				if (initialMin.y > vertices[i + 1])
					initialMin.y = vertices[i + 1];

				if (initialMin.z > vertices[i + 2])
					initialMin.z = vertices[i + 2];
			}
		}

		bool Collision(AABB object_to_check)
		{
			quat quater;

			rotation /= 6.28f;
			rotation / 3.f;

			quater = angleAxis(360.f, rotation);

			pointMax = rotate(quater, initialMax);
			pointMin = rotate(quater, initialMin);

			bool X = (object_to_check.pointMax.x >= pointMin.x && object_to_check.pointMax.x <= pointMax.x) || (object_to_check.pointMin.x <= pointMax.x && object_to_check.pointMin.x >= pointMin.x);

			return X;

		}
	};

	struct Object
	{
		vec3 position{};
		vec3 scale{};
		vec3 rotation;



	};


}