#pragma once
#include "Framework/World.h"
#include "Core/Math/Transform.h"
#include "Renderer/Renderer.h"


namespace nc
{
	

	class World0575 : public World
	{
		struct light_t
	{
		enum lightType
		{
			Point, Directional, Spot
		};
		lightType type;
		glm::vec3 position;
		glm::vec3 color;
		glm::vec3 direction;
		float intensity;
		float range;
		float innerAngle;
		float outerAngle;
	};
	public:
		bool Initialize() override;
		void Shutdown() override;
		void Update(float dt) override;
		void Draw(Renderer& renderer) override;

	private:
		float m_time = 0;
		float m_speed = 5;
		float m_refraction = 1;
		glm::vec3 ambient{ 0.2f, 0.2f, 0.2f };
	};
}