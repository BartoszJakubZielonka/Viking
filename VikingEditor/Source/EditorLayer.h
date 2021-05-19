#pragma once

#include <Viking.h>
#include "Panels/SceneHierarchyPanel.h"

#include "Viking/Renderer/EditorCamera.h"

class EditorLayer : public Viking::Layer
{
public:
    EditorLayer();
    ~EditorLayer() override = default;

    void onAttach() override;
    void onDetach() override;

    void onUpdate(Viking::Timestep ts) override;
    void onImGuiRender() override;
    void onEvent(Viking::Event& e) override;
private:
    bool onKeyPressed(Viking::KeyPressedEvent& e);
    bool onMouseButtonPressed(Viking::MouseButtonPressedEvent& e);

    void newScene();
    void openScene();
    void saveSceneAs();

    Viking::OrthographicCameraController mCameraController;

    // Temp
    Viking::Ref<Viking::VertexArray> mSquareVA;
    Viking::Ref<Viking::Shader> mFlatColorShader;
    Viking::Ref<Viking::Framebuffer> mFramebuffer;

    Viking::Ref<Viking::Scene> mActiveScene;
    Viking::Entity mSquareEntity;
    Viking::Entity mCameraEntity;
    Viking::Entity mSecondCamera;

    Viking::Entity mHoveredEntity;

    bool mPrimaryCamera = true;

    Viking::EditorCamera mEditorCamera;

    Viking::Ref<Viking::Texture2D> mCheckerboardTexture;

    bool mViewportFocused{ false }, mViewportHovered{ false };
    glm::vec2 mViewportSize = { 0.0f, 0.0f };
    glm::vec2 mViewportBounds[2];

    glm::vec4 mSquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

    int mGizmoType{ -1 };

    // Panels
    SceneHierarchyPanel mSceneHierarchyPanel;
};
