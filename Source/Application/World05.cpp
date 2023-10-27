#include "World05.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "Input/InputSystem.h"
#include <glm/glm/gtx/color_space.hpp>

#define INTERLEAVE


namespace nc
{
    bool World05::Initialize()
    {
        //m_material = GET_RESOURCE(Material, "materials/quad.mtrl");
        /*
        m_program = GET_RESOURCE(Program, "shaders/unlit_texture.prog");
        m_program->Use();

        m_texture = GET_RESOURCE(Texture, "textures/llama.jpg");
        m_texture->Bind();
        m_texture->SetActive(GL_TEXTURE0);
        */

        m_scene = std::make_unique<Scene>();
        m_scene->Load("scenes/scene.json");
        m_scene->Initialize();
        /*
        {
            auto actor = CREATE_CLASS(Actor);
            actor->name = "actor1";
            actor->transform.position = glm::vec3{ 0, 0, 0 };
            auto modelComponent = CREATE_CLASS(ModelComponent);
            modelComponent->m_model = std::make_shared<Model>();
            modelComponent->m_model->SetMaterial(GET_RESOURCE(Material, "materials/squirrel.mtrl"));
            modelComponent->m_model->Load("models/squirrel.glb", glm::vec3{ 0, -0.7f, 0 }, glm::vec3{ 0 }, glm::vec3{ 0.4f });
            actor->AddComponent(std::move(modelComponent));
            m_scene->Add(std::move(actor));
        }

        {
            auto actor = CREATE_CLASS(Actor);
            actor->name = "actor2";
            actor->transform.position = glm::vec3{ 0, 0, 0 };
            auto modelComponent = CREATE_CLASS(ModelComponent);
            modelComponent->m_model = std::make_shared<Model>();
            modelComponent->m_model->SetMaterial(GET_RESOURCE(Material, "materials/squirrel.mtrl"));
            modelComponent->m_model->Load("models/squirrel.glb", glm::vec3{ 10, -0.7f, 0 }, glm::vec3{ 0 }, glm::vec3{ 0.4f });
            actor->AddComponent(std::move(modelComponent));
            m_scene->Add(std::move(actor));
        }*/

        {
            auto actor = CREATE_CLASS(Actor);
            actor->name = "light1";
            actor->transform.position = glm::vec3{ 3, 3, 3 };
            auto lightComponent = CREATE_CLASS(LightComponent);
            lightComponent->type = LightComponent::eType::Point;
            lightComponent->color = glm::rgbColor(glm::vec3{ randomf() * 360, 1, 1 });
            lightComponent->intensity = 1;
            lightComponent->range = 20;
            lightComponent->innerAngle = 10.0f;
            lightComponent->outerAngle = 30.0f;
            actor->AddComponent(std::move(lightComponent));
            m_scene->Add(std::move(actor));
        }
        return true;
    }

    void World05::Shutdown()
    {
    }

    void World05::Update(float dt)
    {

        ENGINE.GetSystem<Gui>()->BeginFrame();
        m_scene->Update(dt);
        m_scene->ProcessGui();

        auto actor = m_scene->GetActorByName<Actor>("actor1");
        actor->transform.position.x += (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? m_speed * -dt : ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? m_speed * dt : 0) / 2;
        actor->transform.position.y += (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? m_speed * -dt : ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? m_speed * dt : 0) / 2;
        actor->transform.position.z += (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_Q) ? m_speed * -dt : ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_E) ? m_speed * dt : 0) / 2;
        m_time += dt;
        auto material = actor->GetComponent<ModelComponent>()->m_model->GetMaterial();
        material->ProcessGUI();
        material->Bind();
        material->GetProgram()->SetUniform("model", actor->transform.GetMatrix());

        glm::mat4 view = glm::lookAt(glm::vec3({ 0, 0, 3 }), glm::vec3({ 0, 0, 0 }), glm::vec3({ 0, 1, 0 }));
        material->GetProgram()->SetUniform("view", view);
        glm::mat4 projection = glm::perspective(glm::radians(90.0f), float(ENGINE.GetSystem<Renderer>()->GetWidth()) / ENGINE.GetSystem<Renderer>()->GetHeight(), 0.01f, 1000.0f);
        
        material->GetProgram()->SetUniform("projection", projection);
        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World05::Draw(Renderer& renderer)
    {
        // pre-render
        renderer.BeginFrame();
        
        //light gui


        // render
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        m_scene->Draw(renderer);

        ENGINE.GetSystem<Gui>()->Draw();

        // post-render
        renderer.EndFrame();
    }
}
