#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <math.h>

/**
 * @brief Structure representing a 3D vector.
 *
 * The vector has three components: x, y, and z.
 */
typedef struct {
    double x; ///< The x-coordinate of the vector
    double y; ///< The y-coordinate of the vector
    double z; ///< The z-coordinate of the vector
} Vector3D;

/**
 * @brief Creates a new 3D vector with specified x, y, and z components.
 *
 * @param x The x-coordinate of the vector.
 * @param y The y-coordinate of the vector.
 * @param z The z-coordinate of the vector.
 * @return Vector3D The newly created vector.
 */
Vector3D Vector3D_new(double x, double y, double z);

/**
 * @brief Adds two 3D vectors component-wise.
 *
 * @param v1 The first vector.
 * @param v2 The second vector.
 * @return Vector3D The result of adding v1 and v2.
 */
Vector3D Vector3D_add(Vector3D v1, Vector3D v2);

/**
 * @brief Subtracts the second vector from the first vector component-wise.
 *
 * @param v1 The first vector (minuend).
 * @param v2 The second vector (subtrahend).
 * @return Vector3D The result of subtracting v2 from v1.
 */
Vector3D Vector3D_subtract(Vector3D v1, Vector3D v2);

/**
 * @brief Scales a 3D vector by a scalar, multiplying each component by the scalar.
 *
 * @param v The vector to scale.
 * @param scalar The scaling factor.
 * @return Vector3D The scaled vector.
 */
Vector3D Vector3D_scale(Vector3D v, double scalar);

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
double Vector3D_dot(Vector3D v1, Vector3D v2);

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
Vector3D Vector3D_cross(Vector3D v1, Vector3D v2);

/**
 * @brief Computes the magnitude (length) of a 3D vector.
 *
 * The magnitude is calculated as the square root of the sum
 * of the squares of the vector's components.
 *
 * @param v The vector whose magnitude is to be calculated.
 * @return double The magnitude (length) of the vector.
 */
double Vector3D_magnitude(Vector3D v);

/**
 * @brief Normalizes a 3D vector to make its magnitude equal to 1.
 *
 * If the vector's magnitude is zero, it returns a zero vector.
 *
 * @param v The vector to normalize.
 * @return Vector3D The normalized vector with a magnitude of 1, or a zero vector if the original vector's magnitude is zero.
 */
Vector3D Vector3D_normalize(Vector3D v);

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
int Vector3D_equals(Vector3D v1, Vector3D v2, double tolerance);

/**
 * @brief Reflects a vector around a given normal.
 *
 * The function calculates the reflection of a vector off a surface with a given normal.
 *
 * @param vector The original vector to be reflected (e.g., light direction).
 * @param normal The normal vector of the surface being reflected off.
 * @return Vector3D The reflected vector.
 */
Vector3D Vector3D_reflect(Vector3D vector, Vector3D normal);

/**
 * @brief Converts this vector to a string.
 *
 * @param v The vector to convert
 * @param s The buffer to print to
 * @return
 */
void Vector3D_toString(Vector3D v, char* s);

#endif
