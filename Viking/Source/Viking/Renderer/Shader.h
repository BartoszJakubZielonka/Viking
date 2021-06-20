//
// Created by batzi on 15.05.2021.
//

#ifndef VIKING_SHADER_H
#define VIKING_SHADER_H

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

namespace Viking {
    class Shader {
    public:
        virtual ~Shader() = default;

        virtual void bind() const = 0;

        virtual void setUniform(const std::string& name, int value) = 0;
        virtual void setUniform(const std::string& name, int* values, uint32_t count) = 0;
        virtual void setUniform(const std::string& name, float value) = 0;
        virtual void setUniform(const std::string& name, const glm::vec2& value) = 0;
        virtual void setUniform(const std::string& name, const glm::vec3& value) = 0;
        virtual void setUniform(const std::string& name, const glm::vec4& value) = 0;
        virtual void setUniform(const std::string& name, const glm::mat4& value) = 0;

        [[nodiscard]] virtual const std::string& getName() const = 0;

        [[nodiscard]] static Ref<Shader> create(const std::string& filepath);
    };
}

#endif //VIKING_SHADER_H
