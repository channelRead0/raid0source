#include "pch.h"
#include "raid0.hpp"

#include "hook/hook.hpp"
#include "hook/javahook.hpp"
#include "hook/detours/onswapbuffers.hpp"

#include "hook/detours/wglswapbuffers.hpp"
#include "hook/detours/strictmathatan2.hpp"
#include "hook/detours/onwindowsdisplayupdate.hpp"
#include "hook/detours/java/addtosendqueue.hpp"
#include "hook/detours/java/attacktargetentitywithcurrentItem.hpp"
#include "hook/detours/java/channelread0.hpp"
#include "hook/detours/java/clickmouse.hpp"
#include "hook/detours/java/swingitem.hpp"
#include "hook/detours/java/clickblock.hpp"
#include "hook/detours/java/handlestatusupdate.hpp"
#include "hook/detours/java/handleentityvelocity.hpp"
#include "hook/detours/java/attackentity.hpp"
#include "hook/detours/java/getmouseover.hpp"
#include "hook/detours/java/runtick.hpp"
#include "hook/detours/java/updatecameraandrender.hpp"
#include "hook/detours/java/renderworldpass.hpp"
#include "hook/detours/java/loadworld.hpp"

#include "src/sdk/internal/gameclass/gameclass.hpp"
#include "module/module.hpp"
#include "module/modules/render/render.hpp"
#include "module/client/altmanager.hpp"
#include "sdk/internal/register/envregister.hpp"

Molly::Molly() = default;

bool Molly::AttachJNI() const
{
	jsize JvmCount = 0;
	if (JNI_GetCreatedJavaVMs(&JvmPtrM, 1, &JvmCount) != JNI_OK || JvmCount == 0)
	{
		return false;
	}

	if (JvmPtrM->GetEnv(reinterpret_cast<void**>(&JniEnvPtrM), JNI_VERSION_1_8) != JNI_EDETACHED) {
		return false;
	}

	if (JvmPtrM->AttachCurrentThread(reinterpret_cast<void**>(&JniEnvPtrM), nullptr) != JNI_OK) {
		return false;
	}

	if (JvmPtrM->GetEnv(reinterpret_cast<void**>(&JvmtiPtrM), JVMTI_VERSION_1_2) != JNI_OK) {
		return false;
	}

	EnvRegister::RegisterThreadEnv(JniEnvPtrM);

	return true;
}

bool Molly::DetachJNI() const
{
	EnvRegister::DestroyThreadEnv();

	JvmPtrM->DetachCurrentThread();
	JvmtiPtrM->DisposeEnvironment();

	Sleep(100);

	JvmPtrM = nullptr;
	JniEnvPtrM = nullptr;
	JvmtiPtrM = nullptr;

	return true;
}

bool Molly::Authenticate() const
{
	//char KeyBuffer[33];
	//const auto LogfileName = L"a52b23c1ab16d3765b85fd9881edba68.csa";

	//if (!FileExistsInAppData(LogfileName)) {
	//	abort();
	//}

	//const auto LogPath = GetAppDataFilePath(LogfileName);
	//const auto LogFileContent = ReadFileContent(LogPath);

	//if (LogFileContent.size() != 32)
	//{
	//	abort();
	//	assert(false);
	//	for (long long int i = 0; ++i; (&i)[i] = i);
	//	return false;
	//}

	//const auto Auth = std::make_shared<::Authentication>(LogFileContent);
	//if (Auth->Authenticate() == LOGGED_IN) 
	//{
	//	//InstancePtr->DoIPC();
	//	return true;
	//}

	//abort();
	//assert(false);
	//for (long long int i = 0; ++i; (&i)[i] = i);
	//return false;
	return false;
}

