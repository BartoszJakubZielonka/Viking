#include <Viking.h>
#include <Viking/Core/EntryPoint.h>

#include "Sandbox2D.h"

class Sandbox : public Viking::Application {
public:
    Sandbox(Viking::ApplicationCommandLineArgs args) : Application("Sandbox", args)
    {
        pushLayer(new Sandbox2D());
    }

    ~Sandbox() override = default;
};

Viking::Application* Viking::createApplication(ApplicationCommandLineArgs args)
{
    return new Sandbox(args);
}
