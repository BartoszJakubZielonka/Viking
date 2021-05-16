//
// Created by batzi on 16.05.2021.
//

#include "vipch.h"
#include "Scene.h"

#include "Components.h"
#include "Viking/Renderer/Renderer2D.h"

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

    void Scene::destroyEntity(Entity entity)
    {
        mRegistry.destroy(entity);
    }

    void Scene::onUpdateRuntime(Timestep ts) {
        //Update Scripts
        {
            mRegistry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
                // TODO: Move to Scene::OnScenePlay
                if (!nsc.Instance)
                {
                    nsc.Instance = nsc.InstantiateScript();
                    nsc.Instance->mEntity = Entity{ entity, this };
                    nsc.Instance->OnCreate();
                }

                nsc.Instance->OnUpdate(ts);
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
                    cameraTransform = transform.GetTransform();
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

                Renderer2D::drawSprite(transform.GetTransform(), sprite, (int)entity);
            }

            Renderer2D::endScene();
        }
    }

    void Scene::onUpdateEditor(Timestep ts, EditorCamera &camera) {
        Renderer2D::beginScene(camera);

        auto group = mRegistry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (auto entity : group)
        {
            auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

            Renderer2D::drawSprite(transform.GetTransform(), sprite, (int)entity);
        }

        Renderer2D::endScene();
    }

    Entity Scene::getPrimaryCameraEntity() {
        auto view = mRegistry.view<CameraComponent>();
        for (auto entity : view)
        {
            const auto& camera = view.get<CameraComponent>(entity);
            if (camera.Primary)
                return Entity{entity, this};
        }
        return {};
    }

    template<typename T>
    void Scene::onComponentAdded(Entity entity, T &component) {
        static_assert(false);
    }

    template<>
    void Scene::onComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
    {
        if (mViewportWidth > 0 && mViewportHeight > 0)
            component.Camera.SetViewportSize(mViewportWidth, mViewportHeight);
    }

    template<>
    void Scene::onComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
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