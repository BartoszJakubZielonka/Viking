#pragma once

#include "Viking.h"

class Sandbox2D final: public Viking::Layer
{
public:
    Sandbox2D();
    ~Sandbox2D() override = default;

    void onAttach() override;
    void onDetach() override;

    void onUpdate(Viking::Timestep ts) override;
    void onImGuiRender() override;
    void onEvent(Viking::Event& event) override;

private:
    Viking::OrthographicCameraController mCameraController;

    //temp
    Viking::Ref<Viking::VertexArray> mSquareVA;
    Viking::Ref<Viking::Shader> mFlatColorShader;

    Viking::Ref<Viking::Texture2D> mCheckerboardTexture;

    glm::vec4 mSquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};
