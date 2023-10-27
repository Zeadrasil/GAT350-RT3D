#pragma once
#include "Framework/World.h"
#include "Core/Math/Transform.h"
#include "Renderer/Renderer.h"


namespace nc
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

	class World04 : public World
	{
	public:
		bool Initialize() override;
		void Shutdown() override;
		void Update(float dt) override;
		void Draw(Renderer& renderer) override;

	private:
		float m_time = 0;
		float m_speed = 5;
		glm::vec2 offset{ 0, 0 };
		glm::vec2 tiling{ 1, 1 };
		Transform m_transform;
		//GLuint m_vao = 0;
		res_t<VertexBuffer> m_vertexBuffer;
		res_t<Material> m_material;
		res_t<Program> m_program;
		res_t<Texture> m_texture;

		res_t<Model> m_model;

		glm::vec3 ambient{ 0.2f, 0.2f, 0.2f };
		std::vector<light_t> m_lights;
		int selected = 0;

	};
}