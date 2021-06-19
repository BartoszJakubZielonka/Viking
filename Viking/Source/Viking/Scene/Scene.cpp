#include "vipch.h"
#include "Scene.h"

#include "Components.h"
#include "Viking/Renderer/Renderer2D.h"
#include "Viking/Renderer/Renderer.h"

#include <glm/glm.hpp>

#include "Entity.h"

namespace Viking {
    Entity Scene::createEntity(const std::string& name)
    {
        Entity entity = { mRegistry.create(), this };
        entity.addComponent<TransformComponent>();
        auto& tag = entity.addComponent<TagComponent>();
        tag.Tag = name.empty() ? "Entity" : name;
        return entity;
    }

    void Scene::destroyEntity(const Entity entity)
    {
        mRegistry.destroy(entity);
    }

    void Scene::onUpdateRuntime(const Timestep ts) {
        //Update Scripts
        {
            mRegistry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
                // TODO: Move to Scene::OnScenePlay
                if (!nsc.Instance)
                {
                    nsc.Instance = nsc.instantiateScript();
                    nsc.Instance->mEntity = Entity{ entity, this };
                    nsc.Instance->onCreate();
                }

                nsc.Instance->onUpdate(ts);
            });
        }

        //Render 2D
        Camera* mainCamera{nullptr};
        glm::mat4 cameraTransform;
        {
            auto view = mRegistry.view<TransformComponent, CameraComponent>();
            for (auto entity : view)
            {
                auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

                if (camera.Primary)
                {
                    mainCamera = &camera.Camera;
                    cameraTransform = transform.getTransform();
                    break;
                }
            }
        }

        if (mainCamera)
        {
            Renderer2D::beginScene(*mainCamera, cameraTransform);

            auto group = mRegistry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for (auto entity : group)
            {
                auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

                Renderer2D::drawSprite(transform.getTransform(), sprite, static_cast<int>(entity));
            }

            Renderer2D::endScene();
        }
    }

    void Scene::onUpdateEditor(Timestep ts, EditorCamera &camera) {
        Renderer2D::beginScene(camera);

        auto spriteGroup = mRegistry.group<SpriteRendererComponent>(entt::get<TransformComponent>);
        for (auto entity : spriteGroup)
        {
            auto [transform, sprite] = spriteGroup.get<TransformComponent, SpriteRendererComponent>(entity);

            Renderer2D::drawSprite(transform.getTransform(), sprite, static_cast<int>(entity));
        }

        Renderer2D::endScene();

        Renderer::beginScene(camera);

        auto group = mRegistry.group<MeshRendererComponent>(entt::get<TransformComponent>);
        for (auto entity : group)
        {
            auto [transform, mesh] = group.get<TransformComponent, MeshRendererComponent>(entity);

            Renderer::submitMesh(transform.getTransform(), mesh, static_cast<int>(entity));
        }

        Renderer::endScene();
    }

    void Scene::onViewportResize(uint32_t width, uint32_t height)
    {
        mViewportWidth = width;
        mViewportHeight = height;

        // Resize our non-FixedAspectRatio cameras
        auto view = mRegistry.view<CameraComponent>();
        for (auto entity : view)
        {
            if (auto & cameraComponent = view.get<CameraComponent>(entity); !cameraComponent.FixedAspectRatio)
                cameraComponent.Camera.setViewportSize(width, height);
        }
    }

    Entity Scene::getPrimaryCameraEntity() {
        auto view = mRegistry.view<CameraComponent>();
        for (auto entity : view)
        {
            if (const auto & camera = view.get<CameraComponent>(entity); camera.Primary)
                return Entity{entity, this};
        }
        return {};
    }

    template<typename T>
    void Scene::onComponentAdded(Entity entity, T &component) {
        static_assert(false);
    }

    template<>
    void Scene::onComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
    {
    }

    template<>
    void Scene::onComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
    {
        if (mViewportWidth > 0 && mViewportHeight > 0)
            component.Camera.setViewportSize(mViewportWidth, mViewportHeight);
    }

    template<>
    void Scene::onComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
    {
    }

    template<>
    void Scene::onComponentAdded<MeshRendererComponent>(Entity entity, MeshRendererComponent& component)
    {
    }

    template<>
    void Scene::onComponentAdded<TagComponent>(Entity entity, TagComponent& component)
    {
    }

    template<>
    void Scene::onComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
    {
    }
}
