#include "pch.h"

#include "noitemrelease.hpp"
#include <cheat/hook/javahook.hpp>

// ngl this entire packet stuff needs a recode, for some reason its crashing
// ... after its registered 
// could be the hook? idk


// RTHIS MODULE IS CRASHING FOR SOME REASON, ITS EXACTLY THE SAME CODE FROM MY PROJECT SO I THINK IT MIGHT BE TTHE HOOK OR MAPPER
void NoItemRelease::OnPacketSend(JNIEnv* Env, const std::shared_ptr<PacketSendData>& PacketData, bool* ShouldCancel)
{
	GameClasses ThreadClasses;
	if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses)) {
		return;
	}

	const auto LocalPlayer = ThreadClasses.Player;

	if (!LocalPlayer->IsUsingItem(Env)) {
		return;
	}

	const auto HeldItem = LocalPlayer->GetHeldItemPtr(Env);

	bool IsItemWhitelisted = false;

	if (HeldItem != nullptr)
	{
		if (HeldItem->IsSword(Env)) {
			IsItemWhitelisted = true;
		}

		if (HeldItem->IsFood(Env)) {
			IsItemWhitelisted = true;
		}

		if (HeldItem->IsPotion(Env)) {
			IsItemWhitelisted = true;
		}
	}

	if (!IsItemWhitelisted) {
		return;
	}

	if (PacketData->PacketInstance->IsPlayerDiggingPacket(Env))
	{
		HookManager->SetReturnValue<uint64_t>(ShouldCancel, 0);
		*ShouldCancel = true;
		return;
	}
}
