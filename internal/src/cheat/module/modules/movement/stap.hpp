#pragma once
#include "../../module.hpp"

class STap : public Module
{
private:
	std::atomic<int> Tick = 0;
	std::shared_ptr<Player> AttackedEntity = nullptr;

	RandomNumberGenerator Random;
public:
	enum STAP_MODES
	{
		PASSIVE = 0,
		AGGRESSIVE = 1,
	};

	inline static int Mode = PASSIVE;

	inline static float MaintainDistance = 2.7F;

	inline static float DistanceMin = 0.F;
	inline static float DistanceMax = 3.0F;

	inline static float DelayMax = 100.F;
	inline static float DelayMin = 50.F;

	virtual void OnTick(JNIEnv* Env) override;
	virtual void OnHandleStatusUpdate(JNIEnv* Env, const std::shared_ptr<Player>& TargetInstance, jbyte Byte, bool* ShouldCancel) override;
};