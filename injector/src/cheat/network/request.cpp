#include "pch.h"
#include "request.hpp"
#include <src/globals/globals.hpp>

std::string Request::Post()
{
	VMProtectBeginUltra("Request");

	if (!NwtworkUtilities::IsConnected()) {
		return {};
	}

	const auto ServerResponse = cpr::Post
	(
		cpr::Url{ Url.data() },
		cpr::Body{ BodyData },
		cpr::Header{ {"Content-Type", "application/json"} }
	);

	return ServerResponse.text;

	VMProtectEnd();
}
