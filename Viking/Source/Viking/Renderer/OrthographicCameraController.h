//
// Created by batzi on 15.05.2021.
//

#ifndef VIKING_ORTHOGRAPHICCAMERACONTROLLER_H
#define VIKING_ORTHOGRAPHICCAMERACONTROLLER_H

#include "Viking/Renderer/OrthographicCamera.h"
#include "Viking/Core/Timestep.h"

#include "Viking/Events/ApplicationEvent.h"
#include "Viking/Events/MouseEvent.h"

namespace Viking {
    class OrthographicCameraController {
    public:
        explicit OrthographicCameraController(float aspectRatio, bool rotation = false);

        void onUpdate(Timestep ts);
        void onEvent(Event& e);

        void onResize(float width, float height);

        OrthographicCamera& GetCamera() {
            return mCamera;
        }
        [[nodiscard]] const OrthographicCamera& GetCamera() const {
            return mCamera;
        }

        [[nodiscard]] float getZoomLevel() const {
            return mZoomLevel;
        }

        void setZoomLevel(float level) {
            mZoomLevel = level;
        }

    private:
        bool onMouseScrolled(MouseScrolledEvent& e);
        bool onWindowResized(WindowResizeEvent& e);

        float mAspectRatio;
        float mZoomLevel{ 1.0f };
        OrthographicCamera mCamera;

        bool mRotation;

        glm::vec3 mCameraPosition = { 0.0f, 0.0f, 0.0f };
        float mCameraRotation{ 0.0f }; //In degrees, in the anti-clockwise direction
        float mCameraTranslationSpeed{ 5.0f }, mCameraRotationSpeed{ 180.0f };
    };
}

#endif //VIKING_ORTHOGRAPHICCAMERACONTROLLER_H
