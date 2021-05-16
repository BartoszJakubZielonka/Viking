//
// Created by batzi on 16.05.2021.
//

#ifndef VIKING_EDITORCAMERA_H
#define VIKING_EDITORCAMERA_H

#include "Camera.h"
#include "Viking/Core/Timestep.h"
#include "Viking/Events/Event.h"
#include "Viking/Events/MouseEvent.h"

#include <glm/glm.hpp>

namespace Viking {
    class EditorCamera final: public Camera {
    public:
        EditorCamera() = default;
        EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

        void onUpdate(Timestep ts);
        void onEvent(Event& e);

        [[nodiscard]] inline float getDistance() const {
            return mDistance;
        }
        inline void setDistance(float distance) {
            mDistance = distance;
        }

        inline void setViewportSize(float width, float height) {
            mViewportWidth = width;
            mViewportHeight = height;
            updateProjection();
        }

        [[nodiscard]] const glm::mat4& getViewMatrix() const {
            return mViewMatrix;
        }
        [[nodiscard]] glm::mat4 getViewProjection() const {
            return mProjection * mViewMatrix;
        }

        [[nodiscard]] glm::vec3 getUpDirection() const;
        [[nodiscard]] glm::vec3 getRightDirection() const;
        [[nodiscard]] glm::vec3 getForwardDirection() const;
        [[nodiscard]] const glm::vec3& getPosition() const {
            return mPosition;
        }
        [[nodiscard]] glm::quat getOrientation() const;

        [[nodiscard]] float getPitch() const {
            return mPitch;
        }
        [[nodiscard]] float getYaw() const {
            return mYaw;
        }

    private:
        void updateProjection();
        void updateView();

        bool onMouseScroll(MouseScrolledEvent& e);

        void mousePan(const glm::vec2& delta);
        void mouseRotate(const glm::vec2& delta);
        void mouseZoom(float delta);

        [[nodiscard]] glm::vec3 calculatePosition() const;

        [[nodiscard]] std::pair<float, float> panSpeed() const;
        [[nodiscard]] float rotationSpeed() const;
        [[nodiscard]] float zoomSpeed() const;

        float mFOV{45.0f}, mAspectRatio{1.778f}, mNearClip{0.1f}, mFarClip{1000.0f};

        glm::mat4 mViewMatrix;
        glm::vec3 mPosition = { 0.0f, 0.0f, 0.0f };
        glm::vec3 mFocalPoint = { 0.0f, 0.0f, 0.0f };

        glm::vec2 mInitialMousePosition = { 0.0f, 0.0f };

        float mDistance{10.0f};
        float mPitch{0.0f}, mYaw{0.0f};

        float mViewportWidth{1280}, mViewportHeight{720};
    };
}

#endif //VIKING_EDITORCAMERA_H
