#include "pch.h"

#include "module.hpp"

#include "src/cheat/raid0.hpp"
#include "modules/combat/aimassist.hpp"
#include "modules/combat/autoclicker.hpp"
#include "modules/combat/example.hpp"
#include "modules/combat/blockhit.hpp"
#include "modules/combat/hitselect.hpp"
#include "modules/combat/velocity.hpp"
#include "modules/combat/reach.hpp"
#include "modules/movement/wtap.hpp"
#include "modules/movement/stap.hpp"
#include "modules/movement/blink.hpp"
#include "modules/movement/sprint.hpp"
#include "modules/movement/nojumpdelay.hpp"
#include "modules/movement/jumpreset.hpp"
#include "modules/render/render.hpp"
#include "modules/render/arraylist.hpp"
#include "modules/render/zerohud.hpp"
#include "modules/render/particles.hpp"
#include "modules/utils/backtrack.hpp"
#include "modules/utils/cheststealer.hpp"
#include "modules/utils/refill.hpp"
#include "modules/utils/backtrack.hpp"
#include "modules/utils/fastplace.hpp"
#include "modules/utils/bridgeassist.hpp"
#include "modules/utils/throwpot.hpp"
#include "modules/utils/autotool.hpp"
#include "modules/utils/data.hpp"
#include "modules/utils/framespoofer.hpp"
#include "modules/utils/noitemrelease.hpp"
#include "modules/utils/lagrange.hpp"
#include "modules/world/antibot.hpp"
#include "modules/world/teams.hpp"
#include "modules/world/friends.hpp"
#include "modules/utils/rightclicker.hpp"
#include "sdk/internal/register/envregister.hpp"

#include <src/cheat/gui/spotify/spotify.hpp>

#include "src/sdk/game/minecraft/packet.hpp"

void ModuleManager::RunAll(JNIEnv* env)
{
	for (auto& [Name, Mod] : ModuleList)
	{
		Mod->Run(env);
	}
}

void ModuleManager::RegisterModules()
{
	//auto NoItemReleaseMod = new NoItemRelease();

	//NoItemReleaseMod->Enable();

	////ModuleList.push_back(new FrameSpoofer()); 
	//ModuleList.push_back(NoItemReleaseMod);
}

void ModuleManager::RegisterThreads()
{
	ThreadedModuleList["AimAssist"] = new AimAssist();
	ThreadedModuleList["AutoClicker"] = new AutoClicker();
	ThreadedModuleList["Render"] = new Render();
	ThreadedModuleList["PlayerESP"] = new PlayerESP();
	ThreadedModuleList["Radar"] = new Radar();
	ThreadedModuleList["BridgeAssist"] = new BridgeAssist();
	ThreadedModuleList["RightClicker"] = new RightClicker();
	ThreadedModuleList["WTap"] = new WTap();
	ThreadedModuleList["STap"] = new STap();
	ThreadedModuleList["Velocity"] = new Velocity();
	ThreadedModuleList["BackTrack"] = new BackTrack();
	ThreadedModuleList["AntiBot"] = new AntiBot();
	ThreadedModuleList["NameTags"] = new NameTags();
	ThreadedModuleList["ArrayList"] = new ArrayList();
	ThreadedModuleList["Reach"] = new Reach();
	ThreadedModuleList["Example"] = new Example();
	ThreadedModuleList["ThrowPot"] = new ThrowPot();
	ThreadedModuleList["ChestStealer"] = new ChestStealer();
	ThreadedModuleList["Refill"] = new Refill();
	ThreadedModuleList["Sprint"] = new Sprint();
	ThreadedModuleList["Teams"] = new Teams();
	ThreadedModuleList["Friends"] = new Friends();
	ThreadedModuleList["Blink"] = new Blink();
	ThreadedModuleList["FastPlace"] = new FastPlace();
	ThreadedModuleList["AutoTool"] = new AutoTool();
	ThreadedModuleList["LagRange"] = new LagRange();
	ThreadedModuleList["NoJumpDelay"] = new NoJumpDelay();
	ThreadedModuleList["JumpReset"] = new JumpReset();
	ThreadedModuleList["BlockHit"] = new BlockHit();
	ThreadedModuleList["ZeroHud"] = new ZeroHud();
	//ThreadedModuleList["NoItemRelease"] = new NoItemRelease();
	ThreadedModuleList["Particles"] = new Particles();
	//ThreadedModuleList["HitSelect"] = new HitSelect();

	for (auto& [Name, Mod] : ThreadedModuleList)
	{
		auto ModuleThread = std::thread([&Mod]()
		{
			JNIEnv* Env;

			if (InstancePtr->GetJvm()->GetEnv(reinterpret_cast<void**>(&Env), JNI_VERSION_1_8) != JNI_EDETACHED) {
				return;
			}

			if (InstancePtr->GetJvm()->AttachCurrentThreadAsDaemon(reinterpret_cast<void**>(&Env), nullptr) != JNI_OK) {
				return;
			}

			EnvRegister::RegisterThreadEnv(Env);

			while (!INSTANCE_DESTRUCT && INITIALIZING)
			{
				Sleep(1000);
			}

			GameClasses ThreadClasses;
			while (!INSTANCE_DESTRUCT && !ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses))
			{
				Sleep(1000);
			}

			while (!INSTANCE_DESTRUCT)
			{
				// jni frame soon 

				Env->PushLocalFrame(100);
				Mod->Run(Env);
				Env->PopLocalFrame(NULL);

				Sleep(1);
			}

			EnvRegister::DestroyThreadEnv();
			//ClassLoaderFactory.DeleteInstance();
			InstancePtr->GetJvm()->DetachCurrentThread();
		});

		ThreadList.push_back(std::move(ModuleThread));
	}

	auto SpotifyThread = std::thread([]()
	{
		while (INITIALIZING)
		{
			Sleep(1000);
		}

		while (!INSTANCE_DESTRUCT)
		{
			SpotifyHandlerPtr->InfoThread();
			Sleep(1000);
		}
	});

	ThreadList.push_back(std::move(SpotifyThread));
}

void ModuleManager::DestroyModules()
{
	for (auto& [Name, Mod] : ModuleList)
	{
		delete Mod;
	}

	ModuleList.clear();
}

void ModuleManager::DestroyThreads()
{
	for (auto& ModuleThread : ThreadList)
	{
		if (ModuleThread.joinable())
		{
			ModuleThread.join();
		}
	}

	for (auto& [Name, Mod] : ThreadedModuleList)
	{
		delete Mod;
	}

	ThreadedModuleList.clear();
}

std::unordered_map<std::string, Module*>  ModuleManager::ModuleList;
std::vector<std::thread> ModuleManager::ThreadList;
std::unordered_map<std::string, Module*>  ModuleManager::ThreadedModuleList;