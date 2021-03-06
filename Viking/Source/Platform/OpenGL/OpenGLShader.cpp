//
// Created by batzi on 16.05.2021.
//
#include "vipch.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <fstream>
#include <glad/gl.h>

#include <glm/gtc/type_ptr.hpp>

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>
#include <utility>

#include "Viking/Core/Timer.h"

namespace Viking {
    namespace Utils {
        static GLenum shaderTypeFromString(const std::string& type)
        {
            if (type == "vertex")
                return GL_VERTEX_SHADER;
            if (type == "fragment" || type == "pixel")
                return GL_FRAGMENT_SHADER;

            VI_CORE_ASSERT(false, "Unknown shader type!");
            return 0;
        }

        static shaderc_shader_kind glShaderStageToShaderC(const GLenum stage)
        {
            switch (stage)
            {
                case GL_VERTEX_SHADER:   return shaderc_glsl_vertex_shader;
                case GL_FRAGMENT_SHADER: return shaderc_glsl_fragment_shader;
            }
            VI_CORE_ASSERT(false);
            return static_cast<shaderc_shader_kind>(0);
        }

        static const char* glShaderStageToString(const GLenum stage)
        {
            switch (stage)
            {
                case GL_VERTEX_SHADER:   return "GL_VERTEX_SHADER";
                case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
            }
            VI_CORE_ASSERT(false);
            return nullptr;
        }

        static const char* getCacheDirectory()
        {
            // TODO: make sure the assets directory is valid
            return "assets/cache/shader/opengl";
        }

        static void createCacheDirectoryIfNeeded()
        {
            if (std::string cacheDirectory = getCacheDirectory(); !std::filesystem::exists(cacheDirectory))
                std::filesystem::create_directories(cacheDirectory);
        }

        static const char* glShaderStageCachedOpenGLFileExtension(const uint32_t stage)
        {
            switch (stage)
            {
                case GL_VERTEX_SHADER:    return ".cached_opengl.vert";
                case GL_FRAGMENT_SHADER:  return ".cached_opengl.frag";
            }
            VI_CORE_ASSERT(false);
            return "";
        }

        static const char* glShaderStageCachedVulkanFileExtension(const uint32_t stage)
        {
            switch (stage)
            {
                case GL_VERTEX_SHADER:    return ".cached_vulkan.vert";
                case GL_FRAGMENT_SHADER:  return ".cached_vulkan.frag";
            }
            VI_CORE_ASSERT(false);
            return "";
        }
    }


    OpenGLShader::OpenGLShader(const std::string &filepath): mFilePath(filepath) {
        VI_PROFILE_FUNCTION();

        Utils::createCacheDirectoryIfNeeded();

        {
            const std::string source = readFile(filepath);
            const auto shaderSources = preProcess(source);
            Timer timer;
            compileOrGetVulkanBinaries(shaderSources);
            compileOrGetOpenGLBinaries();
            createProgram();
            VI_CORE_WARN("Shader creation took {0} ms", timer.elapsedMillis());
        }

        // Extract name from filepath
        auto lastSlash = filepath.find_last_of("/\\");
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        const auto lastDot = filepath.rfind('.');
        const auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
        mName = filepath.substr(lastSlash, count);
    }

    OpenGLShader::OpenGLShader(std::string name, const std::string &vertexSrc, const std::string &fragmentSrc): mName(std::move(name)) {
        VI_PROFILE_FUNCTION();

        std::unordered_map<GLenum, std::string> sources;
        sources[GL_VERTEX_SHADER] = vertexSrc;
        sources[GL_FRAGMENT_SHADER] = fragmentSrc;

        compileOrGetVulkanBinaries(sources);
        compileOrGetOpenGLBinaries();
        createProgram();
    }

    OpenGLShader::~OpenGLShader() {
        VI_PROFILE_FUNCTION();

        glDeleteProgram(mRendererID);
    }

    void OpenGLShader::bind() const {
        VI_PROFILE_FUNCTION();

        glUseProgram(mRendererID);
    }

    void OpenGLShader::setUniform(const std::string &name, int value) {
        VI_PROFILE_FUNCTION();

        uploadUniformInt(name, value);
    }

    void OpenGLShader::setUniform(const std::string &name, int *values, uint32_t count) {
        uploadUniformIntArray(name, values, count);
    }

    void OpenGLShader::setUniform(const std::string &name, float value) {
        VI_PROFILE_FUNCTION();

        uploadUniformFloat(name, value);
    }

    void OpenGLShader::setUniform(const std::string &name, const glm::vec2 &value) {
        VI_PROFILE_FUNCTION();

        uploadUniformFloat2(name, value);
    }

    void OpenGLShader::setUniform(const std::string &name, const glm::vec3 &value) {
        VI_PROFILE_FUNCTION();

        uploadUniformFloat3(name, value);
    }

