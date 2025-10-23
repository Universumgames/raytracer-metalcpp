#pragma once
#include <cstdarg>
#include <nlohmann/json.hpp>

namespace RayTracing {
    template<unsigned int X,
        typename T,
        typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    struct Vector {
    public:
        enum Direction: unsigned {
            X_AXIS = 0,
            Y_AXIS = 1,
            Z_AXIS = 2,
            W_AXIS = 3
        };

        T values[X] = {};

        Vector(T initial = 0) {
            for (unsigned int i = 0; i < X; i++) {
                values[i] = initial;
            }
        }

        // Vector(T value1, T values, ...) requires (X >= 1) {
        //     this->values[0] = value1;
        //     std::va_list args;
        //     va_start(args, values);
        //     for (unsigned int i = 1; i < X; i++) {
        //         this->values[i] = va_arg(args, T);;
        //     }
        // }

        Vector(T x, T y) requires (X == 2) {
            values[X_AXIS] = x;
            values[Y_AXIS] = y;
        }

        Vector(T x, T y, T z) requires (X == 3) {
            values[X_AXIS] = x;
            values[Y_AXIS] = y;
            values[Z_AXIS] = z;
        }

        Vector(T x, T y, T z, T w) requires (X == 4) {
            values[X_AXIS] = x;
            values[Y_AXIS] = y;
            values[Z_AXIS] = z;
            values[W_AXIS] = w;
        }

        Vector(T values[X]) {
            for (unsigned int i = 0; i < X; i++) {
                this->values[i] = values[i];
            }
        }

        Vector(const std::vector<T> &values) {
            for (unsigned int i = 0; i < X; i++) {
                this->values[i] = values[i];
            }
        }

        Vector(const Vector &other) {
            for (unsigned int i = 0; i < X; i++) {
                values[i] = other.values[i];
            }
        }

        /// Dimension expansion
        Vector(const Vector<X - 1, T> &other, T value) {
            for (unsigned int i = 0; i < X - 1; i++) {
                values[i] = other.values[i];
            }
            values[X - 1] = value;
        }

        Vector setValue(unsigned dim, T value) {
            values[dim] = value;
            return *this;
        }

        [[nodiscard]] T getValue(unsigned dim) const {
            return values[dim];
        }

        [[nodiscard]] float length() const {
            float sum = 0;
            for (unsigned int i = 0; i < X; i++) {
                sum += values[i] * values[i];
            }
            return sqrt(sum);
        }

        [[nodiscard]] Vector normalized() const {
            float length = this->length();
            T v[X];
            for (unsigned int i = 0; i < X; i++) {
                v[i] = values[i] / length;
            }
            return Vector(v);
        }

        [[nodiscard]] float dot(const Vector &other) const {
            float sum = 0;
            for (unsigned int i = 0; i < X; i++) {
                sum += values[i] * other.values[i];
            }
            return sum;
        }

        static float dot(const Vector &a, const Vector &b) {
            float sum = 0;
            for (unsigned int i = 0; i < X; i++) {
                sum += a.values[i] * b.values[i];
            }
            return sum;
        }

        [[nodiscard]] bool isNaN() const {
            for (unsigned int i = 0; i < X; i++) {
                if (std::isnan(values[i])) {
                    return true;
                }
            }
            return false;
        }

        template<unsigned int M = X>
        typename std::enable_if<M == 3, Vector<3, T> >::type
        cross(const Vector<3, T> &rhs) const {
            Vector<3, T> result;
            result.values[0] = values[1] * rhs.values[2] - values[2] * rhs.values[1];
            result.values[1] = values[2] * rhs.values[0] - values[0] * rhs.values[2];
            result.values[2] = values[0] * rhs.values[1] - values[1] * rhs.values[0];
            return result;
        }

        template<unsigned int M = X>
        typename std::enable_if<M == 3, Vector<3, T> >::type
        static cross(const Vector<3, T> &a, const Vector<3, T> &b) {
            return a.cross(b);
        }

        Vector operator/(T t) const {
            T v[X];
            for (unsigned int i = 0; i < X; i++) {
                v[i] = values[i] / t;
            }
            return Vector(v);
        }

        Vector operator*(T t) const {
            T v[X];
            for (unsigned int i = 0; i < X; i++) {
                v[i] = values[i] * t;
            }
            return Vector(v);
        }

        Vector operator/(Vector<X, T> &other) const {
            T v[X];
            for (unsigned int i = 0; i < X; i++) {
                v[i] = values[i] / other.values[i];
            }
            return Vector(v);
        }

        Vector operator*(const Vector<X, T> &other) const {
            T v[X];
            for (unsigned int i = 0; i < X; i++) {
                v[i] = values[i] * other.values[i];
            }
            return Vector(v);
        }

        Vector operator+(const Vector<X, T> &other) const {
            T v[X];
            for (unsigned int i = 0; i < X; i++) {
                v[i] = values[i] + other.values[i];
            }
            return Vector(v);
        }

        Vector operator-(const Vector<X, T> &other) const {
            T v[X];
            for (unsigned int i = 0; i < X; i++) {
                v[i] = values[i] - other.values[i];
            }
            return Vector(v);
        }

        bool operator==(const Vector<X, T> &other) const {
            for (unsigned int i = 0; i < X; i++) {
                if (values[i] != other.values[i]) {
                    return false;
                }
            }
            return true;
        }

        Vector &operator+=(const Vector &other) {
            for (unsigned int i = 0; i < X; i++) {
                values[i] += other.values[i];
            }
            return *this;
        }

        T operator[](unsigned i) const {
            return values[i];
        }

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Vector, values)

