#include "pch.h"
#include "cheststealer.hpp"

int decidedTimer = 0;

bool hideChestGui, allowMouseInput, closeAfterContainer;

bool gotItems;
int ticksInChest;

bool lastInChest;
void ChestStealer::OnTick(JNIEnv* Env)
{
	//GameClasses ThreadClasses;
	//if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses)) {
	//	return;
	//}

	//const auto& LocalPlayer = ThreadClasses.Player;
	//const auto& World = ThreadClasses.World;
	//const auto& Minecraft = ThreadClasses.Minecraft;
	//const auto& CurrentScreen = Minecraft->GetCurrentScreenPtr(Env);

	//static std::shared_ptr<CTimer> StartTimer = std::make_shared<CTimer>();
	//static std::shared_ptr<CTimer> Timer = std::make_shared<CTimer>();

	////if (mc.thePlayer.ticksExisted <= 60) return;
	//if (!lastInChest) StartTimer->Reset();
	//lastInChest = CurrentScreen->IsInChest(Env);

	//if (Timer == nullptr || StartTimer == nullptr) {
	//	std::cout << "NULL" << std::endl;
	//}

	//if (CurrentScreen->IsInChest(Env)) 
	//{
	//	/*if (hideGui.isEnabled()) {
	//		hideChestGui = true;
	//	}*/

	//	/*if (mouseInput.isEnabled()) {
	//		allowMouseInput = true;
	//	}*/

	//	/*if (chestName.isEnabled()) {
	//		final String name = ((GuiChest)mc.currentScreen).lowerChestInventory.getDisplayName().getUnformattedText();

	//		if (!name.toLowerCase().contains("chest")) return;
	//	}*/

	//	/*if (hideGui.isEnabled()) {
	//		final ScaledResolution SR = new ScaledResolution(mc);
	//		final String t = "Stealing chest... Press " + Keyboard.getKeyName(mc.gameSettings.keyBindInventory.getKeyCode()) + " to close the chest";
	//		int o;

	//		o = ticksInChest * 50;

	//		if (o > 255)
	//			o = 255;

	//		comfortaa.drawStringWithShadow(t, SR.getScaledWidth() / 2f - comfortaa.getWidth(t) / 2f, SR.getScaledHeight() - 80, ThemeUtil.getThemeColorInt(ThemeType.GENERAL));
	//	}*/

	//	if (decidedTimer == 0) {
	//		int delayFirst = (int)std::floor((((MinDelay) < (MaxDelay)) ? (MinDelay) : (MaxDelay)));
	//		int delaySecond = (int)std::ceil((((MinDelay) > (MaxDelay)) ? (MinDelay) : (MaxDelay)));

	//		decidedTimer = Random.GenerateInt(delayFirst, delaySecond);
	//	}

	//	if (Timer->HasPassed(decidedTimer))
	//	{
	//		auto ContainerChest = LocalPlayer->GetOpenContainerPtr(Env);
	//		auto Size = ContainerChest->InventorySlots(Env)->Size(Env);
	//		std::cout << Size << std::endl;
	//		
	//		for (int i = 0; i < Size; i++) {
	//			std::cout << "C2" << std::endl;
	//			auto stack = ContainerChest->GetLowerChestInventory(Env)->GetStackInSlot(Env, i);
	//			if (stack /*&&*/ /*(itemWhitelisted(stack) && !stealTrashItems.isEnabled())*/) 
	//			{
	//				std::cout << "C3" << std::endl;
	//				Minecraft->GetPlayerControllerPtr(Env)->ClickWindow(Env, ContainerChest->GetWindowID(Env), i, 0, 1, LocalPlayer.get());
	//				Timer->Reset();

	//				int delayFirst = (int)std::floor((((MinDelay) < (MaxDelay)) ? (MinDelay) : (MaxDelay)));
	//				int delaySecond = (int)std::ceil((((MinDelay) > (MaxDelay)) ? (MinDelay) : (MaxDelay)));

	//				decidedTimer = Random.GenerateInt(delayFirst, delaySecond);

	//				gotItems = true;
	//				return;
	//			}
	//		}

	//		if (CloseOnSteal && (gotItems || ticksInChest > 10)) {
	//			//mc.thePlayer.closeScreen();
	//			LocalPlayer->CloseScreen(Env);
	//		}
	//	}
	//}
	//else {
	//	hideChestGui = false;
	//	allowMouseInput = false;
	//}
	/*if (!this->GetState() || UI_OPEN)
	{
		Sleep(5);
		return;
	}

	GameClasses ThreadClasses;
	if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses))
	{
		Sleep(1);
		return;
	}

	const auto& LocalPlayer = ThreadClasses.Player;
	const auto& World = ThreadClasses.World;
	const auto& Minecraft = ThreadClasses.Minecraft;
	const auto& PlayerController = Minecraft->GetPlayerControllerPtr(Env);
	const auto& CurrentContainer = LocalPlayer->GetOpenContainerPtr(Env);
	const auto& CurrentScreen = Minecraft->GetCurrentScreenPtr(Env);
	const auto& Inventory = LocalPlayer->GetInventoryPlayerPtr(Env);

	if (!CurrentScreen->IsInInventory(Env))
	{
		Sleep(10);
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

	if (FreeSlotCount == 0)
	{
		Sleep(10);
		return;
	}

	auto CheckWindowSanity = [Minecraft, Env]() -> bool
	{
		const auto CurrentScreen = Minecraft->GetCurrentScreenPtr(Env);

		return CurrentScreen->IsInChest(Env);
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

		const auto ItemStackName = StackInSlot->GetDisplayName(Env);
		if (ItemStackName.contains("Splash") && ItemStackName.contains("Healing")) {
			Slots.push_back(SlotIndex);
		}
	}

	if (Slots.size() == 0)
	{
		Sleep(1);
		return;
	}

	if (RandomSlots) {
		std::ranges::shuffle(Slots, std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));
	}

	Sleep(Random.GenerateInt(StealDelay, StealDelay + 20));

	if (!CheckWindowSanity())
	{
		Sleep(1);
		return;
	}

	auto Index = 0;
	for (auto Slot : Slots)
	{
		if (Index == FreeSlotCount) {
			break;
		}

		if (!CheckWindowSanity())
		{
			Sleep(1);
			return;
		}

		PlayerController->ClickWindow(Env, CurrentContainer->GetWindowID(Env), Slot, 0, 1, LocalPlayer.get());
		Sleep(Random.GenerateInt(StealDelay, StealDelay + 10));
		Index++;
	}

	if (CloseOnSteal && CheckWindowSanity())
	{
		Sleep(Random.GenerateInt(StealDelay, StealDelay + 20));
		LocalPlayer->CloseScreen(Env);
	}

	if (DisableOnSteal) {
		this->Disable();
	}

	Sleep(1);*/

	if (!this->GetState() || UI_OPEN) {
		return;
	}

	GameClasses ThreadClasses;
	if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses)) {
		return;
	}

	const auto& LocalPlayer = ThreadClasses.Player;
	const auto& World = ThreadClasses.World;
	const auto& Minecraft = ThreadClasses.Minecraft;
	const auto& PlayerController = Minecraft->GetPlayerControllerPtr(Env);
	const auto& GameSettings = Minecraft->GetGameSettingsPtr(Env);
	const auto& KeyBindInventory = GameSettings->GetKeyBindInventoryPtr(Env);

	static std::shared_ptr<CTimer> TimerA = std::make_shared<CTimer>();
	static DWORD ChestOpenDelay = StealDelay + Random.GenerateInt(-(StealDelay * 0.20), StealDelay * 0.20);

	if (!TimerA->HasPassed(ChestOpenDelay)) {
		return;
	}

	ChestOpenDelay = StealDelay + Random.GenerateInt(-(StealDelay * 0.20), StealDelay * 0.20);

	const auto& OpenContainer = LocalPlayer->GetOpenContainerPtr(Env);

	if (OpenContainer->GetInstanceObject() == nullptr || !OpenContainer->IsChest(Env)) {
		return;
	}

	const auto& LowerChestInventory = OpenContainer->GetLowerChestInventory(Env);

	std::vector<int> Slots;
	for (int SlotIndex = 0; SlotIndex < LowerChestInventory->GetSizeInventory(Env); SlotIndex++)
	{
		const auto StackInSlot = LowerChestInventory->GetStackInSlot(Env, SlotIndex);
		if (!StackInSlot) {
			continue;
		}

		if (!StackInSlot->IsValid(Env)) {
			continue;
		}

		Slots.emplace_back(SlotIndex);
	}

	if (RandomSlots) {
		std::ranges::shuffle(Slots, std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));
	}

	auto ContainsSpace = [Env, LocalPlayer]() -> bool
	{
		const auto& Inventory = LocalPlayer->GetInventoryPlayerPtr(Env);

		//bool ContainsSpace = false;
		for (int SlotIndex = 0; SlotIndex < 36; SlotIndex++)
		{
			const auto SlotItemStack = Inventory->ItemStackInSlot(Env, SlotIndex);

			if (!SlotItemStack) {
				return true;
			}

			if (!SlotItemStack->IsValid(Env)) {
				return true;
			}
		}

		return false;
	};

	auto ContainsItemsInChest = [Env, LocalPlayer]() -> bool
	{
		/*const auto& LowerChestInventory = LocalPlayer->GetOpenContainerPtr(Env)->GetLowerChestInventory(Env);

		if (!LowerChestInventory) {
			return false;
		}*/

		for (int SlotIndex = 0; SlotIndex < LocalPlayer->GetOpenContainerPtr(Env)->GetLowerChestInventory(Env)->GetSizeInventory(Env); SlotIndex++)
		{
			const auto StackInSlot = LocalPlayer->GetOpenContainerPtr(Env)->GetLowerChestInventory(Env)->GetStackInSlot(Env, SlotIndex);
			if (!StackInSlot) {
				continue;
			}

			if (!StackInSlot->IsValid(Env)) {
				continue;
			}

			return true;
		}

		return false;
	};

	for (auto SlotIndex : Slots)
	{
		const auto& StackInSlot = LowerChestInventory->GetStackInSlot(Env, SlotIndex);

		if (StackInSlot->GetInstanceObject() == nullptr) {
			continue;
		}

		static std::shared_ptr<CTimer> TimerB = std::make_shared<CTimer>();
		static DWORD ItemStealDelay = StealDelay + Random.GenerateInt(-(StealDelay * 0.20), StealDelay * 0.20);

		if (!TimerB->HasPassed(ItemStealDelay)) {
			continue;
		}

		ItemStealDelay = StealDelay + Random.GenerateInt(-(StealDelay * 0.20), StealDelay * 0.20);

		if (!ContainsSpace())
		{
			if (CloseOnSteal) 
			{
				auto CloseDelay = StealDelay + Random.GenerateInt(-(StealDelay * 0.20), StealDelay * 0.20);

				Sleep(CloseDelay);

				LocalPlayer->CloseScreen(Env);
			}

			if (DisableOnSteal) {
				this->Disable();
			}

			return;
		}

		PlayerController->ClickWindow(Env, OpenContainer->GetWindowID(Env), SlotIndex, 0, 1, LocalPlayer.get());
		TimerB->Reset();
	}

	const auto ChestStolen = !ContainsItemsInChest();

	static std::shared_ptr<CTimer> TimerC = std::make_shared<CTimer>();
	static DWORD CloseDelay = StealDelay + Random.GenerateInt(-(StealDelay * 0.20), StealDelay * 0.20);

	//if (CloseOnSteal && ChestStolen) 
	//{
	//	if (!TimerC->HasPassed(CloseDelay)) {
	//		return;
	//	}

	//	CloseDelay = StealDelay + Random.GenerateInt(-(StealDelay * 0.20), StealDelay * 0.20);

	//	//THIS CRASHES WHEN THE CHEST IS FULL
	//	//LocalPlayer->CloseScreen(Env);
	//	//todo: Fix this goofy ass code 
	//	/*KeyBindInventory->SetPressed(Env, true);
	//	Sleep(10);
	//	KeyBindInventory->SetPressed(Env, false);*/
	//}

	if (DisableOnSteal /*&& ChestStolen*/) {
		this->Disable();
	}
	/*if (CloseOnSteal)
	{
		Sleep(Random.GenerateInt(StartDelay, StartDelay + 50));

		LocalPlayer->CloseScreen(Env);
	}*/


	TimerA->Reset();

	//CurrentContainer->PromoteInstanceObject(Env);

	//if (!CurrentContainer)
	//{
	//	Sleep(1);
	//	return;
	//}

	//if (!CurrentContainer->IsChest(Env)) 
	//{
	//	Sleep(1);
	//	return;
	//}

	//const auto LowerInventory = CurrentContainer->GetLowerChestInventory(Env);
	//const auto LowerInventorySize = LowerInventory->GetSizeInventory(Env);
	//const auto Inventory = LocalPlayer->GetInventoryPlayerPtr(Env);

	//Inventory->PromoteInstanceObject(Env);

	///*if (TryAvoidGui) {

	//}*/

	//std::vector<int> Slots;
	//for (int SlotIndex = 0; SlotIndex < LowerInventorySize; SlotIndex++)
	//{
	//	const auto StackInSlot = LowerInventory->GetStackInSlot(Env, SlotIndex);
	//	if (!StackInSlot) {
	//		continue;
	//	}

	//	if (!StackInSlot->IsValid(Env)) {
	//		continue;
	//	}

	//	Slots.emplace_back(SlotIndex);
	//}

	//if (Slots.size() <= 0)
	//{
	//	Sleep(1);
	//	return;
	//}

	//auto CheckWindowSanity = [Minecraft, Env]() -> bool
	//{
	//	const auto CurrentScreen = Minecraft->GetCurrentScreenPtr(Env);
	//	CurrentScreen->PromoteInstanceObject(Env);

	//	return CurrentScreen->IsInChest(Env);
	//};

	//if (RandomSlots) {
	//	std::ranges::shuffle(Slots, std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));
	//}

	//Sleep(Random.GenerateInt(StartDelay, StartDelay + 50));

	//if (!CheckWindowSanity())
	//{
	//	Sleep(1);
	//	return;
	//}

	//for (auto Slot : Slots)
	//{
	//	bool ContainsSpace = false;
	//	for (int SlotIndex = 0; SlotIndex < 36; SlotIndex++)
	//	{
	//		const auto SlotItemStack = Inventory->ItemStackInSlot(Env, SlotIndex);

	//		if (!SlotItemStack) {
	//			ContainsSpace = true;
	//		}

	//		if (!SlotItemStack->IsValid(Env)) {
	//			ContainsSpace = true;
	//		}
	//	}

	//	if (!ContainsSpace) {
	//		break;
	//	}

	//	if (!CheckWindowSanity())
	//	{
	//		Sleep(1);
	//		return;
	//	}

	//	PlayerController->ClickWindow(Env, CurrentContainer->GetWindowID(Env), Slot, 0, 1, LocalPlayer.get());
	//	Sleep(Random.GenerateInt(StealDelay, StealDelay + 10));
	//}

	//if (CloseOnSteal && CheckWindowSanity())
	//{
	//	Sleep(Random.GenerateInt(StartDelay, StartDelay + 50));

	//	LocalPlayer->CloseScreen(Env);
	//}

	//if (DisableOnSteal) {
	//	this->Disable();
	//}
	//
	//Sleep(1);
}

