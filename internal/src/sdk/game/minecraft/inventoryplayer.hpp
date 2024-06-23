#pragma once
#include "player.hpp"
#include "src/sdk/internal/templates/classbase.hpp"

class InventoryPlayer : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> InventoryPlayerMapper = nullptr;
public:
	InventoryPlayer(JNIEnv* Env);
	InventoryPlayer(JNIEnv* Env, jobject InstanceObject);

	int GetCurrentSlot(JNIEnv* Env) const;
	void SetCurrentSlot(JNIEnv* Env, const int Slot) const;
	void OpenInventory(JNIEnv* Env, const jobject Player) const;

	std::shared_ptr<::ItemStack> ItemStackInSlot(JNIEnv* Env, const int Slot) const;

	~InventoryPlayer();
};