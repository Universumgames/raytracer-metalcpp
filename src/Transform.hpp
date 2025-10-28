#pragma once
#include "math/matrices.hpp"

namespace RayTracing {
    struct Transform {
    private:
        Mat4x4 transformationMatrix{};
        Mat4x4 inverseTransformationMatrix{};
        Mat4x4 rotationMatrix{};
        Mat4x4 inverseRotationMatrix{};
        Mat4x4 scaleMatrix;
        Mat4x4 inverseScaleMatrix;
        Mat4x4 translationMatrix{};
        Mat4x4 inverseTranslationMatrix{};
        Vec3 position{};
        //Quaternion quaternion;
        Vec3 rotation{};
        Vec3 scale{1, 1, 1};

    public:
        Transform() = default;

        Transform(const Vec3 &position, const Vec3 &rad, const Vec3 &scale = Vec3(1)) : position(position),
            rotation(rad), scale(scale) {
        }

        [[nodiscard]] Vec3 getTranslation() const;

        [[nodiscard]] Vec3 getScale() const;

        [[nodiscard]] Vec3 getRotation() const;

        void setTranslation(const Vec3 &translation);

        void setScale(const Vec3 &scale);

        void setRotation(const Vec3 &rotation);

        void update();

        [[nodiscard]] Mat4x4 getTransformMatrix() const;

        [[nodiscard]] Mat4x4 getInverseTransformMatrix() const;

        [[nodiscard]] Mat4x4 getInverseRotationMatrix() const;

        [[nodiscard]] Mat4x4 getTransformationMatrix() const;

        [[nodiscard]] Vec3 getTransformedPosition(const Vec3 &pos) const;

        [[nodiscard]] Vec3 getInverseTransformedPosition(const Vec3 &pos) const;

        [[nodiscard]] Vec3 getTransformedNormal(const Vec3 &pos) const;

        [[nodiscard]] Vec3 getTransformedRayDirection(const Vec3 &dir) const;

        [[nodiscard]] Mat4x4 getTranslationMatrix() const;

        [[nodiscard]] Mat4x4 getRotationMatrix() const;

        [[nodiscard]] Mat4x4 getScaleMatrix() const;

    private:
        Mat4x4 calcTranslationMatrix(const Vec3 &translation);

        Mat4x4 calcInverseTranslationMatrix();

        Mat4x4 calcScaleMatrix(const Vec3 &scale);

        Mat4x4 calcInverseScaleMatrix();

        Mat4x4 calcRotationMatrix(const Vec3 &rotation);

        Mat4x4 calcInverseRotationMatrix();

    protected:
        friend class SequentialRayTracer;
        friend struct Ray;
    };
}