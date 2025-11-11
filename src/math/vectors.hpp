#pragma once
#include <cstdarg>
#include <random>
#include <nlohmann/json.hpp>

#ifdef USE_SHADER_METAL
#include <simd/simd.h>
#endif

#define deg2rad(deg) (deg * (float)M_PI / 180.0f)
#define rad2deg(rad) (rad * 180.0f / (float)M_PI)

namespace RayTracing {
    /**
     * Generic vector class
     * @tparam X number of dimensions
     * @tparam T type of the vector values
     */
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

        /// Default constructor initializing all values to initial
        Vector(T initial = 0) {
            for (unsigned int i = 0; i < X; i++) {
                values[i] = initial;
            }
        }

        /// Constructor for 2D vector
        Vector(T x, T y) requires (X == 2) {
            values[X_AXIS] = x;
            values[Y_AXIS] = y;
        }

        /// Constructor for 3D vector
        Vector(T x, T y, T z) requires (X == 3) {
            values[X_AXIS] = x;
            values[Y_AXIS] = y;
            values[Z_AXIS] = z;
        }

        /// Constructor for 4D vector
        Vector(T x, T y, T z, T w) requires (X == 4) {
            values[X_AXIS] = x;
            values[Y_AXIS] = y;
            values[Z_AXIS] = z;
            values[W_AXIS] = w;
        }

        /// Constructor from array
        Vector(T values[X]) {
            for (unsigned int i = 0; i < X; i++) {
                this->values[i] = values[i];
            }
        }

        /// Constructor from vector
        Vector(const std::vector<T> &values) {
            for (unsigned int i = 0; i < X; i++) {
                this->values[i] = values[i];
            }
        }

        /// Copy constructor
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
        std::enable_if_t<M == 3, Vector<3, T> >
        cross(const Vector<3, T> &rhs) const {
            Vector<3, T> result;
            result.values[0] = values[1] * rhs.values[2] - values[2] * rhs.values[1];
            result.values[1] = values[2] * rhs.values[0] - values[0] * rhs.values[2];
            result.values[2] = values[0] * rhs.values[1] - values[1] * rhs.values[0];
            return result;
        }

        /**
         * Cross product between two vectors (only for 3D vectors)
         * @param a vector a
         * @param b vector b
         * @return cross product
         */
        static Vector<3, T> cross(const Vector<3, T> &a, const Vector<3, T> &b) {
            return a.cross(b);
        }

        /**
         * Scalar division
         * @param t scalar value
         * @return element wise divided vector
         */
        Vector operator/(T t) const {
            T v[X];
            for (unsigned int i = 0; i < X; i++) {
                v[i] = values[i] / t;
            }
            return Vector(v);
        }

        /**
         * Scalar multiplication
         * @param t scalar value
         * @return element wise multiplied vector
         */
        Vector operator*(T t) const {
            T v[X];
            for (unsigned int i = 0; i < X; i++) {
                v[i] = values[i] * t;
            }
            return Vector(v);
        }

        /**
         * Element wise division with another vector
         * @param other other vector
         * @return element wise divided vector
         */
        Vector operator/(Vector<X, T> &other) const {
            T v[X];
            for (unsigned int i = 0; i < X; i++) {
                v[i] = values[i] / other.values[i];
            }
            return Vector(v);
        }

        /**
         * Element wise multiplication with another vector
         * @param other other vector
         * @return element wise multiplied vector
         */
        Vector operator*(const Vector<X, T> &other) const {
            T v[X];
            for (unsigned int i = 0; i < X; i++) {
                v[i] = values[i] * other.values[i];
            }
            return Vector(v);
        }

        /**
         * Vector addition
         * @param other other vector
         * @return added vector
         */
        Vector operator+(const Vector<X, T> &other) const {
            T v[X];
            for (unsigned int i = 0; i < X; i++) {
                v[i] = values[i] + other.values[i];
            }
            return Vector(v);
        }