bool Molly::InitializeGame() const
{
#ifdef SHOW_CONSOLE
	std::cout << "Waiting for JVM... " << std::endl;
#endif
	while (GetModuleHandleA("jvm.dll") == 0) {
		Sleep(500);
	}

#ifdef SHOW_CONSOLE
	std::cout << "Waiting for OpenGL... " << std::endl;
#endif
	while (GetModuleHandleA("opengl32.dll") == 0) {
		Sleep(500);
	}

#ifdef SHOW_CONSOLE
	std::cout << "Waiting for WS2... " << std::endl;
#endif
	while (GetModuleHandleA("Ws2_32.dll") == 0) {
		Sleep(500);
	}
	
	if (!ClassLoaderPtr->GetClass(JniEnvPtrM, "net/minecraft/client/Minecraft"))
	{
#ifdef SHOW_CONSOLE
		std::cerr << "Failed to find Minecraft!" << std::endl;
#endif
		return false;
	}

	const auto ProcessArgs = GetCommandLineA();

	const bool IsLunar = std::strstr(ProcessArgs, ".lunarclient") != nullptr;
	const bool Is1_8 = (std::strstr(ProcessArgs, "1.8") != nullptr) || (std::strstr(ProcessArgs, "1_8") != nullptr);
	const bool Is1_7 = (std::strstr(ProcessArgs, "1.7") != nullptr) || (std::strstr(ProcessArgs, "1_7") != nullptr);

	if (IsLunar && Is1_8) 
	{
		GAME_VER = LUNAR_1_8;
		return true;
	}

	if (IsLunar && Is1_7)
	{
		GAME_VER = LUNAR_1_7;
		return true;
	}

	return false;
}

void Molly::DestroyGame() const
{
	for (const auto& CachedClass : Cache::ClassCache)
	{
		JvmtiPtrM->Deallocate(reinterpret_cast<unsigned char*>(CachedClass.second.ClassFieldsPointer));
		JvmtiPtrM->Deallocate(reinterpret_cast<unsigned char*>(CachedClass.second.ClassMethodsPointer));

		/*delete CachedClass.second.ClassMethodsPointer;
		delete CachedClass.second.ClassFieldsPointer;*/

#ifdef SHOW_CONSOLE
		std::cout << "Deallocated " << CachedClass.first << std::endl;
#endif
	}


#ifdef SHOW_CONSOLE
	std::cout << "Dangling ENV count: " << RegisteredEnv::RegisteredEnv.size() << std::endl;
#endif

	RegisteredEnv::RegisteredEnv.clear();
}

void Molly::DoIPC() const
{
	static bool ThreadStarted = false;

	std::thread IPCThread([]()
	{
		DWORD lastcheck = GetTickCount64();
		ClientData* DataBuffer = nullptr;

		bool DataBufferFound = false;
		HANDLE DataBufferHandle = nullptr;

		while (!INSTANCE_DESTRUCT)
		{
		/*	while (!DataBuffer->LLLLC)
			{
				if (GetTickCount64() - lastcheck > 20000)
				{
					INSTANCE_DESTRUCT = true;
					return;
				}
				Sleep(1);
			}

			if (DataBuffer->LLLLC)
			{
				std::cout << "Companion is working" << std::endl;
			}*/

			if (!DataBufferFound)
			{
				DataBufferHandle = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, "pagefile32");
				DataBuffer = static_cast<ClientData*>(MapViewOfFile(DataBufferHandle, FILE_MAP_ALL_ACCESS, 0, 0, 0));
				DataBufferFound = true;
			}

			DataBuffer->LLW = GetTickCount64(); DataBuffer->LLLLW = INSTANCE_DESTRUCT; DataBuffer->LLLLI = true;

		/*	std::cout << "Buffer $>" << DataBuffer << std::endl;
			std::cout << (GetTickCount64() - DataBuffer->LLLW) << std::endl;*/

			static bool Once = false;
			if (GetTickCount64() - DataBuffer->LLLW > 15000)
			{
				if (!Once)
				{
					Once = true;
				}
				else {
					INSTANCE_DESTRUCT = true; 

					if (DataBufferHandle)
					{
						CloseHandle(DataBufferHandle);
						UnmapViewOfFile(DataBuffer);
					}
					return;
				}

				/*
				INSTANCE_DESTRUCT = true;
				return;*/
			}

			Sleep(2000);
		}

		DataBuffer->LLLLW = INSTANCE_DESTRUCT;

		if (DataBufferHandle)
		{
			CloseHandle(DataBufferHandle);
			UnmapViewOfFile(DataBuffer);
		}
	});

	if (!ThreadStarted)
	{
		IPCThread.detach();
		ThreadStarted = true;
	}
	
}

JavaVM* Molly::GetJvm() const
{
	return JvmPtrM;
}

JNIEnv* Molly::GetJniEnv() const
{
	return JniEnvPtrM;
}

jvmtiEnv* Molly::GetJvmtiEnv() const
{
	return JvmtiPtrM;
}

void Molly::CreateModules() const
{
	ModuleManager::RegisterModules();
	ModuleManager::RegisterThreads();
}

