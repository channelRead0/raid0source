#include "pch.h"
#include "gamesettings.hpp"

#include "sdk/internal/register/envregister.hpp"

GameSettings::GameSettings(JNIEnv* Env)
{
	this->GameSettingsMapper = this->GameSettingsMapper ? this->GameSettingsMapper : std::make_shared<GameClass>(Env, "net/minecraft/client/settings/GameSettings");
}

GameSettings::GameSettings(JNIEnv* Env, jobject InstanceObject)
{
	this->GameSettingsMapper = this->GameSettingsMapper ? this->GameSettingsMapper : std::make_shared<GameClass>(Env, "net/minecraft/client/settings/GameSettings");
	this->InstanceObject = InstanceObject;
}

std::shared_ptr<KeyBinding> GameSettings::GetKeyBindSneakPtr(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<KeyBinding>(Env, jobject());
	}

	if (const auto KeyBindSneakObject = GameSettingsMapper->GetObjectField(Env, InstanceObject, "keyBindSneak")) {
		return std::make_shared<KeyBinding>(Env, KeyBindSneakObject);
	}

	return std::make_shared<KeyBinding>(Env, jobject());
}//keyBindForward

std::shared_ptr<KeyBinding> GameSettings::GetKeyBindForwardPtr(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<KeyBinding>(Env, jobject());
	}

	if (const auto KeyBindForwardObject = GameSettingsMapper->GetObjectField(Env, InstanceObject, "keyBindForward")) {
		return std::make_shared<KeyBinding>(Env, KeyBindForwardObject);
	}

	return std::make_shared<KeyBinding>(Env, jobject());
}

std::shared_ptr<KeyBinding> GameSettings::GetKeyBindBackPtr(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<KeyBinding>(Env, jobject());
	}

	if (const auto KeyBindBackObject = GameSettingsMapper->GetObjectField(Env, InstanceObject, "keyBindBack")) {
		return std::make_shared<KeyBinding>(Env, KeyBindBackObject);
	}

	return std::make_shared<KeyBinding>(Env, jobject());
}

std::shared_ptr<KeyBinding> GameSettings::GetKeyBindInventoryPtr(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<KeyBinding>(Env, jobject());
	}

	if (const auto KeyBindInventoryObject = GameSettingsMapper->GetObjectField(Env, InstanceObject, "keyBindInventory")) {
		return std::make_shared<KeyBinding>(Env, KeyBindInventoryObject);
	}

	return std::make_shared<KeyBinding>(Env, jobject());
}

std::shared_ptr<KeyBinding> GameSettings::GetKeyBindJumpPtr(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<KeyBinding>(Env, jobject());
	}

	if (const auto KeyBindJumpObject = GameSettingsMapper->GetObjectField(Env, InstanceObject, "keyBindJump")) {
		return std::make_shared<KeyBinding>(Env, KeyBindJumpObject);
	}

	return std::make_shared<KeyBinding>(Env, jobject());
}

std::shared_ptr<KeyBinding> GameSettings::GetKeyBindSprint(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<KeyBinding>(Env, jobject());
	}

	if (const auto KeyBindSprint = GameSettingsMapper->GetObjectField(Env, InstanceObject, "keyBindSprint")) {
		return std::make_shared<KeyBinding>(Env, KeyBindSprint);
	}

	return std::make_shared<KeyBinding>(Env, jobject());
}

int GameSettings::GetThirdPersonView(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return {};
	}

	return GameSettingsMapper->GetIntField(Env, InstanceObject, "thirdPersonView");
}

float GameSettings::MouseSensitivity(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return GameSettingsMapper->GetFloatField(Env, InstanceObject, "mouseSensitivity");
}

GameSettings::~GameSettings()
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
