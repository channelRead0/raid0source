#include "crypto.h"

const char* crypto::generate_key_char(int length)
{
	const std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

	std::random_device random_device;
	std::mt19937 generate(random_device());
	std::uniform_int_distribution<int> distribution(0, charset.size() - 1);

	std::string random_string;
	for (int i = 0; i < length; ++i) {
		random_string += charset[distribution(generate)];
	}

	const char* random_characters = random_string.c_str();

	return random_characters;
}

bool crypto::compare(volatile const char* p1, volatile const char* p2, const size_t n)
{
	volatile char c = 0;
	for (size_t i = 0; i < n; ++i)
		c |= p1[i] ^ p2[i];
	return (c == 0);
}