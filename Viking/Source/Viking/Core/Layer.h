//
// Created by batzi on 15.05.2021.
//

#ifndef VIKING_LAYER_H
#define VIKING_LAYER_H

#include "Viking/Core/Base.h"
#include "Viking/Core/Timestep.h"
#include "Viking/Events/Event.h"

namespace Viking {
    class Layer {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer() = default;

        virtual void onAttach() {}
        virtual void onDetach() {}
        virtual void onUpdate(Timestep ts) {}
        virtual void onImGuiRender() {}
        virtual void onEvent(Event& event) {}

        [[nodiscard]] const std::string& getName() const {
            return mDebugName;
        }

    private:
        std::string mDebugName;
    };
}

#endif //VIKING_LAYER_H
