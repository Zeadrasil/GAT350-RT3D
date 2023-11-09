#include "RotateComponent.h"
#include "imgui/imgui.h"
#include "Framework/Actor.h"
namespace nc
{
	CLASS_DEFINITION(RotateComponent);
	void RotateComponent::ProcessGui()
	{
		ImGui::DragFloat3("Rotate", glm::value_ptr(euler));
	}
	void RotateComponent::Read(const json_t& value)
	{
		READ_NAME_DATA(value, "rotation", euler);
	}
	void RotateComponent::Update(float dt)
	{
		m_owner->transform.rotation *= EulerToQuaternion(euler);
		m_owner->transform.euler = QuaternionToEuler(m_owner->transform.rotation);
	}

}