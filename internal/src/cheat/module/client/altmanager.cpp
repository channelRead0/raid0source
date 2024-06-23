#include "pch.h"
#include "altmanager.hpp"

#include <src/sdk/game/minecraft/mcsession.hpp>
#include <src/sdk/game/minecraft/minecraft.hpp>
#include <sdk/internal/register/envregister.hpp>

void AltManager::LoginOffline(JNIEnv* Env)
{
	
}

void AltManager::Reset(JNIEnv* Env)
{
	
}

bool SkipEvent = false;
void AltManager::SendPacketDetour(Frame* Frame, Thread* Thread, bool* ShouldCancel)
{
	if (!UseCrack) {
		return;
	}

	if (SkipEvent) {
		return;
	}

	if (INSTANCE_DESTRUCT) {
		return;
	}

	if (!InstancePtr->GetJniEnv()) {
		return;
	}

	JNIEnv* Env = Thread->GetEnv();

	EnvRegister::RegisterThreadEnv(Env);

	const auto GameProfileInstance = HookManager->GetObjectParam(Frame, 1);
	const auto InstanceObject = HookManager->GetObjectParam(Frame, 0);

	const auto ID = Env->GetFieldID(Env->GetObjectClass(GameProfileInstance), "id", "Ljava/util/UUID;");
	const auto Name = Env->GetFieldID(Env->GetObjectClass(GameProfileInstance), "name", "Ljava/lang/String;");

	jstring ProfileName = Env->NewStringUTF("latinatoes");

	Env->SetObjectField(GameProfileInstance, ID, jobject());
	Env->SetObjectField(GameProfileInstance, Name, ProfileName);

	Env->DeleteLocalRef(ProfileName);
}

AltManager* AltManagerPtr = new AltManager();