#pragma once
#include <string>
#include <nlohmann/json.hpp>

namespace JsonUtilities
{
	inline bool TryParse(nlohmann::json* Json, std::string Content)
	{
        try 
		{
			const auto Temp = nlohmann::json::parse(Content);
			*Json = Temp;
			return true;
        }
        catch (const nlohmann::json::parse_error& Exception) {
			return false;
        }
	}
};