#include "EditorLayer.h"
#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Viking/Scene/SceneSerializer.h"

#include "Viking/Utils/PlatformUtils.h"

#include "ImGuizmo.h"

#include "Viking/Math/Math.h"

EditorLayer::EditorLayer(): Layer("EditorLayer"), mCameraController(1280.0f / 720.0f), mSquareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
{
}

void EditorLayer::onAttach()
{
    VI_PROFILE_FUNCTION();

    mCheckerboardTexture = Viking::Texture2D::create("assets/textures/Checkerboard.png");

    Viking::FramebufferSpecification fbSpec;
    fbSpec.Attachments = {Viking::FramebufferTextureFormat::RGBA8, Viking::FramebufferTextureFormat::RED_INTEGER, Viking::FramebufferTextureFormat::Depth };
    fbSpec.Width = 1280;
    fbSpec.Height = 720;
    mFramebuffer = Viking::Framebuffer::create(fbSpec);

    mActiveScene = Viking::createRef<Viking::Scene>();

    auto commandLineArgs = Viking::Application::get().getCommandLineArgs();
    if (commandLineArgs.Count > 1)
    {
        auto sceneFilePath = commandLineArgs[1];
        Viking::SceneSerializer serializer(mActiveScene);
        serializer.deserialize(sceneFilePath);
    }

    mEditorCamera = Viking::EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);
    mSceneHierarchyPanel.setContext(mActiveScene);
}

void EditorLayer::onDetach()
{
    VI_PROFILE_FUNCTION();
}

void EditorLayer::onUpdate(Viking::Timestep ts)
{
    VI_PROFILE_FUNCTION();

    //Resize
    if (Viking::FramebufferSpecification spec = mFramebuffer->getSpecification(); mViewportSize.x > 0.0f && mViewportSize.y > 0.0f && /* zero sized framebuffer is invalid */(spec.Width != mViewportSize.x || spec.Height != mViewportSize.y))
    {
        mFramebuffer->resize((uint32_t)mViewportSize.x, (uint32_t)mViewportSize.y);
        mCameraController.onResize(mViewportSize.x, mViewportSize.y);
        mEditorCamera.setViewportSize(mViewportSize.x, mViewportSize.y);
        mActiveScene->onViewportResize((uint32_t)mViewportSize.x, (uint32_t)mViewportSize.y);
    }

    // Update
    if (mViewportFocused)
        mCameraController.onUpdate(ts);

    mEditorCamera.onUpdate(ts);

    // Render
    Viking::Renderer2D::resetStats();
    mFramebuffer->bind();
    Viking::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
    Viking::RenderCommand::clear();

    // Clear our entity ID attachment to -1
    mFramebuffer->clearAttachment(1, -1);

    // Update scene
    mActiveScene->onUpdateEditor(ts, mEditorCamera);

    auto [mx, my] = ImGui::GetMousePos();
    mx -= mViewportBounds[0].x;
    my -= mViewportBounds[0].y;
    glm::vec2 viewportSize = mViewportBounds[1] - mViewportBounds[0];
    my = viewportSize.y - my;
    int mouseX = (int)mx;
    int mouseY = (int)my;

    if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
    {
        int pixelData = mFramebuffer->readPixel(1, mouseX, mouseY);
        mHoveredEntity = pixelData == -1 ? Viking::Entity() : Viking::Entity((entt::entity)pixelData, mActiveScene.get());
    }

    mFramebuffer->unbind();
}

void EditorLayer::onImGuiRender()
{
    VI_PROFILE_FUNCTION();

    // Note: Switch this to true to enable dockspace
    static bool dockspaceOpen = true;
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
    float minWinSizeX = style.WindowMinSize.x;
    style.WindowMinSize.x = 370.0f;
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    style.WindowMinSize.x = minWinSizeX;

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows, 
            // which we can't undo at the moment without finer window depth/z control.
            //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);1
            if (ImGui::MenuItem("New", "Ctrl+N"))
                newScene();

            if (ImGui::MenuItem("Open...", "Ctrl+O"))
                openScene();

            if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
                saveSceneAs();

            if (ImGui::MenuItem("Exit")) Viking::Application::get().close();
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    mSceneHierarchyPanel.onImGuiRender();

    ImGui::Begin("Stats");

    std::string name = "None";
    if (mHoveredEntity)
        name = mHoveredEntity.getComponent<Viking::TagComponent>().Tag;
    ImGui::Text("Hovered Entity: %s", name.c_str());

    auto stats = Viking::Renderer2D::getStats();
    ImGui::Text("Renderer2D Stats:");
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quads: %d", stats.QuadCount);
    ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
    ImGui::Text("Indices: %d", stats.getTotalIndexCount());

    ImGui::End();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
    ImGui::Begin("Viewport");
    auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
    auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
    auto viewportOffset = ImGui::GetWindowPos();
    mViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
    mViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

    mViewportFocused = ImGui::IsWindowFocused();
    mViewportHovered = ImGui::IsWindowHovered();
    Viking::Application::get().getImGuiLayer()->blockEvents(!mViewportFocused && !mViewportHovered);

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    mViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

    uint64_t textureID = mFramebuffer->getColorAttachmentRendererId();
    ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ mViewportSize.x, mViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

    // Gizmos
    Viking::Entity selectedEntity = mSceneHierarchyPanel.getSelectedEntity();
    if (selectedEntity && mGizmoType != -1)
    {
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();

        ImGuizmo::SetRect(mViewportBounds[0].x, mViewportBounds[0].y, mViewportBounds[1].x - mViewportBounds[0].x, mViewportBounds[1].y - mViewportBounds[0].y);

        // Camera

        // Runtime camera from entity
        // auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
        // const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
        // const glm::mat4& cameraProjection = camera.GetProjection();
        // glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

        // Editor camera
        const glm::mat4& cameraProjection = mEditorCamera.getProjection();
        glm::mat4 cameraView = mEditorCamera.getViewMatrix();

        // Entity transform
        auto& tc = selectedEntity.getComponent<Viking::TransformComponent>();
        glm::mat4 transform = tc.getTransform();

        // Snapping
        bool snap = Viking::Input::isKeyPressed(Viking::Key::LeftControl);
        float snapValue = 0.5f; // Snap to 0.5m for translation/scale
        // Snap to 45 degrees for rotation
        if (mGizmoType == ImGuizmo::OPERATION::ROTATE)
            snapValue = 45.0f;

        float snapValues[3] = { snapValue, snapValue, snapValue };

        ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
            (ImGuizmo::OPERATION)mGizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
            nullptr, snap ? snapValues : nullptr);

        if (ImGuizmo::IsUsing())
        {
            glm::vec3 translation, rotation, scale;
            Viking::Math::decomposeTransform(transform, translation, rotation, scale);

            glm::vec3 deltaRotation = rotation - tc.Rotation;
            tc.Translation = translation;
            tc.Rotation += deltaRotation;
            tc.Scale = scale;
        }
    }


    ImGui::End();
    ImGui::PopStyleVar();

    ImGui::End();
}

