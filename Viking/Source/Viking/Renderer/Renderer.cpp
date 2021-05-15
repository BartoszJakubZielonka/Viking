//
// Created by batzi on 15.05.2021.
//
#include "vipch.h"
#include "Viking/Renderer/Renderer.h"
#include "Viking/Renderer/Renderer2D.h"

namespace Viking {
    Scope<Renderer::SceneData> Renderer::sSceneData = createScope<Renderer::SceneData>();

    void Renderer::init() {
        VI_PROFILE_FUNCTION();

        RenderCommand::init();
        Renderer2D::init();
    }

    void Renderer::shutdown() {
        Renderer2D::shutdown();
    }

    void Renderer::onWindowResize(uint32_t width, uint32_t height) {
        RenderCommand::setViewport(0, 0, width, height);
    }

    void Renderer::beginScene(OrthographicCamera &camera) {
        sSceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::endScene() {

    }

    void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
    {
        shader->bind();
        shader->setMat4("u_ViewProjection", sSceneData->ViewProjectionMatrix);
        shader->setMat4("u_Transform", transform);

        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
}