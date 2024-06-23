#pragma once

#include "player.hpp"
#include "world.hpp"
#include "itemstack.hpp"

#include "src/sdk/internal/templates/classbase.hpp"

class PlayerControllerMP : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> PlayerControllerMPMapper = nullptr;
public:
	PlayerControllerMP(JNIEnv* Env);
	PlayerControllerMP(JNIEnv* Env, jobject InstanceObject);

	bool IsHittingBlock(JNIEnv* Env) const;
	void ClickWindow(JNIEnv* Env, int A, int B, int C, int D, Player* Player) const;
	bool SendUseItem(JNIEnv* Env, Player* Player, World* World, ItemStack* ItemStack) const;
	bool IsInCreativeMode(JNIEnv* Env) const;

	int ThirdPersonView(JNIEnv* Env) const;

	~PlayerControllerMP();
};

//TODO: check differences and define them in World1_7 derivation, 1.8 should be fully working
//use respective class version derivation on declaration, methods with differences will be added here
class PlayerControllerMP1_7 : public PlayerControllerMP
{
public:
};

class PlayerControllerMP1_8 : public PlayerControllerMP
{
public:
};