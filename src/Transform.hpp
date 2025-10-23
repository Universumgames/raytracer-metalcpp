#pragma once
#include "math/matrices.hpp"

namespace RayTracing {
    struct Transform {
    private:
        Mat4x4 transformationMatrix{};
        Mat4x4 rotationMatrix{};
        Mat4x4 scaleMatrix;
        Mat4x4 translationMatrix{};
        Vec3 position{};
        //Quaternion quaternion;
        Vec3 rotation{};
        Vec3 scale{1, 1, 1};

    public:

        Transform() = default;

        Transform(const Vec3 &position, const Vec3 &rad, const Vec3 &scale = Vec3(1)) : position(position),
            rotation(rad), scale(scale) {
        }

        Vec3 getTranslation() const;

        Vec3 getScale() const;

        Vec3 getRotation() const;

        void setTranslation(const Vec3 &translation);

        void setScale(const Vec3 &scale);

        void setRotation(const Vec3 &rotation);

        void update();

    private:
        Mat4x4 calcTranslationMatrix();

        Mat4x4 calcScaleMatrix();

        Mat4x4 calcRotationMatrix();

    protected:
        Mat4x4 getTransformationMatrix() const;

        Vec3 getTransformedPosition(const Vec3 &pos) const;

        Vec3 getTransformedNormal(const Vec3 &pos) const;

        friend class SequentialRayTracer;
    };
}
