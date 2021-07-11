#include "vipch.h"

#include "Platform/OpenGL/OpenGLMaterial.h"

namespace Viking
{
    OpenGLMaterial::OpenGLMaterial(const std::string& name, const Ref<Shader> shader): mShader(shader), mMaterialName(name)
    {
        VI_PROFILE_FUNCTION();
    }

    void OpenGLMaterial::set(const std::string& uniformName, const glm::vec3 value)
    {
        VI_PROFILE_FUNCTION();
        mShader->setUniform(uniformName, value);
    }
}
