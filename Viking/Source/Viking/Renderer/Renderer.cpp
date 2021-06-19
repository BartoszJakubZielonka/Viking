//
// Created by batzi on 15.05.2021.
//
#include "vipch.h"
#include "Viking/Renderer/Renderer.h"

#include "UniformBuffer.h"
#include "glad/gl.h"
#include "Viking/Renderer/Renderer2D.h"

namespace Viking {
    Scope<Renderer::SceneData> Renderer::sSceneData = createScope<SceneData>();

    struct RendererData
    {
        static const uint32_t MAX_TEXTURE_SLOTS{ 32 }; //TODO RenderCaps.

        Ref<Shader> textureShader{ nullptr };

        Ref<Texture2D> whitTexture{ nullptr };

        std::array<Ref<Texture2D>, MAX_TEXTURE_SLOTS> textureSlots;
        uint32_t textureSlotIndex{ 1 }; // 0 -> whiteTexture

        glm::mat4 mViewProj = glm::mat4();

        struct CameraData
        {
            glm::mat4 viewProjection;
        };
        CameraData cameraBuffer;
        Ref<UniformBuffer> cameraUniformBuffer;
    };

    static RendererData sData;

    void Renderer::init() {
        VI_PROFILE_FUNCTION();

        RenderCommand::init();
        Renderer2D::init();

        sData.textureShader = Shader::create(R"(assets/shaders/Texture.glsl)");

        sData.whitTexture = Texture2D::create(1, 1);
        auto whiteTextureData{ 0xffffffff };
        sData.whitTexture->setData(&whiteTextureData, sizeof uint32_t);

        int32_t samplers[RendererData::MAX_TEXTURE_SLOTS];
        for(uint32_t i = 0; i < RendererData::MAX_TEXTURE_SLOTS; i++)
            samplers[i] = i;

        sData.textureShader->bind();
        sData.textureShader->setInt("u_DiffuseTexture", 0);

        sData.cameraUniformBuffer = UniformBuffer::create(sizeof(RendererData::CameraData), 0);
    }

    void Renderer::shutdown() {
        Renderer2D::shutdown();
    }

    void Renderer::onWindowResize(uint32_t width, uint32_t height) {
        RenderCommand::setViewport(0, 0, width, height);
    }

    void Renderer::beginScene(const Camera& camera, const glm::mat4& transform)
    {
        VI_PROFILE_FUNCTION();
    }

    void Renderer::beginScene(const EditorCamera& camera)
    {
        VI_PROFILE_FUNCTION();
        sData.textureShader->bind();
        sData.textureShader->setMat4("u_ViewProj", camera.getViewProjection());
        sData.mViewProj = camera.getViewProjection();
    }

    void Renderer::endScene()
    {
        VI_PROFILE_FUNCTION();
        sData.textureShader->unbind();
    }

    void Renderer::submitMesh(const glm::mat4& transform, MeshRendererComponent& src, int entityID)
    {
        VI_PROFILE_FUNCTION();
        drawMesh(src.mesh, transform, entityID);
    }

    void Renderer::submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
    {
        shader->bind();
        shader->setMat4("u_ViewProjection", sSceneData->ViewProjectionMatrix);
        shader->setMat4("u_Transform", transform);

        vertexArray->bind();
        RenderCommand::drawIndexed(vertexArray);
    }

    void Renderer::drawMesh(const Ref<Mesh>& mesh, const glm::mat4& transform, int32_t entityId)
    {
        VI_PROFILE_FUNCTION();
        mesh->mVertexBuffer->bind();
        mesh->mVertexArray->bind();
        mesh->mIndexBuffer->bind();

        sData.cameraBuffer.viewProjection = sData.mViewProj * glm::inverse(transform);
        sData.cameraUniformBuffer->setData(&sData.cameraBuffer, sizeof(RendererData::CameraData));

        sData.whitTexture->bind();
        sData.textureShader->setFloat4("u_Color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        sData.textureShader->setInt("u_EntityId", entityId);

        for (auto& submesh : mesh->mSubMeshes)
        {
            sData.textureShader->setMat4("u_Model", transform * submesh.transform);

            glDrawElementsBaseVertex(GL_TRIANGLES, submesh.indexCount, GL_UNSIGNED_INT, reinterpret_cast<void*>(sizeof(uint32_t) * submesh.baseIndex), submesh.baseVertex);
        }

        mesh->mVertexBuffer->unbind();
        mesh->mVertexArray->unbind();
        mesh->mIndexBuffer->unbind();
    }
}
