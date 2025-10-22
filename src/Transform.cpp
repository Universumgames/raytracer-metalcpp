#include "Transform.hpp"

namespace RayTracing {
    Mat3x3 Transform::getRotationMatrix() const {
        return rotationMatrix;
    }

}