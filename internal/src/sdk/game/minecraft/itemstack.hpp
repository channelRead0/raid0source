#pragma once

#include "minecraft.hpp"
#include "src/sdk/internal/templates/classbase.hpp"

class Item : public ClassBase {
private:
	inline static std::shared_ptr<GameClass> ItemMapper = nullptr;
public:
	Item(JNIEnv* Env);
	Item(JNIEnv* Env, jobject InstanceObject);

	std::string GetUnlocalizedName(JNIEnv* Env) const;

	~Item();
};

class ItemStack : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> ItemMapper = nullptr;
	inline static std::shared_ptr<GameClass> ItemStackMapper = nullptr;
	inline static std::shared_ptr<GameClass> ItemSwordMapper = nullptr;
	inline static std::shared_ptr<GameClass> ItemAxeMapper = nullptr;
	inline static std::shared_ptr<GameClass> ItemPickAxeMapper = nullptr;
	inline static std::shared_ptr<GameClass> ItemPotionMapper = nullptr;
	inline static std::shared_ptr<GameClass> ItemFoodMapper = nullptr;
	inline static std::shared_ptr<GameClass> ItemBlockMapper = nullptr;

public:
	ItemStack(JNIEnv* Env);
	ItemStack(JNIEnv* Env, jobject InstanceObject);

	std::shared_ptr<Item> GetItemPtr(JNIEnv* Env) const;

	std::string GetDisplayName(JNIEnv* Env) const;
	std::string GetUnlocalizedName(JNIEnv* Env) const;
	int GetMetadata(JNIEnv* Env) const;

	bool IsValid(JNIEnv* Env) const;
	bool IsFist(JNIEnv* Env) const;
	bool IsPickAxe(JNIEnv* Env) const;
	bool IsSword(JNIEnv* Env) const;
	bool IsAxe(JNIEnv* Env) const;
	bool IsFood(JNIEnv* Env) const;
	bool IsPotion(JNIEnv* Env) const;
	bool IsStick(JNIEnv* Env) const;
	bool IsBlock(JNIEnv* Env) const;

	float GetStrVsBlock(JNIEnv* Env, jobject Block) const;

	~ItemStack();
};


class ItemPotion : public ClassBase {
private:
	inline static std::shared_ptr<GameClass> ItemPotionMapper = nullptr;
	inline static std::shared_ptr<GameClass> PotionEffectMapper = nullptr;
public:
	ItemPotion(JNIEnv* Env);
	ItemPotion(JNIEnv* Env, jobject InstanceObject);

	bool IsSplash(JNIEnv* Env);
	int GetPrimaryPotionEffect(JNIEnv* Env, jobject Stack) const;
	bool ContainsEffectFromID(JNIEnv* Env, jobject Stack, int ID) const;

	~ItemPotion();
};

//TODO: check differences and define them in World1_7 derivation, 1.8 should be fully working
//use respective class version derivation on declaration, methods with differences will be added here