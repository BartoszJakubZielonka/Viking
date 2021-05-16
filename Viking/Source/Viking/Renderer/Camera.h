//
// Created by batzi on 16.05.2021.
//

#ifndef VIKING_CAMERA_H
#define VIKING_CAMERA_H

#include <glm/glm.hpp>

namespace Viking {
    class Camera {
    public:
        Camera() = default;
        explicit Camera(const glm::mat4& projection): mProjection(projection) {}
        ~Camera() = default;

        [[nodiscard]] const glm::mat4& getProjection() const {
            return mProjection;
        }

    protected:
        glm::mat4 mProjection = glm::mat4(1.0f);
    };
}

#endif //VIKING_CAMERA_H
