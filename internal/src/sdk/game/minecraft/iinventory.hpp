#pragma once

#include "player.hpp"
#include "src/sdk/internal/templates/classbase.hpp"

class IInventory : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> IInventoryMapper = nullptr;
	//net/minecraft/client/gui/inventory/GuiInventory
public:
	IInventory(JNIEnv* Env);
	IInventory(JNIEnv* Env, jobject InstanceObject);

	int GetSizeInventory(JNIEnv* Env) const;
	std::shared_ptr<ItemStack> GetStackInSlot(JNIEnv* Env, const int Slot) const;

	~IInventory();
};