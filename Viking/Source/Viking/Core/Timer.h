//
// Created by batzi on 15.05.2021.
//

#ifndef VIKING_TIMER_H
#define VIKING_TIMER_H

#include <chrono>

namespace Viking {
    class Timer {
    public:
        Timer() {
            reset();
        }

        void reset() {
            mStart = std::chrono::high_resolution_clock::now();
        }

        float elapsed() {
            return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - mStart).count() * 0.001f * 0.001f * 0.001f;
        }

        float elapsedMillis() {
            return elapsed() * 1000.0f;
        }

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> mStart;
    };
}

#endif //VIKING_TIMER_H
