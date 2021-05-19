#pragma once

#include "Viking/Core/Base.h"
#include "Viking/Scene/Scene.h"
#include "Viking/Scene/Entity.h"

class SceneHierarchyPanel
{
public:
    SceneHierarchyPanel() = default;
    SceneHierarchyPanel(const Viking::Ref<Viking::Scene>& scene);

    void setContext(const Viking::Ref<Viking::Scene>& scene);

    void onImGuiRender();

    Viking::Entity getSelectedEntity() const { return mSelectionContext; }
    void setSelectedEntity(Viking::Entity entity);
 private:
    void drawEntityNode(Viking::Entity entity);
    void drawComponents(Viking::Entity entity);

    Viking::Ref<Viking::Scene> mContext;
    Viking::Entity mSelectionContext;
    };
