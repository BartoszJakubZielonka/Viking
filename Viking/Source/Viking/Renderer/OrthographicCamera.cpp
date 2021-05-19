//
// Created by batzi on 15.05.2021.
//
#include "vipch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Viking {

    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top): mProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), mViewMatrix(1.0f) {
        VI_PROFILE_FUNCTION();
        mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
    }

    void OrthographicCamera::setProjection(float left, float right, float bottom, float top) {
        VI_PROFILE_FUNCTION();

        mProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
        mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
    }

    void OrthographicCamera::recalculateViewMatrix() {
        VI_PROFILE_FUNCTION();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), mPosition) * glm::rotate(glm::mat4(1.0f), glm::radians(mRotation), glm::vec3(0, 0, 1));

        mViewMatrix = glm::inverse(transform);
        mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
    }
}
