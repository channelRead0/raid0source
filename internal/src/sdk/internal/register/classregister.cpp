#include "pch.h"
#include "classregister.hpp"

std::unordered_map<std::thread::id, GameClasses> ClassRegister::RegisteredClasses;
GameClasses ClassRegister::GetThreadClasses(JNIEnv* Env)
{
	const auto CallingThreadID = std::this_thread::get_id();

	const auto FoundThreadClasses = RegisteredClasses.find(CallingThreadID);

	if (FoundThreadClasses != RegisteredClasses.end()) {
		return FoundThreadClasses->second;
	}

	std::shared_ptr<Minecraft> MinecraftInstance = std::make_shared<Minecraft>(Env);
	std::shared_ptr<Player> PlayerInstance = std::make_shared<Player>(Env);
	std::shared_ptr<World> WorldInstance = std::make_shared<World>(Env);

	const auto ThreadGameClasses = GameClasses
	(
		MinecraftInstance,
		PlayerInstance,
		WorldInstance
	);

	RegisteredClasses[CallingThreadID] = ThreadGameClasses;
	  
	return ThreadGameClasses;
}

void ClassRegister::DestroyThreadClasses()
{
	const auto CallingThreadID = std::this_thread::get_id();

	for (auto& [ThreadID, ThreadClasses] : RegisteredClasses)
	{
		if (ThreadID == CallingThreadID) 
		{
			ThreadClasses.Minecraft.reset();
			ThreadClasses.Player.reset();
			ThreadClasses.World.reset();

			RegisteredClasses.erase(ThreadID);
		}
	}
}

bool ClassRegister::CheckSanityAndGetClasses(JNIEnv* Env, GameClasses* GameClassesPtr)
{
	const auto ThreadGameClasses = ClassRegister::GetThreadClasses(Env);

	const auto MinecraftClass = Minecraft(Env);

	if (!MinecraftClass.GetInstanceObject()) {
		return false;
	}

	const auto MinecraftObject = MinecraftClass.GetInstanceObject();
	ThreadGameClasses.Minecraft->UpdateInstanceObject(MinecraftObject);

	const auto PlayerObject = ThreadGameClasses.Minecraft->GetThePlayer(Env);
	const auto WorldObject = ThreadGameClasses.Minecraft->GetTheWorld(Env);

	if (!PlayerObject || !WorldObject) {
		return false;
	}

	ThreadGameClasses.Player->UpdateInstanceObject(PlayerObject);
	ThreadGameClasses.World->UpdateInstanceObject(WorldObject);

	*GameClassesPtr = ThreadGameClasses;
	return true;
}