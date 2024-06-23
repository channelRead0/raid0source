#include "pch.h"
#include "timer.hpp"

#include "sdk/internal/register/envregister.hpp"

Timer::Timer(JNIEnv* Env)
{
	this->TimerMapper = this->TimerMapper ? this->TimerMapper : std::make_shared<GameClass>(Env, "net/minecraft/util/Timer");
}

Timer::Timer(JNIEnv* Env, jobject InstanceObject)
{
	this->TimerMapper = this->TimerMapper ? this->TimerMapper : std::make_shared<GameClass>(Env, "net/minecraft/util/Timer");
	this->InstanceObject = InstanceObject;
}

float Timer::GetRenderPartialTicks(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return TimerMapper->GetFloatField(Env, InstanceObject, "renderPartialTicks");
}

Timer::~Timer()
{
	const auto Env = EnvRegister::GetThreadEnv();

	if (Env == nullptr) {
		return;
	}

	if (InstanceObject && !Env->IsSameObject(InstanceObject, NULL))
	{
		const auto ReferenceType = Env->GetObjectRefType(InstanceObject);

		if (ReferenceType == JNIGlobalRefType || ReferenceType == JNIWeakGlobalRefType) { Env->DeleteGlobalRef(InstanceObject); }
		if (ReferenceType == JNILocalRefType) { Env->DeleteLocalRef(InstanceObject); }
	}

	InstanceObject = nullptr;
}
