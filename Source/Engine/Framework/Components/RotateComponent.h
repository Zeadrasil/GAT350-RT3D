#pragma once
#include "Component.h"
namespace nc
{
	class RotateComponent : public Component
	{
	public:
		CLASS_DECLARATION(RotateComponent);
		void ProcessGui() override;
		void Update(float dt) override;
		glm::vec3 euler{ 0 };
	};
}

