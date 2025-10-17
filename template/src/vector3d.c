#include "../include/vector3d.h"

#include <stdio.h>

/**
 * @brief Creates a new 3D vector with the given coordinates.
 *
 * @param x The x-coordinate of the vector.
 * @param y The y-coordinate of the vector.
 * @param z The z-coordinate of the vector.
 * @return Vector3D The newly created vector with specified x, y, and z values.
 */
Vector3D Vector3D_new(const double x, const double y, const double z) {
    const Vector3D v = {x, y, z};
    return v;
}

/**
 * @brief Adds two 3D vectors component-wise.
 *
 * @param v1 The first vector.
 * @param v2 The second vector.
 * @return Vector3D The result of adding v1 and v2.
 */
Vector3D Vector3D_add(const Vector3D v1, const Vector3D v2) {
    return Vector3D_new(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

/**
 * @brief Subtracts the second vector from the first vector component-wise.
 *
 * @param v1 The first vector (minuend).
 * @param v2 The second vector (subtrahend).
 * @return Vector3D The result of subtracting v2 from v1.
 */
Vector3D Vector3D_subtract(const Vector3D v1, const Vector3D v2) {
    return Vector3D_new(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

/**
 * @brief Scales a 3D vector by a scalar, multiplying each component by the scalar.
 *
 * @param v The vector to scale.
 * @param scalar The scaling factor.
 * @return Vector3D The scaled vector.
 */
Vector3D Vector3D_scale(const Vector3D v, const double scalar) {
    return Vector3D_new(v.x * scalar, v.y * scalar, v.z * scalar);
}

/**
 * @brief Computes the dot product of two 3D vectors.
 *
 * The dot product is a measure of how parallel two vectors are.
 * It is zero if the vectors are perpendicular.
 *
 * @param v1 The first vector.
 * @param v2 The second vector.
 * @return double The dot product of v1 and v2.
 */
double Vector3D_dot(const Vector3D v1, const Vector3D v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

/**
 * @brief Computes the cross product of two 3D vectors.
 *
 * The cross product of two vectors is a vector that is perpendicular
 * to both, with a magnitude equal to the area of the parallelogram
 * that the vectors span.
 *
 * @param v1 The first vector.
 * @param v2 The second vector.
 * @return Vector3D The cross product of v1 and v2.
 */
Vector3D Vector3D_cross(const Vector3D v1, const Vector3D v2) {
    return Vector3D_new(
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    );
}

/**
 * @brief Computes the magnitude (length) of a 3D vector.
 *
 * The magnitude is calculated as the square root of the sum
 * of the squares of the vector's components.
 *
 * @param v The vector whose magnitude is to be calculated.
 * @return double The magnitude (length) of the vector.
 */
double Vector3D_magnitude(const Vector3D v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

/**
 * @brief Normalizes a 3D vector to make its magnitude equal to 1.
 *
 * If the vector's magnitude is zero, it returns a zero vector.
 *
 * @param v The vector to normalize.
 * @return Vector3D The normalized vector with a magnitude of 1, or a zero vector if the original vector's magnitude is zero.
 */
Vector3D Vector3D_normalize(const Vector3D v) {
    const double mag = Vector3D_magnitude(v);
    if (mag == 0) {
        return Vector3D_new(0, 0, 0); // Return a zero vector if magnitude is 0
    }
    return Vector3D_scale(v, 1.0 / mag);
}

/**
 * @brief Reflects a vector around a given normal.
 *
 * The function calculates the reflection of a vector off a surface with a given normal.
 *
 * @param vector The original vector to be reflected (e.g., light direction).
 * @param normal The normal vector of the surface being reflected off.
 * @return Vector3D The reflected vector.
 */
Vector3D Vector3D_reflect(const Vector3D vector, const Vector3D normal) {
    // Calculate 2 * (vector · normal) * normal
    const double dot_product = Vector3D_dot(vector, normal);
    const Vector3D scaled_normal = Vector3D_scale(normal, 2 * dot_product);

    // Reflect the vector: vector - scaled_normal
    return Vector3D_subtract(vector, scaled_normal);
}

/**
 * @brief Compares two 3D vectors for equality within a specified tolerance.
 *
 * The tolerance is used to account for floating-point inaccuracies.
 *
 * @param v1 The first vector.
 * @param v2 The second vector.
 * @param tolerance The tolerance within which the vectors are considered equal.
 * @return int Returns 1 if the vectors are equal within the tolerance, 0 otherwise.
 */
int Vector3D_equals(const Vector3D v1, const Vector3D v2, const double tolerance) {
    return fabs(v1.x - v2.x) < tolerance &&
           fabs(v1.y - v2.y) < tolerance &&
           fabs(v1.z - v2.z) < tolerance;
}

/**
 * @brief Converts this vector to a string.
 *
 * @param v The vector to convert
 * @param s The buffer to print to
 * @return
 */
void Vector3D_toString(const Vector3D v, char* s)
{
    sprintf(s, "(%f, %f, %f)", v.x, v.y, v.z);
}