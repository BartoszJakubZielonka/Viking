#pragma once

#include "Viking/Renderer/Shader.h"

namespace Viking
{
    class Material
    {
    public:
        virtual ~Material() = default;

        virtual void set(const std::string& uniformName, const glm::vec3 value) = 0;

        static Ref<Material> create(const std::string& name, const Ref<Shader>&);
    };
}
