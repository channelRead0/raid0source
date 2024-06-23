#pragma once
#include <cpr/cpr.h>

namespace NwtworkUtilities
{
	inline bool IsConnected()
	{
		cpr::Response Response = cpr::Get(cpr::Url{ "https://www.google.com/" });
		return (Response.status_code == 200);
	}
}