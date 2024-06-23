#pragma once
#include "concurrent_vector.h"

#include "../../module.hpp"

class Blink : public Module
{
private:
	std::atomic<bool> DoBlink = false;
	concurrency::concurrent_vector<std::shared_ptr<PacketSendData>> SendPacketQueue;

	std::mutex Mutex;
	concurrency::concurrent_vector<std::shared_ptr<PacketReceiveData>> ReceivedPacketQueue;
public:
	enum ON_TIMEOUT
	{
		DISABLE = 0,
		AUTOSEND = 1,
	};

	enum DIRECTION
	{
		SEND = 0,
		RECEIVE = 1, 
		BOTH = 2,
	};

	inline static bool DisableOnLocalDamage = false;
	inline static bool DisableOnTargetDamage = false;

	inline static int TimeoutMode = AUTOSEND;
	inline static int TimeBeforeAutoSend = 10;
	inline static int Timeout = 1000;
	inline static int Direction = BOTH;

	/*virtual void Run(JNIEnv* Env) override;
	virtual void OnPacketReceive(JNIEnv* Env, const std::shared_ptr<PacketReceiveData>& PacketData, bool* ShouldCancel) override;
	virtual void OnPacketSend(JNIEnv* Env, const std::shared_ptr<PacketSendData>& PacketPtr, bool* ShouldCancel) override;
	virtual void OnAttackEntity(JNIEnv* Env, jobject TargetInstance, bool* ShouldCancel) override;*/
};