#pragma once

#include "player.hpp"
#include "src/sdk/internal/templates/classbase.hpp"

class GuiScreen : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> GuiScreenMapper = nullptr;
	inline static std::shared_ptr<GameClass> GuiInventoryMapper = nullptr;
	inline static std::shared_ptr<GameClass> GuiChatMapper = nullptr;
	inline static std::shared_ptr<GameClass> GuiChestMapper = nullptr;
	//net/minecraft/client/gui/inventory/GuiInventory
public:
	GuiScreen(JNIEnv* Env);
	GuiScreen(JNIEnv* Env, jobject InstanceObject);

	std::shared_ptr<GuiScreen> NewGuiInventory(JNIEnv* Env, Player* PlayerPtr) const;

	bool IsInMenu() const;
	bool IsInInventory(JNIEnv* Env) const;
	bool IsInChat(JNIEnv* Env) const;
	bool IsInChest(JNIEnv* Env) const;

	~GuiScreen();
};

//TODO: check differences and define them in World1_7 derivation, 1.8 should be fully working
//use respective class version derivation on declaration, methods with differences will be added here
class GuiScreen1_7 : public GuiScreen
{
public:
};

class GuiScreen1_8 : public GuiScreen
{
public:
};