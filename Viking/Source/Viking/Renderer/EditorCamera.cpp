//
// Created by batzi on 16.05.2021.
//
#include "vipch.h"
#include "EditorCamera.h"

#include "Viking/Core/Input.h"
#include "Viking/Core/KeyCodes.h"
#include "Viking/Core/MouseCodes.h"

#include <glfw/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Viking {

    EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip): mFOV(fov), mAspectRatio(aspectRatio), mNearClip(nearClip), mFarClip(farClip), Camera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip)) {
        updateView();
    }

    void EditorCamera::onUpdate(Timestep ts) {
        if(Input::isKeyPressed(Key::LeftAlt)) {
            const glm::vec2& mouse{ Input::getMouseX(), Input::getMouseY() };
            glm::vec2 delta = (mouse - mInitialMousePosition) * 0.003f;
            mInitialMousePosition = mouse;

            if (Input::isMouseButtonPressed(Mouse::ButtonMiddle))
                MousePan(delta);
            else if (Input::isMouseButtonPressed(Mouse::ButtonLeft))
                MouseRotate(delta);
            else if (Input::isMouseButtonPressed(Mouse::ButtonRight))
                MouseZoom(delta.y);
        }

        updateView();
    }

    void EditorCamera::onEvent(Event &e) {
        EventDispatcher dispatcher(e);
        dispatcher.dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(EditorCamera::onMouseScroll));
    }

    glm::vec3 EditorCamera::getUpDirection() const
    {
        return glm::rotate(getOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::vec3 EditorCamera::getRightDirection() const
    {
        return glm::rotate(getOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
    }

    glm::vec3 EditorCamera::getForwardDirection() const
    {
        return glm::rotate(getOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
    }

    glm::quat EditorCamera::getOrientation() const
    {
        return glm::quat(glm::vec3(-mPitch, -mYaw, 0.0f));
    }

    void EditorCamera::updateProjection() {
        mAspectRatio = mViewportWidth / mViewportHeight;
        mProjection = glm::perspective(glm::radians(mFOV), mAspectRatio, mNearClip, mFarClip);
    }

    void EditorCamera::updateView() {
        // m_Yaw = m_Pitch = 0.0f; // Lock the camera's rotation
        mPosition = calculatePosition();

        glm::quat orientation = getOrientation();
        mViewMatrix = glm::translate(glm::mat4(1.0f), mPosition) * glm::toMat4(orientation);
        mViewMatrix = glm::inverse(mViewMatrix);
    }

    bool EditorCamera::onMouseScroll(MouseScrolledEvent &e) {
        float delta = e.getYOffset() * 0.1f;
        mouseZoom(delta);
        updateView();
        return false;
    }

    void EditorCamera::mousePan(const glm::vec2& delta)
    {
        auto [xSpeed, ySpeed] = panSpeed();
        mFocalPoint += -getRightDirection() * delta.x * xSpeed * mDistance;
        mFocalPoint += getUpDirection() * delta.y * ySpeed * mDistance;
    }

    void EditorCamera::mouseRotate(const glm::vec2& delta)
    {
        float yawSign = getUpDirection().y < 0 ? -1.0f : 1.0f;
        mYaw += yawSign * delta.x * rotationSpeed();
        mPitch += delta.y * rotationSpeed();
    }

    void EditorCamera::mouseZoom(float delta)
    {
        mDistance -= delta * zoomSpeed();
        if (mDistance < 1.0f)
        {
            mFocalPoint += getForwardDirection();
            mDistance = 1.0f;
        }
    }

    glm::vec3 EditorCamera::calculatePosition() const
    {
        return mFocalPoint - getForwardDirection() * mDistance;
    }

    std::pair<float, float> EditorCamera::panSpeed() const
    {
        float x = std::min(mViewportWidth / 1000.0f, 2.4f); // max = 2.4f
        float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

        float y = std::min(mViewportHeight / 1000.0f, 2.4f); // max = 2.4f
        float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

        return { xFactor, yFactor };
    }

    float EditorCamera::rotationSpeed() const
    {
        return 0.8f;
    }

    float EditorCamera::zoomSpeed() const
    {
        float distance = mDistance * 0.2f;
        distance = std::max(distance, 0.0f);
        float speed = distance * distance;
        speed = std::min(speed, 100.0f); // max speed = 100
        return speed;
    }
}