        static Vector zero() {
            return Vector((T) 0);
        }

        static Vector up() requires (X >= 2) { return Vector((T) 0).setValue(Z_AXIS, 1); }
        static Vector down() requires (X >= 2) { return Vector((T) 0).setValue(Z_AXIS, -1); }
        static Vector right() requires (X >= 2) { return Vector((T) 0).setValue(X_AXIS, 1); }
        static Vector left() requires (X >= 2) { return Vector((T) 0).setValue(X_AXIS, -1); }
        static Vector forward() requires (X >= 3) { return Vector((T) 0).setValue(Y_AXIS, 1); }
        static Vector backward() requires (X >= 3) { return Vector((T) 0).setValue(Y_AXIS, -1); }

        // for 2d to 3d the axis names stay identical
        T &x() requires (X >= 1) { return values[X_AXIS]; }
        T &y() requires (X >= 2) { return values[Y_AXIS]; }
        T &z() requires (X >= 3) { return values[Z_AXIS]; }
        T &w() requires (X >= 4) { return values[W_AXIS]; }

        T getX() const requires (X >= 1) { return values[X_AXIS]; }
        T getY() const requires (X >= 2) { return values[Y_AXIS]; }
        T getZ() const requires (X >= 3) { return values[Z_AXIS]; }
        T getW() const requires (X >= 4) { return values[W_AXIS]; }

        Vector<X - 1, T> cutoff() {
            Vector<X - 1, T> result;
            for (unsigned int i = 0; i < X - 1; i++) {
                result.values[i] = values[i];
            }
            return result;
        }
    };

    typedef Vector<2, float> Vec2;
    typedef Vector<3, float> Vec3;
    typedef Vector<4, float> Vec4;
    typedef Vector<2, unsigned> Vec2u;

#define deg2rad(deg) (deg * (float)M_PI / 180.0f)

    struct Quaternion : public Vector<4, float> {
        static Quaternion fromEuler(const Vec3 &rad) {
            double roll = rad.getY();
            double pitch = rad.getX();
            double yaw = rad.getZ();
            double cr = cos(roll * 0.5);
            double sr = sin(roll * 0.5);
            double cp = cos(pitch * 0.5);
            double sp = sin(pitch * 0.5);
            double cy = cos(yaw * 0.5);
            double sy = sin(yaw * 0.5);

            Quaternion q;
            q.x() = cr * cp * cy + sr * sp * sy;
            q.y() = sr * cp * cy - cr * sp * sy;
            q.z() = cr * sp * cy + sr * cp * sy;
            q.w() = cr * cp * sy - sr * sp * cy;
            return q;
        }

        static Quaternion fromEulerDegree(const Vec3 &deg) {
            return Quaternion::fromEuler(Vec3{deg2rad(deg.getX()),deg2rad(deg.getY()), deg2rad(deg.getZ())});
        }
    };
}