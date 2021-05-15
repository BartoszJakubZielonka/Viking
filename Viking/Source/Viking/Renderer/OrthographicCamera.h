//
// Created by batzi on 15.05.2021.
//

#ifndef VIKING_ORTHOGRAPHICCAMERA_H
#define VIKING_ORTHOGRAPHICCAMERA_H

#include <glm/glm.hpp>

namespace Viking {
    class OrthographicCamera {
    public:
        OrthographicCamera(float left, float right, float bottom, float top);

        void setProjection(float left, float right, float bottom, float top);

        [[nodiscard]] const glm::vec3& getPosition() const {
            return mPosition;
        }
        void setPosition(const glm::vec3& position) {
            mPosition = position;
            recalculateViewMatrix();
        }

        [[nodiscard]] float getRotation() const {
            return mRotation;
        }
        void setRotation(float rotation) {
            mRotation = rotation;
            recalculateViewMatrix();
        }

        [[nodiscard]] const glm::mat4& getProjectionMatrix() const {
            return mProjectionMatrix;
        }
        [[nodiscard]] const glm::mat4& getViewMatrix() const {
            return mViewMatrix;
        }
        [[nodiscard]] const glm::mat4& getViewProjectionMatrix() const {
            return mViewProjectionMatrix;
        }
    private:
        void recalculateViewMatrix();

        glm::mat4 mProjectionMatrix{};
        glm::mat4 mViewMatrix{};
        glm::mat4 mViewProjectionMatrix{};

        glm::vec3 mPosition = { 0.0f, 0.0f, 0.0f };
        float mRotation{ 0.0f };
    };
}

#endif //VIKING_ORTHOGRAPHICCAMERA_H
