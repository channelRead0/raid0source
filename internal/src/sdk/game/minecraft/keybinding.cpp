#include "pch.h"
#include "keybinding.hpp"

#include "sdk/internal/register/envregister.hpp"

KeyBinding::KeyBinding(JNIEnv* Env)
{
	this->KeyBindingMapper = this->KeyBindingMapper ? this->KeyBindingMapper : std::make_shared<GameClass>(Env, "net/minecraft/client/settings/KeyBinding");
	this->KeyBoardMapper = this->KeyBoardMapper ? this->KeyBoardMapper : std::make_shared<GameClass>(Env, "org.lwjgl.input.Keyboard");
}

KeyBinding::KeyBinding(JNIEnv* Env, jobject InstanceObject)
{
	this->KeyBindingMapper = this->KeyBindingMapper ? this->KeyBindingMapper : std::make_shared<GameClass>(Env, "net/minecraft/client/settings/KeyBinding");
	this->KeyBoardMapper = this->KeyBoardMapper ? this->KeyBoardMapper : std::make_shared<GameClass>(Env, "org.lwjgl.input.Keyboard");
	this->InstanceObject = InstanceObject;
}

void KeyBinding::SetPressed(JNIEnv* Env, const bool Pressed) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	KeyBindingMapper->SetBooleanField(Env, InstanceObject, "pressed", Pressed);
}

bool KeyBinding::GetPressed(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return false;
	}

	return KeyBindingMapper->GetBooleanField(Env, InstanceObject, "pressed");
}

int KeyBinding::GetKeyCode(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return 0;
	}

	return KeyBindingMapper->CallIntMethod(Env, InstanceObject, "getKeyCode");
}

bool KeyBinding::KeyboardPressed(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return false;
	}

	return KeyBoardMapper->CallBooleanMethod(Env, KeyBoardMapper->GetClass(), "isKeyDown", this->GetKeyCode(Env));
}

KeyBinding::~KeyBinding()
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
