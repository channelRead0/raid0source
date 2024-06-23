#pragma once
#include "sdk/game/minecraft/minecraft.hpp"
#include "sdk/game/minecraft/world.hpp"

struct GameClasses
{
	std::shared_ptr<Minecraft> Minecraft;
	std::shared_ptr<Player> Player;
	std::shared_ptr<World> World;
};

class ClassRegister
{
public:
	static std::unordered_map<std::thread::id, GameClasses> RegisteredClasses;
	static GameClasses GetThreadClasses(JNIEnv* Env);
	static void DestroyThreadClasses();
	static bool CheckSanityAndGetClasses(JNIEnv* Env, GameClasses* GameClassesPtr);
};