//
// Created by batzi on 16.05.2021.
//

#ifndef VIKING_PLATFORMUTILS_H
#define VIKING_PLATFORMUTILS_H

#include <string>

namespace Viking {
    class FileDialogs {
    public:
        // These return empty strings if cancelled
        static std::string openFile(const char* filter);
        static std::string saveFile(const char* filter);
    };
}

#endif //VIKING_PLATFORMUTILS_H
