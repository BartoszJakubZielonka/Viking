//
// Created by batzi on 15.05.2021.
//

#ifndef VIKING_RENDERER2D_H
#define VIKING_RENDERER2D_H

#include "Viking/Renderer/OrthographicCamera.h"

#include "Viking/Renderer/Texture.h"

#include "Viking/Renderer/Camera.h"
#include "Viking/Renderer/EditorCamera.h"

#include "Viking/Scene/Components.h"

namespace Viking {
    class Renderer2D {
    public:
        static void init();
        static void shutdown();

        static void beginScene(const Camera& camera, const glm::mat4& transform);
        static void beginScene(const EditorCamera& camera);
        static void endScene();
        static void flush();

        static void drawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);
        static void drawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), int entityID = -1);

        static void drawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID);

        // Stats
        struct Statistics {
            uint32_t DrawCalls{0};
            uint32_t QuadCount{0};

            [[nodiscard]] uint32_t getTotalVertexCount() const { return QuadCount * 4; }
            [[nodiscard]] uint32_t getTotalIndexCount() const { return QuadCount * 6; }
        };

        static void resetStats();
        static Statistics getStats();

        static void startBatch();
        static void nextBatch();
    };
}

#endif //VIKING_RENDERER2D_H
