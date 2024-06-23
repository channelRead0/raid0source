#include "pch.h"
#include "itemstack.hpp"

#include "sdk/internal/register/envregister.hpp"

ItemStack::ItemStack(JNIEnv* Env)
{
	this->ItemMapper = this->ItemMapper ? this->ItemMapper : std::make_shared<GameClass>(Env, "net/minecraft/item/Item");
	this->ItemStackMapper = this->ItemStackMapper ? this->ItemStackMapper : std::make_shared<GameClass>(Env, "net/minecraft/item/ItemStack");
	this->ItemSwordMapper = this->ItemSwordMapper ? this->ItemSwordMapper : std::make_shared<GameClass>(Env, "net/minecraft/item/ItemSword");
	this->ItemPickAxeMapper = this->ItemPickAxeMapper ? this->ItemPickAxeMapper : std::make_shared<GameClass>(Env, "net/minecraft/item/ItemPickaxe");
	this->ItemAxeMapper = this->ItemAxeMapper ? this->ItemAxeMapper : std::make_shared<GameClass>(Env, "net/minecraft/item/ItemAxe");
	this->ItemFoodMapper = this->ItemFoodMapper ? this->ItemFoodMapper : std::make_shared<GameClass>(Env, "net/minecraft/item/ItemFood");
	this->ItemPotionMapper = this->ItemPotionMapper ? this->ItemPotionMapper : std::make_shared<GameClass>(Env, "net/minecraft/item/ItemPotion");
	this->ItemBlockMapper = this->ItemBlockMapper ? this->ItemBlockMapper : std::make_shared<GameClass>(Env, "net/minecraft/item/ItemBlock");
}

ItemStack::ItemStack(JNIEnv* Env, jobject InstanceObject)
{
	this->ItemMapper = this->ItemMapper ? this->ItemMapper : std::make_shared<GameClass>(Env, "net/minecraft/item/Item");
	this->ItemStackMapper = this->ItemStackMapper ? this->ItemStackMapper : std::make_shared<GameClass>(Env, "net/minecraft/item/ItemStack");
	this->ItemSwordMapper = this->ItemSwordMapper ? this->ItemSwordMapper : std::make_shared<GameClass>(Env, "net/minecraft/item/ItemSword");
	this->ItemAxeMapper = this->ItemAxeMapper ? this->ItemAxeMapper : std::make_shared<GameClass>(Env, "net/minecraft/item/ItemAxe");
	this->ItemPickAxeMapper = this->ItemPickAxeMapper ? this->ItemPickAxeMapper : std::make_shared<GameClass>(Env, "net/minecraft/item/ItemPickaxe");
	this->ItemFoodMapper = this->ItemFoodMapper ? this->ItemFoodMapper : std::make_shared<GameClass>(Env, "net/minecraft/item/ItemFood");
	this->ItemPotionMapper = this->ItemPotionMapper ? this->ItemPotionMapper : std::make_shared<GameClass>(Env, "net/minecraft/item/ItemPotion");
	this->ItemBlockMapper = this->ItemBlockMapper ? this->ItemBlockMapper : std::make_shared<GameClass>(Env, "net/minecraft/item/ItemBlock");
	this->InstanceObject = InstanceObject;
}

std::shared_ptr<Item> ItemStack::GetItemPtr(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	if (const auto GetItemObject = ItemStackMapper->CallObjectMethod(Env, InstanceObject, "getItem"); !Env->IsSameObject(GetItemObject, NULL)) {
		return std::make_shared<Item>(Env, GetItemObject);
	}

	return std::make_shared<Item>(Env, jobject());
}

std::string ItemStack::GetDisplayName(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return {};
	}

	return ItemStackMapper->CallStringMethod(Env, InstanceObject, "getDisplayName");
}

std::string ItemStack::GetUnlocalizedName(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return {};
	}

	return ItemStackMapper->CallStringMethod(Env, InstanceObject, "getUnlocalizedName");
}

int ItemStack::GetMetadata(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return ItemMapper->GetIntField(Env, InstanceObject, "metadata");
}

bool ItemStack::IsValid(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return false;
	}

	const auto ItemObject = GetItemPtr(Env);

	if (Env->IsSameObject(ItemObject->GetInstanceObject(), NULL)) {
		return false;
	}

	return true;
}

bool ItemStack::IsFist(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return false;
}

bool ItemStack::IsPickAxe(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}
	const auto ItemObject = GetItemPtr(Env);

	if (Env->IsSameObject(ItemObject->GetInstanceObject(), NULL)) {
		return{};
	}

	return Env->IsInstanceOf(ItemObject->GetInstanceObject(), ItemPickAxeMapper->GetClass());
}

