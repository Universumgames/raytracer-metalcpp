#pragma once
#include "../Color.hpp"
#include "../math/vectors.hpp"
#include "BoundigBox.hpp"
#include "../Transform.hpp"


namespace RayTracing {
    struct SerializableRayTraceableObject {
        RGBA8 color;
        Vec3 position;
        Vec3 rotation;
    };

    class RayTraceableObject {
    private:
    public:
        RGBf color{};
        Transform transform{};
        BoundingBox boundingBox{};

        RayTraceableObject() = default;

        RayTraceableObject(const RGBf &color = {}, const Vec3 &position = {}, const Vec3 &scale = Vec3(1),
                           const Vec3 rad = {}) : color(color), transform({position, rad, scale}) {
        }

        virtual ~RayTraceableObject() = default;

        virtual void updateBoundingBox() = 0;
    };
}