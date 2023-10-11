#include "World03.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "Input/InputSystem.h"

#define INTERLEAVE


namespace nc
{
    bool World03::Initialize()
    {
        m_program = GET_RESOURCE(Program, "shaders/unlit_color.prog");
        m_program->Use();

        #ifdef INTERLEAVE

        float vertexData[] = {
            -0.1f, -0.1f, 0.0f, 0.9f, 0.3f, 0.1f,
             -0.1f, 0.1f, 0.0f, 0.1f, 0.9f, 0.3f,
             0.1f,  -0.1f, 0.0f, 0.3f, 0.1f, 0.9f,
             0.1f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f
        };
        float colorData[] =
        {
            0.9f, 0.3f, 0.1f,
            0.1f, 0.9f, 0.3f,
            0.3f, 0.1f, 0.9f,
            0.0f, 0.0f, 0.0f
        };

        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glEnableVertexAttribArray(0);
        glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
        glBindVertexBuffer(0, vbo, 0, 6 * sizeof(GLfloat));
        glVertexAttribBinding(0, 0);

        glEnableVertexAttribArray(1);
        glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
        glBindVertexBuffer(1, vbo, 0, 6 * sizeof(GLfloat));
        glVertexAttribBinding(1, 1);
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

        m_transform.position.z = -10.0f;
        return true;
    }

    void World03::Shutdown()
    {
    }

    void World03::Update(float dt)
    {
        ENGINE.GetSystem<Gui>()->BeginFrame();
        ImGui::Begin("Transform");
        ImGui::DragFloat3("Position", &m_transform.position[0]);
        ImGui::DragFloat3("Scale", &m_transform.scale[0]);
        ImGui::End();
        m_transform.rotation.x = dt * 10;
        m_transform.position.x += (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? m_speed * -dt : ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? m_speed * dt : 0) / 2;
        m_transform.position.y += (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? m_speed * -dt : ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? m_speed * dt : 0) / 2;
        m_transform.position.z += (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_Q) ? m_speed * -dt : ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_E) ? m_speed * dt : 0) / 2;
        m_time += dt;


        
        glm::mat4 position = glm::translate(glm::mat4({ 1 }), m_transform.position);
        glm::mat4 rotation = glm::rotate(glm::mat4({ 1 }), m_time, glm::vec3({ 0,0,1 }));
        glm::mat4 model = position * rotation;
        GLint uniform = glGetUniformLocation(m_program->m_program, "model");
        glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(model));

        //m_program->SetUniform("model", m_transform.GetMatrix());

        glm::mat4 view = glm::lookAt(glm::vec3({ 0, 4, 5 }), glm::vec3({ 0, 0, 0 }), glm::vec3({ 0, 1, 0 }));
        //m_program->SetUniform("view", view);
        uniform = glGetUniformLocation(m_program->m_program, "view");
        glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(view));

        glm::mat4 projection = glm::perspective(glm::radians(90.0f), 800.0f / 600.0f, 0.01f, 1000.0f);
        //m_program->SetUniform("projection", projection);
        uniform = glGetUniformLocation(m_program->m_program, "projection");
        glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(projection));

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World03::Draw(Renderer& renderer)
    {
        // pre-render
        renderer.BeginFrame();
        
        // render
        glBindVertexArray(m_vao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        ENGINE.GetSystem<Gui>()->Draw();

        // post-render
        renderer.EndFrame();
    }
}
