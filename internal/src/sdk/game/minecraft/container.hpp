#pragma once

#include "player.hpp"
#include "slot.hpp"
#include "src/sdk/internal/templates/classbase.hpp"
#include "iinventory.hpp"
#include "java/list.hpp"

class Container : public ClassBase
{
protected:
	inline static std::shared_ptr<GameClass> ContainerMapper = nullptr;
	inline static std::shared_ptr<GameClass> ContainerChestMapper = nullptr;
public:
	Container(JNIEnv* Env);
	Container(JNIEnv* Env, jobject InstanceObject);

	std::shared_ptr<Slot> GetSlot(JNIEnv* Env, int Slot) const;
	std::shared_ptr<List> InventorySlots(JNIEnv* Env) const;

	bool IsChest(JNIEnv* Env) const;
	int GetWindowID(JNIEnv* Env) const;

	std::shared_ptr<IInventory> GetLowerChestInventory(JNIEnv* Env) const;

	~Container();
};