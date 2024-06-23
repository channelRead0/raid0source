#pragma once
#include "sdk/internal/register/classregister.hpp"
#include <numbers>

#include "maths/random/rng.hpp"

class StopWatch
{
public:
	StopWatch(std::chrono::milliseconds EveryXms);
	bool IsElapsed();
	void SetEvery(std::chrono::milliseconds EveryXms);
	void Reset();
private:
	std::chrono::steady_clock::time_point TimerBegin;
	std::chrono::milliseconds TargetMs;
};

inline StopWatch::StopWatch(const std::chrono::milliseconds EveryXms)
{
	SetEvery(EveryXms);
}

inline bool StopWatch::IsElapsed()
{
	const std::chrono::steady_clock::time_point TimePointNow = std::chrono::steady_clock::now();
	if (duration_cast<std::chrono::milliseconds>(TimePointNow - TimerBegin) >= TargetMs)
	{
		TimerBegin = TimePointNow;
		return true;
	}
	return false;
}

inline void StopWatch::SetEvery(const std::chrono::milliseconds EveryXms)
{
	TimerBegin = std::chrono::steady_clock::now();
	TargetMs = EveryXms;
}

inline void StopWatch::Reset()
{
	TimerBegin = std::chrono::steady_clock::now();
}

namespace ClassUtils
{
	inline bool CheckSanityAndGetClasses(JNIEnv* Env, GameClasses* GameClassesPtr)
	{
		const auto ThreadGameClasses = ClassRegister::GetThreadClasses(Env);

		if (!ThreadGameClasses.Minecraft->GetInstanceObject()) {
			return false;
		}

		const auto PlayerObject = ThreadGameClasses.Minecraft->GetThePlayer(Env);
		const auto WorldObject = ThreadGameClasses.Minecraft->GetTheWorld(Env);

		if (!PlayerObject || !WorldObject) {
			return false;
		}

		ThreadGameClasses.Player->UpdateInstanceObject(PlayerObject);

		//deleting cus no worldclass impl
		Env->DeleteLocalRef(WorldObject);

		*GameClassesPtr = ThreadGameClasses;
		return true;
	}
}

namespace MathUtils
{
	inline float Wrap180(float value)
	{
		if (value >= 180.f)
			value -= 360.f;
		if (value < -180.f)
			value += 360.f;
		return value;
	}

	inline float CalculateDeltaPitch(const float DistanceY, const float DistanceXZ, const float CurrentPitch) {
		return -(atan(DistanceY / DistanceXZ) * 180 / std::numbers::pi_v<float>) - CurrentPitch;
	}

	inline float Rad2Deg(float X)
	{
		return X * 180.f / M_PI;
	}

	inline Vector3<double> GetAngles(Vector3<double> Local, Vector3<double> Other)
	{
		double d_x = (double)Other.X - (double)Local.X;
		double d_y = (double)Other.Y - (double)Local.Y;
		double d_z = (double)Other.Z - (double)Local.Z;

		double hypothenuse = sqrt(d_x * d_x + d_z * d_z);
		float yaw = Rad2Deg(atan2(d_z, d_x)) - 90.f;
		float pitch = Rad2Deg(-atan2(d_y, hypothenuse));

		return Vector3<double>(yaw, pitch, 0.F);
	}

	inline float CalculateDeltaYaw(const float DistanceX, const float DistanceZ, const float CurrentYaw) {
		return Wrap180(fmodf(atan2f(DistanceZ, DistanceX) * 180.F / std::numbers::pi_v<float> -90.F - CurrentYaw, 360.f));
	}

	inline float ProportionalValue(const float X, const float RangeMax, const float RangeMin, const float ValueMin, const float ValueMax)
	{
		if (X < RangeMin || X > RangeMax) {
			return 0.0;
		}

		const double Distance = X - RangeMin;
		const double Range = RangeMax - RangeMin;

		const double Proportion = 1.0 - (Distance / Range);
		const double Y = ValueMin + (Proportion * (ValueMax - ValueMin));
		return Y;
	}

