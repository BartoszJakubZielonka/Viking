//
// Created by batzi on 15.05.2021.
//
#include "vipch.h"
#include "LayerStack.h"

namespace Viking {
    LayerStack::~LayerStack() {
        for(Layer* layer: mLayers) {
            layer->onDetach();
            delete layer;
        }
    }

    void LayerStack::pushLayer(Layer *layer) {
        mLayers.emplace(mLayers.begin() + mLayerInsertIndex, layer);
        mLayerInsertIndex++;
    }

    void LayerStack::PushOverlay(Layer* overlay)
    {
        mLayers.emplace_back(overlay);
    }

    void LayerStack::PopLayer(Layer* layer)
    {
        auto it = std::find(mLayers.begin(), mLayers.begin() + mLayerInsertIndex, layer);
        if (it != mLayers.begin() + mLayerInsertIndex)
        {
            layer->OnDetach();
            mLayers.erase(it);
            mLayerInsertIndex--;
        }
    }

    void LayerStack::PopOverlay(Layer* overlay)
    {
        auto it = std::find(mLayers.begin() + mLayerInsertIndex, mLayers.end(), overlay);
        if (it != mLayers.end())
        {
            overlay->OnDetach();
            mLayers.erase(it);
        }
    }
}