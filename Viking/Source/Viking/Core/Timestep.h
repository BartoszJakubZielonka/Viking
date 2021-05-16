//
// Created by batzi on 15.05.2021.
//

#ifndef VIKING_TIMESTEP_H
#define VIKING_TIMESTEP_H

namespace Viking {
    class Timestep {
    public:
        Timestep(float time = 0.0f): mTime(time) {}

        operator float() const {
            return mTime;
        }

        [[nodiscard]] float getSeconds() const {
            return mTime;
        }

        [[nodiscard]] float getMilliseconds() const {
            return mTime * 1000.0f;
        }

    private:
        float mTime;
    };
}

#endif //VIKING_TIMESTEP_H
