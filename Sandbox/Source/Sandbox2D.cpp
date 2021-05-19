#include "Sandbox2D.h"
#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Sandbox2D::Sandbox2D(): Layer("Sandbox2D"), mCameraController( 1280.0f / 720.0f), mSquareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
{
}

void Sandbox2D::onAttach()
{
    VI_PROFILE_FUNCTION();
    mCheckerboardTexture = Viking::Texture2D::create("assets/textures/Checkerboard.png");
}

void Sandbox2D::onDetach()
{
    VI_PROFILE_FUNCTION();
}

void Sandbox2D::onUpdate(Viking::Timestep ts)
{
    VI_PROFILE_FUNCTION();

    //Update
    mCameraController.onUpdate(ts);

    //Render
    Viking::Renderer2D::resetStats();
    {
        VI_PROFILE_SCOPE("Renderer prep");
        Viking::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        Viking::RenderCommand::clear();
    }

    {
        static float rotation = 0.0f;
        rotation += ts * 50.0f;

        VI_PROFILE_SCOPE("Renderer Draw");
        Viking::Renderer2D::beginScene(mCameraController.GetCamera());
        Viking::Renderer2D::drawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, -45.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
        Viking::Renderer2D::drawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
        Viking::Renderer2D::drawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, mSquareColor);
        Viking::Renderer2D::drawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, mCheckerboardTexture, 10.0f);
        Viking::Renderer2D::drawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, mCheckerboardTexture, 20.0f);
        Viking::Renderer2D::endScene();

        Viking::Renderer2D::beginScene(mCameraController.getCamera());
        for (float y = -5.0f; y < 5.0f; y += 0.5f)
        {
            for (float x = -5.0f; x < 5.0f; x += 0.5f)
            {
                glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
                Viking::Renderer2D::drawQuad({ x, y }, { 0.45f, 0.45f }, color);
            }
        }
        Viking::Renderer2D::endScene();
    }
}

void Sandbox2D::onImGuiRender()
{
    VI_PROFILE_FUNCTION();

    ImGui::Begin("Settings");

    auto stats = Viking::Renderer2D::getStats();
    ImGui::Text("Renderer2D Stats:");
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quads: %d", stats.QuadCount);
    ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
    ImGui::Text("Indices: %d", stats.getTotalIndexCount());

    ImGui::ColorEdit4("Square Color", glm::value_ptr(mSquareColor));
    ImGui::End();
}

void Sandbox2D::onEvent(Viking::Event& event)
{
    mCameraController.onEvent(event);
}
