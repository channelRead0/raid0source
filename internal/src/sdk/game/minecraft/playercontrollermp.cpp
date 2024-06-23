#include "pch.h"
#include "playercontrollermp.hpp"

#include "sdk/internal/register/envregister.hpp"

PlayerControllerMP::PlayerControllerMP(JNIEnv* Env)
{
	this->PlayerControllerMPMapper = this->PlayerControllerMPMapper ? this->PlayerControllerMPMapper : std::make_shared<GameClass>(Env, "net/minecraft/client/multiplayer/PlayerControllerMP");
}

PlayerControllerMP::PlayerControllerMP(JNIEnv* Env, jobject InstanceObject)
{
	this->PlayerControllerMPMapper = this->PlayerControllerMPMapper ? this->PlayerControllerMPMapper : std::make_shared<GameClass>(Env, "net/minecraft/client/multiplayer/PlayerControllerMP");
	this->InstanceObject = InstanceObject;
}

bool PlayerControllerMP::IsHittingBlock(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return PlayerControllerMPMapper->GetBooleanField(Env, InstanceObject, "isHittingBlock");
}

void PlayerControllerMP::ClickWindow(JNIEnv* Env, int A, int B, int C, int D, Player* Player) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	PlayerControllerMPMapper->CallObjectMethod(Env, InstanceObject, "windowClick", A, B, C, D, Player->GetInstanceObject());
}

bool PlayerControllerMP::SendUseItem(JNIEnv* Env, Player* Player, World* World, ItemStack* ItemStack) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return {};
	}

	return PlayerControllerMPMapper->CallBooleanMethod(Env, InstanceObject, "sendUseItem", Player->GetInstanceObject(), World->GetInstanceObject(), ItemStack->GetInstanceObject());
}

bool PlayerControllerMP::IsInCreativeMode(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return {};
	}

	return PlayerControllerMPMapper->CallBooleanMethod(Env, InstanceObject, "isInCreativeMode");
}

int PlayerControllerMP::ThirdPersonView(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return {};
	}

	return PlayerControllerMPMapper->CallBooleanMethod(Env, InstanceObject, "thirdPersonView");
}

PlayerControllerMP::~PlayerControllerMP()
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
