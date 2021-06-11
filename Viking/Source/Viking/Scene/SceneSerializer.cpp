//
// Created by batzi on 16.05.2021.
//

#include "vipch.h"
#include "SceneSerializer.h"

#include "Entity.h"
#include "Components.h"

#include <fstream>

#include <yaml-cpp/yaml.h>

namespace YAML {
    template<>
    struct convert<glm::vec3>
    {
        static Node encode(const glm::vec3& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, glm::vec3& rhs)
        {
            if (!node.IsSequence() || node.size() != 3)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };

    template<>
    struct convert<glm::vec4>
    {
        static Node encode(const glm::vec4& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, glm::vec4& rhs)
        {
            if (!node.IsSequence() || node.size() != 4)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };
}

namespace Viking {
    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
        return out;
    }

    static void serializeEntity(YAML::Emitter& out, Entity entity)
    {
        out << YAML::BeginMap; // Entity
        out << YAML::Key << "Entity" << YAML::Value << "12837192831273"; // TODO: Entity ID goes here

        if (entity.hasComponent<TagComponent>())
        {
            out << YAML::Key << "TagComponent";
            out << YAML::BeginMap; // TagComponent

            auto& tag = entity.getComponent<TagComponent>().Tag;
            out << YAML::Key << "Tag" << YAML::Value << tag;

            out << YAML::EndMap; // TagComponent
        }

        if (entity.hasComponent<TransformComponent>())
        {
            out << YAML::Key << "TransformComponent";
            out << YAML::BeginMap; // TransformComponent

            auto& tc = entity.getComponent<TransformComponent>();
            out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
            out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
            out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

            out << YAML::EndMap; // TransformComponent
        }

        if (entity.hasComponent<CameraComponent>())
        {
            out << YAML::Key << "CameraComponent";
            out << YAML::BeginMap; // CameraComponent

            auto& cameraComponent = entity.getComponent<CameraComponent>();
            auto& camera = cameraComponent.Camera;

            out << YAML::Key << "Camera" << YAML::Value;
            out << YAML::BeginMap; // Camera
            out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.getProjectionType();
            out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.getPerspectiveVerticalFOV();
            out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.getPerspectiveNearClip();
            out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.getPerspectiveFarClip();
            out << YAML::Key << "OrthographicSize" << YAML::Value << camera.getOrthographicSize();
            out << YAML::Key << "OrthographicNear" << YAML::Value << camera.getOrthographicNearClip();
            out << YAML::Key << "OrthographicFar" << YAML::Value << camera.getOrthographicFarClip();
            out << YAML::EndMap; // Camera

            out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
            out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;

            out << YAML::EndMap; // CameraComponent
        }

        if (entity.hasComponent<SpriteRendererComponent>())
        {
            out << YAML::Key << "SpriteRendererComponent";
            out << YAML::BeginMap; // SpriteRendererComponent

            auto& spriteRendererComponent = entity.getComponent<SpriteRendererComponent>();
            out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.color;

            out << YAML::EndMap; // SpriteRendererComponent
        }

        out << YAML::EndMap; // Entity
    }


    void SceneSerializer::serialize(const std::string &filepath) {
        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "Scene" << YAML::Value << "Untitled";
        out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

        mScene->mRegistry.each([&](auto entityID)
        {
            Entity entity = { entityID, mScene.get() };
            if (!entity)
                return;

            serializeEntity(out, entity);
        });

        out << YAML::EndSeq;
        out << YAML::EndMap;

        std::ofstream fout(filepath);
        fout << out.c_str();
    }

    void SceneSerializer::serializeRuntime(const std::string &filepath) {
        // Not implemented
        VI_CORE_ASSERT(false, "Not implemented");
    }

    bool SceneSerializer::deserialize(const std::string &filepath) {
        YAML::Node data = YAML::LoadFile(filepath);
        if (!data["Scene"])
            return false;

        std::string sceneName = data["Scene"].as<std::string>();
        VI_CORE_TRACE("Deserializing scene '{0}'", sceneName);

        auto entities = data["Entities"];
        if (entities)
        {
            for (auto entity : entities)
            {
                uint64_t uuid = entity["Entity"].as<uint64_t>(); // TODO

                std::string name;
                auto tagComponent = entity["TagComponent"];
                if (tagComponent)
                    name = tagComponent["Tag"].as<std::string>();

                VI_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);

                Entity deserializedEntity = mScene->createEntity(name);

                auto transformComponent = entity["TransformComponent"];
                if (transformComponent)
                {
                    // Entities always have transforms
                    auto& tc = deserializedEntity.getComponent<TransformComponent>();
                    tc.Translation = transformComponent["Translation"].as<glm::vec3>();
                    tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
                    tc.Scale = transformComponent["Scale"].as<glm::vec3>();
                }

                auto cameraComponent = entity["CameraComponent"];
                if (cameraComponent)
                {
                    auto& cc = deserializedEntity.addComponent<CameraComponent>();

                    auto& cameraProps = cameraComponent["Camera"];
                    cc.Camera.setProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

                    cc.Camera.setPerspectiveVerticalFOV(cameraProps["PerspectiveFOV"].as<float>());
                    cc.Camera.setPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
                    cc.Camera.setPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

                    cc.Camera.setOrthographicSize(cameraProps["OrthographicSize"].as<float>());
                    cc.Camera.setOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
                    cc.Camera.setOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

                    cc.Primary = cameraComponent["Primary"].as<bool>();
                    cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
                }

                auto spriteRendererComponent = entity["SpriteRendererComponent"];
                if (spriteRendererComponent)
                {
                    auto& src = deserializedEntity.addComponent<SpriteRendererComponent>();
                    src.color = spriteRendererComponent["Color"].as<glm::vec4>();
                }
            }
        }

        return true;
    }

    bool SceneSerializer::deserializeRuntime(const std::string &filepath) {
        // Not implemented
        VI_CORE_ASSERT(false, "Not implemented");
        return false;
    }
}
