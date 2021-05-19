//
// Created by batzi on 15.05.2021.
//

#ifndef VIKING_VIPCH_H
#define VIKING_VIPCH_H

#include "Viking/Core/PlatformDetection.h"

#ifdef VI_PLATFORM_WINDOWS
#ifndef NOMINMAX
		// See github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
		#define NOMINMAX
	#endif
#endif

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Viking/Core/Base.h"

#include "Viking/Core/Log.h"

#include "Viking/Debug/Instrumentor.h"

#ifdef VI_PLATFORM_WINDOWS
#include <Windows.h>
#endif

#endif //VIKING_VIPCH_H
