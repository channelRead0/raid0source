#include "pch.h"
#include <fstream>
#include <shellapi.h>

#include "cheat/hook/hook.hpp"
#include "cheat/hook/javahook.hpp"
#include "cheat/hook/detours/wglswapbuffers.hpp"
#include "sdk/game/minecraft/minecraft.hpp"
#include "sdk/internal/register/classregister.hpp"
#include "sdk/internal/register/envregister.hpp"
#include "src/sdk/internal/gameclass/gameclass.hpp"
#include "src/cheat/gui/spotify/spotify.hpp"

void StartupRoutine()
{
	InstancePtr = std::make_unique<Molly>();
	HookManager = std::make_unique<JavaHook>();
	APIHookManager = std::make_unique<Hook>();
	ClassLoaderPtr = std::make_unique<ClassLoader>();

	////AUTH IS VERY MESSY LOL
	//if (!InstancePtr->Authenticate())
	//{
	//	abort();
	//	assert(false;);
	//	for (long long int i = 0; ++i; (&i)[i] = i);
	//}

	if (!InstancePtr->AttachJNI()) {
		return;
	}

	if (!ClassLoaderPtr->SetupClassLoader(JniEnvPtrM))
	{
		InstancePtr->DetachJNI();
		return;
	}

	if (!InstancePtr->InitializeGame()) 
	{
		InstancePtr->DetachJNI();
		return;
	}

	if (!APIHookManager->Initialize())
	{
		InstancePtr->DetachJNI();
		return;
	}


	if (!HookManager->Initialize()) 
	{
		InstancePtr->DetachJNI();
		return;
	}

	InstancePtr->CreateModules();
	Sleep(1000);
	InstancePtr->CreateHooks();

	Sleep(1000);

	INITIALIZING = false;
	//UI_OPEN = true;

	while (!INSTANCE_DESTRUCT)
	{
		//IPC HERE
		Sleep(1000);
	}

	INSTANCE_DESTRUCT = true;

	Sleep(1000);

	InstancePtr->DeleteHooks();
	InstancePtr->DestroyModules();
	ClassLoaderPtr->DestroyClassLoader(JniEnvPtrM);
	InstancePtr->DestroyGame();
	InstancePtr->DetachJNI();
}

int __stdcall Main(const HMODULE HModule)
{
//#ifdef SHOW_CONSOLE 
	AllocateConsole();
//#endif

	//std::ofstream nul("NUL");
	//std::streambuf* old_err = std::cerr.rdbuf(nul.rdbuf());

	StartupRoutine();

#ifdef SHOW_CONSOLE
	std::cout << "Main Thread Exitting" << std::endl;
#endif

	//Sleep(5000);

// SHOW_CONSOLE  
	DeAllocateConsole();
//#endif

	FreeLibraryAndExitThread(HModule, 0);

	return 0;
}

BOOL APIENTRY DllMain(const HMODULE HModule, const DWORD  ULReasonForCall, const LPVOID LPReserved)
{
	if (ULReasonForCall != DLL_PROCESS_ATTACH) {
		return TRUE;
	}

	DisableThreadLibraryCalls(HModule);

	const auto Handle = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Main), HModule, 0, nullptr);

	if (Handle != nullptr) {
		CloseHandle(Handle);
	}

	return TRUE;
}
