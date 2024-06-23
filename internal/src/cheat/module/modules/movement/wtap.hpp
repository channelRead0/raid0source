#pragma once
#include "../../module.hpp"

class WTap : public Module
{
private:
	std::atomic<int> Tick = 0;
	std::shared_ptr<Player> AttackedEntity = nullptr;

	RandomNumberGenerator Random;
public:
	
	/*enum MODE
	{
		PRE,
		POST,
	};*/

	//todo: inline static int Mode = PRE;

	inline static float DistanceMin = 0.F;
	inline static float DistanceMax = 3.0F;

	inline static float DelayMax = 100.F;
	inline static float DelayMin = 50.F;

	virtual void OnTick(JNIEnv* Env) override;

	virtual void OnHandleStatusUpdate(JNIEnv* Env, const std::shared_ptr<Player>& TargetInstance, jbyte Byte, bool* ShouldCancel) override;
};