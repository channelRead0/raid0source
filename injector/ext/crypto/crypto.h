#pragma once
#include <cstdint>
#include <random>
#include <intrin.h>
#include "plusaes.hpp"
#include <ctime>

class crypto
{
public:
	const char* generate_key_char(int length);
	bool compare(volatile const char* p1, volatile const char* p2, const size_t n);
};