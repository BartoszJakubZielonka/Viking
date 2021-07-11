#pragma once

#include "Viking/Renderer/Material.h"

namespace Viking
{
    class OpenGLMaterial: public Material
    {
    public:
        OpenGLMaterial() = delete;
        OpenGLMaterial(const std::string& name, Ref<Shader>);
        ~OpenGLMaterial() override = default;

        void set(const std::string& uniformName, const glm::vec3 value) override;
    private:
        Ref<Shader> mShader;

        std::string mMaterialName;
    };
}