        /**
         * Vector subtraction
         * @param other other vector
         * @return subtracted vector
         */
        Vector operator-(const Vector<X, T> &other) const {
            T v[X];
            for (unsigned int i = 0; i < X; i++) {
                v[i] = values[i] - other.values[i];
            }
            return Vector(v);
        }

        /**
         * Equality check
         * @param other other vector
         * @return true if each dimension is equal
         */
        bool operator==(const Vector<X, T> &other) const {
            for (unsigned int i = 0; i < X; i++) {
                if (values[i] != other.values[i]) {
                    return false;
                }
            }
            return true;
        }

        /// In-place addition
        Vector &operator+=(const Vector &other) {
            for (unsigned int i = 0; i < X; i++) {
                values[i] += other.values[i];
            }
            return *this;
        }

        /// In-place subtraction
        Vector &operator/=(T t) {
            for (unsigned int i = 0; i < X; i++) {
                values[i] /= t;
            }
            return *this;
        }

        /// Get value at dimension
        T operator[](unsigned i) const {
            return values[i];
        }

        /// Get value at direction
        T operator[](Direction d) const {
            return values[d];
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

        /// Convert radians to degrees (only for 3D vectors)
        [[nodiscard]] Vector<X, T> asDegreeToRadian() const requires (X == 3) {
            return {
                deg2rad(values[0]),
                deg2rad(values[1]),
                deg2rad(values[2])
            };
        }

        /// Generate a random vector with normally distributed components
        static Vector<X, T> random() {
            T v[X];
            static float sigma = 0.8f;
            static thread_local std::mt19937 gen(std::random_device{}());
            static std::normal_distribution<T> dist(0.0f, sigma);
            for (unsigned int i = 0; i < X; i++) {
                v[i] = dist(gen);
            }
            return Vector<X, T>(v);
        }

#ifdef USE_SHADER_METAL
        /// Convert to Metal simd type uint2
        [[nodiscard]] simd::uint2 toMetal() const requires (X == 2 && std::is_same<T, unsigned>::value) {
            return simd::uint2{values[0], values[1]};
        }

        /// Convert to Metal simd type float3
        [[nodiscard]] simd::float3 toMetal() const requires (X == 3 && std::is_same<T, float>::value) {
            return simd::float3{values[0], values[1], values[2]};
        }

        /// Convert to Metal simd type float4
        [[nodiscard]] simd::float4 toMetal() const requires (X == 4 && std::is_same<T, float>::value) {
            return simd::float4{values[0], values[1], values[2], values[3]};
        }
#endif
    };

    /// 2D float vector
    typedef Vector<2, float> Vec2;
    /// 3D float vector
    typedef Vector<3, float> Vec3;
    /// 4D float vector
    typedef Vector<4, float> Vec4;
    /// 2D unsigned vector
    typedef Vector<2, unsigned> Vec2u;

    /// Quaternion represented as a 4D float vector
    struct Quaternion : public Vector<4, float> {
        /**
         * Get quaternion from euler angles in radians
         * @param rad euler angles in radians
         * @return quaternion
         */
        static Quaternion fromEuler(const Vec3 &rad) {
            auto yaw = rad.getY();
            auto pitch = rad.getX();
            auto roll = rad.getZ();
            // Assuming the angles are in radians.
            float cy = std::cos(yaw * 0.5f);
            float sy = std::sin(yaw * 0.5f);
            float cp = std::cos(pitch * 0.5f);
            float sp = std::sin(pitch * 0.5f);
            float cr = std::cos(roll * 0.5f);
            float sr = std::sin(roll * 0.5f);

            Quaternion q;
            q.w() = cr * cp * cy + sr * sp * sy;
            q.x() = sr * cp * cy - cr * sp * sy;
            q.y() = cr * sp * cy + sr * cp * sy;
            q.z() = cr * cp * sy - sr * sp * cy;

            return q;
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