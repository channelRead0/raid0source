#include "pch.h"
#include "guiscreen.hpp"

#include "sdk/internal/register/envregister.hpp"

GuiScreen::GuiScreen(JNIEnv* Env)
{
	this->GuiScreenMapper = this->GuiScreenMapper ? this->GuiScreenMapper : std::make_shared<GameClass>(Env, "net/minecraft/client/gui/GuiScreen");
	this->GuiChatMapper = this->GuiChatMapper ? this->GuiChatMapper : std::make_shared<GameClass>(Env, "net/minecraft/client/gui/GuiChat");
	this->GuiInventoryMapper = this->GuiInventoryMapper ? this->GuiInventoryMapper : std::make_shared<GameClass>(Env, "net/minecraft/client/gui/inventory/GuiInventory");
	this->GuiChestMapper = this->GuiChestMapper ? this->GuiChestMapper : std::make_shared<GameClass>(Env, "net/minecraft/client/gui/inventory/GuiChest");
}

GuiScreen::GuiScreen(JNIEnv* Env, jobject InstanceObject)
{
	this->GuiScreenMapper = this->GuiScreenMapper ? this->GuiScreenMapper : std::make_shared<GameClass>(Env, "net/minecraft/client/gui/GuiScreen");
	this->GuiChatMapper = this->GuiChatMapper ? this->GuiChatMapper : std::make_shared<GameClass>(Env, "net/minecraft/client/gui/GuiChat");
	this->GuiInventoryMapper = this->GuiInventoryMapper ? this->GuiInventoryMapper : std::make_shared<GameClass>(Env, "net/minecraft/client/gui/inventory/GuiInventory");
	this->GuiChestMapper = this->GuiChestMapper ? this->GuiChestMapper : std::make_shared<GameClass>(Env, "net/minecraft/client/gui/inventory/GuiChest");
	this->InstanceObject = InstanceObject;
}

std::shared_ptr<GuiScreen> GuiScreen::NewGuiInventory(JNIEnv* Env, Player* PlayerPtr) const
{
	const auto InitializerMethod = GuiInventoryMapper->RegisterAndFetchMethodID("<init>");

	if (!InitializerMethod) {
		return std::make_shared<GuiScreen>(Env, jobject());
	}

	if (const auto GuiInventoryObject = Env->NewObject(GuiInventoryMapper->GetClass(), InitializerMethod, PlayerPtr->GetInstanceObject())) {
		return std::make_shared<GuiScreen>(Env, Env->NewLocalRef(GuiInventoryObject));
	}

	return std::make_shared<GuiScreen>(Env, jobject());
}

bool GuiScreen::IsInMenu() const
{
	return InstanceObject != NULL;
}

bool GuiScreen::IsInInventory(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return false;
	}

	return Env->IsInstanceOf(InstanceObject, GuiInventoryMapper->GetClass());
}

bool GuiScreen::IsInChat(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return false;
	}

	return Env->IsInstanceOf(InstanceObject, GuiChatMapper->GetClass());
}

bool GuiScreen::IsInChest(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return false;
	}

	return Env->IsInstanceOf(InstanceObject, GuiChestMapper->GetClass());
}

GuiScreen::~GuiScreen()
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
