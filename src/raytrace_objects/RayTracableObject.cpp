#include "RayTracableObject.hpp"

#include <iostream>
#include <stl_reader.h>

namespace RayTracing {
    Mat3x3 RayTraceableObject::updateRotationMatrix() {
        transform.updateRotationMatrix();
        return transform.getRotationMatrix();
    }

    Mat3x3 RayTraceableObject::getRotationMatrix() const {
        return transform.getRotationMatrix();
    }

    Vec3 RayTraceableObject::getTranslatedRotatedPoint(const Mat3x3 &rotationMatrix, const Vec3& point) const {
        return rotationMatrix * point - transform.position;
    }

    Vec3 RayTraceableObject::getRotatedNormal(const Mat3x3 &rotationMatrix, const Vec3 &normal) {
        return rotationMatrix * normal;
    }


    void RayTraceableObject::rotateEuler(Vec3 rad) {
        double roll = rad.x();
        double pitch = rad.z();
        double yaw = rad.y();
        double cr = cos(roll * 0.5);
        double sr = sin(roll * 0.5);
        double cp = cos(pitch * 0.5);
        double sp = sin(pitch * 0.5);
        double cy = cos(yaw * 0.5);
        double sy = sin(yaw * 0.5);

        Quaternion q;
        q.w() = cr * cp * cy + sr * sp * sy;
        q.x() = sr * cp * cy - cr * sp * sy;
        q.y() = cr * sp * cy + sr * cp * sy;
        q.z() = cr * cp * sy - sr * sp * cy;

        this->transform.quaternion += q;
    }


}
