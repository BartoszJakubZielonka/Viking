
#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>

#include "Viking/Scene/Components.h"
#include <cstring>

/* The Microsoft C++ compiler is non-compliant with the C++ standard and needs
 * the following definition to disable a security warning on std::strncpy().
 */
#ifdef _MSVC_LANG
#define _CRT_SECURE_NO_WARNINGS
#endif

SceneHierarchyPanel::SceneHierarchyPanel(const Viking::Ref<Viking::Scene>& context)
{
    setContext(context);
}

void SceneHierarchyPanel::setContext(const Viking::Ref<Viking::Scene>& context)
{
    mContext = context;
    mSelectionContext = {};
}

void SceneHierarchyPanel::onImGuiRender()
{
    ImGui::Begin("Scene Hierarchy");

    mContext->mRegistry.each([&](auto entityID)
    {
        Viking::Entity entity{ entityID , mContext.get() };
        drawEntityNode(entity);
    });

    if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
        mSelectionContext = {};

    // Right-click on blank space
    if (ImGui::BeginPopupContextWindow(0, 1, false))
    {
        if (ImGui::MenuItem("Create Empty Entity"))
            mContext->createEntity("Empty Entity");

        ImGui::EndPopup();
    }

    ImGui::End();

    ImGui::Begin("Properties");
    if (mSelectionContext)
    {
        drawComponents(mSelectionContext);
    }

    ImGui::End();
}

void SceneHierarchyPanel::setSelectedEntity(Viking::Entity entity)
{
    mSelectionContext = entity;
}

void SceneHierarchyPanel::drawEntityNode(Viking::Entity entity)
{
    auto& tag = entity.getComponent<Viking::TagComponent>().Tag;

    ImGuiTreeNodeFlags flags = ((mSelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
    flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
    bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
    if (ImGui::IsItemClicked())
        mSelectionContext = entity;

    bool entityDeleted = false;
    if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::MenuItem("Delete Entity"))
            entityDeleted = true;

        ImGui::EndPopup();
    }

    if (opened)
    {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
        bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
        if (opened)
            ImGui::TreePop();
        ImGui::TreePop();
    }

    if (entityDeleted)
    {
        mContext->destroyEntity(entity);
        if (mSelectionContext == entity)
            mSelectionContext = {};
    }
}

static void drawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
{
    ImGuiIO& io = ImGui::GetIO();
    auto boldFont = io.Fonts->Fonts[0];

    ImGui::PushID(label.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::Text(label.c_str());
    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
    ImGui::PushFont(boldFont);
    if (ImGui::Button("X", buttonSize))
        values.x = resetValue;
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Y", buttonSize))
        values.y = resetValue;
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Z", buttonSize))
        values.z = resetValue;
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();

    ImGui::PopStyleVar();

    ImGui::Columns(1);

    ImGui::PopID();
}

template<typename T, typename UIFunction>
static void drawComponent(const std::string& name, Viking::Entity entity, UIFunction uiFunction)
{
    const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
    if (entity.hasComponent<T>())
    {
        auto& component = entity.getComponent<T>();
        ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImGui::Separator();
        bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
        ImGui::PopStyleVar();
        ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
        if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
        {
            ImGui::OpenPopup("ComponentSettings");
        }

        bool removeComponent = false;
        if (ImGui::BeginPopup("ComponentSettings"))
        {
            if (ImGui::MenuItem("Remove component"))
                removeComponent = true;

            ImGui::EndPopup();
        }

        if (open)
        {
            uiFunction(component);
            ImGui::TreePop();
        }

        if (removeComponent)
            entity.removeComponent<T>();
    }
}

void SceneHierarchyPanel::drawComponents(Viking::Entity entity)
{
    if (entity.hasComponent<Viking::TagComponent>())
    {
        auto& tag = entity.getComponent<Viking::TagComponent>().Tag;

        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        std::strncpy(buffer, tag.c_str(), sizeof(buffer));
        if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
        {
            tag = std::string(buffer);
        }
    }

    ImGui::SameLine();
    ImGui::PushItemWidth(-1);

    if (ImGui::Button("Add Component"))
        ImGui::OpenPopup("AddComponent");

    if (ImGui::BeginPopup("AddComponent"))
    {
        if (ImGui::MenuItem("Camera"))
        {
            if (!mSelectionContext.hasComponent<Viking::CameraComponent>())
                mSelectionContext.addComponent<Viking::CameraComponent>();
            else
                VI_CORE_WARN("This entity already has the Camera Component!");
            ImGui::CloseCurrentPopup();
        }

        if (ImGui::MenuItem("Sprite Renderer"))
        {
            if (!mSelectionContext.hasComponent<Viking::SpriteRendererComponent>())
                mSelectionContext.addComponent<Viking::SpriteRendererComponent>();
            else
                VI_CORE_WARN("This entity already has the Sprite Renderer Component!");
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    ImGui::PopItemWidth();

    drawComponent<Viking::TransformComponent>("Transform", entity, [](auto& component)
    {
        drawVec3Control("Translation", component.Translation);
        glm::vec3 rotation = glm::degrees(component.Rotation);
        drawVec3Control("Rotation", rotation);
        component.Rotation = glm::radians(rotation);
        drawVec3Control("Scale", component.Scale, 1.0f);
    });

    drawComponent<Viking::CameraComponent>("Camera", entity, [](auto& component)
    {
        auto& camera = component.Camera;

        ImGui::Checkbox("Primary", &component.Primary);

        const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
        const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.getProjectionType()];
        if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
        {
            for (int i = 0; i < 2; i++)
            {
                bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
                if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
                {
                    currentProjectionTypeString = projectionTypeStrings[i];
                    camera.setProjectionType((Viking::SceneCamera::ProjectionType)i);
                }

                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }

        if (camera.getProjectionType() == Viking::SceneCamera::ProjectionType::Perspective)
        {
            float perspectiveVerticalFov = glm::degrees(camera.getPerspectiveVerticalFOV());
            if (ImGui::DragFloat("Vertical FOV", &perspectiveVerticalFov))
                camera.setPerspectiveVerticalFOV(glm::radians(perspectiveVerticalFov));

            float perspectiveNear = camera.getPerspectiveNearClip();
            if (ImGui::DragFloat("Near", &perspectiveNear))
                camera.setPerspectiveNearClip(perspectiveNear);

            float perspectiveFar = camera.getPerspectiveFarClip();
            if (ImGui::DragFloat("Far", &perspectiveFar))
            camera.setPerspectiveFarClip(perspectiveFar);
        }

        if (camera.getProjectionType() == Viking::SceneCamera::ProjectionType::Orthographic)
        {
            float orthoSize = camera.getOrthographicSize();
            if (ImGui::DragFloat("Size", &orthoSize))
                camera.setOrthographicSize(orthoSize);

            float orthoNear = camera.getOrthographicNearClip();
            if (ImGui::DragFloat("Near", &orthoNear))
                camera.setOrthographicNearClip(orthoNear);

            float orthoFar = camera.getOrthographicFarClip();
            if (ImGui::DragFloat("Far", &orthoFar))
                camera.setOrthographicFarClip(orthoFar);

            ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);
        }
    });

    drawComponent<Viking::SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component)
    {
        ImGui::ColorEdit4("Color", glm::value_ptr(component.color));
    });
}
