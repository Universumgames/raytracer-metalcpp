#pragma once
#include <nlohmann/json.hpp>

#include "SFML/System/Vector2.hpp"
#include "SFML/System/Vector3.hpp"


namespace RayTracing {
    template <typename T,
    typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    struct Vector2 {
        T x, y;
        Vector2() : x(0), y(0) {}
        Vector2(T x, T y) : x(x), y(y) {}
        Vector2(const Vector2 &other) : x(other.x), y(other.y) {}

        float length() const {
            return sqrt(x * x + y * y);
        }

        Vector2 operator /(T t) {
            return Vector2(x / t, y / t);
        }
        Vector2 operator/(const Vector2 &other) {
            return Vector2(x / other.x, y / other.y);
        }
        Vector2 operator*(T t) {
            return Vector2(x * t, y * t);
        }
        Vector2 operator*(const Vector2 &other) {
            return Vector2(x * other.x, y * other.y);
        }
        Vector2 operator-(const Vector2 &other) {
            return Vector2(x - other.x, y - other.y);
        }
        Vector2 operator+(const Vector2 &other) {
            return Vector2(x + other.x, y + other.y);
        }
        Vector2 normalized() const {
            return {x / length(), y / length()};
        }

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Vector2, x, y)

        static Vector2 up(){ return Vector2(0, 1); }
        static Vector2 down(){ return Vector2(0, -1); }
        static Vector2 left(){ return Vector2(-1, 0); }
        static Vector2 right(){ return Vector2(1, 0); }

        static Vector2 zero() { return Vector2(0, 0); }
    };

    typedef Vector2<float> Vec2;

    template <typename T,
    typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    struct Vector3 {
        T x, y, z;

        Vector3() : x(0), y(0), z(0) {}
        Vector3(T x, T y, T z) : x(x), y(y), z(z) {}
        Vector3(const Vector3 &other) : x(other.x), y(other.y), z(other.z) {}
        explicit Vector3(const Vector2<T> &other, T z = 0) : x(other.x), y(other.y), z(z) {}

        float length() const {
            return sqrt(x * x + y * y + z * z);
        }
        Vector3 operator/(T t) {
            return Vector3(x / t, y / t, z / t);
        }
        Vector3 operator/(const Vector3 &other) {
            return Vector3(x / other.x, y / other.y, z / other.z);
        }
        Vector3 operator*(T t) {
            return Vector3(x * t, y * t, z * t);
        }
        Vector3 operator*(const Vector3 &other) {
            return Vector3(x * other.x, y * other.y, z * other.z);
        }
        Vector3 operator-(const Vector3 &other) {
            return Vector3(x - other.x, y - other.y, z - other.z);
        }
        Vector3 operator+(const Vector3 &other) {
            return Vector3(x + other.x, y + other.y, z + other.z);
        }

        Vector3 normalized() const {
            return {x / length(), y / length(), z / length()};
        }
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Vector3, x, y, z)

        static Vector3 up(){ return Vector3(0, 1, 0); }
        static Vector3 down(){ return Vector3(0, -1, 0); }
        static Vector3 left(){ return Vector3(-1, 0, 0); }
        static Vector3 right(){ return Vector3(1, 0, 0); }
        static Vector3 forward(){ return Vector3(0, 0, 1); }
        static Vector3 back(){ return Vector3(0, 0, -1); }

        static Vector3 zero() { return Vector3(0, 0, 0); }
    };

    typedef Vector3<float> Vec3;

    template <typename T,
    typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    struct Vector4 {
        T x, y, z, w;
        Vector4() : x(0), y(0), z(0), w(0) {}
        Vector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
        Vector4(const Vector4& other) : x(other.x), y(other.y), z(other.z), w(other.w) {}
        explicit Vector4(const Vector3<T>& other, T w = 0): x(other.x), y(other.y), z(other.z), w(w) {}

        float length() const {
            return sqrt(x * x + y * y + z * z + w * w);
        }

        Vector4 operator/(T t) {
            return Vector4(x / t, y / t, z / t, w / t);
        }
        Vector4 operator*(T t) {
            return Vector4(x * t, y * t, z * t, w * t);
        }
        Vector4 operator-(const Vector4 &other) {
            return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
        }
        Vector4 operator+(const Vector4 &other) {
            return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
        }
        Vector4 normalized() const {
            return {x / length(), y / length(), z / length(), w / length()};
        }
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Vector4, x, y, z, w)

        static Vector4 zero() { return Vector4(0, 0, 0, 0); }
    };

}
