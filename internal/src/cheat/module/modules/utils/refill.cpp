#include "pch.h"
#include "refill.hpp"

void Refill::Run(JNIEnv* Env)
{
	if (!this->GetState() || UI_OPEN) {
		return;
	}

	GameClasses ThreadClasses;
	if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses)) {
		return;
	}

	//NEXT UPDATE TODO:
	/*if (RefillMode == AUTOMATICALLY_OPEN)
	{
		static bool KeyBindPressed = false;

		const auto Module = ModuleManager::GetModuleByName("Refill");

		if (GetAsyncKeyState(Module->GetKey()) & 0x8000)
		{
			if (!KeyBindPressed)
			{
				KeyBindPressed = true;
				goto DoRefill;
			}
		}
		else {
			KeyBindPressed = false;
		}

		return;

	}

	DoRefill:*/

	const auto& LocalPlayer = ThreadClasses.Player;
	const auto& World = ThreadClasses.World;
	const auto& Minecraft = ThreadClasses.Minecraft;
	const auto& PlayerController = Minecraft->GetPlayerControllerPtr(Env);
	const auto& CurrentContainer = LocalPlayer->GetOpenContainerPtr(Env);
	const auto& CurrentScreen = Minecraft->GetCurrentScreenPtr(Env);
	const auto& Inventory = LocalPlayer->GetInventoryPlayerPtr(Env);

	if (RefillMode == ON_OPEN_INVENTORY && !CurrentScreen->IsInInventory(Env)) {
		return;
	}

	auto FreeSlotCount = 0;

	for (int SlotIndex = 0; SlotIndex < 9; SlotIndex++)
	{
		const auto SlotItemStack = Inventory->ItemStackInSlot(Env, SlotIndex);

		if (!SlotItemStack) {
			FreeSlotCount++;
		}

		if (!SlotItemStack->IsValid(Env)) {
			FreeSlotCount++;
		}
	}

	if (FreeSlotCount == 0) {
		return;
	}

	bool WindowSpawned = false;
	if (RefillMode == AUTOMATICALLY_OPEN && !CurrentScreen->IsInMenu())
	{
		const auto GuiInventory = std::make_shared<GuiScreen>(Env)->NewGuiInventory(Env, LocalPlayer.get());

		if (!GuiInventory) {
			return;
		}

		Minecraft->DisplayGuiScreen(Env, GuiInventory.get());
		WindowSpawned = true;
		/*Inventory->OpenInventory(Env, LocalPlayer->GetInstanceObject());*/
	}

	auto CheckWindowSanity = [Minecraft, Env]() -> bool
	{
		const auto CurrentScreen = Minecraft->GetCurrentScreenPtr(Env);

		return CurrentScreen->IsInInventory(Env);
	};

	std::vector<int> Slots;
	for (int SlotIndex = 9; SlotIndex < 36; SlotIndex++)
	{
		const auto Slot = CurrentContainer->GetSlot(Env, SlotIndex);
		const auto StackInSlot = Slot->GetItemStack(Env);

		if (!StackInSlot) {
			continue;
		}

		if (!StackInSlot->IsValid(Env)) {
			continue;
		}

		const auto ItemInSlot = StackInSlot->GetItemPtr(Env);

		if (!ItemInSlot) {
			continue;
		}
		
		const auto PotionItem = std::make_shared<ItemPotion>(Env, ItemInSlot->GetInstanceObject());
		const auto PrimaryEffect = PotionItem->GetPrimaryPotionEffect(Env, StackInSlot->GetInstanceObject());
		
		if (/*PotionItem->ContainsEffectFromID(Env, StackInSlot->GetInstanceObject(), 12) || */PotionItem->ContainsEffectFromID(Env, StackInSlot->GetInstanceObject(), 6) /*|| PotionItem->ContainsEffectFromID(Env, StackInSlot->GetInstanceObject(), 18)*/) {
			Slots.emplace_back(SlotIndex);
		}
	}

	if (Slots.size() == 0) 
	{
		if (WindowSpawned) {
			LocalPlayer->CloseScreen(Env);
		}

		return;
	}

	if (RandomSlots) {
		std::ranges::shuffle(Slots, std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));
	}

	Sleep(Random.GenerateInt(StartDelay, StartDelay + 20));

	if (!CheckWindowSanity()) {
		return;
	}

	auto Index = 0;
	for (auto Slot : Slots)
	{
		if (Index == FreeSlotCount) {
			break;
		}

		if (!CheckWindowSanity()) {
			return;
		}

		PlayerController->ClickWindow(Env, CurrentContainer->GetWindowID(Env), Slot, 0, 1, LocalPlayer.get());
		Sleep(Random.GenerateInt(RefillDelay, RefillDelay + 10));
		Index++;
	}

	if (CloseOnSteal && CheckWindowSanity())
	{
		Sleep(Random.GenerateInt(StartDelay, StartDelay + 20));
		LocalPlayer->CloseScreen(Env);
	}

	if (DisableOnSteal || RefillMode == AUTOMATICALLY_OPEN) {
		this->Disable();
	}
}