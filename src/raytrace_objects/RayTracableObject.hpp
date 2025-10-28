#pragma once
#include "../Color.hpp"
#include "../math/vectors.hpp"
#include "BoundigBox.hpp"
#include "../Transform.hpp"


namespace RayTracing {
    /// Serializable representation of a ray traceable object - base struct
    struct SerializableRayTraceableObject {
        RGBA8 color;
        Vec3 position;
        Vec3 rotation;
    };

    /// Base class for all ray traceable objects
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

        /// Update the bounding box of the object
        virtual void updateBoundingBox() = 0;
    };
}