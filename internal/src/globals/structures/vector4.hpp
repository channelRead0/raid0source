#pragma once

#include <iostream>
#include <cmath>

template <typename T>
class Vector4 {
public:
    T X;
    T Y;
    T Z;
    T W;

    Vector4();
    Vector4(T A, T B, T C, T D);

    T Distance(const Vector4<T>& Other) const;
    Vector4 Multiply(const std::vector<float>& Matrix) const;
    Vector4 operator-(const Vector4<T>& Other) const;
    Vector4 operator+(const Vector4<T>& Other) const;
};

template <typename T>
Vector4<T>::Vector4() : X(0), Y(0), Z(0), W(0) {}

template <typename T>
Vector4<T>::Vector4(T A, T B, T C, T D) : X(A), Y(B), Z(C), W(D) {}

template <typename T>
T Vector4<T>::Distance(const Vector4<T>& Other) const {
    return sqrt(pow(X - Other.X, 2.0) + pow(Y - Other.Y, 2.0) + pow(Z - Other.Z, 2.0) + pow(W - Other.W, 2.0));
}

template <typename T>
Vector4<T> Vector4<T>::Multiply(const std::vector<float>& Matrix) const
{
   /* return Vector4
	(
        static_cast<float>(X) * Matrix[0] + static_cast<float>(Y) * Matrix[4] + static_cast<float>(Z) * Matrix[8] + static_cast<float>(W) * Matrix[12],
        static_cast<float>(X) * Matrix[1] + static_cast<float>(Y) * Matrix[5] + static_cast<float>(Z) * Matrix[9] + static_cast<float>(W) * Matrix[13],
        static_cast<float>(X) * Matrix[2] + static_cast<float>(Y) * Matrix[6] + static_cast<float>(Z) * Matrix[10] + static_cast<float>(W) * Matrix[14],
        static_cast<float>(X) * Matrix[3] + static_cast<float>(Y) * Matrix[7] + static_cast<float>(Z) * Matrix[11] + static_cast<float>(W) * Matrix[15]
    );*/
    return Vector4(
        static_cast<float>(X) * Matrix[0] + static_cast<float>(Y) * Matrix[4] + static_cast<float>(Z) * Matrix[8] + static_cast<float>(W) * Matrix[12],
        static_cast<float>(X) * Matrix[1] + static_cast<float>(Y) * Matrix[5] + static_cast<float>(Z) * Matrix[9] + static_cast<float>(W) * Matrix[13],
        static_cast<float>(X) * Matrix[2] + static_cast<float>(Y) * Matrix[6] + static_cast<float>(Z) * Matrix[10] + static_cast<float>(W) * Matrix[14],
        static_cast<float>(X) * Matrix[3] + static_cast<float>(Y) * Matrix[7] + static_cast<float>(Z) * Matrix[11] + static_cast<float>(W) * Matrix[15]
    );
}

template <typename T>
Vector4<T> Vector4<T>::operator-(const Vector4<T>& Other) const {
    return Vector4(X - Other.X, Y - Other.Y, Z - Other.Z, W - Other.W);
}

template <typename T>
Vector4<T> Vector4<T>::operator+(const Vector4<T>& Other) const {
    return Vector4(X + Other.X, Y + Other.Y, Z + Other.Z, W + Other.W);
}

// Overloaded << operator for Vector4
template <typename T>
std::ostream& operator<<(std::ostream& Os, const Vector4<T>& Vec)
{
    Os << "Vector4<" << typeid(T).name() << ">(" << Vec.X << ", " << Vec.Y << ", " << Vec.Z << ", " << Vec.W << ")";
    return Os;
}