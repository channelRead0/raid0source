#include "pch.h"
#include "iinventory.hpp"
#include <sdk/internal/register/envregister.hpp>

IInventory::IInventory(JNIEnv* Env)
{
	this->IInventoryMapper = this->IInventoryMapper ? this->IInventoryMapper : std::make_shared<GameClass>(Env, "net/minecraft/inventory/IInventory");
}

IInventory::IInventory(JNIEnv* Env, jobject InstanceObject)
{
	this->IInventoryMapper = this->IInventoryMapper ? this->IInventoryMapper : std::make_shared<GameClass>(Env, "net/minecraft/inventory/IInventory");
	this->InstanceObject = InstanceObject;
}

int IInventory::GetSizeInventory(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return 0;
	}

	return IInventoryMapper->CallIntMethod(Env, InstanceObject, "getSizeInventory");
}

std::shared_ptr<ItemStack> IInventory::GetStackInSlot(JNIEnv* Env, const int Slot) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<ItemStack>(Env, jobject());
	}

	if (const auto ItemStackObject = IInventoryMapper->CallObjectMethod(Env, InstanceObject, "getStackInSlot", Slot)) {
		return std::make_shared<ItemStack>(Env, ItemStackObject);
	}

	return std::make_shared<ItemStack>(Env, jobject());
}

IInventory::~IInventory()
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
