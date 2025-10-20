#pragma once
#include <type_traits>

namespace RayTracing {
    template<typename T,
        typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    class Matrix2 {
        public:
        T values[2][2];

    };
}
