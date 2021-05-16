//
// Created by batzi on 16.05.2021.
//

#ifndef VIKING_SCENECAMERA_H
#define VIKING_SCENECAMERA_H

#include "Viking/Renderer/Camera.h"

namespace Viking {
    class SceneCamera: public Camera {
    public:
        enum class ProjectionType {
            Perspective = 0,
            Orthographic = 1
        };

        SceneCamera();
        ~SceneCamera() = default;

        void setPerspective(float verticalFOV, float nearClip, float farClip);
        void setOrthographic(float size, float nearClip, float farClip);

        void setViewportSize(uint32_t width, uint32_t height);

        float getPerspectiveVerticalFOV() const {
            return mPerspectiveFOV;
        }

        void setPerspectiveVerticalFOV(float verticalFov) {
            mPerspectiveFOV = verticalFov;
            recalculateProjection();
        }

        float getPerspectiveNearClip() const {
            return mPerspectiveNear;
        }

        void setPerspectiveNearClip(float nearClip) {
            mPerspectiveNear = nearClip;
            recalculateProjection();
        }

        float getPerspectiveFarClip() const {
            return mPerspectiveFar;
        }

        void setPerspectiveFarClip(float farClip) {
            mPerspectiveFar = farClip;
            recalculateProjection();
        }

        float getOrthographicSize() const {
            return mOrthographicSize;
        }

        void setOrthographicSize(float size) {
            mOrthographicSize = size;
            recalculateProjection();
        }

        float getOrthographicNearClip() const {
            return mOrthographicNear;
        }

        void setOrthographicNearClip(float nearClip) {
            mOrthographicNear = nearClip;
            recalculateProjection();
        }

        float getOrthographicFarClip() const {
            return mOrthographicFar;
        }

        void setOrthographicFarClip(float farClip) {
            mOrthographicFar = farClip;
            recalculateProjection();
        }

        ProjectionType getProjectionType() const {
            return mProjectionType;
        }

        void setProjectionType(ProjectionType type) {
            mProjectionType = type;
            recalculateProjection();
        }

    private:
        void recalculateProjection();

        ProjectionType mProjectionType{ProjectionType::Perspective};

        float mPerspectiveFOV{glm::radians(45.0f)};
        float mPerspectiveNear{0.01f}, mPerspectiveFar{1000.0f};

        float mOrthographicSize{10.0f};
        float mOrthographicNear{-1.0f}, mOrthographicFar{1.0f};

        float mAspectRatio{0.0f};
    };
}

#endif //VIKING_SCENECAMERA_H
