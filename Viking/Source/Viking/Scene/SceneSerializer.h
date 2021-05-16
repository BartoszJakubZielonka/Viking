//
// Created by batzi on 16.05.2021.
//

#ifndef VIKING_SCENESERIALIZER_H
#define VIKING_SCENESERIALIZER_H

#include "Scene.h"

namespace Viking {
    class SceneSerializer {
    public:
        SceneSerializer(const Ref<Scene>& scene): mScene(scene) {}

        void Serialize(const std::string& filepath);
        void SerializeRuntime(const std::string& filepath);

        bool deserialize(const std::string& filepath);
        bool deserializeRuntime(const std::string& filepath);

    private:
        Ref<Scene> mScene;
    };
}

#endif //VIKING_SCENESERIALIZER_H
