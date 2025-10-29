#pragma once

//#define DEBUG_INITIAL_RAY_GENERATION // generate python files to debug ray directions


#define TODO() \
    do { \
        throw std::runtime_error("TODO at " __FILE__ ":" + std::to_string(__LINE__)); \
    } while (0)

#define TODO_MSG(msg) \
    do { \
        throw std::runtime_error(std::string("TODO at ") + __FILE__ + ":" + std::to_string(__LINE__) + " - " + msg); \
    } while (0)


#define _USE_MATH_DEFINES
#include <cmath>