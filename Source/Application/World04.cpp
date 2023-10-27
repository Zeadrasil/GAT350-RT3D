#include "World04.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "Input/InputSystem.h"
#include <glm/glm/gtx/color_space.hpp>

#define INTERLEAVE


namespace nc
{
    bool World04::Initialize()
    {
        //m_material = GET_RESOURCE(Material, "materials/quad.mtrl");
        /*
        m_program = GET_RESOURCE(Program, "shaders/unlit_texture.prog");
        m_program->Use();

        m_texture = GET_RESOURCE(Texture, "textures/llama.jpg");
        m_texture->Bind();
        m_texture->SetActive(GL_TEXTURE0);
        */

        auto material = GET_RESOURCE(Material, "materials/squirrel.mtrl");
        m_model = std::make_shared<Model>();
        m_model->SetMaterial(material);
        m_model->Load("models/squirrel.glb", glm::vec3{0}, glm::vec3{90, 0, 0});

        for (int i = 0; i < 3; i++)
        {
            light_t light;
            light.type = light_t::lightType::Point;
            light.color = glm::vec3(1);
            light.direction = glm::vec3{ 0, -1, 0 };
            light.position = glm::vec3(0);
            light.intensity = 1;
            light.range = 5;
            light.innerAngle = glm::radians(10.0f);
            light.outerAngle = glm::radians(30.0f);
            m_lights.push_back(light);
        }

        #ifdef INTERLEAVE
        /*
        float vertexData[] = {
            -0.1f, -0.1f, 0.0f, 0.9f, 0.3f, 0.1f, 0.0f, 0.0f,
             -0.1f, 0.1f, 0.0f, 0.1f, 0.9f, 0.3f, 0.0f, 1.0f,
             0.1f,  -0.1f, 0.0f, 0.3f, 0.1f, 0.9f, 1.0f, 0.0f,
             0.1f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
        };
        float colorData[] =
        {
            0.9f, 0.3f, 0.1f,
            0.1f, 0.9f, 0.3f,
            0.3f, 0.1f, 0.9f,
            0.0f, 0.0f, 0.0f
        };
        m_vertexBuffer =   GET_RESOURCE(VertexBuffer, "vb");
        m_vertexBuffer->CreateVertexBuffer(sizeof(vertexData), 4, vertexData);
        m_vertexBuffer->SetAttribute(0, 3, 8 * sizeof(GLfloat), 0);                  // position 
        m_vertexBuffer->SetAttribute(1, 3, 8 * sizeof(GLfloat), 3 * sizeof(float));  // color 
        m_vertexBuffer->SetAttribute(2, 2, 8 * sizeof(GLfloat), 6 * sizeof(float));  // texcoord*/
        /*
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glEnableVertexAttribArray(0);
        glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
        glBindVertexBuffer(0, vbo, 0, 8 * sizeof(GLfloat));
        glVertexAttribBinding(0, 0);

        glEnableVertexAttribArray(1);
        glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
        glVertexAttribBinding(1, 0);

        glEnableVertexAttribArray(2);
        glVertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float));
        glVertexAttribBinding(2, 0);
        */
        #else

        //vertex data
        float positionData[] = {
            -0.1f, -0.1f, 0.0f,
             0.1f, -0.1f, 0.0f,
             0.1f,  0.1f, 0.0f,
             -0.1f, 0.1f, 0.0f
        };
        float colorData[] =
        {
            0.9f, 0.3f, 0.1f,
            0.1f, 0.9f, 0.3f,
            0.3f, 0.1f, 0.9f,
            0.0f, 0.0f, 0.0f
        };

        GLuint vbo[2];
        glGenBuffers(2, vbo);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(positionData), positionData, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glBindVertexBuffer(0, vbo[0], 0, 3 * sizeof(GLfloat));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glBindVertexBuffer(1, vbo[1], 0, 3 * sizeof(GLfloat));
    #endif 
        
        m_transform.position.z = -1.0f;
        return true;
    }

    void World04::Shutdown()
    {
    }

