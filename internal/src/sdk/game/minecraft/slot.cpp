#include "pch.h"
#include "slot.hpp"

#include "sdk/internal/register/envregister.hpp"

Slot::Slot(JNIEnv* Env)
{
	this->SlotMapper = this->SlotMapper ? this->SlotMapper : std::make_shared<GameClass>(Env, "net/minecraft/inventory/Slot");
}

Slot::Slot(JNIEnv* Env, jobject InstanceObject)
{
	this->SlotMapper = this->SlotMapper ? this->SlotMapper : std::make_shared<GameClass>(Env, "net/minecraft/inventory/Slot");
	this->InstanceObject = InstanceObject;
}

std::shared_ptr<ItemStack> Slot::GetItemStack(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<ItemStack>(Env, jobject());
	}

	if (const auto ItemStackObject = SlotMapper->CallObjectMethod(Env, InstanceObject, "getStack")) {
		return std::make_shared<ItemStack>(Env, ItemStackObject);
	}

	return std::make_shared<ItemStack>(Env, jobject());
}

Slot::~Slot()
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
