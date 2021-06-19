//
// Created by batzi on 15.05.2021.
//

#ifndef VIKING_RENDERER_H
#define VIKING_RENDERER_H

#include "Viking/Renderer/RenderCommand.h"

#include "Viking/Renderer/Camera.h"
#include "Viking/Renderer/EditorCamera.h"
#include "Viking/Renderer/Shader.h"

#include "Viking/Scene/Components.h"

namespace Viking {
    class Renderer {
    public:
        static void init();
        static void shutdown();

        static void onWindowResize(uint32_t width, uint32_t height);

        static void beginScene(const Camera& camera, const glm::mat4& transform);
        static void beginScene(const EditorCamera& camera);
        static void endScene();

        static void submitMesh(const glm::mat4& transform, MeshRendererComponent& src, int entityID);

        static void submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

        static RendererAPI::API getAPI() { return RendererAPI::getAPI(); }
    private:
        static void drawMesh(const Ref<Mesh>& mesh, const glm::mat4& transform, int32_t entityId);

        struct SceneData
        {
            glm::mat4 ViewProjectionMatrix;
        };

        static Scope<SceneData> sSceneData;
    };
}

#endif //VIKING_RENDERER_H