    void World04::Update(float dt)
    {
        ENGINE.GetSystem<Gui>()->BeginFrame();
        ImGui::Begin("Scene");
        ImGui::ColorEdit3("Ambient Color", glm::value_ptr(ambient));
        ImGui::Separator();

        for (int i = 0; i < 3; i++)
        {
            std::string name = "light" + std::to_string(i);
            if (ImGui::Selectable(name.c_str(), selected == i)) selected = i;
        }
        ImGui::End();
        ImGui::Begin("Lighting");
        const char* types[] = {"Point", "Directional", "Spot"};
        ImGui::Combo("Type", (int*)(&m_lights.at(selected).type), types, 3);
        ImGui::DragFloat("Intensity", &m_lights.at(selected).intensity, 0.01f);
        ImGui::ColorEdit3("Color", glm::value_ptr(m_lights.at(selected).color));
        if (m_lights.at(selected).type != light_t::Directional)
        {
            ImGui::DragFloat3("Position", glm::value_ptr(m_lights.at(selected).position), 0.1f);
            ImGui::DragFloat("Range", &m_lights.at(selected).range, 0.01f);
        }
        if(m_lights.at(selected).type != light_t::Point)ImGui::DragFloat3("Direction", glm::value_ptr(m_lights.at(selected).direction), 0.1f);
        if (m_lights.at(selected).type == light_t::Spot)
        {
            ImGui::DragFloat("Inner Angle", &m_lights.at(selected).innerAngle, 0.01f);
            ImGui::DragFloat("OuterAngle", &m_lights.at(selected).outerAngle, 0.01f);
        };
        ImGui::End();
        m_transform.position.x += (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? m_speed * -dt : ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? m_speed * dt : 0) / 2;
        m_transform.position.y += (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? m_speed * -dt : ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? m_speed * dt : 0) / 2;
        m_transform.position.z += (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_Q) ? m_speed * -dt : ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_E) ? m_speed * dt : 0) / 2;
        m_transform.rotation.x += (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_LEFT) ? m_speed * -dt : ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_RIGHT) ? m_speed * dt : 0);
        m_transform.rotation.y += (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_DOWN) ? m_speed * -dt : ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_UP) ? m_speed * dt : 0);
        m_transform.rotation.z += (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_PAGEUP) ? m_speed * -dt : ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_PAGEDOWN) ? m_speed * dt : 0);
        m_time += dt;
        auto material = m_model->GetMaterial();
        material->ProcessGUI();
        material->Bind();
        //m_material->ProcessGUI();
        //m_material->Bind();
        //m_program->SetUniform("offset", offset * m_time);
        //m_program->SetUniform("tiling", tiling);
        material->GetProgram()->SetUniform("model", m_transform.GetMatrix());
        /*
        glm::mat4 position = glm::translate(glm::mat4({ 1 }), m_transform.position);
        glm::mat4 rotation = glm::rotate(glm::mat4({ 1 }), glm::radians(m_transform.rotation.x), glm::vec3({ 1,0,0 }));
        glm::mat4 scale = glm::scale(glm::mat4({ 1 }), m_transform.scale);
        rotation *= glm::rotate(glm::mat4({ 1 }), glm::radians(m_transform.rotation.y), glm::vec3({ 0,1,0 }));
        rotation *= glm::rotate(glm::mat4({ 1 }), glm::radians(m_transform.rotation.z), glm::vec3({ 0,0,1 }));
        glm::mat4 model = position * rotation * scale;*/
        /*
        GLint uniform = glGetUniformLocation(m_program->m_program, "model");
        glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(model));
        */
        //m_program->SetUniform("model", m_transform.GetMatrix());

        glm::mat4 view = glm::lookAt(glm::vec3({ 0, 0, 3 }), glm::vec3({ 0, 0, 0 }), glm::vec3({ 0, 1, 0 }));
        //m_program->SetUniform("view", view);
        //uniform = glGetUniformLocation(m_program->m_program, "view");
        //glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(view));
        material->GetProgram()->SetUniform("view", view);
        glm::mat4 projection = glm::perspective(glm::radians(90.0f), float(ENGINE.GetSystem<Renderer>()->GetWidth()) / ENGINE.GetSystem<Renderer>()->GetHeight(), 0.01f, 1000.0f);
        //m_program->SetUniform("projection", projection);
        material->GetProgram()->SetUniform("projection", projection);
        //uniform = glGetUniformLocation(m_program->m_program, "projection");
        //glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(projection));
        for (int i = 0; i < m_lights.size(); i++)
        {
            std::string name = "lights[" + std::to_string(i) + "]";
            material->GetProgram()->SetUniform(name + ".diffuse", m_lights.at(i).color);
            material->GetProgram()->SetUniform(name + ".type", m_lights.at(i).type);
            if (m_lights.at(i).type != light_t::Directional)
            {
                material->GetProgram()->SetUniform(name + ".position", m_lights.at(i).position);
                material->GetProgram()->SetUniform(name + ".range", m_lights.at(i).range);
            }
            if (m_lights.at(i).type != light_t::Point) material->GetProgram()->SetUniform(name + ".direction", m_lights.at(i).direction);
            material->GetProgram()->SetUniform(name + ".intensity", m_lights.at(i).intensity);
            if (m_lights.at(i).type == light_t::Spot)
            {
                material->GetProgram()->SetUniform(name + ".innerAngle", m_lights.at(i).innerAngle);
                material->GetProgram()->SetUniform(name + ".outerAngle", m_lights.at(i).outerAngle);
            }
        }
        material->GetProgram()->SetUniform("ambient", ambient);
        material->GetProgram()->SetUniform("numLights", (int)m_lights.size());



        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World04::Draw(Renderer& renderer)
    {
        // pre-render
        renderer.BeginFrame();
        
        //light gui


        // render
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        m_model->Draw();

        ENGINE.GetSystem<Gui>()->Draw();

        // post-render
        renderer.EndFrame();
    }
}
