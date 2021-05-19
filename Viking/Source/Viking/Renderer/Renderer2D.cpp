//
// Created by batzi on 15.05.2021.
//
#include "vipch.h"
#include "Renderer2D.h"

#include "Viking/Renderer/VertexArray.h"
#include "Viking/Renderer/Shader.h"
#include "Viking/Renderer/UniformBuffer.h"
#include "Viking/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Viking {
    struct QuadVertex {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;
        float TexIndex;
        float TilingFactor;

        // Editor-only
        int EntityID;
    };

    struct Renderer2DData
    {
        static const uint32_t MaxQuads{ 20000 };
        static const uint32_t MaxVertices{ MaxQuads * 4 };
        static const uint32_t MaxIndices{ MaxQuads * 6 };
        static const uint32_t MaxTextureSlots{ 32 }; // TODO: RenderCaps

        Ref<VertexArray> QuadVertexArray;
        Ref<VertexBuffer> QuadVertexBuffer;
        Ref<Shader> TextureShader;
        Ref<Texture2D> WhiteTexture;

        uint32_t QuadIndexCount{ 0 };
        QuadVertex* QuadVertexBufferBase{ nullptr };
        QuadVertex* QuadVertexBufferPtr{ nullptr };

        std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
        uint32_t TextureSlotIndex{ 1 }; // 0 = white texture

        glm::vec4 QuadVertexPositions[4];

        Renderer2D::Statistics Stats;

        struct CameraData
        {
            glm::mat4 ViewProjection;
        };
        CameraData CameraBuffer;
        Ref<UniformBuffer> CameraUniformBuffer;
    };

    static Renderer2DData sData;

    void Renderer2D::init() {
        VI_PROFILE_FUNCTION();

        sData.QuadVertexArray = VertexArray::create();

        sData.QuadVertexBuffer = VertexBuffer::create(sData.MaxVertices * sizeof(QuadVertex));

        sData.QuadVertexBuffer->setLayout({
            { ShaderDataType::Float3, "a_Position"     },
            { ShaderDataType::Float4, "a_Color"        },
            { ShaderDataType::Float2, "a_TexCoord"     },
            { ShaderDataType::Float,  "a_TexIndex"     },
            { ShaderDataType::Float,  "a_TilingFactor" },
            { ShaderDataType::Int,    "a_EntityID"     }
        });

        sData.QuadVertexArray->addVertexBuffer(sData.QuadVertexBuffer);

        sData.QuadVertexBufferBase = new QuadVertex[sData.MaxVertices];

        auto* quadIndices = new uint32_t[sData.MaxIndices];

        uint32_t offset{ 0 };
        for (uint32_t i = 0; i < sData.MaxIndices; i += 6)
        {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;

            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;

            offset += 4;
        }

        Ref<IndexBuffer> quadIB = IndexBuffer::create(quadIndices, sData.MaxIndices);
        sData.QuadVertexArray->setIndexBuffer(quadIB);
        delete[] quadIndices;

        sData.WhiteTexture = Texture2D::create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        sData.WhiteTexture->setData(&whiteTextureData, sizeof(uint32_t));

        int32_t samplers[sData.MaxTextureSlots];
        for (uint32_t i = 0; i < sData.MaxTextureSlots; i++)
            samplers[i] = i;

        sData.TextureShader = Shader::create("assets/shaders/Texture.glsl");

        // Set first texture slot to 0
        sData.TextureSlots[0] = sData.WhiteTexture;

        sData.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        sData.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
        sData.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
        sData.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

        sData.CameraUniformBuffer = UniformBuffer::create(sizeof(Renderer2DData::CameraData), 0);
    }

    void Renderer2D::shutdown() {
        VI_PROFILE_FUNCTION();
        delete[] sData.QuadVertexBufferBase;
    }

    void Renderer2D::beginScene(const Camera& camera, const glm::mat4& transform)
    {
        VI_PROFILE_FUNCTION();

        sData.CameraBuffer.ViewProjection = camera.getProjection() * glm::inverse(transform);
        sData.CameraUniformBuffer->setData(&sData.CameraBuffer, sizeof(Renderer2DData::CameraData));

        startBatch();
    }

    void Renderer2D::beginScene(const EditorCamera& camera)
    {
        VI_PROFILE_FUNCTION();

        sData.CameraBuffer.ViewProjection = camera.getViewProjection();
        sData.CameraUniformBuffer->setData(&sData.CameraBuffer, sizeof(Renderer2DData::CameraData));

        startBatch();
    }

    void Renderer2D::endScene()
    {
        VI_PROFILE_FUNCTION();

        flush();
    }

    void Renderer2D::startBatch()
    {
        sData.QuadIndexCount = 0;
        sData.QuadVertexBufferPtr = sData.QuadVertexBufferBase;

        sData.TextureSlotIndex = 1;
    }

    void Renderer2D::flush()
    {
        if (sData.QuadIndexCount == 0)
            return; // Nothing to draw

        uint32_t dataSize = (uint32_t)((uint8_t*)sData.QuadVertexBufferPtr - (uint8_t*)sData.QuadVertexBufferBase);
        sData.QuadVertexBuffer->setData(sData.QuadVertexBufferBase, dataSize);

        // Bind textures
        for (uint32_t i = 0; i < sData.TextureSlotIndex; i++)
            sData.TextureSlots[i]->bind(i);

        sData.TextureShader->bind();
        RenderCommand::drawIndexed(sData.QuadVertexArray, sData.QuadIndexCount);
        sData.Stats.DrawCalls++;
    }

    void Renderer2D::nextBatch()
    {
        flush();
        startBatch();
    }

    void Renderer2D::drawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID)
    {
        drawQuad(transform, src.Color, entityID);
    }

    void Renderer2D::resetStats()
    {
        memset(&sData.Stats, 0, sizeof(Statistics));
    }

    Renderer2D::Statistics Renderer2D::getStats()
    {
        return sData.Stats;
    }

    void Renderer2D::drawQuad(const glm::mat4 &transform, const glm::vec4 &color, int entityID) {
        VI_PROFILE_FUNCTION();

        constexpr size_t quadVertexCount{ 4 };
        const float textureIndex{ 0.0f }; // White Texture
        constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
        const float tilingFactor{ 1.0f };

        if (sData.QuadIndexCount >= Renderer2DData::MaxIndices)
            nextBatch();

        for (size_t i = 0; i < quadVertexCount; i++)
        {
            sData.QuadVertexBufferPtr->Position = transform * sData.QuadVertexPositions[i];
            sData.QuadVertexBufferPtr->Color = color;
            sData.QuadVertexBufferPtr->TexCoord = textureCoords[i];
            sData.QuadVertexBufferPtr->TexIndex = textureIndex;
            sData.QuadVertexBufferPtr->TilingFactor = tilingFactor;
            sData.QuadVertexBufferPtr->EntityID = entityID;
            sData.QuadVertexBufferPtr++;
        }

        sData.QuadIndexCount += 6;

        sData.Stats.QuadCount++;
    }

    void Renderer2D::drawQuad(const glm::mat4 &transform, const Ref<Texture2D> &texture, float tilingFactor,
                              const glm::vec4 &tintColor, int entityID) {
        VI_PROFILE_FUNCTION();

        constexpr size_t quadVertexCount{ 4 };
        constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

        if (sData.QuadIndexCount >= Renderer2DData::MaxIndices)
            nextBatch();

        float textureIndex{ 0.0f };
        for (uint32_t i = 1; i < sData.TextureSlotIndex; i++)
        {
            if (*sData.TextureSlots[i] == *texture)
            {
                textureIndex = (float)i;
                break;
            }
        }

        if (textureIndex == 0.0f)
        {
            if (sData.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
                nextBatch();

            textureIndex = (float)sData.TextureSlotIndex;
            sData.TextureSlots[sData.TextureSlotIndex] = texture;
            sData.TextureSlotIndex++;
        }

        for (size_t i = 0; i < quadVertexCount; i++)
        {
            sData.QuadVertexBufferPtr->Position = transform * sData.QuadVertexPositions[i];
            sData.QuadVertexBufferPtr->Color = tintColor;
            sData.QuadVertexBufferPtr->TexCoord = textureCoords[i];
            sData.QuadVertexBufferPtr->TexIndex = textureIndex;
            sData.QuadVertexBufferPtr->TilingFactor = tilingFactor;
            sData.QuadVertexBufferPtr->EntityID = entityID;
            sData.QuadVertexBufferPtr++;
        }

        sData.QuadIndexCount += 6;

        sData.Stats.QuadCount++;
    }
}
