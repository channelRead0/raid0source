#pragma once

#include "keybinding.hpp"
#include "player.hpp"
#include "src/sdk/internal/templates/classbase.hpp"

class GameSettings : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> GameSettingsMapper = nullptr;
public:
	GameSettings(JNIEnv* Env);
	GameSettings(JNIEnv* Env, jobject InstanceObject);

	std::shared_ptr<KeyBinding> GetKeyBindSneakPtr(JNIEnv* Env) const;
	std::shared_ptr<KeyBinding> GetKeyBindForwardPtr(JNIEnv* Env) const;
	std::shared_ptr<KeyBinding> GetKeyBindBackPtr(JNIEnv* Env) const;
	std::shared_ptr<KeyBinding> GetKeyBindInventoryPtr(JNIEnv* Env) const;
	std::shared_ptr<KeyBinding> GetKeyBindJumpPtr(JNIEnv* Env) const;
	std::shared_ptr<KeyBinding> GetKeyBindSprint(JNIEnv* Env) const;

	int GetThirdPersonView(JNIEnv* Env) const;

	float MouseSensitivity(JNIEnv* Env) const;

	~GameSettings();
};

//TODO: check differences and define them in World1_7 derivation, 1.8 should be fully working
//use respective class version derivation on declaration, methods with differences will be added here
class GameSettings1_7 : public GameSettings
{
public:
};

class GameSettings1_8 : public GameSettings
{
public:
};