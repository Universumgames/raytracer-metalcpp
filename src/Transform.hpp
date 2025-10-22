#pragma once
#include "matrices.hpp"

namespace RayTracing {
    struct Transform {
    private:
        Mat3x3 rotationMatrix;

    public:
        Vec3 position;
        Quaternion quaternion;

        Transform() = default;

        Transform(const Vec3 &position, const Quaternion &quaternion) : position(position), quaternion(quaternion) {
        }

        [[nodiscard]] Mat3x3 getRotationMatrix() const;

        void updateRotationMatrix() {
            auto q = quaternion;
            std::vector<std::vector<double> > rotations = {
                {
                    2 * (q[1] * q[1] + q[2] * q[3]) - 1, 2 * (q[2] * q[3] - q[1] * q[0]),
                    2 * (q[2] * q[0] + q[1] * q[3])
                },
                {
                    2 * (q[2] * q[3] + q[1] * q[0]), 2 * (q[1] * q[1] + q[3] * q[3]) - 1,
                    2 * (q[3] * q[0] - q[1] * q[2])
                },
                {
                    2 * (q[2] * q[0] - q[1] * q[3]), 2 * (q[3] * q[0] + q[1] * q[2]),
                    2 * (q[1] * q[1] + q[0] * q[0]) - 1
                }
            };
            rotationMatrix = Mat3x3();
            for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 3; col++) {
                    rotationMatrix.setValue(row, col, (float) rotations[row][col]);
                }
            }
        }
    };
}
