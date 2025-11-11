#pragma once

inline float frac(float f) {
    return f - static_cast<long>(f);
}