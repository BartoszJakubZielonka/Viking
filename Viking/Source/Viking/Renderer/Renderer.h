//
// Created by batzi on 15.05.2021.
//

#ifndef VIKING_RENDERER_H
#define VIKING_RENDERER_H

#include "Viking/Renderer/RenderCommand.h"

#include "Viking/Renderer/OrthographicCamera.h"
#include "Viking/Renderer/Shader.h"

namespace Viking {
    class Renderer {
    public:
        static void init();
        static void shutdown();

        static void onWindowResize(uint32_t width, uint32_t height);

        static void beginScene(OrthographicCamera& camera);
        static void endScene();

        static void submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

        static RendererAPI::API GetAPI() { return RendererAPI::getAPI(); }
    private:
        struct SceneData
        {
            glm::mat4 ViewProjectionMatrix;
        };

        static Scope<SceneData> sSceneData;
    };
}

#endif //VIKING_RENDERER_H
