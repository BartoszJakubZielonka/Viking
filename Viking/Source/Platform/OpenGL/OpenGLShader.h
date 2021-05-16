//
// Created by batzi on 16.05.2021.
//

#ifndef VIKING_OPENGLSHADER_H
#define VIKING_OPENGLSHADER_H

#include "Viking/Renderer/Shader.h"
#include <glm/glm.hpp>

// TODO: REMOVE!
typedef unsigned int GLenum;

namespace Viking {
    class OpenGLShader final: public Shader{
    public:
        OpenGLShader(const std::string& filepath);
        OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
        ~OpenGLShader() override;

        void bind() const override;
        void unbind() const override;

        void setInt(const std::string& name, int value) override;
        void setIntArray(const std::string& name, int* values, uint32_t count) override;

        void setFloat(const std::string& name, float value) override;
        void setFloat2(const std::string& name, const glm::vec2& value) override;
        void setFloat3(const std::string& name, const glm::vec3& value) override;
        void setFloat4(const std::string& name, const glm::vec4& value) override;

        void setMat4(const std::string& name, const glm::mat4& value) override;

        [[nodiscard]] const std::string& getName() const override {
            return mName;
        }

        void uploadUniformInt(const std::string& name, int value);
        void uploadUniformIntArray(const std::string& name, int* values, uint32_t count);

        void uploadUniformFloat(const std::string& name, float value);
        void uploadUniformFloat2(const std::string& name, const glm::vec2& value);
        void uploadUniformFloat3(const std::string& name, const glm::vec3& value);
        void uploadUniformFloat4(const std::string& name, const glm::vec4& value);

        void uploadUniformMat3(const std::string& name, const glm::mat3& matrix);
        void uploadUniformMat4(const std::string& name, const glm::mat4& matrix);

    private:
        std::string readFile(const std::string& filepath);
        std::unordered_map<GLenum, std::string> preProcess(const std::string& source);

        void compileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& shaderSources);
        void compileOrGetOpenGLBinaries();
        void createProgram();
        void reflect(GLenum stage, const std::vector<uint32_t>& shaderData);

        uint32_t mRendererID;
        std::string mFilePath;
        std::string mName;

        std::unordered_map<GLenum, std::vector<uint32_t>> mVulkanSPIRV;
        std::unordered_map<GLenum, std::vector<uint32_t>> mOpenGLSPIRV;

        std::unordered_map<GLenum, std::string> mOpenGLSourceCode;
    };
}

#endif //VIKING_OPENGLSHADER_H
