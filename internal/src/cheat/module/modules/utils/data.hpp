#pragma once
#include "../../module.hpp"

class Data : public Module
{
public:
	void Run(JNIEnv* Env) override;
};

namespace GameData
{
	inline std::mutex GameDataMutex;

	inline std::vector<std::shared_ptr<Player>> PlayerEntityList;
	inline std::vector<std::shared_ptr<Player>> EntityList;
}