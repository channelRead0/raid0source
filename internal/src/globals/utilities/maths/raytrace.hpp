#pragma once
#include <src/globals/structures/vector3.hpp>

class RayTrace
{
public:
	Vector3<double> RayTraceOrigin;
	Vector3<double> RayTraceDirection;
	Vector3<double> RaytraceDirectionInv;

	explicit RayTrace(const Vector3<double>& Origin, const Vector3<double>& Direction)
	{
		RayTraceOrigin = Origin;
		RayTraceDirection = Direction;
		RaytraceDirectionInv = Vector3<double>(1.f / RayTraceDirection.X, 1.f / RayTraceDirection.Y, 1.f / RayTraceDirection.Z);
	}

	bool Trace(const Vector3<double>& Min, const Vector3<double>& Maxs, float& Distance, Vector3<double>& End) const
	{
		// If line is parallel and outside the box it is not possible to intersect 
		if (RayTraceDirection.X == 0.f && (RayTraceOrigin.X < std::min<float>(Min.X, Maxs.X) || RayTraceOrigin.X > std::max<float>(Min.X, Maxs.X))) {
			return false;
		}

		if (RayTraceDirection.Y == 0.f && (RayTraceOrigin.Y < std::min<float>(Min.Y, Maxs.Y) || RayTraceOrigin.Y > std::max<float>(Min.Y, Maxs.Y))) {
			return false;
		}

		if (RayTraceDirection.Z == 0.f && (RayTraceOrigin.Z < std::min<float>(Min.Z, Maxs.Z) || RayTraceOrigin.Z > std::max<float>(Min.Z, Maxs.Z))) {
			return false;
		}

		const float Trace1 = (Min.X - RayTraceOrigin.X) * RaytraceDirectionInv.X;
		const float Trace2 = (Maxs.X - RayTraceOrigin.X) * RaytraceDirectionInv.X;
		const float Trace3 = (Min.Y - RayTraceOrigin.Y) * RaytraceDirectionInv.Y;
		const float Trace4 = (Maxs.Y - RayTraceOrigin.Y) * RaytraceDirectionInv.Y;
		const float Trace5 = (Min.Z - RayTraceOrigin.Z) * RaytraceDirectionInv.Z;
		const float Trace6 = (Maxs.Z - RayTraceOrigin.Z) * RaytraceDirectionInv.Z;

		const float TraceMin = std::max<float>(std::max<float>(std::min<float>(Trace1, Trace2), std::min<float>(Trace3, Trace4)), std::min<float>(Trace5, Trace6));
		const float TraceMax = std::min<float>(std::min<float>(std::max<float>(Trace1, Trace2), std::max<float>(Trace3, Trace4)), std::max<float>(Trace5, Trace6));

		if (TraceMax < 0.f)
		{
			Distance = TraceMax;
			return false;
		}

		if (TraceMin > TraceMax)
		{
			Distance = TraceMax;
			return false;
		}

		Distance = TraceMin;

		End = RayTraceDirection * Distance + RayTraceOrigin;

		return true;
	}
};