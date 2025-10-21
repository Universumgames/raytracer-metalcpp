#include "RayTracableObject.hpp"

#include <iostream>
#include <stl_reader.h>

namespace RayTracing {
    void MeshedRayTraceableObject::loadMesh(const std::string &stlFilePath) {
        mesh = new Mesh();
        try {
            stl_reader::StlMesh<float, unsigned int> stl_mesh(stlFilePath);

            for (size_t itri = 0; itri < stl_mesh.num_tris(); ++itri) {
                std::vector<Vec3> vertices;
                for (size_t icorner = 0; icorner < 3; ++icorner) {
                    const float *c = stl_mesh.tri_corner_coords(itri, icorner);
                    mesh->points.emplace_back(c[0], c[1], c[2]);
                    mesh->indices.emplace_back(mesh->indices.size());
                    vertices.emplace_back(c[0], c[1], c[2]);
                }

                const float *n = stl_mesh.tri_normal(itri);
                mesh->normals.emplace_back(n[0], n[1], n[2]);
                mesh->numTriangles++;
            }
        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }

    Mat3x3 RayTraceableObject::getRotationMatrix() const {
        auto q = quaternion;
        std::vector<std::vector<double>> rotations = {
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
        Mat3x3 rotationMatrix = Mat3x3();
        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 3; col++) {
                rotationMatrix.setValue(row, col, (float)rotations[row][col]);
            }
        }
        return rotationMatrix;
    }

    Vec3 RayTraceableObject::getTranslatedRotatedPoint(const Mat3x3 &rotationMatrix, const Vec3& point) const {
        return getRotatedNormal(rotationMatrix, point) - position;
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

        this->quaternion += q;
    }


}
