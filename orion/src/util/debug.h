#pragma once

#include <iostream>

#ifndef NDEBUG
#   define ASS(condition, message) \
    do { \
        if (! (condition)) { \
            std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
                      << " line " << __LINE__ << ": " << message << std::endl; \
            std::terminate(); \
        } \
    } while (false)
#else
#   define ASS(condition, message) do { } while (false)
#endif