    void OpenGLShader::setUniform(const std::string &name, const glm::vec4 &value) {
        VI_PROFILE_FUNCTION();

        uploadUniformFloat4(name, value);
    }

    void OpenGLShader::setUniform(const std::string &name, const glm::mat4 &value) {
        VI_PROFILE_FUNCTION();

        uploadUniformMat4(name, value);
    }

    void OpenGLShader::uploadUniformInt(const std::string &name, int value) const
    {
        const GLint location = glGetUniformLocation(mRendererID, name.c_str());
        glUniform1i(location, value);
    }

    void OpenGLShader::uploadUniformIntArray(const std::string &name, int *values, uint32_t count) const
    {
        const GLint location = glGetUniformLocation(mRendererID, name.c_str());
        glUniform1iv(location, count, values);
    }

    void OpenGLShader::uploadUniformFloat(const std::string &name, float value) const
    {
        const GLint location = glGetUniformLocation(mRendererID, name.c_str());
        glUniform1f(location, value);
    }

    void OpenGLShader::uploadUniformFloat2(const std::string &name, const glm::vec2 &value) const
    {
        const GLint location = glGetUniformLocation(mRendererID, name.c_str());
        glUniform2f(location, value.x, value.y);
    }

    void OpenGLShader::uploadUniformFloat3(const std::string &name, const glm::vec3 &value) const
    {
        const GLint location = glGetUniformLocation(mRendererID, name.c_str());
        glUniform3f(location, value.x, value.y, value.z);
    }

    void OpenGLShader::uploadUniformFloat4(const std::string &name, const glm::vec4 &value) const
    {
        const GLint location = glGetUniformLocation(mRendererID, name.c_str());
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }

    void OpenGLShader::uploadUniformMat3(const std::string &name, const glm::mat3 &matrix) const
    {
        const GLint location = glGetUniformLocation(mRendererID, name.c_str());
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void OpenGLShader::uploadUniformMat4(const std::string &name, const glm::mat4 &matrix) const
    {
        const GLint location = glGetUniformLocation(mRendererID, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    std::string OpenGLShader::readFile(const std::string &filepath) {
        VI_PROFILE_FUNCTION();

        std::string result;
        std::ifstream in(filepath, std::ios::in | std::ios::binary); // ifstream closes itself due to RAII
        if (in)
        {
            in.seekg(0, std::ios::end);
            if (size_t size = in.tellg(); size != -1)
            {
                result.resize(size);
                in.seekg(0, std::ios::beg);
                in.read(&result[0], size);
            }
            else
            {
                VI_CORE_ERROR("Could not read from file '{0}'", filepath);
            }
        }
        else
        {
            VI_CORE_ERROR("Could not open file '{0}'", filepath);
        }

        return result;
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::preProcess(const std::string &source) {
        VI_PROFILE_FUNCTION();

        std::unordered_map<GLenum, std::string> shaderSources;

        const auto typeToken = "#type";
        const size_t typeTokenLength = strlen(typeToken);
        size_t pos = source.find(typeToken, 0); //Start of shader type declaration line
        while (pos != std::string::npos)
        {
            size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
            VI_CORE_ASSERT(eol != std::string::npos, "Syntax error")
            size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
            std::string type = source.substr(begin, eol - begin);
            VI_CORE_ASSERT(Utils::shaderTypeFromString(type), "Invalid shader type specified")

            size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
            VI_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error")
            pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line

            shaderSources[Utils::shaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
        }

        return shaderSources;
    }

    void OpenGLShader::compileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string> &shaderSources) {
        GLuint program = glCreateProgram();

        shaderc::CompileOptions options;
        options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
        if (const bool optimize = true; optimize)
            options.SetOptimizationLevel(shaderc_optimization_level_performance);

        std::filesystem::path cacheDirectory = Utils::getCacheDirectory();

        auto& shaderData = mVulkanSPIRV;
        shaderData.clear();
        for (auto&& [stage, source] : shaderSources)
        {
            std::filesystem::path shaderFilePath = mFilePath;
            std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + Utils::glShaderStageCachedVulkanFileExtension(stage));

            if (std::ifstream in(cachedPath, std::ios::in | std::ios::binary); in.is_open())
            {
                in.seekg(0, std::ios::end);
                auto size = in.tellg();
                in.seekg(0, std::ios::beg);

                auto& data = shaderData[stage];
                data.resize(size / sizeof(uint32_t));
                in.read(reinterpret_cast<char*>(data.data()), size);
            }
            else
            {
                shaderc::Compiler compiler;
                shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::glShaderStageToShaderC(stage), mFilePath.c_str(), options);
                if (module.GetCompilationStatus() != shaderc_compilation_status_success)
                {
                    VI_CORE_ERROR(module.GetErrorMessage());
                    VI_CORE_ASSERT(false)
                }

                shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

                if (std::ofstream out(cachedPath, std::ios::out | std::ios::binary); out.is_open())
                {
                    auto& data = shaderData[stage];
                    out.write(reinterpret_cast<char*>(data.data()), data.size() * sizeof(uint32_t));
                    out.flush();
                    out.close();
                }
            }
        }

        for (auto&& [stage, data]: shaderData)
            reflect(stage, data);
    }

    void OpenGLShader::compileOrGetOpenGLBinaries() {
        auto& shaderData = mOpenGLSPIRV;

        shaderc::CompileOptions options;
        options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
        if (const bool optimize = false; optimize)
            options.SetOptimizationLevel(shaderc_optimization_level_performance);

        std::filesystem::path cacheDirectory = Utils::getCacheDirectory();

        shaderData.clear();
        mOpenGLSourceCode.clear();
        for (auto&& [stage, spirv] : mVulkanSPIRV)
        {
            std::filesystem::path shaderFilePath = mFilePath;
            std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + Utils::glShaderStageCachedOpenGLFileExtension(stage));

            if (std::ifstream in(cachedPath, std::ios::in | std::ios::binary); in.is_open())
            {
                in.seekg(0, std::ios::end);
                auto size = in.tellg();
                in.seekg(0, std::ios::beg);

                auto& data = shaderData[stage];
                data.resize(size / sizeof(uint32_t));
                in.read(reinterpret_cast<char*>(data.data()), size);
            }
            else
            {
                shaderc::Compiler compiler;
                spirv_cross::CompilerGLSL glslCompiler(spirv);
                mOpenGLSourceCode[stage] = glslCompiler.compile();
                auto& source = mOpenGLSourceCode[stage];

                shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::glShaderStageToShaderC(stage), mFilePath.c_str());
                if (module.GetCompilationStatus() != shaderc_compilation_status_success)
                {
                    VI_CORE_ERROR(module.GetErrorMessage());
                    VI_CORE_ASSERT(false)
                }

                shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

                if (std::ofstream out(cachedPath, std::ios::out | std::ios::binary); out.is_open())
                {
                    auto& data = shaderData[stage];
                    out.write(reinterpret_cast<char*>(data.data()), data.size() * sizeof(uint32_t));
                    out.flush();
                    out.close();
                }
            }
        }
    }

