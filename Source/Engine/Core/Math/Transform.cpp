#include "Transform.h"
#include "Renderer/Gui.h"
#include "MathUtils.h"
namespace nc
{
	void Transform::processGui()
	{
		ImGui::DragFloat3("Position", glm::value_ptr(position), 0.1f);
		ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.1f);
		glm::vec3 oldEuler = euler;
		ImGui::DragFloat3("Rotation", glm::value_ptr(euler));
		if(oldEuler.x != euler.x || oldEuler.y != euler.y || oldEuler.z != euler.z )
		{
			rotation = EulerToQuaternion(euler);
		}
	}
	void Transform::Read(const json_t& value)
	{
		READ_DATA(value, position);
		READ_NAME_DATA(value, "rotation", euler);
		READ_DATA(value, scale);

		rotation = EulerToQuaternion(euler);
	}
}
