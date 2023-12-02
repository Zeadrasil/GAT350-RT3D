#include "Editor.h"
#include "Scene.h"
#include "Components/CameraComponent.h"
namespace nc
{
	void Editor::ProcessGui(Scene* scene)
	{

		if (!active) return;
		ImGui::Begin("Resources");
		ImGui::Text("Filter Settings:");
		ImGui::Checkbox("Textures", &includeTextureResources);
		ImGui::Checkbox("Materials", &includeMaterialResources);
		ImGui::Checkbox("Vertex Buffers", &includeVertexResources);
		ImGui::Checkbox("Models", &includeModelResources);
		ImGui::Checkbox("Programs", &includeProgramResources);
		ImGui::Checkbox("Shaders", &includeShaderResources);
		ImGui::Checkbox("Fonts", &includeFontResources);
		ImGui::Checkbox("All", &includeAllResources);
		ImGui::Separator();
		auto resources = GET_RESOURCES(Resource);
		if (!includeAllResources)
		{
			auto textures = GET_RESOURCES(Texture);
			auto materials = GET_RESOURCES(Material);
			auto vertexBuffers = GET_RESOURCES(VertexBuffer);
			auto models = GET_RESOURCES(Model);
			auto programs = GET_RESOURCES(Program);
			auto shaders = GET_RESOURCES(Shader);
			auto fonts = GET_RESOURCES(Font);
			resources.clear();
			if (includeTextureResources)
			{
				resources.insert(resources.end(), textures.begin(), textures.end());
			}
			if (includeMaterialResources)
			{
				resources.insert(resources.end(), materials.begin(), materials.end());
			}
			if (includeVertexResources)
			{
				resources.insert(resources.end(), vertexBuffers.begin(), vertexBuffers.end());
			}
			if (includeModelResources)
			{
				resources.insert(resources.end(), models.begin(), models.end());
			}
			if (includeProgramResources)
			{
				resources.insert(resources.end(), programs.begin(), programs.end());
			}
			if (includeShaderResources)
			{
				resources.insert(resources.end(), shaders.begin(), shaders.end());
			}
			if (includeFontResources)
			{
				resources.insert(resources.end(), fonts.begin(), fonts.end());
			}
		}
		else
		{
			includeTextureResources = false;
			includeMaterialResources = false;
			includeVertexResources = false;
			includeModelResources = false;
			includeProgramResources = false;
			includeProgramResources = false;
			includeShaderResources = false;
			includeFontResources = false;
		}
		for (auto resource : resources)
		{
			if (!IsEqualIgnoreCase(resource->name, "") && ImGui::Selectable(resource->name.c_str(), resource.get() == m_selected))
			{
				m_selected = resource.get();
			}
		}
		ImGui::End();


		ImGui::Begin("Scene");

		scene->ProcessGui();
		ImGui::Separator();
		ImGui::BeginChild("Actors");
		//actor controls
		if (ImGui::BeginPopupContextWindow())
		{
			auto cameras = scene->GetComponents<CameraComponent>();
			auto camera = !cameras.empty() ? cameras[0] : nullptr;

			if (ImGui::MenuItem("Create Empty")) 
			{
				auto actor = CREATE_CLASS(Actor);
				actor->name = createUnique(createUnique(actor->GetClassName()));
				actor->Initialize();
				m_selected = actor.get();
				scene->Add(std::move(actor));
			}
			if (ImGui::MenuItem("Create Sphere")) 
			{
				auto actor = CREATE_CLASS_BASE(Actor, "Sphere");
				actor->name = createUnique("Sphere");
				actor->Initialize();
				m_selected = actor.get();
				if (camera)
				{
					actor->transform.position = camera->m_owner->transform.position + camera->m_owner->transform.Forward() * 5.0f;
				}
				scene->Add(std::move(actor));
			}
			if (ImGui::MenuItem("Create Camera")) 
			{
				auto actor = CREATE_CLASS_BASE(Actor, "Camera");
				actor->name = createUnique("Camera");
				actor->Initialize();
				m_selected = actor.get();
				if (camera)
				{
					actor->transform.position = camera->m_owner->transform.position + camera->m_owner->transform.Forward() * 5.0f;
				}
				scene->Add(std::move(actor));
			}
			if (ImGui::MenuItem("Create Cube")) 
			{
				auto actor = CREATE_CLASS_BASE(Actor, "Cube");
				actor->name = createUnique("Cube");
				actor->Initialize();
				m_selected = actor.get();
				if (camera)
				{
					actor->transform.position = camera->m_owner->transform.position + camera->m_owner->transform.Forward() * 5.0f;
				}
				scene->Add(std::move(actor));
			}
			if (ImGui::MenuItem("Create Light")) 
			{
				auto actor = CREATE_CLASS_BASE(Actor, "Light");
				actor->name = createUnique("Light");
				actor->Initialize();
				m_selected = actor.get();
				if (camera)
				{
					actor->transform.position = camera->m_owner->transform.position + camera->m_owner->transform.Forward() * 5.0f;
				}
				scene->Add(std::move(actor));
			}
			ImGui::EndPopup();	
		}

		for (auto& actor : scene->m_actors)
		{
			if (ImGui::Selectable(actor->name.c_str(), actor.get() == m_selected))
			{
				m_selected = actor.get();
			}
		}

		ImGui::EndChild();
		ImGui::End();

		ImGui::Begin("Inspector");
		if (m_selected)
		{
			m_selected->ProcessGui();
			if (ImGui::IsKeyPressed(ImGuiKey_Delete) && dynamic_cast<Actor*>(m_selected))
			{
				scene->Remove((Actor*)m_selected);
				m_selected = nullptr;
			}
		}

		ImGui::End();
	}
	void Editor::Update()
	{
		if (ImGui::IsKeyPressed(ImGuiKey_GraveAccent))
		{
			active = !active;
		}
	}
}