bool ItemStack::IsSword(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	const auto ItemObject = GetItemPtr(Env);

	if (Env->IsSameObject(ItemObject->GetInstanceObject(), NULL)) {
		return{};
	}

	return Env->IsInstanceOf(ItemObject->GetInstanceObject(), ItemSwordMapper->GetClass());
}

bool ItemStack::IsAxe(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	const auto ItemObject = GetItemPtr(Env);

	if (Env->IsSameObject(ItemObject->GetInstanceObject(), NULL)) {
		return{};
	}

	return Env->IsInstanceOf(ItemObject->GetInstanceObject(), ItemAxeMapper->GetClass());
}

bool ItemStack::IsFood(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	const auto ItemObject = GetItemPtr(Env);

	if (Env->IsSameObject(ItemObject->GetInstanceObject(), NULL)) {
		return{};
	}

	return Env->IsInstanceOf(ItemObject->GetInstanceObject(), ItemFoodMapper->GetClass());
}

bool ItemStack::IsPotion(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	const auto ItemObject = GetItemPtr(Env);

	if (Env->IsSameObject(ItemObject->GetInstanceObject(), NULL)) {
		return{};
	}

	return Env->IsInstanceOf(ItemObject->GetInstanceObject(), ItemPotionMapper->GetClass());
}

bool ItemStack::IsStick(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	const auto ItemObject = GetItemPtr(Env);

	if (Env->IsSameObject(ItemObject->GetInstanceObject(), NULL)) {
		return{};
	}

	return (ItemObject->GetUnlocalizedName(Env) == std::string("item.stick"));
}

bool ItemStack::IsBlock(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	const auto ItemObject = GetItemPtr(Env);

	if (Env->IsSameObject(ItemObject->GetInstanceObject(), NULL)) {
		return{};
	}

	return Env->IsInstanceOf(ItemObject->GetInstanceObject(), ItemBlockMapper->GetClass());
}

float ItemStack::GetStrVsBlock(JNIEnv* Env, jobject Block) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return 0.F;
	}

	return ItemStackMapper->CallFloatMethod(Env, InstanceObject, "getStrVsBlock", Block);
}

ItemStack::~ItemStack()
{
	/*const auto Env = EnvRegister::GetThreadEnv();

	if (Env == nullptr) {
		return;
	}

	if (InstanceObject && InstanceObject != NULL)
	{
		const auto ReferenceType = Env->GetObjectRefType(InstanceObject);

		if (ReferenceType == JNIGlobalRefType || ReferenceType == JNIWeakGlobalRefType) { Env->DeleteGlobalRef(InstanceObject); }
		if (ReferenceType == JNILocalRefType) { Env->DeleteLocalRef(InstanceObject); }
	}

	InstanceObject = nullptr;*/
}

Item::Item(JNIEnv* Env)
{
	this->ItemMapper = this->ItemMapper ? this->ItemMapper : std::make_shared<GameClass>(Env, "net/minecraft/item/Item");
}

Item::Item(JNIEnv* Env, jobject InstanceObject)
{
	this->InstanceObject = InstanceObject;
	this->ItemMapper = this->ItemMapper ? this->ItemMapper : std::make_shared<GameClass>(Env, "net/minecraft/item/Item");
}

std::string Item::GetUnlocalizedName(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return ItemMapper->CallStringMethod(Env, InstanceObject, "getUnlocalizedName");
}

Item::~Item()
{
	/*const auto Env = EnvRegister::GetThreadEnv();

	if (Env == nullptr) {
		return;
	}

	if (InstanceObject && InstanceObject != NULL)
	{
		const auto ReferenceType = Env->GetObjectRefType(InstanceObject);

		if (ReferenceType == JNIGlobalRefType || ReferenceType == JNIWeakGlobalRefType) { Env->DeleteGlobalRef(InstanceObject); }
		if (ReferenceType == JNILocalRefType) { Env->DeleteLocalRef(InstanceObject); }
	}

	InstanceObject = nullptr;*/
}

ItemPotion::ItemPotion(JNIEnv* Env)
{
	this->ItemPotionMapper = this->ItemPotionMapper ? this->ItemPotionMapper : std::make_shared<GameClass>(Env, "net/minecraft/item/ItemPotion");
	this->PotionEffectMapper = this->PotionEffectMapper ? this->PotionEffectMapper : std::make_shared<GameClass>(Env, "net/minecraft/potion/PotionEffect");
}

