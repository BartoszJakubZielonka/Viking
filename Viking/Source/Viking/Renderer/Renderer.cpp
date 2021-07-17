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

    struct MeshVertex {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;
        float TexIndex;
        float TilingFactor;

        // Editor-only
        int EntityID;
    };

    struct RendererData
    {
        static const uint32_t MAX_TEXTURE_SLOTS{ 32 }; //TODO RenderCaps.

        Ref<Shader> meshShader{ nullptr };

        Ref<Texture2D> whitTexture{ nullptr };
        Ref<Texture2D> blackTexture{ nullptr };

        Ref<Library<Shader>> ShaderLibrary{nullptr};

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

        //Shader library
        sData.ShaderLibrary = createRef<Library<Shader>>();
        sData.ShaderLibrary->add("testShader", "assets/shaders/testShader.glsl");
        sData.ShaderLibrary->add("meshShader", "assets/shaders/meshShader.glsl");

        sData.meshShader = sData.ShaderLibrary->get("testShader");

        sData.whitTexture = Texture2D::create(1, 1);
        auto whiteTextureData{ 0xffffffff };
        sData.whitTexture->setData(&whiteTextureData, sizeof uint32_t);

        sData.whitTexture = Texture2D::create(1, 1);
        auto blackTextureData{ 0x000000ff };
        sData.whitTexture->setData(&blackTextureData, sizeof uint32_t);

        int32_t samplers[RendererData::MAX_TEXTURE_SLOTS];
        for(uint32_t i = 0; i < RendererData::MAX_TEXTURE_SLOTS; i++)
            samplers[i] = i;

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
        sData.meshShader->bind();
        sData.meshShader->setUniform("u_ViewProjection", camera.getViewProjection());
        sData.mViewProj = camera.getViewProjection();
    }

    void Renderer::endScene()
    {
        VI_PROFILE_FUNCTION();
    }

    void Renderer::submitMesh(const glm::mat4& transform, MeshRendererComponent& src, int entityID)
    {
        VI_PROFILE_FUNCTION();
        drawMesh(src.mesh, transform, entityID);
    }

    Ref<Library<Shader>> Renderer::getShaderLibrary()
    {
        return sData.ShaderLibrary;
    }

    Ref<Texture2D> Renderer::getWhiteTexture()
    {
        return sData.whitTexture;
    }

    Ref<Texture2D> Renderer::getBlackTexture()
    {
        return sData.blackTexture;
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

        sData.meshShader->setUniform("u_EntityId", entityId);

        for (auto& [baseVertex, baseIndex, materialIndex, indexCount, transform, nodeName, meshName] : mesh->mSubMeshes)
        {
            sData.meshShader->setUniform("u_Model", transform * transform);

            glDrawElementsBaseVertex(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, reinterpret_cast<void*>(sizeof(uint32_t) * baseIndex), baseVertex);
        }

        mesh->mVertexBuffer->unbind();
        mesh->mVertexArray->unbind();
        mesh->mIndexBuffer->unbind();
    }
}
