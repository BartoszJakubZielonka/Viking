//
// Created by batzi on 15.05.2021.
//

#ifndef VIKING_LAYERSTACK_H
#define VIKING_LAYERSTACK_H

#include "Viking/Core/Base.h"
#include "Viking/Core/Layer.h"

#include <vector>

namespace Viking {
    class LayerStack {
    public:
        LayerStack() = default;
        ~LayerStack();

        void pushLayer(Layer* layer);
        void pushOverlay(Layer* overlay);
        void popLayer(Layer* layer);
        void popOverlay(Layer* overlay);

        std::vector<Layer*>::iterator begin() { return mLayers.begin(); }
        std::vector<Layer*>::iterator end() { return mLayers.end(); }
        std::vector<Layer*>::reverse_iterator rbegin() { return mLayers.rbegin(); }
        std::vector<Layer*>::reverse_iterator rend() { return mLayers.rend(); }

        [[nodiscard]] std::vector<Layer*>::const_iterator begin() const { return mLayers.begin(); }
        [[nodiscard]] std::vector<Layer*>::const_iterator end()	const { return mLayers.end(); }
        [[nodiscard]] std::vector<Layer*>::const_reverse_iterator rbegin() const { return mLayers.rbegin(); }
        [[nodiscard]] std::vector<Layer*>::const_reverse_iterator rend() const { return mLayers.rend(); }

    private:
        std::vector<Layer*> mLayers;
        unsigned int mLayerInsertIndex{ 0 };
    };
}

#endif //VIKING_LAYERSTACK_H
