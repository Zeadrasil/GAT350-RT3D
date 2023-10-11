#pragma once
#include "Vector2.h"
#include "Matrix33.h"
#include "Core/Json.h"
#include <glm/glm/glm.hpp>
#include <glm/glm/gtx/euler_angles.hpp>
#include <glm/glm/gtx/transform.hpp>

namespace nc
{
	class Transform
	{
	public:
		glm::vec3 position{ 0 };
		glm::vec3 rotation{ 0 };
		glm::vec3 scale{ 1 };

	public:
		Transform() = default;
		Transform(const glm::vec3& position, glm::vec3 rotation, glm::vec3 scale) :
			position{ position },
			rotation{ rotation },
			scale{ scale }
		{}

		glm::mat4 GetMatrix() const
		{
			glm::mat4 ms = glm::scale(scale);
			glm::mat4 mr = glm::eulerAngleYXZ(glm::radians(rotation.y), glm::radians(rotation.x), glm::radians(rotation.z));
			glm::mat4 mt = glm::translate(position);
			glm::mat4 mx = mt * ms * mr;

			return mx;
		}

		void Read(const json_t& value);
	};
}