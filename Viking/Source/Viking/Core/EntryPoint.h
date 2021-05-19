//
// Created by batzi on 15.05.2021.
//

#include "Viking/Core/Base.h"
#include "Viking/Core/Application.h"

#ifndef VIKING_ENTRYPOINT_H
#define VIKING_ENTRYPOINT_H

#ifdef VI_PLATFORM_WINDOWS
extern Viking::Application* createApplication(Viking::ApplicationCommandLineArgs args);

int main(int argc, char** argv) {
    Viking::Log::init();

    VI_PROFILE_BEGIN_SESSION("Startup", "VikingProfile-Startup.json");
    auto app = createApplication({ argc, argv });
    VI_PROFILE_END_SESSION();

    VI_PROFILE_BEGIN_SESSION("Runtime", "VikingProfile-Runtime.json");
    app->run();
    VI_PROFILE_END_SESSION();

    VI_PROFILE_BEGIN_SESSION("Shutdown", "VikingProfile-Shutdown.json");
    delete app;
    VI_PROFILE_END_SESSION();
}
#endif

#endif //VIKING_ENTRYPOINT_H
