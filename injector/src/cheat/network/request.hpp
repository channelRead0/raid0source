#pragma once
#include <string>
#include <cpr/cpr.h>
#include <WinInet.h>
#include <nlohmann/json.hpp>

class Request
{
private:
	std::string Url;
	std::string BodyData;
public:
	Request(std::string Url, std::string BodyData) 
	{
		this->Url = Url;
		this->BodyData = BodyData;
	}

	std::string Post();
};