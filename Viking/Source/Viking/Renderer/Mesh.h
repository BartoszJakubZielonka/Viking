#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "Viking/Renderer/Material.h"
#include "Viking/Renderer/Shader.h"
#include "Viking/Renderer/Texture.h"
#include "Viking/Renderer/IndexBuffer.h"
#include "Viking/Renderer/VertexBuffer.h"
#include "Viking/Renderer/VertexArray.h"

namespace Viking
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 tangent;
        glm::vec3 biTangent;
        glm::vec2 texCoords;
    };

    struct Index
    {
        uint32_t v1, v2, v3;
    };

    static_assert(sizeof Index == 3 * sizeof uint32_t);

    struct Triangle
    {
        Vertex v0, v1, v2;

        Triangle(const Vertex& vertex0, const Vertex& vertex1, const Vertex& vertex2) : v0(vertex0), v1(vertex1), v2(vertex2) {}
    };

    class SubMesh
    {
    public:
        uint32_t baseVertex{ 0 };
        uint32_t baseIndex{ 0 };
        uint32_t materialIndex{ 0 };
        uint32_t indexCount{ 0 };

        glm::mat4 transform = glm::mat4(0.0f);

        std::string nodeName{}, meshName{};
    };

    class Mesh
    {
    public:
        explicit Mesh(const std::string& filename);
        Mesh(const std::vector<Vertex>& vertices, const std::vector<Index>& indices, const glm::mat4& transform);
        ~Mesh() = default;

        auto getSubMeshes() -> std::vector<SubMesh>& { return mSubMeshes; }
        [[nodiscard]] auto getSubMeshes() const -> const std::vector<SubMesh>& { return mSubMeshes; }

        [[nodiscard]] auto getFilePath() const -> const std::string& { return mFilePath; }

        [[nodiscard]] auto getTriangleCache(const uint32_t index) const -> std::vector<Triangle> { return mTriangleCache.at(index); }

    private:
        void traverseNodes(aiNode* node, const glm::mat4& parentTransform = glm::mat4(1.0f), uint32_t level = 0);

        std::vector<SubMesh> mSubMeshes;
        std::vector<Ref<Texture2D>> mTextures;

        std::unique_ptr<Assimp::Importer> mImporter;

        glm::mat4 mInverseTransform;

        Ref<VertexBuffer> mVertexBuffer;
        Ref<IndexBuffer> mIndexBuffer;
        Ref<VertexArray> mVertexArray;

        std::vector<Vertex> mStaticVertices;
        std::vector<Index> mIndices;

        const aiScene* mScene;

        //Materials
        Ref<Shader> mMeshShader;
        std::vector<Ref<Material>> mMaterials;

        std::unordered_map<uint32_t, std::vector<Triangle>> mTriangleCache;

        std::string mFilePath;

        friend class Renderer;
    };
}
