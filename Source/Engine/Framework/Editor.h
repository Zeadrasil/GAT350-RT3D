#pragma once
namespace nc
{
	class Editor
	{
	public:
		void ProcessGui(class Scene* scene);
		void Update();
	private:
		bool active = true;
		bool includeVertexResources = true;
		bool includeTextureResources = true;
		bool includeShaderResources = true;
		bool includeProgramResources = true;
		bool includeModelResources = true;
		bool includeFontResources = true;
		bool includeMaterialResources = true;
		bool includeAllResources = false;
		class Object* m_selected = nullptr;
	};
}

