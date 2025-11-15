#pragma once

#include <iostream>
#include <cassert>

// Флаг отладочной сборки
#ifndef DEBUG_BUILD
#define DEBUG_BUILD 1
#endif

// Макросы для условного логирования и ассертов
#if DEBUG_BUILD
    #define DEBUG_LOG(message) std::cout << "DebugLog: " << message << std::endl
    #define DEBUG_ASSERT(condition) assert(condition)
#else
    #define DEBUG_LOG(message)
    #define DEBUG_ASSERT(condition)
#endif
