#include "pch.h"
#include "inventoryplayer.hpp"
#include <sdk/internal/register/envregister.hpp>

InventoryPlayer::InventoryPlayer(JNIEnv* Env)
{
	this->InventoryPlayerMapper = this->InventoryPlayerMapper ? this->InventoryPlayerMapper : std::make_shared<GameClass>(Env, "net/minecraft/entity/player/InventoryPlayer");
}

InventoryPlayer::InventoryPlayer(JNIEnv* Env, jobject InstanceObject)
{
	this->InventoryPlayerMapper = this->InventoryPlayerMapper ? this->InventoryPlayerMapper : std::make_shared<GameClass>(Env, "net/minecraft/entity/player/InventoryPlayer");
	this->InstanceObject = InstanceObject;
}

int InventoryPlayer::GetCurrentSlot(JNIEnv* Env) const 
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return InventoryPlayerMapper->GetIntField(Env, InstanceObject, "currentItem");
}

void InventoryPlayer::SetCurrentSlot(JNIEnv* Env, const int Slot) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	return InventoryPlayerMapper->SetIntField(Env, InstanceObject, "currentItem", Slot);
}

std::shared_ptr<::ItemStack> InventoryPlayer::ItemStackInSlot(JNIEnv* Env, const int Slot) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<ItemStack>(Env, jobject());
	}

	if (const auto ItemStackObject = InventoryPlayerMapper->CallObjectMethod(Env, InstanceObject, "getStackInSlot", Slot)) {
		return std::make_shared<ItemStack>(Env, ItemStackObject);
	}

	return std::make_shared<ItemStack>(Env, jobject());
}

void InventoryPlayer::OpenInventory(JNIEnv* Env, const jobject Player) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	InventoryPlayerMapper->CallVoidMethod(Env, InstanceObject, "openInventory", Player);
}

InventoryPlayer::~InventoryPlayer()
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