    void OpenGLShader::createProgram() {
        GLuint program = glCreateProgram();

        std::vector<GLuint> shaderIDs;
        for (auto&& [stage, spirv] : mOpenGLSPIRV)
        {
            GLuint shaderId = shaderIDs.emplace_back(glCreateShader(stage));
            glShaderBinary(1, &shaderId, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), sizeof(uint32_t) * spirv.size());
            glSpecializeShader(shaderId, "main", 0, nullptr, nullptr);
            glAttachShader(program, shaderId);
        }

        glLinkProgram(program);

        GLint isLinked;
        glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
            VI_CORE_ERROR("Shader linking failed ({0}):\n{1}", mFilePath, infoLog.data());

            glDeleteProgram(program);

            for (auto id : shaderIDs)
                glDeleteShader(id);
        }

        for (auto id : shaderIDs)
        {
            glDetachShader(program, id);
            glDeleteShader(id);
        }

        mRendererID = program;
    }

    void OpenGLShader::reflect(GLenum stage, const std::vector<uint32_t> &shaderData) {
        const spirv_cross::Compiler compiler(shaderData);
        auto [uniform_buffers, storage_buffers, stage_inputs, stage_outputs, subpass_inputs, storage_images, sampled_images, atomic_counters, acceleration_structures, push_constant_buffers, separate_images, separate_samplers] = compiler.get_shader_resources();

        VI_CORE_TRACE("OpenGLShader::Reflect - {0} {1}", Utils::glShaderStageToString(stage), mFilePath);
        VI_CORE_TRACE("    {0} uniform buffers", uniform_buffers.size());
        VI_CORE_TRACE("    {0} resources", sampled_images.size());

        VI_CORE_TRACE("Uniform buffers:");
        for (const auto& [id, type_id, base_type_id, name] : uniform_buffers)
        {
            const auto& bufferType = compiler.get_type(base_type_id);
            size_t bufferSize = compiler.get_declared_struct_size(bufferType);
            uint32_t binding = compiler.get_decoration(id, spv::DecorationBinding);
            size_t memberCount = bufferType.member_types.size();

            VI_CORE_TRACE("  {0}", name);
            VI_CORE_TRACE("    Size = {0}", bufferSize);
            VI_CORE_TRACE("    Binding = {0}", binding);
            VI_CORE_TRACE("    Members = {0}", memberCount);
        }
    }
}
