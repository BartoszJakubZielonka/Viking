//
// Created by batzi on 15.05.2021.
//
#include "vipch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Viking {

    Ref<Shader> Shader::create(const std::string &filepath) {
        switch (Renderer::getAPI())
        {
            case RendererAPI::API::None:
                VI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return createRef<OpenGLShader>(filepath);
        }

        VI_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Ref<Shader> Shader::create(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc) {
        switch (Renderer::getAPI())
        {
            case RendererAPI::API::None:
                VI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return createRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
        }

        VI_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    void ShaderLibrary::add(const std::string &name, const Ref<Shader> &shader) {
        VI_CORE_ASSERT(!exists(name), "Shader already exists!");
        mShaders[name] = shader;
    }

    void ShaderLibrary::add(const Ref<Shader> &shader) {
        auto& name = shader->getName();
        add(name, shader);
    }

    Ref<Shader> ShaderLibrary::load(const std::string &filepath) {
        auto shader = Shader::create(filepath);
        add(shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::load(const std::string &name, const std::string &filepath) {
        auto shader = Shader::create(filepath);
        add(name, shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::get(const std::string &name) {
        VI_CORE_ASSERT(exists(name), "Shader not found!");
        return mShaders[name];
    }

    bool ShaderLibrary::exists(const std::string &name) const {
        return mShaders.find(name) != mShaders.end();
    }
}
