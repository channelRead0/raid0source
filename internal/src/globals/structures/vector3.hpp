#pragma once
#include "vector2.hpp"
#include "vector4.hpp"

template <typename T>
class Vector3 {
public:
	T X;
	T Y;
	T Z;

	Vector3();
	Vector3(T A, T B, T C);

	template <class U>
	Vector3(const Vector3<U>& Other);

	bool World2Screen(Vector2<float>& ScreenProjection, const std::vector<float>& ModelView, const std::vector<float>& Projection, const int Width, const int Height);

	T Distance(const Vector3<T>& Other) const;
	T DistanceSq(const Vector3<T>& Other) const;
	Vector3 operator-(const Vector3<T>& Other) const;
	Vector3 operator+(const Vector3<T>& Other) const;
	Vector3 operator*(const Vector3<T>& Other) const;
	Vector3 operator*(const T& Scalar) const;
	Vector3 operator/(const T& Scalar) const;

	float& operator[](int I) {
		return ((float*)this)[I];
	}

	float operator[](int I) const {
		return ((float*)this)[I];
	}
};

template <typename T>
Vector3<T>::Vector3() : X(0), Y(0), Z(0) {}

template <typename T>
Vector3<T>::Vector3(T A, T B, T C) : X(A), Y(B), Z(C) {}

template <typename T>
template<typename U>
Vector3<T>::Vector3(const Vector3<U>& Other) : X(static_cast<T>(Other.X)), Y(static_cast<T>(Other.Y)), Z(static_cast<T>(Other.Z)) {} // Conversion constructor

template <typename T>
bool Vector3<T>::World2Screen(Vector2<float>& ScreenProjection, const std::vector<float>& ModelView, const std::vector<float>& Projection, const int Width, const int Height)
{
	if (Projection.size() != 16 || ModelView.size() != 16) {
		return false;
	}

	const auto WorldPositionVec4 = Vector4(static_cast<float>(X), static_cast<float>(Y), static_cast<float>(Z), 1.0F).Multiply(ModelView);
	const auto ClipSpacePositionVec4 = WorldPositionVec4.Multiply(Projection);
	const auto NCDClipSpacePos = Vector4{ static_cast<float>(ClipSpacePositionVec4.X / ClipSpacePositionVec4.W), static_cast<float>(ClipSpacePositionVec4.Y / ClipSpacePositionVec4.W), static_cast<float>(ClipSpacePositionVec4.Z / ClipSpacePositionVec4.W), static_cast<float>(0.F) };

	if (NCDClipSpacePos.Z < -1.0 || NCDClipSpacePos.Z > 1.0) {
		return false;
	}

	ScreenProjection.X = Width * ((NCDClipSpacePos.X + 1.0f) / 2.0f);
	ScreenProjection.Y = Height * ((1.0f - NCDClipSpacePos.Y) / 2.0f);

	return true;
}

template <typename T>
T Vector3<T>::Distance(const Vector3<T>& Other) const {
	return sqrtf(pow(X - Other.X, 2.0) + pow(Y - Other.Y, 2.0) + pow(Z - Other.Z, 2.0));
}

template <typename T>
T Vector3<T>::DistanceSq(const Vector3<T>& Other) const 
{
	float dx = X - Other.X;
	float dy = Y - Other.Y;
	float dz = Z - Other.Z;
	return dx * dx + dy * dy + dz * dz;
}

template <typename T>
Vector3<T> Vector3<T>::operator-(const Vector3<T>& Other) const {
	return Vector3(X - Other.X, Y - Other.Y, Z - Other.Z);
}

template <typename T>
Vector3<T> Vector3<T>::operator+(const Vector3<T>& Other) const {
	return Vector3(X + Other.X, Y + Other.Y, Z + Other.Z);
}

template<typename T>
Vector3<T> Vector3<T>::operator*(const Vector3<T>& Other) const
{
	return Vector3(X * Other.X, Y * Other.Y, Z * Other.Z);
}

template <typename T>
Vector3<T> Vector3<T>::operator*(const T& Scalar) const {
	return Vector3(X * Scalar, Y * Scalar, Z * Scalar);
}

template <typename T>
Vector3<T> Vector3<T>::operator/(const T& Scalar) const {
	return Vector3(X / Scalar, Y / Scalar, Z / Scalar);
}

// Overloaded << operator for Vector3
template <typename T>
std::ostream& operator<<(std::ostream& Os, const Vector3<T>& Vec)
{
	Os << "Vector3<" << typeid(T).name() << ">(" << Vec.X << ", " << Vec.Y << ", " << Vec.Z << ")";
	return Os;
}