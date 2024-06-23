#pragma once
#include <src/globals/structures/buffer.hpp> 

#define API_LOGGED_IN -33819032
#define API_EXPIRED_KEY -33819008
#define API_BAD_REQUEST -33818976
#define API_OUTDATED -33818944
#define API_UNAUTHORIZED -33818912
#define API_INVALID_KEY -33818880

enum AUTH_STATUS
{
	LOGGED_IN,
	EXPIRED_KEY,
	BAD_REQUEST,
	OUTDATED,
	UNAUTHORIZED,
	INVALID_KEY,
	INCOMPLETE,
	UNKNOWN,
};

class Authentication
{
private:
	std::shared_ptr<Buffer> DllBuffer;

	std::string Key;
public:
	Authentication(std::string Key) { this->Key = Key; };

	AUTH_STATUS Authenticate();
	std::shared_ptr<Buffer> GetBuffer() { return DllBuffer; }
};

