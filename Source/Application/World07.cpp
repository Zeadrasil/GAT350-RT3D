#include "World07.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "Input/InputSystem.h"
#include <glm/glm/gtx/color_space.hpp>
#include "Framework/Editor.h"

#define INTERLEAVE


namespace nc
{
    bool World07::Initialize()
    {
        //m_material = GET_RESOURCE(Material, "materials/quad.mtrl");
        /*
        m_program = GET_RESOURCE(Program, "shaders/unlit_texture.prog");
        m_program->Use();

        m_texture = GET_RESOURCE(Texture, "textures/llama.jpg");
        m_texture->Bind();
        m_texture->SetActive(GL_TEXTURE0);
        */
        m_editor = std::make_unique<Editor>();
        m_scene = std::make_unique<Scene>();
        m_scene->Load("scenes/skyscene.json");
        m_scene->Load("scenes/scene_editor.json");
        m_scene->Load("scenes/shadowscene.json");
        m_scene->Initialize();



        //create depth texture
        auto texture = std::make_shared<Texture>();
        texture->CreateDepthTexture(32768, 32768);
        auto framebuffer = std::make_shared<Framebuffer>();
        framebuffer->CreateDepthbuffer(texture);
        ADD_RESOURCE("depth_texture", texture);
        ADD_RESOURCE("depth_buffer", framebuffer);

        auto material = GET_RESOURCE(Material, "materials/post_process.mtrl");
        if (material)
        {
            material->albedoTexture = texture;
        }
        
        for(int i = 0; i < 210; i+=7)
        {
            for (int j = 0; j < 115; j+=7)
            {
                auto actor = CREATE_CLASS_BASE(Actor, "tree1");
                int type = random(3);
                if (type == 1)
                {
                    actor = CREATE_CLASS_BASE(Actor, "tree2");
                }
                else
                {
                    actor = CREATE_CLASS_BASE(Actor, "tree3");
                }
                actor->name = createUnique("tree");
                actor->transform.position = glm::vec3(randomf(-2, 2) + i - 50, -1, randomf( -2, 2) - j - 120);
                actor->transform.scale = glm::vec3(randomf(0.5, 2), randomf(0.5, 2), 1);
                actor->Initialize();
                m_scene->Add(std::move(actor));
            }
        }
        auto materials = GET_RESOURCES(Material);
        for (auto material : materials)
        {
            material->depthTexture = texture;
        }
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

    void World07::Shutdown()
    {
    }

    void World07::Update(float dt)
    {
        m_editor->Update();
        ENGINE.GetSystem<Gui>()->BeginFrame();
        m_scene->Update(dt);
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
        auto material = GET_RESOURCE(Material, "materials/shrek.mtrl");
        if (material)
        {
            ImGui::Begin("Shading");
            ImGui::DragInt("Cel Bands", &m_celBands, 1, 0, 10);
            auto program = material->GetProgram();
            program->Use();
            program->SetUniform("celBands", m_celBands);
            ImGui::End();
        }
        material = GET_RESOURCE(Material, "materials/refraction.mtrl");
        if (material)
        {
            ImGui::Begin("Refraction");
            ImGui::DragFloat("IOR", &refraction, 0.01f, 1, 3);
            auto program = material->GetProgram();
            program->Use();
            program->SetUniform("ior", refraction);
            ImGui::End();
        }

        m_editor->ProcessGui(m_scene.get());

        /*
        glm::mat4 view = glm::lookAt(glm::vec3({ 0, 0, 3 }), glm::vec3({ 0, 0, 0 }), glm::vec3({ 0, 1, 0 }));
        material->GetProgram()->SetUniform("view", view);
        glm::mat4 projection = glm::perspective(glm::radians(90.0f), float(ENGINE.GetSystem<Renderer>()->GetWidth()) / ENGINE.GetSystem<Renderer>()->GetHeight(), 0.01f, 1000.0f);
        
        material->GetProgram()->SetUniform("projection", projection);*/
        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World07::Draw(Renderer& renderer)
    {
        /*auto skybox = m_scene->GetActorByName("skybox");
        glDepthMask(false);
        skybox->Draw(renderer);
        glDepthMask(true);*/
        // PASS 1
        auto framebuffer = GET_RESOURCE(Framebuffer, "depth_buffer");
        renderer.SetViewport(framebuffer->GetSize().x, framebuffer->GetSize().y);
        framebuffer->Bind();

        renderer.ClearDepth();
        auto program = GET_RESOURCE(Program, "shaders/shadow_depth.prog");
        program->Use();

        auto lights = m_scene->GetComponents<LightComponent>();
        for (auto light : lights)
        {
            if (light->castShadow)
            {
                glm::mat4 shadowMatrix = light->getShadowMatrix();
                program->SetUniform("shadowVP", shadowMatrix);
            }
        }
        auto models = m_scene->GetComponents<ModelComponent>();
        for (auto model : models)
        {
            if (model->castShadow)
            {
            	glCullFace(GL_FRONT);
                program->SetUniform("model", model->m_owner->transform.GetMatrix());
                model->m_model->Draw();
            }
        }
        framebuffer->Unbind();
        renderer.ResetViewport();
        // PASS 3
        renderer.BeginFrame();
        m_scene->Draw(renderer);
        ENGINE.GetSystem<Gui>()->Draw();
        renderer.EndFrame();
    }
}
