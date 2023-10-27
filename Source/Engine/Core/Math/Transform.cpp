#include "Transform.h"
#include "Renderer/Gui.h"

namespace nc
{
	void Transform::processGui()
	{
		ImGui::DragFloat3("Position", glm::value_ptr(position));
		ImGui::DragFloat3("Scale", glm::value_ptr(scale));
		ImGui::DragFloat3("Rotation", glm::value_ptr(rotation));
	}
	void Transform::Read(const json_t& value)
	{
		/*
		READ_DATA(value, position);
		READ_DATA(value, rotation);
		READ_DATA(value, scale);*/
	}
}
