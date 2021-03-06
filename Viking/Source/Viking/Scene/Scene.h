//
// Created by batzi on 16.05.2021.
//

#ifndef VIKING_SCENE_H
#define VIKING_SCENE_H

#include "Viking/Core/Timestep.h"
#include "Viking/Renderer/EditorCamera.h"

#include "entt.hpp"

class SceneHierarchyPanel;

namespace Viking {
    class Entity;

    class Scene {
    public:
        Scene(const std::string& sceneName ): mSceneName(sceneName) {}
        ~Scene() = default;

        Entity createEntity(const std::string& name = std::string());
        void destroyEntity(Entity entity);

        void onUpdateRuntime(Timestep ts);
        void onUpdateEditor(Timestep ts, EditorCamera& camera);
        void onViewportResize(uint32_t width, uint32_t height);

        Entity getPrimaryCameraEntity();

    private:
        template<typename T>
        void onComponentAdded(Entity entity, T& component);

        std::string mSceneName;

        entt::registry mRegistry;
        uint32_t mViewportWidth = 0, mViewportHeight = 0;

        friend class Entity;
        friend class SceneSerializer;
        friend class SceneHierarchyPanel;
    };
}

#endif //VIKING_SCENE_H
