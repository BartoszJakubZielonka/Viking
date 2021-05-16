//
// Created by batzi on 16.05.2021.
//
#include "vipch.h"
#include "Entity.h"

namespace Viking {
    Entity::Entity(entt::entity handle, Scene* scene): mEntityHandle(handle), mScene(scene) {}
}