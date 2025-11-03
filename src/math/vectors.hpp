#pragma once
#include <cstdarg>
#include <nlohmann/json.hpp>
#ifdef USE_SHADER_METAL
#include <simd/simd.h>
#endif

#define deg2rad(deg) (deg * (float)M_PI / 180.0f)
#define rad2deg(rad) (rad * 180.0f / (float)M_PI)

namespace RayTracing {
    /// Vector class
    template<unsigned int X,
        typename T,
        typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    struct Vector {
    public:
        /// Directions for dimensions
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

        /**
         * Set value at dimension
         * @param dim dimension to set
         * @param value value to set
         * @return
         */
        Vector setValue(Direction dim, T value) {
            values[dim] = value;
            return *this;
        }

        /**
         * Get value at dimension
         * @param dim dimension to get
         * @return value at dimension
         */
        [[nodiscard]] T getValue(Direction dim) const {
            return values[dim];
        }

        /**
         * Get length of vector
         * @return length
         */
        [[nodiscard]] float length() const {
            float sum = 0;
            for (unsigned int i = 0; i < X; i++) {
                sum += values[i] * values[i];
            }
            return sqrt(sum);
        }

        /**
         * Get normalized vector
         * @return normalized vector
         */
        [[nodiscard]] Vector normalized() const {
            float length = this->length();
            T v[X];
            for (unsigned int i = 0; i < X; i++) {
                v[i] = values[i] / length;
            }
            return Vector(v);
        }

        /**
         * Dot product with another vector
         * @param other other vector
         * @return dot product
         */
        [[nodiscard]] float dot(const Vector &other) const {
            float sum = 0;
            for (unsigned int i = 0; i < X; i++) {
                sum += values[i] * other.values[i];
            }
            return sum;
        }

        /**
         * Static dot product between two vectors
         * @param a first vector
         * @param b second vector
         * @return dot product
         */
        static float dot(const Vector &a, const Vector &b) {
            float sum = 0;
            for (unsigned int i = 0; i < X; i++) {
                sum += a.values[i] * b.values[i];
            }
            return sum;
        }

        /**
         * Check if any component is NaN
         * @return true if any component is NaN
         */
        [[nodiscard]] bool isNaN() const {
            for (unsigned int i = 0; i < X; i++) {
                if (std::isnan(values[i])) {
                    return true;
                }
            }
            return false;
        }

        /**
         * Cross product with another vector (only for 3D vectors)
         * @param rhs other vector
         * @return cross product
         */
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

        Vector &operator/=(T t) {
            for (unsigned int i = 0; i < X; i++) {
                values[i] /= t;
            }
            return *this;
        }

        T operator[](unsigned i) const {
            return values[i];
        }

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Vector, values)

        /// Zero vector
        static Vector zero() {
            return Vector((T) 0);
        }

        /// Unit vector in up direction
        static Vector up() requires (X >= 2) { return Vector((T) 0).setValue(Z_AXIS, 1); }
        /// Unit vector in down direction
        static Vector down() requires (X >= 2) { return Vector((T) 0).setValue(Z_AXIS, -1); }
        /// Unit vector in right direction
        static Vector right() requires (X >= 2) { return Vector((T) 0).setValue(X_AXIS, 1); }
        /// Unit vector in left direction
        static Vector left() requires (X >= 2) { return Vector((T) 0).setValue(X_AXIS, -1); }
        /// Unit vector in forward direction
        static Vector forward() requires (X >= 3) { return Vector((T) 0).setValue(Y_AXIS, 1); }
        /// Unit vector in backward direction
        static Vector backward() requires (X >= 3) { return Vector((T) 0).setValue(Y_AXIS, -1); }

        /// x component accessor
        T &x() requires (X >= 1) { return values[X_AXIS]; }
        /// y component accessor
        T &y() requires (X >= 2) { return values[Y_AXIS]; }
        /// z component accessor
        T &z() requires (X >= 3) { return values[Z_AXIS]; }
        /// w component accessor
        T &w() requires (X >= 4) { return values[W_AXIS]; }

        /// x component getter
        [[nodiscard]] T getX() const requires (X >= 1) { return values[X_AXIS]; }
        /// y component getter
        [[nodiscard]] T getY() const requires (X >= 2) { return values[Y_AXIS]; }
        /// z component getter
        [[nodiscard]] T getZ() const requires (X >= 3) { return values[Z_AXIS]; }
        /// w component getter
        [[nodiscard]] T getW() const requires (X >= 4) { return values[W_AXIS]; }

        /**
         * Cut off the last dimension, returning a vector with one less dimension
         * @return vector with one less dimension
         */
        Vector<X - 1, T> cutoff() {
            Vector<X - 1, T> result;
            for (unsigned int i = 0; i < X - 1; i++) {
                result.values[i] = values[i];
            }
            return result;
        }

        Vector<X, T> asDegreeToRadian() const requires (X == 3) {
            return {
                deg2rad(values[0]),
                deg2rad(values[1]),
                deg2rad(values[2])
            };
        }

#ifdef USE_SHADER_METAL
        /// Convert to Metal simd type uint2
        simd::uint2 toMetal() const requires (X == 2 && std::is_same<T, unsigned>::value) {
            return simd::uint2{values[0], values[1]};
        }

        /// Convert to Metal simd type float3
        simd::float3 toMetal() const requires (X == 3 && std::is_same<T, float>::value) {
            return simd::float3{values[0], values[1], values[2]};
        }

        /// Convert to Metal simd type float4
        simd::float4 toMetal() const requires (X == 4 && std::is_same<T, float>::value) {
            return simd::float4{values[0], values[1], values[2], values[3]};
        }
#endif
    };

    typedef Vector<2, float> Vec2;
    typedef Vector<3, float> Vec3;
    typedef Vector<4, float> Vec4;
    typedef Vector<2, unsigned> Vec2u;

    struct Quaternion : public Vector<4, float> {
        /**
         * Get quaternion from euler angles in radians
         * @param rad euler angles in radians
         * @return quaternion
         */
        static Quaternion fromEuler(const Vec3 &rad) {
            auto xQ = Quaternion({
                cos(rad.getX() * 0.5f),
                sin(rad.getX() * 0.5f),
                0,
                0
            });

            auto yQ = Quaternion({
                cos(rad.getY() * 0.5f),
                0,
                sin(rad.getY() * 0.5f),
                0
            });

            auto zQ = Quaternion({
                cos(rad.getZ() * 0.5f),
                0,
                0,
                sin(rad.getZ() * 0.5f)
            });

            auto q = zQ * yQ * xQ;
            return Quaternion(q);
        }

        /**
         * Get quaternion from euler angles in degrees
         * @param deg euler angles in degrees
         * @return quaternion
         */
        static Quaternion fromEulerDegree(const Vec3 &deg) {
            return Quaternion::fromEuler(Vec3{deg2rad(deg.getX()),deg2rad(deg.getY()), deg2rad(deg.getZ())});
        }
    };
}