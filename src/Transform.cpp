#include "Transform.hpp"

#include <complex>

namespace RayTracing {
    Mat4x4 Transform::calcTranslationMatrix() {
        return Mat4x4({
            {1, 0, 0, position.getX()},
            {0, 1, 0, position.getY()},
            {0, 0, 1, position.getZ()},
            {0, 0, 0, 1}
        });
    }

    Mat4x4 Transform::calcScaleMatrix() {
        return Mat4x4({
            {scale.getX(), 0, 0, 0},
            {0, scale.getY(), 0, 0},
            {0, 0, scale.getZ(), 0},
            {0, 0, 0, 1}
        });
    }

    Mat4x4 Transform::calcRotationMatrix() {
        auto q = Quaternion::fromEuler(rotation);
        float w = q.getW(), x = q.getX(), y = q.getY(), z = q.getZ();

        Mat4x4 localRotMat = Mat4x4({
            {1 - 2 * (y * y + z * z), 2 * (x * y - z * w), 2 * (x * z + y * w), 0},
            {2 * (x * y + z * w), 1 - 2 * (x * x + z * z), 2 * (y * z - x * w), 0},
            {2 * (x * z - y * w), 2 * (y * z + x * w), 1 - 2 * (x * x + y * y), 0},
            {0, 0, 0, 1}
        });

        return localRotMat;
    }

    Vec3 Transform::getTranslation() const {
        return position;
    }

    Vec3 Transform::getScale() const {
        return scale;
    }

    Vec3 Transform::getRotation() const {
        return rotation;
    }

    void Transform::setTranslation(const Vec3 &translation) {
        this->position = translation;
    }

    void Transform::setScale(const Vec3 &scale) {
        this->scale = scale;
    }

    void Transform::setRotation(const Vec3 &rotation) {
        this->rotation = rotation;
    }

    Mat4x4 Transform::getTransformationMatrix() const {
        return transformationMatrix;
    }

    void Transform::update() {
        scaleMatrix = calcScaleMatrix();
        rotationMatrix = calcRotationMatrix();
        translationMatrix = calcTranslationMatrix();
        transformationMatrix = translationMatrix * rotationMatrix * scaleMatrix;
    }

    Vec3 Transform::getTransformedPosition(const Vec3 &pos) const {
        return (transformationMatrix * Vec4(pos, 1)).cutoff();
    }

    Vec3 Transform::getTransformedNormal(const Vec3 &pos) const {
        return (rotationMatrix * Vec4(pos, 0)).cutoff();
    }
}
