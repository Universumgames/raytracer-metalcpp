#pragma once
#include <type_traits>
#include <vector>

#include "vectors.hpp"

namespace RayTracing {
    template<unsigned int X,
        unsigned int Y,
        typename T,
        typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    class Matrix {
    public:
        T values[X][Y] = {};

        Matrix(T initial = 0) {
            for (int i = 0; i < X; i++) {
                for (int j = 0; j < Y; j++) {
                    values[i][j] = i;
                }
            }
        }

        Matrix(T values[X][Y]) {
            setValues(values);
        }

        /// [X][Y]
        Matrix(std::vector<std::vector<T> > values) {
            setValues(values);
        }

        Matrix<float>(double values[X][Y]) {
            setValues(values);
        }

        void setValues(std::vector<std::vector<T> > values) {
            for (int i = 0; i < X; i++) {
                for (int j = 0; j < Y; j++) {
                    values[i][j] = values[i][j];
                }
            }
        }

        void setValues(T values[X][Y]) {
            for (int i = 0; i < X; i++) {
                for (int j = 0; j < Y; j++) {
                    values[i][j] = values[i][j];
                }
            }
        }

        void setValue(unsigned int row, unsigned int col, T value) {
            values[row][col] = value;
        }

        T getValue(unsigned int row, unsigned int col) {
            return values[row][col];
        }

        T *operator[](size_t i) { return values[i]; }
        const T *operator[](size_t i) const { return values[i]; }

        // Multiplication: (X×Y) * (Y×Z) = (X×Z)
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
            Vector<X, T> result;
            for (unsigned int i = 0; i < X; ++i) {
                T sum = 0;
                for (unsigned int j = 0; j < Y; ++j)
                    sum += values[i][j] * vec.values[j];
                result.values[i] = sum;
            }
            return result;
        }
    };

    typedef Matrix<3, 3, float> Mat3x3;
    typedef Matrix<4, 4, float> Mat4x4;
}
