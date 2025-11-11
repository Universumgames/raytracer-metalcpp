#pragma once
#include <type_traits>
#include <vector>
#ifdef USE_SHADER_METAL
#include <simd/simd.h>
#endif

#include "vectors.hpp"

namespace RayTracing {
    /**
     * Matrix class
     * @tparam X width of the matrix
     * @tparam Y height of the matrix
     * @tparam T type of the matrix values
     */
    template<unsigned int X,
        unsigned int Y,
        typename T,
        typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    class Matrix {
        /// Matrix values column-major
        T values[X][Y] = {};

    public:
        /// Default constructor initializing all values to initial
        explicit Matrix(T initial = 0) {
            for (int i = 0; i < X; i++) {
                for (int j = 0; j < Y; j++) {
                    values[i][j] = initial;
                }
            }
        }

        /**
         * Initialize matrix with 2D array
         * @param v matrix values column-major
         */
        explicit Matrix(T v[X][Y]) {
            setValues(v);
        }

        /**
         * Initialize matrix with 2D vector
         * @param v matrix values column-major
         */
        explicit Matrix(std::vector<std::vector<T> > v) {
            setValues(v);
        }

        /**
         * Set matrix values from 2D vector
         * @param v matrix values column-major
         */
        void setValues(std::vector<std::vector<T> > v) {
            for (int i = 0; i < X; i++) {
                for (int j = 0; j < Y; j++) {
                    this->values[i][j] = v[i][j];
                }
            }
        }

        /**
         * Set matrix values from 2D array
         * @param v matrix values column-major
         */
        void setValues(T v[X][Y]) {
            for (int i = 0; i < X; i++) {
                for (int j = 0; j < Y; j++) {
                    this->values[i][j] = v[i][j];
                }
            }
        }

        /**
         * Set value at row and column
         * @param row row
         * @param col column
         * @param value value to set
         */
        void setValue(unsigned int row, unsigned int col, T value) {
            values[row][col] = value;
        }

        /**
         * Get value at row and column
         * @param row row
         * @param col column
         * @return value at row and column
         */
        T getValue(unsigned int row, unsigned int col) {
            return values[row][col];
        }

        /// Row access
        T *operator[](size_t i) { return values[i]; }
        /// Row access (const)
        const T *operator[](size_t i) const { return values[i]; }

        // Multiplication: (X×Y) * (Y×Z) = (X×Z)
        /**
         * Multiply matrix with another matrix
         * @tparam Z number of columns in rhs matrix
         * @param rhs right-hand side matrix
         * @return result matrix
         */
        template<unsigned int Z>
        Matrix<X, Z, T> operator*(const Matrix<Y, Z, T> &rhs) const {
            Matrix<X, Z, T> result;

            for (unsigned int i = 0; i < X; ++i) {
                for (unsigned int j = 0; j < Z; ++j) {
                    T sum = 0;
                    for (unsigned int k = 0; k < Y; ++k) {
                        sum += values[i][k] * rhs.values[k][j];
                    }
                    result.values[i][j] = sum;
                }
            }
            return result;
        }

        Vector<X, T> operator*(const Vector<Y, T> &vec) const {
            Vector < X, T > result;
            for (unsigned int i = 0; i < X; ++i) {
                T sum = 0;
                for (unsigned int j = 0; j < Y; ++j)
                    sum += values[i][j] * vec.values[j];
                result.values[i] = sum;
            }
            return result;
        }

        Matrix operator*(T v) const {
            Matrix result;
            for (unsigned int i = 0; i < X; ++i) {
                for (unsigned int j = 0; j < Y; ++j) {
                    result.values[i][j] = values[i][j] * v;
                }
            }
            return result;
        }

        Matrix operator-() {
            return negate();
        }

        /// Negate the matrix
        Matrix negate() {
            return *this * -1;
        }

        /// Inverse the matrix (only for diagonal matrices) (1/a_ii)
        Matrix inverse() {
            Matrix result;
            for (unsigned int i = 0; i < X; ++i) {
                for (unsigned int j = 0; j < Y; ++j) {
                    result.values[i][j] = 1.0f / values[i][i];
                }
            }
            return result;
        }

        /// Create an identity matrix
        static Matrix<X, X, T> identity() {
            Matrix result;
            for (unsigned int i = 0; i < X; ++i) {
                result.values[i][i] = 1;
            }
            return result;
        }

#ifdef USE_SHADER_METAL
        /// Convert to Metal simd float4x4
        simd::float4x4 toMetal() requires (X == 4 && Y == 4 && std::is_same<T, float>::value) {
            simd::float4 columns[4] = {};
            for (unsigned int i = 0; i < 4; i++) {
                for (unsigned int j = 0; j < 4; j++) {
                    columns[j][i] = values[i][j];
                }
            }
            return {columns[0], columns[1], columns[2], columns[3]};
        }
#endif
    };

    typedef Matrix<3, 3, float> Mat3x3;
    typedef Matrix<4, 4, float> Mat4x4;
}