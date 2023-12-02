#include "World06.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "Input/InputSystem.h"
#include <glm/glm/gtx/color_space.hpp>

#define INTERLEAVE


namespace nc
{
    bool World06::Initialize()
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
        m_scene->Load("scenes/scene_framebuffer.json");
        m_scene->Initialize();

        auto texture = std::make_shared<Texture>();
        texture->CreateTexture(512, 512);
        auto framebuffer = std::make_shared<Framebuffer>();
        framebuffer->CreateFramebuffer(texture);
        ADD_RESOURCE("fb_texture", texture);
        ADD_RESOURCE("fb", framebuffer);

        auto material = GET_RESOURCE(Material, "materials/post_process.mtrl");
        if (material)
        {
            material->albedoTexture = texture;
        }
        /*
        for(int i = 0; i < 1; i++)
        {
            auto actor = CREATE_CLASS_BASE(Actor, "actor3");
            actor->name = createUnique("tree");
            actor->transform.position = glm::vec3(randomf(-10, 10), randomf(-10, 10), randomf(-10, 10));
            actor->transform.scale = glm::vec3(randomf(0.3, 10), randomf(0.3, 10), 1);
            actor->Initialize();
            m_scene->Add(std::move(actor));
        }*/
        /*
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
        }

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
        {
            auto actor = CREATE_CLASS(Actor);
            actor->name = "camera1";
            actor->transform.position = glm::vec3{ 0, 0, 0 };
            actor->transform.rotation = glm::vec3{ 0, 180, 0 };

            auto cameraComponent = CREATE_CLASS(CameraComponent);
            cameraComponent->SetPerspective(70.0f, ENGINE.GetSystem<Renderer>()->GetWidth() / (float)ENGINE.GetSystem<Renderer>()->GetHeight(), 0.1f, 100.0f);
            actor->AddComponent(std::move(cameraComponent));
            auto cameraController = CREATE_CLASS(CameraController);
            cameraController->speed = 5;
            cameraController->sensitivity = 0.5f;
            cameraController->m_owner = actor.get();
            cameraController->Initialize();
            actor->AddComponent(std::move(cameraController));
            m_scene->Add(std::move(actor));
        }*/
        return true;
    }

    void World06::Shutdown()
    {
    }

    void World06::Update(float dt)
    {

        ENGINE.GetSystem<Gui>()->BeginFrame();
        m_scene->Update(dt);
        m_scene->ProcessGui();
        /*
        auto actor = m_scene->GetActorByName<Actor>("actor2");
        actor->transform.position.x += (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? m_speed * -dt : ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? m_speed * dt : 0) / 2;
        actor->transform.position.y += (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? m_speed * -dt : ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? m_speed * dt : 0) / 2;
        actor->transform.position.z += (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_Q) ? m_speed * -dt : ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_E) ? m_speed * dt : 0) / 2;
        m_time += dt;
        auto material = actor->GetComponent<ModelComponent>()->material;
        material->ProcessGUI();
        material->Bind();
        material->GetProgram()->SetUniform("model", actor->transform.GetMatrix());
        */
        auto material = GET_RESOURCE(Material, "materials/refraction.mtrl");
        if (material)
        {
            ImGui::Begin("Refraction");
            ImGui::DragFloat("IOR", &m_refraction, 0.01f, 1, 3);
            auto program = material->GetProgram();
            program->Use();
            program->SetUniform("ior", m_refraction);
            ImGui::End();
        }

        ImGui::Begin("Post-Process");
        ImGui::SliderFloat("Blend", &blend, 0, 1);
        bool effect = params & INVERT_MASK;
        if (ImGui::Checkbox("Invert", &effect))
        {
            if (effect)
            {
                params |= INVERT_MASK;
            }
            else
            {
                params &= ~INVERT_MASK;
            }
        }
        effect = params & GRAYSCALE_MASK;
        if (ImGui::Checkbox("Grayscale", &effect))
        {
            if (effect)
            {
                params |= GRAYSCALE_MASK;
            }
            else
            {
                params &= ~GRAYSCALE_MASK;
            }
        }
        effect = params & COLOR_TINT_MASK;
        if (ImGui::Checkbox("Color Tint", &effect))
        {
            if (effect)
            {
                params |= COLOR_TINT_MASK;
            }
            else
            {
                params &= ~COLOR_TINT_MASK;
            }
        }
        if (effect)
        {
            ImGui::ColorEdit4("Color", glm::value_ptr(tintColor));
        }
        effect = params & GRAIN_MASK;
        if (ImGui::Checkbox("Grain", &effect))
        {
            if (effect)
            {
                params |= GRAIN_MASK;
            }
            else
            {
                params &= ~GRAIN_MASK;
            }
        }
        effect = params & SCANLINE_MASK;
        if (ImGui::Checkbox("Scanline", &effect))
        {
            if (effect)
            {
                params |= SCANLINE_MASK;
            }
            else
            {
                params &= ~SCANLINE_MASK;
            }
        }
        effect = params & MIRROR_MASK;
        if (ImGui::Checkbox("Mirror", &effect))
        {
            if (effect)
            {
                params |= MIRROR_MASK;
            }
            else
            {
                params &= ~MIRROR_MASK;
            }
        }
        ImGui::End();

        auto program = GET_RESOURCE(Program, "shaders/post_process.prog");
        if(program)
        {
            program->Use();
            program->SetUniform("blend", blend);
            program->SetUniform("params", params);
            program->SetUniform("tintColor", tintColor);
            program->SetUniform("height", (float)ENGINE.GetSystem<Renderer>()->GetHeight());
        }
        /*
        glm::mat4 view = glm::lookAt(glm::vec3({ 0, 0, 3 }), glm::vec3({ 0, 0, 0 }), glm::vec3({ 0, 1, 0 }));
        material->GetProgram()->SetUniform("view", view);
        glm::mat4 projection = glm::perspective(glm::radians(90.0f), float(ENGINE.GetSystem<Renderer>()->GetWidth()) / ENGINE.GetSystem<Renderer>()->GetHeight(), 0.01f, 1000.0f);
        
        material->GetProgram()->SetUniform("projection", projection);*/
        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World06::Draw(Renderer& renderer)
    {
        m_scene->GetActorByName("cube")->active = false;
        auto framebuffer = GET_RESOURCE(Framebuffer, "fb");
        renderer.SetViewport(framebuffer->GetSize().x, framebuffer->GetSize().y);
        framebuffer->Bind();
        // pre-render
        renderer.BeginFrame();
        
        //light gui


        // render
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        m_scene->Draw(renderer);

        framebuffer->Unbind();
        renderer.ResetViewport();
        m_scene->GetActorByName("cube")->active = true;

        renderer.BeginFrame();
        m_scene->Draw(renderer);
        ENGINE.GetSystem<Gui>()->Draw();

        // post-render
        renderer.EndFrame();

    }
}
