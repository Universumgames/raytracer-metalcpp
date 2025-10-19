#pragma once
#include <vector>

#include "simples.hpp"

namespace RayTracing {
    class RayTraceableObject {
    public:
        virtual ~RayTraceableObject() = default;

        Point position;
        Color color;

        RayTraceableObject(Point position, Color color) : position(position), color(color) {
        }

        virtual bool intersect(Ray ray) { return false; }
    };

    typedef std::vector<RayTraceableObject> Scene;
}
