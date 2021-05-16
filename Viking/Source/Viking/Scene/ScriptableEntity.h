//
// Created by batzi on 16.05.2021.
//

#ifndef VIKING_SCRIPTABLEENTITY_H
#define VIKING_SCRIPTABLEENTITY_H

#include "Entity.h"

namespace Viking {
    class ScriptableEntity {
    public:
        ~ScriptableEntity() = default;

        template<typename T>
        T &getComponent() {
            return mEntity.getComponent<T>();
        }

    protected:
        virtual void onCreate() {}

        virtual void onDestroy() {}

        virtual void onUpdate(Timestep ts) {}

    private:
        Entity mEntity;

        friend class Scene;
    };
}

#endif //VIKING_SCRIPTABLEENTITY_H
