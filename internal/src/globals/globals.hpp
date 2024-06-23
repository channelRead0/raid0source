// ReSharper disable CppInconsistentNaming
#pragma once

#include "structures/axisalignedbb_t.hpp"
#include "structures/vector4.hpp"
#include "structures/vector3.hpp"
#include "structures/vector2.hpp"
#include "utilities/utils_a.hpp"
#include "utilities/utils_b.hpp"
#include "utilities/utils_c.hpp"
#include "utilities/maths/random/rng.hpp"


#define SHOW_CONSOLE 0

enum GAME_VERSION
{
	LUNAR_1_8,
	LUNAR_1_7,
	NOT_FOUND
};

namespace Global
{
	inline bool UI_OPEN = false;
	inline bool INITIALIZING = true;
	inline bool IN_GAME = false;
	inline bool INSTANCE_DESTRUCT = false;
	inline HWND WINDOW_HANDLE;
	inline GAME_VERSION GAME_VER = NOT_FOUND;
	inline std::atomic<float> DeltaTime = 0.F;
	inline float DisplayHeight = 0.F;
	inline float DisplayWidth = 0.F;
}

template<typename T>
class Singleton
{
protected:
	Singleton() { }
	~Singleton() { }

	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

	Singleton(Singleton&&) = delete;
	Singleton& operator=(Singleton&&) = delete;
public:
	static T& Get()
	{
		static T inst{};
		return inst;
	}
};