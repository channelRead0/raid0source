#pragma once

#include "player.hpp"
#include "src/sdk/internal/templates/classbase.hpp"

class KeyBinding : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> KeyBindingMapper = nullptr;
	inline static std::shared_ptr<GameClass> KeyBoardMapper = nullptr;
public:
	KeyBinding(JNIEnv* Env);
	KeyBinding(JNIEnv* Env, jobject InstanceObject);

	void SetPressed(JNIEnv* Env, bool Pressed) const;
	bool GetPressed(JNIEnv* Env) const;
	int GetKeyCode(JNIEnv* Env) const;
	bool KeyboardPressed(JNIEnv* Env) const;

	~KeyBinding();
};