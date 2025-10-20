#include "Color.hpp"

#include "vectors.hpp"

namespace RayTracing {

    Color Color::avg(std::vector<Color> colors) {
        if (colors.empty()) {
            return {};
        }
        if (colors.size() == 1) {
            return colors[0];
        }
        Vec4 avgColor = colors[0];
        for (int i = 1; i < colors.size(); i++) {
            avgColor += colors[i];
        }
        return (avgColor / ((float)colors.size())).asColor();
    }
}