ItemPotion::ItemPotion(JNIEnv* Env, jobject InstanceObject)
{
	this->ItemPotionMapper = this->ItemPotionMapper ? this->ItemPotionMapper : std::make_shared<GameClass>(Env, "net/minecraft/item/ItemPotion");
	this->PotionEffectMapper = this->PotionEffectMapper ? this->PotionEffectMapper : std::make_shared<GameClass>(Env, "net/minecraft/potion/PotionEffect");

	this->InstanceObject = InstanceObject;
}

bool ItemPotion::IsSplash(JNIEnv* Env)
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return ItemPotionMapper->GetIntField(Env, InstanceObject, "isSplash");
}
//MESSY ASH

int ItemPotion::GetPrimaryPotionEffect(JNIEnv* Env, jobject Stack) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return 0;
	}

	static auto ListMapper = std::make_shared<GameClass>(Env, "java/util/List");

	static const auto ToArray = Env->GetMethodID(ListMapper->GetClass(), "toArray", "()[Ljava/lang/Object;");
	static const auto Get = Env->GetMethodID(ListMapper->GetClass(), "get", "(I)Ljava/lang/Object;");

	const auto EffectsObject = ItemPotionMapper->CallObjectMethod(Env, InstanceObject, "getEffects", Stack);

	if (!EffectsObject) {
		return 0;
	}

	const auto Effects = (jobjectArray)Env->CallObjectMethod(EffectsObject, ToArray);

	if (!Effects)
	{
		Env->DeleteLocalRef(Effects);
		return 0;
	}

	if (Env->GetArrayLength(Effects) == 0)
	{
		Env->DeleteLocalRef(Effects);
		Env->DeleteLocalRef(EffectsObject);
		return 0;
	}

	jobject element = Env->GetObjectArrayElement(Effects, 0);

	if (!element)
	{
		Env->DeleteLocalRef(Effects);
		Env->DeleteLocalRef(EffectsObject);
		return 0;
	}

	auto Pid = PotionEffectMapper->CallIntMethod(Env, element, "getPotionID");

	Env->DeleteLocalRef(element);

	Env->DeleteLocalRef(Effects);
	Env->DeleteLocalRef(EffectsObject);

	return Pid;
}

bool ItemPotion::ContainsEffectFromID(JNIEnv* Env, jobject Stack, int ID) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	static auto ListMapper = std::make_shared<GameClass>(Env, "java/util/List");

	static const auto ToArray = Env->GetMethodID(ListMapper->GetClass(), "toArray", "()[Ljava/lang/Object;");
	static const auto Get = Env->GetMethodID(ListMapper->GetClass(), "get", "(I)Ljava/lang/Object;");

	const auto EffectsObject = ItemPotionMapper->CallObjectMethod(Env, InstanceObject, "getEffects", Stack);

	if (!EffectsObject) {
		return false;
	}

	const auto Effects = (jobjectArray)Env->CallObjectMethod(EffectsObject, ToArray);

	if (!Effects) 
	{
		Env->DeleteLocalRef(Effects);
		return false;
	}
	
	if (Env->GetArrayLength(Effects) == 0) 
	{
		Env->DeleteLocalRef(Effects);
		Env->DeleteLocalRef(EffectsObject);
		return false;
	}

	for (int i = 0; i < Env->GetArrayLength(Effects); i++)
	{
		jobject element = Env->GetObjectArrayElement(Effects, i);

		if (!element) {
			continue;
		}
		
		auto Pid = PotionEffectMapper->CallIntMethod(Env, element, "getPotionID");
		Env->DeleteLocalRef(element);

		if (Pid == ID)
		{
			Env->DeleteLocalRef(Effects);
			Env->DeleteLocalRef(EffectsObject);

			return true;
		}
	}
	
	Env->DeleteLocalRef(Effects);
	Env->DeleteLocalRef(EffectsObject);

	return false;


	/*if (Effects.empty()) {
		std::cout << "Emp" << std::endl;
		return false;
	}

	for (auto& Effect : Effects)
	{
		if (auto PotionID = PotionEffectMapper->CallIntMethod(Env, InstanceObject, "getPotionID"); PotionID != ID) {
			std::cout << PotionID << std::endl;
			continue;
		}

		return true;
	}*/
}

ItemPotion::~ItemPotion()
{	
	const auto Env = EnvRegister::GetThreadEnv();

	if (Env == nullptr) {
		return;
	}

	if (InstanceObject && InstanceObject != NULL)
	{
		const auto ReferenceType = Env->GetObjectRefType(InstanceObject);

		if (ReferenceType == JNIGlobalRefType || ReferenceType == JNIWeakGlobalRefType) { Env->DeleteGlobalRef(InstanceObject); }
		if (ReferenceType == JNILocalRefType) { Env->DeleteLocalRef(InstanceObject); }
	}

	InstanceObject = nullptr;
}