void Molly::DestroyModules() const
{
	ModuleManager::DestroyModules();
	ModuleManager::DestroyThreads();
}
#include "src/cheat/drawing/glowrenderer.hpp"
void Molly::CreateHooks() const
{
	const auto OpenGL = GetModuleHandleA("opengl32.dll");
	const auto Lwjgl64 = GetModuleHandleA("lwjgl64.dll");
	const auto Java = GetModuleHandleA("java.dll");
	const auto WglSwapBuffersProc = GetProcAddress(OpenGL, "wglSwapBuffers");
	const auto OnSwapBuffersProc = GetProcAddress(Lwjgl64, "Java_org_lwjgl_opengl_WindowsContextImplementation_nSwapBuffers");
	const auto StrictMathAtan2Proc = GetProcAddress(Java, "Java_java_lang_StrictMath_atan2");
	const auto WindowsDisplayUpdateProc = GetProcAddress(Lwjgl64, "Java_org_lwjgl_opengl_WindowsDisplay_nUpdate");

	auto MinecraftClass = GameClass(JniEnvPtrM, "net/minecraft/client/Minecraft");
	auto NetHandlerPlayClientClass = GameClass(JniEnvPtrM, "net/minecraft/client/network/NetHandlerPlayClient");
	auto EntityPlayerClass = GameClass(JniEnvPtrM, "net/minecraft/entity/player/EntityPlayer");
	auto EntityClass = GameClass(JniEnvPtrM, "net/minecraft/entity/Entity");
	auto EntityLivingBaseClass = GameClass(JniEnvPtrM, "net/minecraft/entity/EntityLivingBase");
	auto EntityRendererClass = GameClass(JniEnvPtrM, "net/minecraft/client/renderer/EntityRenderer");
	auto NetworkManagerClass = GameClass(JniEnvPtrM, "net/minecraft/network/NetworkManager");
	auto RenderLivingEntityClass = GameClass(JniEnvPtrM, "net/minecraft/client/renderer/entity/RendererLivingEntity");
	auto C00PacketLoginStartClass = GameClass(JniEnvPtrM, "net/minecraft/network/login/client/C00PacketLoginStart");
	auto ModelBipedClass = GameClass(JniEnvPtrM, "net/minecraft/client/model/ModelBiped");
	auto PlayerControllerClass = GameClass(JniEnvPtrM, "net/minecraft/client/multiplayer/PlayerControllerMP");
	auto EntityPlayerSPClass = GameClass(JniEnvPtrM, "net/minecraft/client/entity/EntityPlayerSP");

	const auto SetRotationAnglesMid = ModelBipedClass.RegisterAndFetchMethodID("setRotationAngles");
	const auto AddToSendQueueMid = NetHandlerPlayClientClass.RegisterAndFetchMethodID("addToSendQueue");
	const auto HandleEntityVelocityMid = NetHandlerPlayClientClass.RegisterAndFetchMethodID("handleEntityVelocity");
	const auto SetPositionAndRotation2Mid = EntityClass.RegisterAndFetchMethodID("setPositionAndRotation2");
	const auto SetAnglesMid = EntityClass.RegisterAndFetchMethodID("setAngles");
	const auto AttackTargetEntityWithCurrentItemMid = EntityPlayerClass.RegisterAndFetchMethodID("attackTargetEntityWithCurrentItem");
	const auto RunTickMid = MinecraftClass.RegisterAndFetchMethodID("runTick");
	const auto ChannelRead0Mid = NetworkManagerClass.RegisterAndFetchMethodID("channelRead0");
	const auto SwingItemMid = EntityPlayerSPClass.RegisterAndFetchMethodID("swingItem");
	const auto AttackEntityMid = JniEnvPtrM->GetMethodID(PlayerControllerClass.GetClass(), "attackEntity", "(Lnet/minecraft/entity/player/EntityPlayer;Lnet/minecraft/entity/Entity;)V");
	const auto ClickBlockMid = JniEnvPtrM->GetMethodID(PlayerControllerClass.GetClass(), "clickBlock", "(Lnet/minecraft/util/BlockPos;Lnet/minecraft/util/EnumFacing;)Z");
	const auto GetMouseOverMid = EntityRendererClass.RegisterAndFetchMethodID("getMouseOver");
	const auto RenderNameMid = RenderLivingEntityClass.RegisterAndFetchMethodID("renderName");
	const auto UpdateCameraAndRenderMid = EntityRendererClass.RegisterAndFetchMethodID("updateCameraAndRender");
	const auto HandleStatusUpdateMid = EntityLivingBaseClass.RegisterAndFetchMethodID("handleStatusUpdate");
	const auto LoadWorldMid = MinecraftClass.RegisterAndFetchMethodID("loadWorld");
	const auto RenderWorldPassMid = EntityRendererClass.RegisterAndFetchMethodID("renderWorldPass");

	const auto SendPacketMid = JniEnvPtrM->GetMethodID(NetworkManagerClass.GetClass(), "sendPacket", "(Lnet/minecraft/network/Packet;)V");

	const auto C00InitializerMid = JniEnvPtrM->GetMethodID(C00PacketLoginStartClass.GetClass(), "<init>", "(Lcom/mojang/authlib/GameProfile;)V");

	if (!HookManager->CreateHook(ChannelRead0Mid, ChannelRead0::ChannelRead0Detour))
	{
#ifdef SHOW_CONSOLE
		std::cerr << "Failed to hook 2/11\n";
#endif
	}

	if (!HookManager->CreateHook(RenderWorldPassMid, RenderWorldPass::RenderWorldPassDetour))
	{
#ifdef SHOW_CONSOLE
		std::cerr << "Failed to hook 2/11\n";
#endif
	}

	if (!HookManager->CreateHook(HandleStatusUpdateMid, HandleStatusUpdate::HandleStatusUpdateDetour))
	{
#ifdef SHOW_CONSOLE
		std::cerr << "Failed to hook 2/11\n";
#endif
	}

	if (!HookManager->CreateHook(AttackTargetEntityWithCurrentItemMid, AttackTargetEntityWithCurrentItem::AttackTargetEntityWithCurrentItemDetour))
	{
#ifdef SHOW_CONSOLE
		std::cerr << "Failed to hook 1/11\n";
#endif
	}

	if (!HookManager->CreateHook(LoadWorldMid, LoadWorld::LoadWorldDetour))
	{
#ifdef SHOW_CONSOLE
		std::cerr << "Failed to hook 3/11\n";
#endif
	}

	if (!HookManager->CreateHook(AddToSendQueueMid, AddToSendQueue::AddToSendQueueDetour)) 
	{
#ifdef SHOW_CONSOLE
		std::cerr << "Failed to hook 5/11\n";
#endif
	}

	if (!HookManager->CreateHook(RunTickMid, RunTick::RunTickDetour)) 
	{
#ifdef SHOW_CONSOLE
		std::cerr << "Failed to hook 6/11\n";
#endif
	}

	if (!HookManager->CreateHook(C00InitializerMid, AltManager::SendPacketDetour))
	{
#ifdef SHOW_CONSOLE
		std::cerr << "Failed to hook 7/11\n";
#endif
	}
	if (!HookManager->CreateHook(RenderNameMid, Render::RenderName))
	{
#ifdef SHOW_CONSOLE
		std::cerr << "Failed to hook 7/11\n";
#endif
	}

	if (!HookManager->CreateHook(SetAnglesMid, UpdateCameraAndRender::UpdateCameraAndRenderDetour))
	{
#ifdef SHOW_CONSOLE
		std::cerr << "Failed to hook 7/11\n";
#endif
	}

	if (!APIHookManager->CreateHook(WglSwapBuffersProc, &WglSwapBuffersHook::DetourWglSwapBuffers, &WglSwapBuffersOriginal)) 
	{
#ifdef SHOW_CONSOLE
		std::cerr << "Failed to hook 8/11\n";
#endif
	}

	if (!APIHookManager->CreateHook(StrictMathAtan2Proc, &StrictMathAtan2::StrictMathAtan2Detour, &StrictMathAtan2::OriginalStrictMathAtan2)) 
	{
#ifdef SHOW_CONSOLE
		std::cerr << "Failed to hook 9/11\n";
#endif
	}

	if (!APIHookManager->CreateHook(OnSwapBuffersProc, &OnSwapBuffersHook::SwapBuffersDetour, &OnSwapBuffersHook::OriginalSwapBuffers))
	{
#ifdef SHOW_CONSOLE
		std::cerr << "Failed to hook 11/11\n";
#endif
	}
}

void Molly::DeleteHooks() const
{
#ifdef SHOW_CONSOLE
	std::cout << "Deleting JavaHook" << std::endl;
#endif

	HookManager->DestroyHook();
	APIHookManager->DestroyHook();
}

Molly::~Molly() = default;

JavaVM* JvmPtrM;
JNIEnv* JniEnvPtrM;
jvmtiEnv* JvmtiPtrM;
std::unique_ptr<Molly> InstancePtr;
std::unique_ptr<ClassLoader> ClassLoaderPtr;