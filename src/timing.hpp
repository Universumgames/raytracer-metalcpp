#pragma once

#include <iostream>
#include <vector>

#define TIMING_START(name) auto start_##name = std::chrono::high_resolution_clock::now();
#define TIMING_END(name) auto end_##name = std::chrono::high_resolution_clock::now(); \
auto duration_##name = std::chrono::duration_cast<std::chrono::milliseconds>(end_##name - start_##name);\
auto duration_##name##_millis = duration_##name.count();

#define TIMING_LOG(name, component, operation) \
std::cout << "[" << component << "] " << operation << " took " << duration_##name##_millis << " ms\n";

#define TIMING_MILLIS(name) (duration_##name##_millis)