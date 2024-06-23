#pragma once
#include <iostream>
#include <cmath>

template <typename T>
class Vector2 {
public:
    T X;
    T Y;

    Vector2();
    Vector2(T A, T B);

    T Distance(const Vector2<T>& Other) const;
    Vector2 operator-(const Vector2<T>& Other) const;
    Vector2 operator+(const Vector2<T>& Other) const;
    Vector2 Rotate(double Angle, const Vector2<T>& Center);
};

template <typename T>
Vector2<T>::Vector2() : X(0), Y(0) {}

template <typename T>
Vector2<T>::Vector2(T A, T B) : X(A), Y(B) {}

template <typename T>
T Vector2<T>::Distance(const Vector2<T>& Other) const {
    return sqrt(pow(X - Other.X, 2.0) + pow(Y - Other.Y, 2.0));
}

template <typename T>
Vector2<T> Vector2<T>::operator-(const Vector2<T>& Other) const {
    return Vector2(X - Other.X, Y - Other.Y);
}

template <typename T>
Vector2<T> Vector2<T>::operator+(const Vector2<T>& Other) const {
    return Vector2(X + Other.X, Y + Other.Y);
}

template <typename T>
Vector2<T> Vector2<T>::Rotate(double Angle, const Vector2<T>& Center)
{
    double Radians = Angle * M_PI / 180.0;
    double CosTheta = std::cos(Radians);
    double SinTheta = std::sin(Radians);

    // Translate the vector to the origin
    T TranslatedX = X - Center.X;
    T TranslatedY = Y - Center.Y;

    return { TranslatedX * CosTheta - TranslatedY * SinTheta + Center.X , TranslatedX * SinTheta + TranslatedY * CosTheta + Center.Y };
}

// Overloaded << operator for Vector2
template <typename T>
std::ostream& operator<<(std::ostream& Os, const Vector2<T>& Vec)
{
    Os << "Vector2<" << typeid(T).name() << ">(" << Vec.X << ", " << Vec.Y << ")";
    return Os;
}
