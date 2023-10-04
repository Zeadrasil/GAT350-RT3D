#include "World01.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "Input/InputSystem.h"


namespace nc
{
    bool World01::Initialize()
    {

        return true;
    }

    void World01::Shutdown()
    {
    }

    void World01::Update(float dt)
    {
        m_angle = dt * 10;
        m_position.x += (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? -dt : ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? dt : 0) / 10;
        m_position.x *= 0.9;
        m_position.y += (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? -dt : ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? dt : 0) / 10;
        m_position.y *= 0.9;
    }

    void World01::Draw(Renderer& renderer)
    {
        // pre-render
        renderer.BeginFrame();
        
        // render

        /*for (int i = 0; i < 10000; i++)
        {
            renderer.DrawLine(random(renderer.GetWidth() + 150) - 90, random(renderer.GetHeight() + 150) - 90, random(renderer.GetWidth()+ 150) - 90, random(renderer.GetHeight())+150 - 90);
            renderer.SetColor(random(255), random(255), random(255), 255);
        }*/
        glPushMatrix();
        glTranslatef(m_position.x, m_position.y, 0);
        glRotatef(m_angle, 0, 0, 1);
        //for (int i = 0; i < 100; i++)
        //{
            glBegin(GL_TRIANGLES);

            glColor4f(randomf(), randomf(), randomf(), randomf());
            glVertex2f(-0.5, -0.5);
            glColor4f(randomf(), randomf(), randomf(), randomf());
            glVertex2f(0, -0.5);
            glColor4f(randomf(), randomf(), randomf(), randomf());
            glVertex2f(0, 0);

            glEnd();
        //}
        //glPopMatrix();
        
        // post-render
        renderer.EndFrame();
    }
}
