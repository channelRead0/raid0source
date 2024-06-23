#include "pch.h"
#include "container.hpp"

#include "sdk/internal/register/envregister.hpp"

Container::Container(JNIEnv* Env)
{
	this->ContainerMapper = this->ContainerMapper ? this->ContainerMapper : std::make_shared<GameClass>(Env, "net/minecraft/inventory/Container");
	this->ContainerChestMapper = this->ContainerChestMapper ? this->ContainerChestMapper : std::make_shared<GameClass>(Env, "net/minecraft/inventory/ContainerChest");
}

Container::Container(JNIEnv* Env, jobject InstanceObject)
{
	this->ContainerMapper = this->ContainerMapper ? this->ContainerMapper : std::make_shared<GameClass>(Env, "net/minecraft/inventory/Container");
	this->ContainerChestMapper = this->ContainerChestMapper ? this->ContainerChestMapper : std::make_shared<GameClass>(Env, "net/minecraft/inventory/ContainerChest");
	this->InstanceObject = InstanceObject;
}

std::shared_ptr<Slot> Container::GetSlot(JNIEnv* Env, const int Slot) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<::Slot>(Env, jobject());
	}

	const auto GetSlotMethodID = ContainerMapper->GetMethodID("getSlot");

	if (const auto SlotObject = Env->CallObjectMethod(InstanceObject, GetSlotMethodID, Slot)) {
		return std::make_shared<::Slot>(Env, SlotObject);
	}

	return std::make_shared<::Slot>(Env, jobject());
}

std::shared_ptr<List> Container::InventorySlots(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<::List>(Env, jobject());
	}

	if (const auto InventorySlots = ContainerMapper->GetObjectField(Env, InstanceObject, "inventorySlots")) {
		return std::make_shared<::List>(Env, InventorySlots);
	}

	return std::make_shared<::List>(Env, jobject());
}

bool Container::IsChest(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return false;
	}

	return Env->IsInstanceOf(InstanceObject, ContainerChestMapper->GetClass());
}

int Container::GetWindowID(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return 0;
	}

	return ContainerMapper->GetIntField(Env, InstanceObject, "windowId");
}

std::shared_ptr<IInventory> Container::GetLowerChestInventory(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<IInventory>(Env, jobject());
	}

	/*if (!this->IsChest(Env)) {
		return std::make_shared<IInventory>(Env, jobject());
	}*/

	if (const auto LowerChestInventoryObject = ContainerChestMapper->CallObjectMethod(Env, InstanceObject, "getLowerChestInventory")) {
		return std::make_shared<IInventory>(Env, LowerChestInventoryObject);
	}

	return std::make_shared<IInventory>(Env, jobject());
}

Container::~Container()
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
