#include <Viking.h>
#include <Viking/Core/EntryPoint.h>

#include "EditorLayer.h"

class VikingEditor final: public Viking::Application
{
public:
    VikingEditor(Viking::ApplicationCommandLineArgs args): Application("Viking Editor", args)
    {
        pushLayer(new EditorLayer());
    }

    ~VikingEditor() override = default;
};

Viking::Application* createApplication(Viking::ApplicationCommandLineArgs args)
{
    return new VikingEditor(args);
}
