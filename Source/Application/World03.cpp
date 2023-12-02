#include "World03.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "Input/InputSystem.h"

#define INTERLEAVE


namespace nc
{
    bool World03::Initialize()
    {
        m_material = GET_RESOURCE(Material, "materials/quad.mtrl");
        /*
        m_program = GET_RESOURCE(Program, "shaders/unlit_texture.prog");
        m_program->Use();

        m_texture = GET_RESOURCE(Texture, "textures/llama.jpg");
        m_texture->Bind();
        m_texture->SetActive(GL_TEXTURE0);
        */
        #ifdef INTERLEAVE

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
        m_vertexBuffer->SetAttribute(2, 2, 8 * sizeof(GLfloat), 6 * sizeof(float));  // texcoord
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
        m_transform.rotation.z = 180;
        return true;
    }

    void World03::Shutdown()
    {
    }

    void World03::Update(float dt)
    {
        ENGINE.GetSystem<Gui>()->BeginFrame();
        /*ImGui::Begin("Transform");
        ImGui::DragFloat3("Position", &m_transform.position[0]);
        ImGui::DragFloat3("Scale", &m_transform.scale[0]);
        ImGui::DragFloat3("Rotation", &m_transform.rotation[0]);
        ImGui::DragFloat2("Offset", &offset[0]);
        ImGui::DragFloat2("Tiling", &tiling[0]);
        ImGui::End();*/
        m_transform.position.x += (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? m_speed * -dt : ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? m_speed * dt : 0) / 2;
        m_transform.position.y += (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? m_speed * -dt : ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? m_speed * dt : 0) / 2;
        m_transform.position.z += (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_Q) ? m_speed * -dt : ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_E) ? m_speed * dt : 0) / 2;
        m_time += dt;

        m_material->ProcessGui();
        m_material->Bind();
        //m_program->SetUniform("offset", offset * m_time);
        //m_program->SetUniform("tiling", tiling);
        m_material->GetProgram()->SetUniform("model", m_transform.GetMatrix());
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
        m_material->GetProgram()->SetUniform("view", view);
        glm::mat4 projection = glm::perspective(glm::radians(90.0f), 800.0f / 600.0f, 0.01f, 1000.0f);
        //m_program->SetUniform("projection", projection);
        m_material->GetProgram()->SetUniform("projection", projection);
        //uniform = glGetUniformLocation(m_program->m_program, "projection");
        //glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(projection));

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World03::Draw(Renderer& renderer)
    {
        // pre-render
        renderer.BeginFrame();
        
        // render
        m_vertexBuffer->Draw(GL_TRIANGLE_STRIP);

        ENGINE.GetSystem<Gui>()->Draw();

        // post-render
        renderer.EndFrame();
    }
}
