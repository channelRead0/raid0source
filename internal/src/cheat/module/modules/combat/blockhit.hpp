#pragma once
#include <numeric>
#include "cheat/module/module.hpp"

class BlockHit : public Module
{
private:
	std::atomic<int> Tick = 0;
	std::shared_ptr<Player> AttackedEntity = nullptr;

	RandomNumberGenerator Random;
public:
	inline static float BlockHitDelayMax = 50.F;
	inline static float BlockHitDelayMin = 10.F;
	inline static float RangeMin = 0.0F;
	inline static float RangeMax = 3.0F;
	inline static int Chance = 100;

	enum BLOCKHIT_MODES
	{
		PASSIVE = 0, // Passive blocking
		ACTIVE = 1, // Block incoming damage
	};

	inline static int Mode = 1;

	virtual void OnTick(JNIEnv* Env) override;
	virtual void OnHandleStatusUpdate(JNIEnv* Env, const std::shared_ptr<Player>& TargetInstance, jbyte Byte, bool* ShouldCancel) override;
};