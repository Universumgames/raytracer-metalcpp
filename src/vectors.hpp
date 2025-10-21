#pragma once
#include <cstdarg>
#include <nlohmann/json.hpp>
#include "Color.hpp"

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
            values[0] = x;
            values[1] = y;
        }

        Vector(T x, T y, T z) requires (X == 3) {
            values[0] = x;
            values[1] = y;
            values[2] = z;
        }

        Vector(T x, T y, T z, T w) requires (X == 4) {
            values[0] = x;
            values[1] = y;
            values[2] = z;
            values[3] = w;
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

        Vector(const Color &other) requires (X == 4) {
            values[0] = other.r;
            values[1] = other.g;
            values[2] = other.b;
            values[3] = other.a;
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

        T getValue(unsigned dim) const {
            return values[dim];
        }

        [[nodiscard]] float length() const {
            float sum = 0;
            for (unsigned int i = 0; i < X; i++) {
                sum += values[i] * values[i];
            }
            return sqrt(sum);
        }

        Vector normalized() const {
            float length = this->length();
            T v[X];
            for (unsigned int i = 0; i < X; i++) {
                v[i] = values[i] / length;
            }
            return Vector(v);
        }

        float dot(const Vector &other) const {
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

        Vector &operator+=(const Color &other) requires (X == 4) {
            values[0] += (T) other.r;
            values[1] += (T) other.g;
            values[2] += (T) other.b;
            values[3] += (T) other.a;
            return *this;
        }

        T operator[](unsigned i) const {
            return values[i];
        }

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Vector, values)

        [[nodiscard]] Color asColor() const requires (X == 3) {
            return Color(values[0], values[1], values[2], (T) 255);
        }

        [[nodiscard]] Color asColor() const requires (X == 4) {
            return Color(values[0], values[1], values[2], values[3]);
        }

        static Vector zero() {
            return Vector((T) 0);
        }

        static Vector up() requires (X >= 2) { return Vector((T) 0).setValue(Y_AXIS, 1); }
        static Vector down() requires (X >= 2) { return Vector((T) 0).setValue(Y_AXIS, -1); }
        static Vector left() requires (X >= 2) { return Vector((T) 0).setValue(X_AXIS, 0); }
        static Vector right() requires (X >= 2) { return Vector((T) 0).setValue(X_AXIS, 1); }
        static Vector forward() requires (X >= 3) { return Vector((T) 0).setValue(Z_AXIS, 1); }
        static Vector backward() requires (X >= 3) { return Vector((T) 0).setValue(Z_AXIS, -1); }

        // for 2d to 3d the axis names stay identical
        T &x() requires (X >= 1 && X <= 3) { return values[0]; }
        T &y() requires (X >= 2 && X <= 3) { return values[1]; }
        T &z() requires (X == 3) { return values[2]; }

        // in 4d w get pushed before x so the indices change
        T &w() requires (X >= 4) { return values[0]; }
        T &x() requires (X >= 4) { return values[1]; }
        T &y() requires (X >= 4) { return values[2]; }
        T &z() requires (X >= 4) { return values[3]; }
    };

    typedef Vector<2, float> Vec2;
    typedef Vector<3, float> Vec3;
    typedef Vector<4, float> Vec4;
    typedef Vector<4, double> Quaternion;
}
