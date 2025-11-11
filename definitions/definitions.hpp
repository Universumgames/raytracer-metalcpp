#pragma once

//#define DEBUG_INITIAL_RAY_GENERATION // generate python files to debug ray directions


/// Macro to mark to-do locations in code
#define TODO() \
    do { \
        throw std::runtime_error("TODO at " __FILE__ ":" + std::to_string(__LINE__)); \
    } while (0)

/// Macro to mark to-do locations in code with message
#define TODO_MSG(msg) \
    do { \
        throw std::runtime_error(std::string("TODO at ") + __FILE__ + ":" + std::to_string(__LINE__) + " - " + msg); \
    } while (0)


#define _USE_MATH_DEFINES /// for math constants like M_PI
#include <cmath>