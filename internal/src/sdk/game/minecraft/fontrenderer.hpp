#pragma once

#include "player.hpp"
#include "src/sdk/internal/templates/classbase.hpp"

class FontRenderer : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> FontRendererMapper = nullptr;
public:
	FontRenderer(JNIEnv* Env);
	FontRenderer(JNIEnv* Env, jobject InstanceObject);

	~FontRenderer();
};

//TODO: check differences and define them in World1_7 derivation, 1.8 should be fully working
//use respective class version derivation on declaration, methods with differences will be added here
class FontRenderer1_7 : public FontRenderer
{
public:
};

class FontRenderer1_8 : public FontRenderer
{
public:
};