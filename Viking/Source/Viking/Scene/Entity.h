//
// Created by batzi on 16.05.2021.
//

#ifndef VIKING_ENTITY_H
#define VIKING_ENTITY_H

#include "Scene.h"

#include "entt.hpp"

namespace Viking {
    class Entity {
    public:
        Entity() = default;
        Entity(entt::entity handle, Scene* scene);
        Entity(const Entity& other) = default;

        template<typename T, typename... Args>
        T& addComponent(Args&&... args)
        {
            VI_CORE_ASSERT(!hasComponent<T>(), "Entity already has component!");
            T& component = mScene->mRegistry.emplace<T>(mEntityHandle, std::forward<Args>(args)...);
            mScene->onComponentAdded<T>(*this, component);
            return component;
        }

        template<typename T>
        T& getComponent()
        {
            VI_CORE_ASSERT(hasComponent<T>(), "Entity does not have component!");
            return mScene->mRegistry.get<T>(mEntityHandle);
        }

        template<typename T>
        bool hasComponent()
        {
            return mScene->mRegistry.has<T>(mEntityHandle);
        }

        template<typename T>
        void removeComponent()
        {
            VI_CORE_ASSERT(hasComponent<T>(), "Entity does not have component!");
            mScene->mRegistry.remove<T>(mEntityHandle);
        }

        operator bool() const { return mEntityHandle != entt::null; }
        operator entt::entity() const { return mEntityHandle; }
        operator uint32_t() const { return (uint32_t)mEntityHandle; }

        bool operator==(const Entity& other) const
        {
            return mEntityHandle == other.mEntityHandle && mScene == other.mScene;
        }

        bool operator!=(const Entity& other) const
        {
            return !(*this == other);
        }

    private:
        entt::entity mEntityHandle{ entt::null };
        Scene* mScene{nullptr};
    };
}

#endif //VIKING_ENTITY_H