void EditorLayer::onEvent(Viking::Event& e)
{
    mCameraController.onEvent(e);
    mEditorCamera.onEvent(e);

    Viking::EventDispatcher dispatcher(e);
    dispatcher.dispatch<Viking::KeyPressedEvent>(VI_BIND_EVENT_FN(EditorLayer::onKeyPressed));
    dispatcher.dispatch<Viking::MouseButtonPressedEvent>(VI_BIND_EVENT_FN(EditorLayer::onMouseButtonPressed));
}

bool EditorLayer::onKeyPressed(Viking::KeyPressedEvent& e)
{
    // Shortcuts
    if (e.getRepeatCount() > 0)
        return false;

    bool control = Viking::Input::isKeyPressed(Viking::Key::LeftControl) || Viking::Input::isKeyPressed(Viking::Key::RightControl);
    bool shift = Viking::Input::isKeyPressed(Viking::Key::LeftShift) || Viking::Input::isKeyPressed(Viking::Key::RightShift);

    switch (e.getKeyCode())
    {
    case Viking::Key::N:
    {
        if (control)
            newScene();

        break;
    }
    case Viking::Key::O:
    {
        if (control)
            openScene();

        break;
    }
    case Viking::Key::S:
    {
        if (control && shift)
            saveSceneAs();

        break;
    }

    // Gizmos
    case Viking::Key::Q:
    {
        if (!ImGuizmo::IsUsing())
            mGizmoType = -1;
        break;
    }
    case Viking::Key::W:
    {
        if (!ImGuizmo::IsUsing())
            mGizmoType = ImGuizmo::OPERATION::TRANSLATE;
        break;
    }
    case Viking::Key::E:
    {
        if (!ImGuizmo::IsUsing())
            mGizmoType = ImGuizmo::OPERATION::ROTATE;
        break;
    }
    case Viking::Key::R:
    {
        if (!ImGuizmo::IsUsing())
            mGizmoType = ImGuizmo::OPERATION::SCALE;
        break;
    }
    }
}

bool EditorLayer::onMouseButtonPressed(Viking::MouseButtonPressedEvent& e)
{
    if (e.getMouseButton() == Viking::Mouse::ButtonLeft)
    {
        if (mViewportHovered && !ImGuizmo::IsOver() && !Viking::Input::isKeyPressed(Viking::Key::LeftAlt))
            mSceneHierarchyPanel.setSelectedEntity(mHoveredEntity);
    }
    return false;
}

void EditorLayer::newScene()
{
    mActiveScene = Viking::createRef<Viking::Scene>();
    mActiveScene->onViewportResize((uint32_t)mViewportSize.x, (uint32_t)mViewportSize.y);
    mSceneHierarchyPanel.setContext(mActiveScene);
}

void EditorLayer::openScene()
{
    std::string filepath = Viking::FileDialogs::openFile("Viking Scene (*.viking)\0*.viking\0");
    if (!filepath.empty())
    {
        mActiveScene = Viking::createRef<Viking::Scene>();
        mActiveScene->onViewportResize((uint32_t)mViewportSize.x, (uint32_t)mViewportSize.y);
        mSceneHierarchyPanel.setContext(mActiveScene);

        Viking::SceneSerializer serializer(mActiveScene);
        serializer.deserialize(filepath);
    }
}

void EditorLayer::saveSceneAs()
{
    std::string filepath = Viking::FileDialogs::saveFile("Viking Scene (*.viking)\0*.viking\0");
    if (!filepath.empty())
    {
        Viking::SceneSerializer serializer(mActiveScene);
        serializer.serialize(filepath);
    }
}
