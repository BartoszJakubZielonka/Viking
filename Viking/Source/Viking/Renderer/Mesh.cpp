#include "vipch.h"
#include "Viking/Renderer/Mesh.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <assimp/postprocess.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>

namespace Viking
{
    glm::mat4 mat4FromAssimpMat4(const aiMatrix4x4& matrix)
    {
        glm::mat4 result{};
        result[0][0] = matrix.a1; result[1][0] = matrix.a2; result[2][0] = matrix.a3; result[3][0] = matrix.a4;
        result[0][1] = matrix.b1; result[1][1] = matrix.b2; result[2][1] = matrix.b3; result[3][1] = matrix.b4;
        result[0][2] = matrix.c1; result[1][2] = matrix.c2; result[2][2] = matrix.c3; result[3][2] = matrix.c4;
        result[0][3] = matrix.d1; result[1][3] = matrix.d2; result[2][3] = matrix.d3; result[3][3] = matrix.d4;
        return result;
    }

    static const uint32_t S_MESH_IMPORT_FLAGS =
        aiProcess_CalcTangentSpace |        // Create biNormals/tangents just in case
        aiProcess_Triangulate |             // Make sure we're triangles
        aiProcess_SortByPType |             // Split meshes by primitive type
        aiProcess_GenNormals |              // Make sure we have legit normals
        aiProcess_GenUVCoords |             // Convert UVs if required 
        aiProcess_OptimizeMeshes |          // Batch draws where possible
        aiProcess_ValidateDataStructure;    // Validation

    struct LogStream final : public Assimp::LogStream
    {
        static void init()
        {
            if (Assimp::DefaultLogger::isNullLogger())
            {
                Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);
                Assimp::DefaultLogger::get()->attachStream(new LogStream, Assimp::Logger::Err | Assimp::Logger::Warn);
            }
        }

        virtual void write(const char* message) override
        {
            VI_CORE_WARN("Assimp: {0}", message);
        }
    };

    Mesh::Mesh(const std::string& filename)
    {
        LogStream::init();

        VI_CORE_INFO("Loading mesh: {0}", filename.c_str());

        mImporter = std::make_unique<Assimp::Importer>();

        const auto* scene = mImporter->ReadFile(filename, S_MESH_IMPORT_FLAGS);
        if (!scene || !scene->HasMeshes())
            VI_CORE_ERROR("Failed to load mesh file: {0}", filename);

        mScene = scene;

        mInverseTransform = glm::inverse(mat4FromAssimpMat4(scene->mRootNode->mTransformation));

        auto vertexCount{ 0 };
        auto indexCount{ 0 };

        mSubMeshes.reserve(scene->mNumMeshes);
        for (size_t m{ 0 }; m < scene->mNumMeshes; m++)
        {
            auto* mesh = scene->mMeshes[m];

            auto& subMesh = mSubMeshes.emplace_back();
            subMesh.baseVertex = vertexCount;
            subMesh.baseIndex = indexCount;
            subMesh.materialIndex = mesh->mMaterialIndex;
            subMesh.indexCount = mesh->mNumFaces * 3;
            subMesh.meshName = mesh->mName.C_Str();

            vertexCount += mesh->mNumVertices;
            indexCount += subMesh.indexCount;

            VI_CORE_ASSERT(mesh->HasPositions(), "Meshes require positions.")
                VI_CORE_ASSERT(mesh->HasNormals(), "Meshes require normals.")

                for (size_t i{ 0 }; i < mesh->mNumVertices; i++)
                {
                    Vertex vertex{};
                    vertex.position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
                    vertex.normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

                    if (mesh->HasTangentsAndBitangents())
                    {
                        vertex.tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
                        vertex.biTangent = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
                    }

                    if (mesh->HasTextureCoords(0))
                        vertex.texCoords = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
                    else
                        vertex.texCoords = { 0.0f, 0.0f };

                    mStaticVertices.push_back(vertex);
                }

            for (size_t i = 0; i < mesh->mNumFaces; i++)
            {
                VI_CORE_ASSERT(mesh->mFaces[i].mNumIndices == 3, "Must have 3 indices.")
                    Index index = { mesh->mFaces[i].mIndices[0], mesh->mFaces[i].mIndices[1], mesh->mFaces[i].mIndices[2] };

                mIndices.push_back(index);

                mTriangleCache[m].emplace_back(mStaticVertices[index.v1 + subMesh.baseVertex], mStaticVertices[index.v2 + subMesh.baseVertex], mStaticVertices[index.v3 + subMesh.baseVertex]);
            }
        }

        traverseNodes(scene->mRootNode);

        mVertexBuffer = VertexBuffer::create(mStaticVertices.data(), mStaticVertices.size() * sizeof(Vertex));
        mVertexBuffer->setLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float3, "a_Normal" },
            { ShaderDataType::Float3, "a_Tangent" },
            { ShaderDataType::Float3, "a_Binormal" },
            { ShaderDataType::Float2, "a_TexCoord" },
            });

        mIndexBuffer = IndexBuffer::create(mIndices.data(), mIndices.size() * sizeof(Index));

        mVertexArray = VertexArray::create();
        mVertexArray->addVertexBuffer(mVertexBuffer);
        mVertexArray->setIndexBuffer(mIndexBuffer);
    }

    void Mesh::traverseNodes(aiNode* node, const glm::mat4& parentTransform, const uint32_t level)
    {
        const auto transform = parentTransform * mat4FromAssimpMat4(node->mTransformation);
        for (uint32_t i = 0; i < node->mNumMeshes; i++)
        {
            const auto mesh = node->mMeshes[i];
            auto& submesh = mSubMeshes[mesh];
            submesh.nodeName = node->mName.C_Str();
            submesh.transform = transform;
        }

        for (uint32_t i = 0; i < node->mNumChildren; i++)
            traverseNodes(node->mChildren[i], transform, level + 1);
    }
}
