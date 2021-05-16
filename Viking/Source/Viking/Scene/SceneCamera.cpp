//
// Created by batzi on 16.05.2021.
//
#include "vipch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Viking {

    SceneCamera::SceneCamera() {
        recalculateProjection();
    }

    void SceneCamera::setPerspective(float verticalFOV, float nearClip, float farClip) {
        mProjectionType = ProjectionType::Perspective;
        mPerspectiveFOV = verticalFOV;
        mPerspectiveNear = nearClip;
        mPerspectiveFar = farClip;

        recalculateProjection();
    }

    void SceneCamera::setOrthographic(float size, float nearClip, float farClip) {
        mProjectionType = ProjectionType::Orthographic;
        mOrthographicSize = size;
        mOrthographicNear = nearClip;
        mOrthographicFar = farClip;
        recalculateProjection();
    }

    void SceneCamera::setViewportSize(uint32_t width, uint32_t height) {
        VI_CORE_ASSERT(width > 0 && height > 0);
        mAspectRatio = (float)width / (float)height;
        recalculateProjection();
    }

    void SceneCamera::recalculateProjection() {
        if (mProjectionType == ProjectionType::Perspective)
        {
            mProjection = glm::perspective(mPerspectiveFOV, mAspectRatio, mPerspectiveNear, mPerspectiveFar);
        }
        else
        {
            float orthoLeft = -mOrthographicSize * mAspectRatio * 0.5f;
            float orthoRight = mOrthographicSize * mAspectRatio * 0.5f;
            float orthoBottom = -mOrthographicSize * 0.5f;
            float orthoTop = mOrthographicSize * 0.5f;

            mProjection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, mOrthographicNear, mOrthographicFar);
        }
    }
}