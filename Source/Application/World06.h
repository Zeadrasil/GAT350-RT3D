#pragma once
#include "Framework/World.h"
#include "Core/Math/Transform.h"
#include "Renderer/Renderer.h"


namespace nc
{
	

	class World06 : public World
	{
	public:
		const uint32_t INVERT_MASK = 1 << 0;
		const uint32_t GRAYSCALE_MASK = 1 << 1;
		const uint32_t COLOR_TINT_MASK = 1 << 2;
		const uint32_t GRAIN_MASK = 1 << 3;
		const uint32_t SCANLINE_MASK = 1 << 4;
		const uint32_t MIRROR_MASK = 1 << 5;
		bool Initialize() override;
		void Shutdown() override;
		void Update(float dt) override;
		void Draw(Renderer& renderer) override;

	private:
		float m_time = 0;
		float m_speed = 5;
		float blend = 1;
		float m_refraction = 1;
		glm::vec4 tintColor{ 0 };
		uint32_t params = 0;
	};
}