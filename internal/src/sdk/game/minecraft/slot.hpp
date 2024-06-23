#pragma once

#include "itemstack.hpp"
#include "player.hpp"
#include "src/sdk/internal/templates/classbase.hpp"

class Slot : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> SlotMapper = nullptr;
public:
	Slot(JNIEnv* Env);
	Slot(JNIEnv* Env, jobject InstanceObject);

	std::shared_ptr<ItemStack> GetItemStack(JNIEnv* Env) const;

	~Slot();
};

//TODO: check differences and define them in World1_7 derivation, 1.8 should be fully working
//use respective class version derivation on declaration, methods with differences will be added here
class Slot1_7 : public Slot
{
public:
};

class Slot1_8 : public Slot
{
public:
};