#pragma once
#include "../../module.hpp"

class JumpReset : public Module
{
private:
	std::atomic<int> Tick = 0;
	std::atomic<bool> Jumped = false;

	RandomNumberGenerator Random;
public:

	enum JUMP_MODE
	{
		PRE,
		POST,
	};

	inline static int Mode = POST;
	inline static int TickDelay = 10;
	inline static int Chance = 90;

	virtual void OnTick(JNIEnv* Env) override;
	virtual void OnPacketReceive(JNIEnv* Env, const std::shared_ptr<PacketReceiveData>& PacketData, bool* ShouldCancel) override;
};