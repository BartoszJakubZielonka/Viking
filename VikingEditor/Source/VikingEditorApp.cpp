#include <Viking.h>
#include <Viking/Core/EntryPoint.h>

#include "EditorLayer.h"

#ifdef VI_PLATFORM_WINDOWS
extern "C" {
    __declspec(dllexport) uint32_t NvOptimusEnablement = 0x00000001;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif

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
