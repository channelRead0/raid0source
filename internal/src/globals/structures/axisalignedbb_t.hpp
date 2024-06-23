#pragma once
#include <iostream>

#include "vector2.hpp"
#include "vector3.hpp"
#include "vector4.hpp"

template<typename T>
class AxisAlignedBB_T {
public:
    AxisAlignedBB_T() {};
    AxisAlignedBB_T(T MinX, T MinY, T MinZ, T MaxX, T MaxY, T MaxZ) : MinX(MinX), MinY(MinY), MinZ(MinZ), MaxX(MaxX), MaxY(MaxY), MaxZ(MaxZ) {}

    T GetMinX() const { return MinX; }
    T GetMinY() const { return MinY; }
    T GetMinZ() const { return MinZ; }
    T GetMaxX() const { return MaxX; }
    T GetMaxY() const { return MaxY; }
    T GetMaxZ() const { return MaxZ; }

    void SetMinX(T MinX) { this->MinX = MinX; }
    void SetMinY(T MinY) { this->MinY = MinY; }
    void SetMinZ(T MinZ) { this->MinZ = MinZ; }
    void SetMaxX(T MaxX) { this->MaxX = MaxX; }
    void SetMaxY(T MaxY) { this->MaxY = MaxY; }
    void SetMaxZ(T MaxZ) { this->MaxZ = MaxZ; }

    AxisAlignedBB_T<T> GetBoundingBox3D(const Vector3<float>& Position, const Vector3<float>& LastTickPosition, const Vector3<float>& RenderPosition, const float RenderPartialTicks)
    {
        return AxisAlignedBB_T<T>
        {
            this->GetMinX() - RenderPosition.X + -Position.X + LastTickPosition.X + (Position.X - LastTickPosition.X) * RenderPartialTicks,
            this->GetMinY() - RenderPosition.Y + -Position.Y + LastTickPosition.Y + (Position.Y - LastTickPosition.Y) * RenderPartialTicks,
            this->GetMinZ() - RenderPosition.Z + -Position.Z + LastTickPosition.Z + (Position.Z - LastTickPosition.Z) * RenderPartialTicks,
            this->GetMaxX() - RenderPosition.X + -Position.X + LastTickPosition.X + (Position.X - LastTickPosition.X) * RenderPartialTicks,
            this->GetMaxY() - RenderPosition.Y + -Position.Y + LastTickPosition.Y + (Position.Y - LastTickPosition.Y) * RenderPartialTicks,
            this->GetMaxZ() - RenderPosition.Z + -Position.Z + LastTickPosition.Z + (Position.Z - LastTickPosition.Z) * RenderPartialTicks

        };
    }

    Vector4<float> GetScreenPosition(const std::vector<float>& Projection, const std::vector<float>& ModelView, const int Height, const int Width)
    {
        const std::vector<Vector3<float>> Vertices2D
        {
            {static_cast<float>(MinX), static_cast<float>(MinY), static_cast<float>(MinZ)},
		    {static_cast<float>(MinX), static_cast<float>(MaxY), static_cast<float>(MinZ)},
		    {static_cast<float>(MaxX), static_cast<float>(MaxY), static_cast<float>(MinZ)},
		    {static_cast<float>(MaxX), static_cast<float>(MinY), static_cast<float>(MinZ)},
		    {static_cast<float>(MinX), static_cast<float>(MinY), static_cast<float>(MaxZ)},
		    {static_cast<float>(MinX), static_cast<float>(MaxY), static_cast<float>(MaxZ)},
		    {static_cast<float>(MaxX), static_cast<float>(MaxY), static_cast<float>(MaxZ)},
		    {static_cast<float>(MaxX), static_cast<float>(MinY), static_cast<float>(MaxZ)},


            /*Vector3<float>{MinX, MinY, MinZ},
            Vector3<float>{MinX, MaxY, MinZ},
            Vector3<float>{MaxX, MaxY, MinZ},
            Vector3<float>{MaxX, MinY, MinZ},
            Vector3<float>{MinX, MinY, MaxZ},
            Vector3<float>{MinX, MaxY, MaxZ},
            Vector3<float>{MaxX, MaxY, MaxZ},
            Vector3<float>{MaxX, MinY, MaxZ},*/
        };

        Vector4 ScreenVertices = { FLT_MAX, FLT_MAX, -1.F, -1.F };

        for (int VertexIndex = 0; VertexIndex < 8; VertexIndex++)
        {
            Vector2<float> ScreenPosition;
        	auto CurrentVertex = Vertices2D[VertexIndex];

            if (!CurrentVertex.World2Screen(ScreenPosition, ModelView, Projection, Width, Height)) {
                continue;
            }

            ScreenVertices.X = fminf(ScreenPosition.X, ScreenVertices.X);
            ScreenVertices.Y = fminf(ScreenPosition.Y, ScreenVertices.Y);
            ScreenVertices.Z = fmaxf(ScreenPosition.X, ScreenVertices.Z);
            ScreenVertices.W = fmaxf(ScreenPosition.Y, ScreenVertices.W);
        }

        return ScreenVertices;
    }

    bool Contains(T X, T Y, T Z) const {
        return X >= MinX && X <= MaxX && Y >= MinY && Y <= MaxY && Z >= MinZ && Z <= MaxZ;
    }

    AxisAlignedBB_T<T> operator+(const AxisAlignedBB_T<T>& other) const {
        return AxisAlignedBB_T<T>(MinX + other.MinX, MinY + other.MinY, MinZ + other.MinZ, MaxX + other.MaxX, MaxY + other.MaxY, MaxZ + other.MaxZ);
    }

    AxisAlignedBB_T<T> operator-(const AxisAlignedBB_T<T>& other) const {
        return AxisAlignedBB_T<T>(MinX - other.MinX, MinY - other.MinY, MinZ - other.MinZ, MaxX - other.MaxX, MaxY - other.MaxY, MaxZ - other.MaxZ);
    }

    AxisAlignedBB_T<T> operator*(const float other) const {
        return AxisAlignedBB_T<T>(MinX * other, MinY * other, MinZ * other, MaxX * other, MaxY * other, MaxZ * other);
    }

    T MinX, MinY, MinZ;
    T MaxX, MaxY, MaxZ;
};

template<typename T>
std::ostream& operator<<(std::ostream& Os, const AxisAlignedBB_T<T>& AxisAlignedBoundingBox)
{
	Os << "AxisAlignedBB: "
	<< "[" << AxisAlignedBoundingBox.GetMinX() << ", " << AxisAlignedBoundingBox.GetMinY() << ", " << AxisAlignedBoundingBox.GetMinZ() << "] - "
	<< "[" << AxisAlignedBoundingBox.GetMaxX() << ", " << AxisAlignedBoundingBox.GetMaxY() << ", " << AxisAlignedBoundingBox.GetMaxZ() << "]";

    return Os;
}