	inline float AngleStep(const float Step, float Angle, const float Speed)
	{
		auto Random = RandomNumberGenerator();

		Angle *= 0.15f;

		const float Steps = Angle < 0.0f ? floorf(std::clamp((0.5f * Speed), 1.f, 3.f) * Angle * 5.0f) : ceilf(std::clamp((0.5f * Speed), 1.f, 3.f) * Angle * 5.0f);

		return Steps * Step * 0.15f;
	}

	inline float LimitChange(const float Change, const float Speed)
	{
		if (Change == 0.0f)
			return 0.0f;

		float Ret = Change;

		if (Ret > Speed)
		{
			Ret = Speed;
		}
		else if (Ret < -Speed)
		{
			Ret = -Speed;
		}

		//Ret += fabs(Ret) / 12.f;

		return Ret;
	}

	inline void sin_cos(float radians, float* sine, float* cosine)
	{
		*sine = sin(radians);
		*cosine = cos(radians);
	}

	enum
	{
		PITCH = 0, // up / down
		YAW, // left / right
		ROLL // fall over
	};

	static constexpr float pirad = 0.01745329251f;

	static constexpr float deg2rad(const float& x)
	{
		return x * pirad;
	}

	inline void AngleVectors(const Vector3<float>& angles, Vector3<float>* forward = nullptr, Vector3<float>* right = nullptr, Vector3<float>* up = nullptr)
	{
		float sr, sp, sy, cr, cp, cy;

		sin_cos(deg2rad(angles[PITCH]), &sp, &cp);
		sin_cos(deg2rad(angles[YAW]), &sy, &cy);
		sin_cos(deg2rad(angles[ROLL]), &sr, &cr);

		if (forward)
		{
			forward->X = -cp * sy;
			forward->Y = -sp;
			forward->Z = cp * cy;
		}

		if (right)
		{
			right->X = -sr * -sp * sy - -cr * cy;
			right->Y = sr * -cp;
			right->Z = sr * -sp * cy - -cr * sy;
		}

		if (up)
		{
			up->X = cr * sp * sy + -sr * cy;
			up->Y = cr * -cp;
			up->Z = -cr * sp * cy - sr * sy;
		}
	}

	inline std::string Format(const char* Fmt, float Data)
	{
		char buffer[50];

		std::sprintf(buffer, Fmt, Data);

		return std::string(buffer);
	}

	inline Vector3<float> FromHSB(float h, float s, float v)
	{
		float _r, _g, _b;

		float _h = h == 1.0f ? 0 : h * 6.0f;
		auto i = static_cast<int>(trunc(_h));
		float f = _h - i;

		const float p = v * (1.f - s);
		const float q = v * (1.f - s * f);
		const float t = v * (1.f - s * (1.f - f));

		switch (i) {
		case 0:
			_r = v;
			_g = t;
			_b = p;
			break;

		case 1:
			_r = q;
			_g = v;
			_b = p;
			break;

		case 2:
			_r = p;
			_g = v;
			_b = t;
			break;

		case 3:
			_r = p;
			_g = q;
			_b = v;
			break;

		case 4:
			_r = t;
			_g = p;
			_b = v;
			break;

		default:
			_r = v;
			_g = p;
			_b = q;
			break;
		}

		return Vector3<float>((unsigned char)(_r * 255), (unsigned char)(_g * 255), (unsigned char)(_b * 255));
	}
}

class CTimer {
private:
	unsigned long long Time = 0;
public:
	CTimer();
	CTimer(unsigned long long Time) : Time(Time) {}

	void Reset();
	unsigned long long GetTime();
	bool HasPassed(long Miliseconds);
};

inline CTimer::CTimer()
{
	this->Time = GetTickCount64();
}

inline void CTimer::Reset()
{
	this->Time = GetTickCount64();
}

inline unsigned long long CTimer::GetTime()
{
	return this->Time;
}

inline bool CTimer::HasPassed(long Miliseconds)
{
	return (GetTickCount64() - this->GetTime() >= Miliseconds);
}