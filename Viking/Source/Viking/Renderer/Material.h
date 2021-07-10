#pragma once

#include "Viking/Renderer/Shader.h"

namespace Viking
{
    class Material
    {
    public:
        static Ref<Material> create(const std::string& name, const Ref<Shader>&);
    protected:
        Ref<Shader> mShader;
    };
}
