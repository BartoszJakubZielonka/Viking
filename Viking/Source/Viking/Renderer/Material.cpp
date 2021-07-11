#include "vipch.h"

#include "Material.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLMaterial.h"

namespace Viking
{
    Ref<Material> Material::create(const std::string& name, const Ref<Shader>& shader)
    {
        VI_PROFILE_FUNCTION();

        switch (Renderer::getAPI())
        {
        case RendererAPI::API::None:
            VI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
            return nullptr;
        case RendererAPI::API::OpenGL:
            return createRef<OpenGLMaterial>(name, shader);
        }

        VI_CORE_ASSERT(false, "Unknown RendererAPI!")
        return nullptr;
    }
}
