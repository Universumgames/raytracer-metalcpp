#pragma once
#include "../Color.hpp"
#include "../math/vectors.hpp"
#include "BoundigBox.hpp"
#include "../Transform.hpp"


namespace RayTracing {
    /// Serializable representation of a ray traceable object - base struct
    struct SerializableRayTraceableObject {
        RGBA8 color;
        float specularIntensity;
        float position[3];
        float rotation[3];
    };

    /// Base class for all ray traceable objects
    class RayTraceableObject {
    private:
    public:
        RGBf color{};
        float specularIntensity{};
        Transform transform{};
        BoundingBox boundingBox{};

        RayTraceableObject() = default;

        explicit RayTraceableObject(const RGBf &color = {}, float specularIntensity = 1, const Vec3 &position = Vec3{},
                                    const Vec3 &scale = Vec3(1),
                                    const Vec3 &rad = Vec3{}) : color(color), specularIntensity(specularIntensity),
                                                                transform({position, rad, scale}) {
        }

        virtual ~RayTraceableObject() = default;

        /// Update the bounding box of the object
        virtual void updateBoundingBox() = 0;
    };
}