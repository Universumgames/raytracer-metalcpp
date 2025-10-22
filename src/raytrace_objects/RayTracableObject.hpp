#pragma once
#include <utility>

#include "../Color.hpp"
#include "../matrices.hpp"
#include "../vectors.hpp"
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

        RayTraceableObject(const RGBf &color = {}, const Vec3 &position = {},
                           const Quaternion &quaternion = {}) : color(color), transform({position, quaternion}) {
        }

        virtual ~RayTraceableObject() = default;

        Mat3x3 updateRotationMatrix();

        __deprecated
        Mat3x3 getRotationMatrix() const;

        __deprecated
        Vec3 getTranslatedRotatedPoint(const Mat3x3 &rotationMatrix, const Vec3 &point) const;

        __deprecated
        static Vec3 getRotatedNormal(const Mat3x3 &rotationMatrix, const Vec3 &normal);

        __deprecated
        void rotateEuler(Vec3 rad);

        virtual void updateBoundingBox() = 0;
    };
}
