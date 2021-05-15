//
// Created by batzi on 15.05.2021.
//
#include "vipch.h"
#include "RenderCommand.h"

namespace Viking {
    Scope<RendererAPI> RenderCommand::sRendererAPI = RendererAPI::create();
}