//
// Created by batzi on 15.05.2021.
//
#include "vipch.h"
#include "OrthographicCameraController.h"

#include "Viking/Core/Input.h"
#include "Viking/Core/KeyCodes.h"

namespace Viking {

    OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation): mAspectRatio(aspectRatio), mCamera(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel), mRotation(rotation) {

    }

    void OrthographicCameraController::onUpdate(Timestep ts) {
        VI_PROFILE_FUNCTION();

        if (Input::isKeyPressed(Key::A))
        {
            mCameraPosition.x -= cos(glm::radians(mCameraRotation)) * mCameraTranslationSpeed * ts;
            mCameraPosition.y -= sin(glm::radians(mCameraRotation)) * mCameraTranslationSpeed * ts;
        }
        else if (Input::isKeyPressed(Key::D))
        {
            mCameraPosition.x += cos(glm::radians(mCameraRotation)) * mCameraTranslationSpeed * ts;
            mCameraPosition.y += sin(glm::radians(mCameraRotation)) * mCameraTranslationSpeed * ts;
        }

        if (Input::isKeyPressed(Key::W))
        {
            mCameraPosition.x += -sin(glm::radians(mCameraRotation)) * mCameraTranslationSpeed * ts;
            mCameraPosition.y += cos(glm::radians(mCameraRotation)) * mCameraTranslationSpeed * ts;
        }
        else if (Input::isKeyPressed(Key::S))
        {
            mCameraPosition.x -= -sin(glm::radians(mCameraRotation)) * mCameraTranslationSpeed * ts;
            mCameraPosition.y -= cos(glm::radians(mCameraRotation)) * mCameraTranslationSpeed * ts;
        }

        if (mRotation)
        {
            if (Input::isKeyPressed(Key::Q))
                mCameraRotation += mCameraRotationSpeed * ts;
            if (Input::isKeyPressed(Key::E))
                mCameraRotation -= mCameraRotationSpeed * ts;

            if (mCameraRotation > 180.0f)
                mCameraRotation -= 360.0f;
            else if (mCameraRotation <= -180.0f)
                mCameraRotation += 360.0f;

            mCamera.setRotation(mCameraRotation);
        }

        mCamera.setPosition(mCameraPosition);

        mCameraTranslationSpeed = mZoomLevel;
    }

    void OrthographicCameraController::onEvent(Event& e) {
        VI_PROFILE_FUNCTION();

        EventDispatcher dispatcher(e);
        dispatcher.dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::onMouseScrolled));
        dispatcher.dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::onWindowResized));
    }

    void OrthographicCameraController::onResize(float width, float height) {
        mAspectRatio = width / height;
        mCamera.SetProjection(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel);
    }

    bool OrthographicCameraController::onMouseScrolled(MouseScrolledEvent &e) {
        VI_PROFILE_FUNCTION();

        mZoomLevel -= e.getYOffset() * 0.25f;
        mZoomLevel = std::max(mZoomLevel, 0.25f);
        mCamera.SetProjection(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel);
        return false;
    }

    bool OrthographicCameraController::onWindowResized(WindowResizeEvent &e) {
        VI_PROFILE_FUNCTION();

        onResize((float)e.getWidth(), (float)e.getHeight());
        return false;
    }

}