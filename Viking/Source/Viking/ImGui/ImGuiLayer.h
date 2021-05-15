//
// Created by batzi on 15.05.2021.
//

#ifndef VIKING_IMGUILAYER_H
#define VIKING_IMGUILAYER_H

#include "Viking/Core/Layer.h"

#include "Viking/Events/ApplicationEvent.h"
#include "Viking/Events/KeyEvent.h"
#include "Viking/Events/MouseEvent.h"

namespace Viking {
    class ImGuiLayer: public Layer {
    public:
        ImGuiLayer();
        ~ImGuiLayer() = default;

        virtual void onAttach() override;
        virtual void onDetach() override;
        virtual void onEvent(Event& event) override;

        void begin();
        void end();

        void blockEvents(bool block) {
            mBlockEvents = block;
        }

        void setDarkThemeColors();

    private:
        bool mBlockEvents{true};
        float mTime{0.0f};
    };
}

#endif //VIKING_IMGUILAYER_H
