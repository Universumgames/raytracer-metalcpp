#include "Transform.hpp"

namespace RayTracing {
    Mat4x4 Transform::calcTranslationMatrix(const Vec3 &translation) {
        return Mat4x4({
            {1, 0, 0, translation.getX()},
            {0, 1, 0, translation.getY()},
            {0, 0, 1, translation.getZ()},
            {0, 0, 0, 1}
        });
    }

    Mat4x4 Transform::calcInverseTranslationMatrix() {
        return calcTranslationMatrix(position * -1.0f);
    }

    Mat4x4 Transform::calcScaleMatrix(const Vec3 &scale) {
        return Mat4x4({
            {scale.getX(), 0, 0, 0},
            {0, scale.getY(), 0, 0},
            {0, 0, scale.getZ(), 0},
            {0, 0, 0, 1}
        });
    }

    Mat4x4 Transform::calcInverseScaleMatrix() {
        return calcScaleMatrix(Vec3(
            1.0f / scale.getX(),
            1.0f / scale.getY(),
            1.0f / scale.getZ()
        ));
    }


    Mat4x4 Transform::calcRotationMatrix(const Vec3 &rotation) {
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

    Mat4x4 Transform::calcInverseRotationMatrix() {
        return calcRotationMatrix(rotation * -1.0f);
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

    Mat4x4 Transform::getInverseTransformMatrix() const {
        return inverseTransformationMatrix;
    }

    Mat4x4 Transform::getInverseRotationMatrix() const {
        return inverseRotationMatrix;
    }

    Mat4x4 Transform::getInverseScaleMatrix() const {
        return inverseScaleMatrix;
    }

    void Transform::update() {
        scaleMatrix = calcScaleMatrix(scale);
        inverseScaleMatrix = calcInverseScaleMatrix();
        rotationMatrix = calcRotationMatrix(rotation);
        inverseRotationMatrix = calcInverseRotationMatrix();
        translationMatrix = calcTranslationMatrix(position);
        inverseTranslationMatrix = calcInverseTranslationMatrix();
        transformationMatrix = translationMatrix * rotationMatrix * scaleMatrix;
        inverseTransformationMatrix = inverseScaleMatrix * inverseRotationMatrix * inverseTranslationMatrix;
    }

    Vec3 Transform::getTransformedPosition(const Vec3 &pos) const {
        return (transformationMatrix * Vec4(pos, 1)).cutoff();
    }

    Vec3 Transform::getTransformedNormal(const Vec3 &pos) const {
        return (rotationMatrix * Vec4(pos, 1)).cutoff();
    }

    Vec3 Transform::getTransformedRayDirection(const Vec3 &dir) const {
        return (inverseRotationMatrix * inverseScaleMatrix * Vec4(dir, 1)).cutoff();
    }

    Vec3 Transform::getInverseTransformedPosition(const Vec3 &pos) const {
        return (inverseTransformationMatrix * Vec4(pos, 1)).cutoff();
    }

    Mat4x4 Transform::getTransformMatrix() const {
        return transformationMatrix;
    }

    Mat4x4 Transform::getTranslationMatrix() const {
        return translationMatrix;
    }

    Mat4x4 Transform::getRotationMatrix() const {
        return rotationMatrix;
    }

    Mat4x4 Transform::getScaleMatrix() const {
        return scaleMatrix;
    }
}