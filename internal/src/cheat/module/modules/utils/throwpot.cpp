#include "pch.h"
#include "throwpot.hpp"

void ThrowPot::Run(JNIEnv* Env)
{
	if (!this->GetState() || UI_OPEN) {
		return;
	}

	GameClasses ThreadClasses;
	if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses)) {
		return;
	}

	static bool KeyBindPressed = false;

	if (GetAsyncKeyState(ThrowBind) & 0x8000)
	{
		if (!KeyBindPressed)
		{
			KeyBindPressed = true;
			goto DoThrowPot;
		}
	}
	else {
		KeyBindPressed = false;
	}

	return;

	DoThrowPot:
	std::vector<int> Slots;

	const auto& LocalPlayer = ThreadClasses.Player;
	const auto& World = ThreadClasses.World;
	const auto& Minecraft = ThreadClasses.Minecraft;
	const auto& Inventory = LocalPlayer->GetInventoryPlayerPtr(Env);
	const auto& PlayerController = Minecraft->GetPlayerControllerPtr(Env);

	const auto InitialSlot = Inventory->GetCurrentSlot(Env);

	for (int SlotIndex = 0; SlotIndex < 9; SlotIndex++)
	{
		const auto SlotItemStack = Inventory->ItemStackInSlot(Env, SlotIndex);

		if (!SlotItemStack) {
			continue;
		}

		if (!SlotItemStack->IsPotion(Env)) {
			continue;
		}

		const auto PotionItem = std::make_shared<ItemPotion>(Env, SlotItemStack->GetItemPtr(Env)->GetInstanceObject());
		/*const auto PotionItem = std::make_shared<ItemPotion>(Env, SlotItemStack->GetItemPtr(Env)->GetInstanceObject());

		if (PotionItem->ContainsEffectFromID(Env, SlotItemStack->GetInstanceObject(), 6) || PotionItem->ContainsEffectFromID(Env, SlotItemStack->GetInstanceObject(), 12)) {
			Slots.emplace_back(SlotIndex);
		}*/
		if (/*PotionItem->ContainsEffectFromID(Env, SlotItemStack->GetInstanceObject(), 12) || */PotionItem->ContainsEffectFromID(Env, SlotItemStack->GetInstanceObject(), 6)/* || PotionItem->ContainsEffectFromID(Env, SlotItemStack->GetInstanceObject(), 18)*/) {
			Slots.emplace_back(SlotIndex);
		}
	}

	if (Slots.empty()) {
		return;
	}

	//std::cout << float(LocalPlayer->GetMaxHealth(Env) * (TargetHealth / 100.F)) << std::endl;

	const auto HealTarget = (LocalPlayer->GetMaxHealth(Env) * (TargetHealth / 100.F));
	const auto HealthDifference = HealTarget - LocalPlayer->GetHealth(Env);

	if (LocalPlayer->GetHealth(Env) >= HealTarget) {
		return;
	}

	if (RandomSlots) {
		std::ranges::shuffle(Slots, std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));
	}

	if (ThrowMode == DOUBLE)
	{
		if (Slots.size() == 0) {
			return;
		}

		int PotCount = static_cast<int>(std::ceil(HealthDifference / 8));

		if (PotCount == 0) {
			return;
		}

		if (Slots.size() == 1 || PotCount == 1)
		{
			const auto SlotToSwap = Slots[0];

			Sleep(SwitchDelay);
			Inventory->SetCurrentSlot(Env, SlotToSwap);

			Sleep(ThrowDelay);
			const auto ItemStack = LocalPlayer->GetHeldItemPtr(Env);
			PlayerController->SendUseItem(Env, LocalPlayer.get(), World.get(), ItemStack.get());

			Sleep(SwitchDelay);
			Inventory->SetCurrentSlot(Env, InitialSlot);
		}

		if (Slots.size() > 1 && PotCount > 1)
		{
			for (int Index = 0; Index < 2; Index++)
			{
				Sleep(SwitchDelay);
				Inventory->SetCurrentSlot(Env, Slots[Index]);

				Sleep(ThrowDelay);
				const auto ItemStack = LocalPlayer->GetHeldItemPtr(Env);
				PlayerController->SendUseItem(Env, LocalPlayer.get(), World.get(), ItemStack.get());
			}

			Sleep(SwitchDelay);
			Inventory->SetCurrentSlot(Env, InitialSlot);
		}
	}

	if (ThrowMode == DYNAMIC)
	{
		int PotCount = static_cast<int>(std::ceil(HealthDifference / 8));

		if (PotCount == 0) {
			return;
		}

		if (PotCount > Slots.size()) {
			PotCount = Slots.size();
		}

		for (int i = 0; i < PotCount; i++)
		{
			if (LocalPlayer->GetHealth(Env) >= HealTarget) {
				break;
			}

			const auto SlotToSwap = Slots[i];

			Sleep(SwitchDelay);
			Inventory->SetCurrentSlot(Env, SlotToSwap);

			Sleep(ThrowDelay);
			const auto ItemStack = LocalPlayer->GetHeldItemPtr(Env);
			PlayerController->SendUseItem(Env, LocalPlayer.get(), World.get(), ItemStack.get());
		}

		Sleep(SwitchDelay);
		Inventory->SetCurrentSlot(Env, InitialSlot);
	}

	if (ThrowMode == SINGLE)
	{
		if (Slots.size() == 0) {
			return;
		}

		const auto SlotToSwap = Slots[0];

		Sleep(SwitchDelay);
		Inventory->SetCurrentSlot(Env, SlotToSwap);

		Sleep(ThrowDelay);
		const auto ItemStack = LocalPlayer->GetHeldItemPtr(Env);
		PlayerController->SendUseItem(Env, LocalPlayer.get(), World.get(), ItemStack.get());

		Sleep(SwitchDelay);
		Inventory->SetCurrentSlot(Env, InitialSlot);
	}

	Sleep(1);
}
