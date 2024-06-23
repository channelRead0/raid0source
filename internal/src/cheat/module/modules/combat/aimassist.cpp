#include "pch.h"
#include "aimassist.hpp"

#include <cmath>

#include "cheat/module/modules/utils/data.hpp"
#include "cheat/module/modules/world/antibot.hpp"
#include "cheat/module/modules/world/friends.hpp"
#include "cheat/module/modules/world/teams.hpp"
#include "sdk/game/minecraft/movingobjectposition.hpp"
#include "sdk/game/minecraft/world.hpp"
#include "sdk/internal/register/classregister.hpp"


float convert_legit_value(float step, float angle, float speed)
{
	angle *= 0.15f;

	float steps = angle < 0.0f ? floorf(std::clamp(0.5f * speed, 1.f, 3.f) * angle * 5.0f) : ceilf(std::clamp(0.5f * speed, 1.f, 3.f) * angle * 5.0f);

	return steps * step * 0.15f;
};

static void Clamp90(double* Angle)
{
	if (*Angle >= 90.0)
	{
		*Angle = 89.0;
	}

	if (*Angle <= -90.0)
	{
		*Angle = -89.0;
	}
}

//
//void AimAssist::Run(JNIEnv* Env)
//{
//	if (!this->GetState() || UI_OPEN)
//	{
//		return;
//	}
//
//	if (OnlyClicking && !(GetAsyncKeyState(VK_LBUTTON) & 0x8000))
//	{
//		return;
//	}
//
//	GameClasses ThreadClasses;
//	if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses))
//	{
//		return;
//	}
//
//	const auto& LocalPlayer = ThreadClasses.Player;
//	const auto& World = ThreadClasses.World;
//	const auto& Minecraft = ThreadClasses.Minecraft;
//	const auto& MovingObjectPosition = std::make_shared<::MovingObjectPosition>(Env, Minecraft->GetObjectMouseOver(Env));
//	const auto& GameSettings = ThreadClasses.Minecraft->GetGameSettingsPtr(Env);
//	const auto& PlayerController = Minecraft->GetPlayerControllerPtr(Env);
//
//	if (!Minecraft->InGameHasFocus(Env))
//	{
//		return;
//	}
//
//	static int TargetID = 0;
//	std::shared_ptr<Player> TargetPlayer = nullptr;
//
//	const std::vector<std::shared_ptr<Player>>& PlayerList = ThreadClasses.World->GetPlayerEntitiesWithinAABBVector(Env, ThreadClasses.Player.get(), ThreadClasses.Player->GetPos(Env), AimAssist::MaximumRange);
//
//	if (PlayerList.empty())
//	{
//		return;
//	}
//
//	if (TargetingMode == SINGLE && TargetID != 0)
//	{
//		bool Found = false;
//		for (const auto& Player : PlayerList)
//		{
//			if (!Player->GetInstanceObject() || Env->IsSameObject(Player->GetInstanceObject(), NULL)) {
//				continue;
//			}
//
//			if (Player->IsDead(Env)) {
//				continue;
//			}
//
//			if (Player->GetEntityID(Env) == TargetID)
//			{
//				const auto DistanceToLocal = Player->GetDistance(Env, LocalPlayer.get());
//
//				if (DistanceToLocal > MaximumRange || DistanceToLocal < MinimumRange) {
//					continue;
//				}
//
//				TargetPlayer = Player;
//				Found = true;
//				break;
//			}
//		}
//
//		if (!Found)
//		{
//			TargetPlayer = nullptr;
//			TargetID = 0;
//		}
//	}
//
//	if (TargetingMode == SINGLE && TargetID == 0 && TargetPlayer == nullptr)
//	{
//		float BestFov = 1000.F;
//		float Health = 1000.F;
//		float Distance = MaximumRange;
//
//		for (const auto& Player : PlayerList)
//		{
//			if (Env->IsSameObject(Player->GetInstanceObject(), NULL)) {
//				continue;
//			}
//
//			if (Env->IsSameObject(LocalPlayer->GetInstanceObject(), Player->GetInstanceObject())) {
//				continue;
//			}
//
//			if (Player->IsDead(Env)) {
//				continue;
//			}
//
//			const auto PlayerName = Player->GetName(Env);
//
//			if (AntiBot::IsBot(Env, World.get(), LocalPlayer.get(), Player.get())) {
//				continue;
//			}
//
//			if (Teams::IsOnSameTeam(Env, LocalPlayer.get(), Player.get())) {
//				continue;
//			}
//
//			if (Friends::IsFriend(Player->GetEntityID(Env))) {
//				continue;
//			}
//
//			if (!AimAtInvisible && Player->IsInvisible(Env)) {
//				continue;
//			}
//
//
//			if (!AimThroughWalls && !LocalPlayer->IsEntitySeen(Env, Player.get())) {
//				continue;
//			}
//
//			const auto PlayerHealth = Player->GetHealth(Env);
//
//			if (PlayerHealth < 0.F) {
//				continue;
//			}
//
//			const auto DistanceToLocal = Player->GetDistance(Env, LocalPlayer.get());
//
//			if (DistanceToLocal > MaximumRange || DistanceToLocal < MinimumRange) {
//				continue;
//			}
//
//			if (FilteringMode == CLOSEST)
//			{
//				if (DistanceToLocal < Distance && DistanceToLocal > MinimumRange)
//				{
//					TargetID = Player->GetEntityID(Env);
//					TargetPlayer = Player;
//					Distance = DistanceToLocal;
//
//					continue;
//				}
//			}
//
//			if (FilteringMode == LOWEST_HEALTH)
//			{
//				if (PlayerHealth < Health)
//				{
//					TargetID = Player->GetEntityID(Env);
//					TargetPlayer = Player;
//					Health = PlayerHealth;
//
//					continue;
//				}
//			}
//
//			if (FilteringMode == FOV)
//			{
//				const auto FovToLocal = fabs(LocalPlayer->GetFovTo(Env, Player.get()));
//				if (FovToLocal < BestFov)
//				{
//					TargetID = Player->GetEntityID(Env);
//					TargetPlayer = Player;
//					BestFov = FovToLocal;
//
//					continue;
//				}
//			}
//		}
//	}
//
//	if (TargetingMode == SWITCH)
//	{
//		float BestFov = 1000.F;
//		float Health = 1000.F;
//		float Distance = MaximumRange;
//
//		for (const auto& Player : PlayerList)
//		{
//			if (Env->IsSameObject(Player->GetInstanceObject(), NULL)) {
//				continue;
//			}
//
//			if (Env->IsSameObject(LocalPlayer->GetInstanceObject(), Player->GetInstanceObject())) {
//				continue;
//			}
//
//			if (Player->IsDead(Env)) {
//				continue;
//			}
//
//			const auto PlayerName = Player->GetName(Env);
//
//			if (AntiBot::IsBot(Env, World.get(), LocalPlayer.get(), Player.get())) {
//				continue;
//			}
//
//			if (Teams::IsOnSameTeam(Env, LocalPlayer.get(), Player.get())) {
//				continue;
//			}
//
//			if (Friends::IsFriend(Player->GetEntityID(Env))) {
//				continue;
//			}
//
//			if (!AimAtInvisible && Player->IsInvisible(Env)) {
//				continue;
//			}
//
//			if (!AimThroughWalls && !LocalPlayer->IsEntitySeen(Env, Player.get())) {
//				continue;
//			}
//
//			const auto PlayerHealth = Player->GetHealth(Env);
//
//			if (PlayerHealth < 0.F) {
//				continue;
//			}
//
//			const auto DistanceToLocal = Player->GetDistance(Env, LocalPlayer.get());
//
//			if (DistanceToLocal > MaximumRange || DistanceToLocal < MinimumRange) {
//				continue;
//			}
//
//			if (FilteringMode == CLOSEST)
//			{
//				if (DistanceToLocal < Distance && DistanceToLocal > MinimumRange)
//				{
//					TargetPlayer = Player;
//					Distance = DistanceToLocal;
//
//					continue;
//				}
//			}
//
//			if (FilteringMode == LOWEST_HEALTH)
//			{
//				if (PlayerHealth < Health)
//				{
//					TargetPlayer = Player;
//					Health = PlayerHealth;
//
//					continue;
//				}
//			}
//
//			if (FilteringMode == FOV)
//			{
//				const auto FovToLocal = fabs(LocalPlayer->GetFovTo(Env, Player.get()));
//				if (FovToLocal < BestFov)
//				{
//					TargetPlayer = Player;
//					BestFov = FovToLocal;
//
//					//std::cout << "Best Fov: " << BestFov << std::endl;
//
//					continue;
//				}
//			}
//		}
//	}
//
//	if (TargetPlayer == nullptr)
//	{
//		return;
//	}
//
//	if (!AimAtInvisible && TargetPlayer->IsInvisible(Env))
//	{
//		return;
//	}
//
//	const auto ItemStack = LocalPlayer->GetHeldItemPtr(Env);
//
//	bool IsItemWhitelisted = false;
//
//	if (WhitelistSwords && ItemStack->IsSword(Env)) {
//		IsItemWhitelisted = true;
//	}
//
//	if (WhitelistAxes && ItemStack->IsAxe(Env)) {
//		IsItemWhitelisted = true;
//	}
//
//	if (WhitelistSticks && ItemStack->IsStick(Env)) {
//		IsItemWhitelisted = true;
//	}
//
//	if (WhitelistPickAxe && ItemStack->IsPickAxe(Env)) {
//		IsItemWhitelisted = true;
//	}
//
//	if (WhitelistFist && ItemStack->IsFist(Env)) {
//		IsItemWhitelisted = true;
//	}
//
//	if (ItemWhitelist && !IsItemWhitelisted)
//	{
//		return;
//	}
//
//	static DWORD LastBlockAimTime = 0;
//	static bool IsAimingBlock = false;
//	if (!IsAimingBlock && MovingObjectPosition->IsAimingBlock(Env))
//	{
//		IsAimingBlock = true;
//		LastBlockAimTime = GetTickCount64();
//	}
//
//	if (IsAimingBlock && !MovingObjectPosition->IsAimingBlock(Env))
//	{
//		IsAimingBlock = false;
//		LastBlockAimTime = 0;
//	}
//
//	if (BreakBlocks && (IsAimingBlock && (GetTickCount64() - LastBlockAimTime > 350) || PlayerController->IsHittingBlock(Env)))
//	{
//		return;
//	}
//
//	const auto LocalPlayerPosition = LocalPlayer->GetPos(Env);
//	const auto TargetPosition = TargetPlayer->GetPos(Env);
//
//	const auto PositionX = TargetPosition.X;
//	const auto PositionY = TargetPosition.Y /*- 2.F TargetPlayer->GetEyeHeight(Env)*/;
//	const auto PositionZ = TargetPosition.Z;
//
//	auto DistanceX = TargetPosition.X - LocalPlayerPosition.X;
//	auto DistanceZ = TargetPosition.Z - LocalPlayerPosition.Z;
//
//	auto ClosestPoint = Vector3<double>(TargetPosition);
//	double ClosestDistance = (std::numeric_limits<double>::max)();
//
//	const auto StaticPoint = Vector3<double>(TargetPosition.X, TargetPosition.Y, TargetPosition.Z);
//
//	if (AimMode == MULTIPOINT)
//	{
//		for (int Index = 0; Index <= 30; Index++)
//		{
//			const auto IteratorIndex = Index / 100.F;
//
//			const std::vector<Vector3<double>> HitboxPositions
//			{
//				{ AimMode == 0 ? TargetPosition.X - 0.30F : TargetPosition.X, TargetPosition.Y, AimMode == 0 ? TargetPosition.Z + IteratorIndex : TargetPosition.Z },
//				{ AimMode == 0 ? TargetPosition.X - 0.30F : TargetPosition.X, TargetPosition.Y, AimMode == 0 ? TargetPosition.Z - IteratorIndex : TargetPosition.Z },
//				{ AimMode == 0 ? TargetPosition.X + 0.30F : TargetPosition.X, TargetPosition.Y, AimMode == 0 ? TargetPosition.Z + IteratorIndex : TargetPosition.Z },
//				{ AimMode == 0 ? TargetPosition.X + 0.30F : TargetPosition.X, TargetPosition.Y, AimMode == 0 ? TargetPosition.Z - IteratorIndex : TargetPosition.Z },
//
//				{ AimMode == 0 ? TargetPosition.X + IteratorIndex : TargetPosition.X, TargetPosition.Y, AimMode == 0 ? TargetPosition.Z - 0.30F : TargetPosition.Z },
//				{ AimMode == 0 ? TargetPosition.X - IteratorIndex : TargetPosition.X, TargetPosition.Y, AimMode == 0 ? TargetPosition.Z - 0.30F : TargetPosition.Z },
//				{ AimMode == 0 ? TargetPosition.X + IteratorIndex : TargetPosition.X, TargetPosition.Y, AimMode == 0 ? TargetPosition.Z + 0.30F : TargetPosition.Z },
//				{ AimMode == 0 ? TargetPosition.X - IteratorIndex : TargetPosition.X, TargetPosition.Y, AimMode == 0 ? TargetPosition.Z + 0.30F : TargetPosition.Z },
//			};
//
//			for (const auto& HitboxPosition : HitboxPositions)
//			{
//				const auto DistanceToLocal = LocalPlayerPosition.DistanceSq(HitboxPosition);
//				if (DistanceToLocal < ClosestDistance)
//				{
//					const auto MultipointDifference = HitboxPosition - StaticPoint;
//					const auto MultipointAimPoint = Vector3<double>(MultipointDifference.X * MultiPointOffset, MultipointDifference.Y * MultiPointOffset, MultipointDifference.Z * MultiPointOffset);
//
//					ClosestPoint = StaticPoint + MultipointAimPoint;
//					ClosestDistance = DistanceToLocal;
//				}
//			}
//		}
//	}
//
//	auto TargetDistanceX = ClosestPoint.X - LocalPlayerPosition.X;
//	auto TargetDistanceY = ClosestPoint.Y - LocalPlayerPosition.Y;
//	auto TargetDistanceZ = ClosestPoint.Z - LocalPlayerPosition.Z;
//	auto TargetDistanceXZ = static_cast<float>(sqrt(pow(PositionX, 2) + pow(PositionZ, 2)));
//
//	const auto LocalPitch = LocalPlayer->GetRotationPitch(Env);
//	const auto LocalYaw = LocalPlayer->GetRotationYaw(Env);
//	const auto LocalYawLast = LocalPlayer->GetPrevRotationYaw(Env);
//	const auto LocalPitchLast = LocalPlayer->GetPrevRotationPitch(Env);
//
//	const auto Angles = GetAngles(LocalPlayerPosition, ClosestPoint);
//	auto DeltaPitch = /*CalculateDeltaPitch(TargetDistanceY, TargetDistanceXZ, LocalPitch)*/-(LocalPitch - Angles.Y);
//
//	auto DeltaYaw = CalculateDeltaYaw(TargetDistanceX, TargetDistanceZ, LocalYaw);
//	if (fabs(DeltaYaw) > MaximumFov || fabs(DeltaYaw) < MinimumFov)
//	{
//		return;
//	}
//
//	if (fabs(DeltaYaw) < .5F) {
//		return;
//	}
//
//	if (fabs(LocalPitch) >= 89.F) {
//		return;
//	}
//
//	static float PreviousPitch = LocalPlayer->GetRotationPitch(Env);
//	static auto FrameTimeCorrection = DeltaTime / 0.01F;
//
//	auto RoundRotation = [GameSettings, Env](float Rotation) -> float
//		{
//			auto F = GameSettings->MouseSensitivity(Env) * 0.6f + 0.2f;
//			auto F1 = F * F * F * 1.2F;
//
//			return Rotation - std::fmod(Rotation, F1);
//		};
//
//	DeltaYaw *= Random.GenerateFloatNormal(0.025f + (0.025 * (HorizontalSpeed / 30.F)), 0.225F + (0.225 * (HorizontalSpeed / 30.F))) * ((HorizontalSpeed + Random.GenerateFloatNormal(HorizontalSpeed > 5.F ? -5.f : 0.F, 5.f)) + Random.GenerateFloatNormal(0.5F + (0.5F * (HorizontalSpeed / 30.F)), 5.F + (5.F * (HorizontalSpeed / 30.F))));
//
//	if (abs(DeltaYaw) <= 5.f && Random.GenerateInt(1, 8) <= 2) {
//		DeltaYaw *= -1;
//	}
//
//	/*if (fabs(DeltaYaw) <= 5.F)
//	{
//		return;
//	}*/
//
//	const float ChangeYaw = RoundRotation(DeltaYaw * Random.GenerateFloatNormal(0.005f, 0.0001f));
//	const float ChangePrevYaw = RoundRotation(DeltaYaw * Random.GenerateFloatNormal(0.005f, 0.0001f));
//
//	if (fabs(DeltaYaw) < 10.F && fabs(ChangeYaw) <= 0.001 || fabs(ChangePrevYaw) <= 0.001)
//	{
//		return;
//	}
//
//	LocalPlayer->SetRotationYaw(Env, LocalPlayer->GetRotationYaw(Env) + RoundRotation(ChangeYaw));
//	LocalPlayer->SetPrevRotationYaw(Env, LocalPlayer->GetPrevRotationYaw(Env) + RoundRotation(ChangePrevYaw));
//
//	static float LastPitch = 0.F;
//
//	if ((!Vertical /*|| fabs(DeltaPitch) < 5.F*/) && LastPitch == LocalPitch && Random.GenerateFloat(0.F, 1.F) <= ((fabs(ChangeYaw) < 0.1F || fabs(ChangePrevYaw) < 0.1F) ? 0.15F : 0.05F) && abs(LocalPitch) < 90 /*&& fabs(DeltaYaw) > 6.F*/)
//	{
//		if (fabs(ChangeYaw) == 0.F || fabs(ChangePrevYaw) == 0.F)
//		{
//			return;
//		}
//
//		//const float newRand = Random.GenerateFloat(-0.05f, 0.05f);
//		float Offset = Random.GenerateFloat(-0.1f, 0.1f);
//		//Offset = std::lerp(Offset, 2.F, SineOffset);
//
//		float OffsetCurrent = RoundRotation((Offset)+Random.GenerateFloatNormal(-0.1f, 0.1f));
//		float OffsetPrev = RoundRotation((Offset)+Random.GenerateFloatNormal(-0.1f, 0.1f));
//
//		OffsetCurrent /= 6.F;
//		OffsetPrev /= 6.F;
//
//		float PitchOffsetCurrent = (0.025F + (0.025F * (HorizontalSpeed / 30.F)));
//		while (OffsetCurrent == 0.F)
//		{
//			PitchOffsetCurrent += Random.GenerateFloat(0.0025F + (0.0025F * (HorizontalSpeed / 30.F)), 0.012F + (0.012 * (HorizontalSpeed / 30.F)));
//			OffsetCurrent = RoundRotation((Offset)+Random.GenerateFloatNormal(-PitchOffsetCurrent, PitchOffsetCurrent));
//		}
//
//		float PitchOffsetPrev = (0.0255F + (0.025F * (HorizontalSpeed / 30.F)));
//		while (OffsetPrev == 0.F)
//		{
//			PitchOffsetPrev += Random.GenerateFloat(0.0025F + (0.0025F * (HorizontalSpeed / 30.F)), 0.012F + (0.012 * (HorizontalSpeed / 30.F)));
//			OffsetPrev = RoundRotation((Offset)+Random.GenerateFloatNormal(-PitchOffsetPrev, PitchOffsetPrev));
//		}
//
//		const float RotationPitch = LocalPlayer->GetRotationPitch(Env) + RoundRotation(OffsetCurrent);
//		const float PrevRotationPitch = LocalPlayer->GetPrevRotationPitch(Env) + RoundRotation(OffsetPrev);
//
//		if (fabs(RotationPitch) >= 89.F || fabs(PrevRotationPitch) >= 89.F) {
//			return;
//		}
//
//		LocalPlayer->SetRotationPitch(Env, RotationPitch);
//		LocalPlayer->SetPrevRotationPitch(Env, PrevRotationPitch);
//	}
//
//	if (Vertical /*&& lastPitch == LocalPitch*/ && Random.GenerateFloat(0.F, 1.F) <= 0.85F && abs(LocalPitch) < 90 /*&& abs(DeltaYaw) > 5.F*/ /*&& fabs(DeltaPitch) > 5.F*/)
//	{
//		/*if (fabs(DeltaPitch) <= 5.F)
//		{
//			DeltaPitch = CalculateDeltaPitch(Random.GenerateFloat(TargetDistanceY - 0.1F, TargetDistanceY + 0.1F), TargetDistanceXZ, LocalPitch);
//		}*/
//		if (fabs(ChangeYaw) == 0.F || fabs(ChangePrevYaw) == 0.F)
//		{
//			return;
//		}
//
//		DeltaPitch *= Random.GenerateFloatNormal(0.025f + (0.025 * (VerticalSpeed / 30.F)), 0.225F + (0.225 * (VerticalSpeed / 30.F))) * ((VerticalSpeed + Random.GenerateFloatNormal(VerticalSpeed > 5.F ? -5.f : 0.F, 5.f)) + Random.GenerateFloatNormal(0.5F + (0.5F * (VerticalSpeed / 30.F)), 5.F + (5.F * (VerticalSpeed / 30.F))));
//
//		if (abs(DeltaPitch) <= 5.f && Random.GenerateInt(1, 8) <= 2) {
//			DeltaPitch *= -1;
//		}
//
//		const float PitchOffsetCurrent = RoundRotation(DeltaPitch * Random.GenerateFloatNormal(0.005f, 0.0001f));
//		const float PitchOffsetPrev = RoundRotation(DeltaPitch * Random.GenerateFloatNormal(0.005f, 0.0001f));
//
//		if (Random.GenerateFloat(0.F, 1.F) <= ((fabs(ChangeYaw) < 0.1F || fabs(ChangePrevYaw) < 0.1F /*|| fabs(PitchOffsetCurrent) < 0.1F || fabs(PitchOffsetPrev) < 0.1F*/) ? 0.25F : 0.05F))
//		{
//			//const float newRand = Random.GenerateFloat(-0.05f, 0.05f);
//			float Offset = Random.GenerateFloat(-0.1f, 0.1f);
//			float OffsetB = (fabs(PitchOffsetCurrent) < 0.1F || fabs(PitchOffsetPrev) < 0.1F) ? Random.GenerateFloat(0.1f, 0.25f) : Random.GenerateFloat(0.0f, 0.1f);
//			//Offset = std::lerp(Offset, 2.F, SineOffset);
//
//			float OffsetCurrent = RoundRotation((Offset)+Random.GenerateFloatNormal(-OffsetB, OffsetB));
//			float OffsetPrev = RoundRotation((Offset)+Random.GenerateFloatNormal(-OffsetB, OffsetB));
//
//			OffsetCurrent /= (fabs(PitchOffsetCurrent) < 0.1F || fabs(PitchOffsetPrev) < 0.1F) ? 2.F : 6.F;
//			OffsetPrev /= (fabs(PitchOffsetCurrent) < 0.1F || fabs(PitchOffsetPrev) < 0.1F) ? 2.F : 6.F;
//
//			float PitchOffsetCurrent = (0.025F + (0.025F * (HorizontalSpeed / 30.F)));
//			while (OffsetCurrent == 0.F)
//			{
//				PitchOffsetCurrent += Random.GenerateFloat(0.0025F + (0.0025F * (HorizontalSpeed / 30.F)), 0.012F + (0.012 * (HorizontalSpeed / 30.F)));
//				OffsetCurrent = RoundRotation((Offset)+Random.GenerateFloatNormal(-PitchOffsetCurrent, PitchOffsetCurrent));
//			}
//
//			float PitchOffsetPrev = (0.0255F + (0.025F * (HorizontalSpeed / 30.F)));
//			while (OffsetPrev == 0.F)
//			{
//				PitchOffsetPrev += Random.GenerateFloat(0.0025F + (0.0025F * (HorizontalSpeed / 30.F)), 0.012F + (0.012 * (HorizontalSpeed / 30.F)));
//				OffsetPrev = RoundRotation((Offset)+Random.GenerateFloatNormal(-PitchOffsetPrev, PitchOffsetPrev));
//			}
//
//			const float RotationPitch = LocalPlayer->GetRotationPitch(Env) + RoundRotation(OffsetCurrent);
//			const float PrevRotationPitch = LocalPlayer->GetPrevRotationPitch(Env) + RoundRotation(OffsetPrev);
//
//			if (fabs(RotationPitch) >= 89.F || fabs(PrevRotationPitch) >= 89.F) {
//				return;
//			}
//
//			LocalPlayer->SetRotationPitch(Env, RotationPitch);
//			LocalPlayer->SetPrevRotationPitch(Env, PrevRotationPitch);
//
//			return;
//		}
//
//		const float RotationPitch = LocalPlayer->GetRotationPitch(Env) + RoundRotation(PitchOffsetCurrent);
//		const float RotationPitchPrev = LocalPlayer->GetRotationPitch(Env) + RoundRotation(PitchOffsetPrev);
//
//		/*if (fabs(DeltaPitch) > 3.F && fabs(ChangeYaw))
//		const float OffsetYaw = */
//
//		if (fabs(RotationPitch) >= 89.F || fabs(RotationPitchPrev) >= 89.F) {
//			return;
//		}
//
//		LocalPlayer->SetRotationPitch(Env, RotationPitch);
//		LocalPlayer->SetPrevRotationPitch(Env, RotationPitchPrev);
//	}
//
//	LastPitch = LocalPlayer->GetRotationPitch(Env);
//}

void AimAssist::Run(JNIEnv* Env)
{
	auto ResetAngles = [&]()->void
		{
			YawChange.store(0.F);
			PitchChange.store(0.F);
		};

	if (!this->GetState() || UI_OPEN)
	{
		ResetAngles();
		return;
	}

	if (OnlyClicking && !(GetAsyncKeyState(VK_LBUTTON) & 0x8000))
	{
		ResetAngles();
		return;
	}

	GameClasses ThreadClasses;
	if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses))
	{
		ResetAngles();
		return;
	}

	const auto& LocalPlayer = ThreadClasses.Player;
	const auto& World = ThreadClasses.World;
	const auto& Minecraft = ThreadClasses.Minecraft;
	const auto& Timer = Minecraft->GetTimerPtr(Env);
	const auto& MovingObjectPosition = std::make_shared<::MovingObjectPosition>(Env, Minecraft->GetObjectMouseOver(Env));
	const auto& GameSettings = ThreadClasses.Minecraft->GetGameSettingsPtr(Env);
	const auto& PlayerController = Minecraft->GetPlayerControllerPtr(Env);

	if (!Minecraft->InGameHasFocus(Env))
	{
		ResetAngles();
		return;
	}

	static int TargetID = 0;
	std::shared_ptr<Player> TargetPlayer = nullptr;

	const std::vector<std::shared_ptr<Player>>& PlayerList = ThreadClasses.World->GetPlayerEntitiesWithinAABBVector(Env, ThreadClasses.Player.get(), ThreadClasses.Player->GetPos(Env), AimAssist::MaximumRange);

	if (PlayerList.empty())
	{
		ResetAngles();
		return;
	}

	if (TargetingMode == SINGLE && TargetID != 0)
	{
		bool Found = false;
		for (const auto& Player : PlayerList)
		{
			if (!Player->GetInstanceObject() || Env->IsSameObject(Player->GetInstanceObject(), NULL)) {
				continue;
			}

			if (Player->IsDead(Env)) {
				continue;
			}

			if (Player->GetEntityID(Env) == TargetID)
			{
				const auto DistanceToLocal = Player->GetDistance(Env, LocalPlayer.get());

				if (DistanceToLocal > MaximumRange || DistanceToLocal < MinimumRange) {
					continue;
				}

				TargetPlayer = Player;
				Found = true;
				break;
			}
		}

		if (!Found)
		{
			TargetPlayer = nullptr;
			TargetID = 0;
		}
	}

	if (TargetingMode == SINGLE && TargetID == 0 && TargetPlayer == nullptr)
	{
		float BestFov = 1000.F;
		float Health = 1000.F;
		float Distance = MaximumRange;

		for (const auto& Player : PlayerList)
		{
			if (Env->IsSameObject(Player->GetInstanceObject(), NULL)) {
				continue;
			}

			if (Env->IsSameObject(LocalPlayer->GetInstanceObject(), Player->GetInstanceObject())) {
				continue;
			}

			if (Player->IsDead(Env)) {
				continue;
			}

			const auto PlayerName = Player->GetName(Env);

			if (AntiBot::IsBot(Env, World.get(), LocalPlayer.get(), Player.get())) {
				continue;
			}

			if (Teams::IsOnSameTeam(Env, LocalPlayer.get(), Player.get())) {
				continue;
			}

			if (Friends::IsFriend(Player->GetEntityID(Env))) {
				continue;
			}

			if (!AimAtInvisible && Player->IsInvisible(Env)) {
				continue;
			}


			if (!AimThroughWalls && !LocalPlayer->IsEntitySeen(Env, Player.get())) {
				continue;
			}

			const auto PlayerHealth = Player->GetHealth(Env);

			if (PlayerHealth < 0.F) {
				continue;
			}

			const auto DistanceToLocal = Player->GetDistance(Env, LocalPlayer.get());

			if (DistanceToLocal > MaximumRange || DistanceToLocal < MinimumRange) {
				continue;
			}

			if (FilteringMode == CLOSEST)
			{
				if (DistanceToLocal < Distance && DistanceToLocal > MinimumRange)
				{
					TargetID = Player->GetEntityID(Env);
					TargetPlayer = Player;
					Distance = DistanceToLocal;

					continue;
				}
			}

			if (FilteringMode == LOWEST_HEALTH)
			{
				if (PlayerHealth < Health)
				{
					TargetID = Player->GetEntityID(Env);
					TargetPlayer = Player;
					Health = PlayerHealth;

					continue;
				}
			}

			if (FilteringMode == FOV)
			{
				const auto FovToLocal = fabs(LocalPlayer->GetFovTo(Env, Player.get()));
				if (FovToLocal < BestFov)
				{
					TargetID = Player->GetEntityID(Env);
					TargetPlayer = Player;
					BestFov = FovToLocal;

					continue;
				}
			}
		}
	}

	if (TargetingMode == SWITCH)
	{
		float BestFov = 1000.F;
		float Health = 1000.F;
		float Distance = MaximumRange;

		for (const auto& Player : PlayerList)
		{
			if (Env->IsSameObject(Player->GetInstanceObject(), NULL)) {
				continue;
			}

			if (Env->IsSameObject(LocalPlayer->GetInstanceObject(), Player->GetInstanceObject())) {
				continue;
			}

			if (Player->IsDead(Env)) {
				continue;
			}

			const auto PlayerName = Player->GetName(Env);

			if (AntiBot::IsBot(Env, World.get(), LocalPlayer.get(), Player.get())) {
				continue;
			}

			if (Teams::IsOnSameTeam(Env, LocalPlayer.get(), Player.get())) {
				continue;
			}

			if (Friends::IsFriend(Player->GetEntityID(Env))) {
				continue;
			}

			if (!AimAtInvisible && Player->IsInvisible(Env)) {
				continue;
			}

			if (!AimThroughWalls && !LocalPlayer->IsEntitySeen(Env, Player.get())) {
				continue;
			}

			const auto PlayerHealth = Player->GetHealth(Env);

			if (PlayerHealth < 0.F) {
				continue;
			}

			const auto DistanceToLocal = Player->GetDistance(Env, LocalPlayer.get());

			if (DistanceToLocal > MaximumRange || DistanceToLocal < MinimumRange) {
				continue;
			}

			if (FilteringMode == CLOSEST)
			{
				if (DistanceToLocal < Distance && DistanceToLocal > MinimumRange)
				{
					TargetPlayer = Player;
					Distance = DistanceToLocal;

					continue;
				}
			}

			if (FilteringMode == LOWEST_HEALTH)
			{
				if (PlayerHealth < Health)
				{
					TargetPlayer = Player;
					Health = PlayerHealth;

					continue;
				}
			}

			if (FilteringMode == FOV)
			{
				const auto FovToLocal = fabs(LocalPlayer->GetFovTo(Env, Player.get()));
				if (FovToLocal < BestFov)
				{
					TargetPlayer = Player;
					BestFov = FovToLocal;

					//std::cout << "Best Fov: " << BestFov << std::endl;

					continue;
				}
			}
		}
	}

	if (TargetPlayer == nullptr)
	{
		ResetAngles();
		return;
	}

	if (!AimAtInvisible && TargetPlayer->IsInvisible(Env))
	{
		ResetAngles();
		return;
	}

	const auto ItemStack = LocalPlayer->GetHeldItemPtr(Env);

	bool IsItemWhitelisted = false;

	if (WhitelistSwords && ItemStack->IsSword(Env)) {
		IsItemWhitelisted = true;
	}

	if (WhitelistAxes && ItemStack->IsAxe(Env)) {
		IsItemWhitelisted = true;
	}

	if (WhitelistSticks && ItemStack->IsStick(Env)) {
		IsItemWhitelisted = true;
	}

	if (WhitelistPickAxe && ItemStack->IsPickAxe(Env)) {
		IsItemWhitelisted = true;
	}

	if (WhitelistFist && ItemStack->IsFist(Env)) {
		IsItemWhitelisted = true;
	}

	if (ItemWhitelist && !IsItemWhitelisted)
	{
		ResetAngles();
		return;
	}

	static DWORD LastBlockAimTime = 0;
	static bool IsAimingBlock = false;
	if (!IsAimingBlock && MovingObjectPosition->IsAimingBlock(Env))
	{
		IsAimingBlock = true;
		LastBlockAimTime = GetTickCount64();
	}

	if (IsAimingBlock && !MovingObjectPosition->IsAimingBlock(Env))
	{
		IsAimingBlock = false;
		LastBlockAimTime = 0;
	}

	if (BreakBlocks && (IsAimingBlock && (GetTickCount64() - LastBlockAimTime > 350) || PlayerController->IsHittingBlock(Env)))
	{
		ResetAngles();
		return;
	}

	const auto LocalPlayerPosition = LocalPlayer->GetPos(Env);
	const auto TargetPosition = TargetPlayer->GetPos(Env);

	const auto PositionX = TargetPosition.X;
	const auto PositionY = TargetPosition.Y /*- 2.F TargetPlayer->GetEyeHeight(Env)*/;
	const auto PositionZ = TargetPosition.Z;

	auto DistanceX = TargetPosition.X - LocalPlayerPosition.X;
	auto DistanceZ = TargetPosition.Z - LocalPlayerPosition.Z;

	auto ClosestPoint = Vector3<double>(TargetPosition);
	double ClosestDistance = (std::numeric_limits<double>::max)();

	const auto StaticPoint = Vector3<double>(TargetPosition.X, TargetPosition.Y, TargetPosition.Z);
	Vector3<double> MultipointDifference = { 0.0, 0.0, 0.0 };

	if (AimMode == MULTIPOINT)
	{
		for (int Index = 0; Index <= 30; Index++)
		{
			const auto IteratorIndex = Index / 100.F;

			const std::vector<Vector3<double>> HitboxPositions
			{
				{ AimMode == 0 ? TargetPosition.X - 0.30F : TargetPosition.X, TargetPosition.Y, AimMode == 0 ? TargetPosition.Z + IteratorIndex : TargetPosition.Z },
				{ AimMode == 0 ? TargetPosition.X - 0.30F : TargetPosition.X, TargetPosition.Y, AimMode == 0 ? TargetPosition.Z - IteratorIndex : TargetPosition.Z },
				{ AimMode == 0 ? TargetPosition.X + 0.30F : TargetPosition.X, TargetPosition.Y, AimMode == 0 ? TargetPosition.Z + IteratorIndex : TargetPosition.Z },
				{ AimMode == 0 ? TargetPosition.X + 0.30F : TargetPosition.X, TargetPosition.Y, AimMode == 0 ? TargetPosition.Z - IteratorIndex : TargetPosition.Z },

				{ AimMode == 0 ? TargetPosition.X + IteratorIndex : TargetPosition.X, TargetPosition.Y, AimMode == 0 ? TargetPosition.Z - 0.30F : TargetPosition.Z },
				{ AimMode == 0 ? TargetPosition.X - IteratorIndex : TargetPosition.X, TargetPosition.Y, AimMode == 0 ? TargetPosition.Z - 0.30F : TargetPosition.Z },
				{ AimMode == 0 ? TargetPosition.X + IteratorIndex : TargetPosition.X, TargetPosition.Y, AimMode == 0 ? TargetPosition.Z + 0.30F : TargetPosition.Z },
				{ AimMode == 0 ? TargetPosition.X - IteratorIndex : TargetPosition.X, TargetPosition.Y, AimMode == 0 ? TargetPosition.Z + 0.30F : TargetPosition.Z },
			};

			for (const auto& HitboxPosition : HitboxPositions)
			{
				const auto DistanceToLocal = LocalPlayerPosition.DistanceSq(HitboxPosition);
				if (DistanceToLocal < ClosestDistance)
				{
					MultipointDifference = HitboxPosition - StaticPoint;
					const auto MultipointAimPoint = Vector3<double>(MultipointDifference.X * MultiPointOffset, MultipointDifference.Y * MultiPointOffset, MultipointDifference.Z * MultiPointOffset);

					ClosestPoint = StaticPoint + MultipointAimPoint;
					ClosestDistance = DistanceToLocal;
				}
			}
		}
	}

	auto TargetDistanceX = ClosestPoint.X - LocalPlayerPosition.X;
	auto TargetDistanceY = ClosestPoint.Y - LocalPlayerPosition.Y;
	auto TargetDistanceZ = ClosestPoint.Z - LocalPlayerPosition.Z;
	auto TargetDistanceXZ = static_cast<float>(sqrt(pow(PositionX, 2) + pow(PositionZ, 2)));
	auto TargetDistance = LocalPlayer->GetDistance(Env, TargetPlayer.get());

	const auto RandomScale = RandomizationScale / 100.F;
	float RandomMagnitude = (0.01F * (RandomizationMagnitude / 100.F)) * (std::clamp(TargetDistance, 0.1F, 3.F) / 3.F);

	static Vector3<float> PreviousMagnitudeScaledRandom =
	{
		Random.GenerateFloat(-(RandomMagnitude * RandomScale), (RandomMagnitude * RandomScale), RandomizationScale),
		Random.GenerateFloat(-(RandomMagnitude * RandomScale), (RandomMagnitude* RandomScale), RandomizationScale),
		Random.GenerateFloat(-(RandomMagnitude * RandomScale), (RandomMagnitude * RandomScale), RandomizationScale)
	};

	Vector3<float> MagnitudeScaledRandom = 
	{
		Random.GenerateFloat(-(RandomMagnitude * RandomScale), (RandomMagnitude * RandomScale), RandomizationScale),
		Random.GenerateFloat(-(RandomMagnitude * RandomScale), (RandomMagnitude * RandomScale), RandomizationScale),
		Random.GenerateFloat(-(RandomMagnitude * RandomScale), (RandomMagnitude * RandomScale), RandomizationScale)
	};

	ClosestPoint.X += MagnitudeScaledRandom.X * 0.1F;
	ClosestPoint.Y += MagnitudeScaledRandom.Y * 0.1F;
	ClosestPoint.Z += MagnitudeScaledRandom.Z * 0.1F;

	PreviousMagnitudeScaledRandom = MagnitudeScaledRandom;

	const auto LocalPitch = LocalPlayer->GetRotationPitch(Env);
	const auto LocalYaw = LocalPlayer->GetRotationYaw(Env);
	const auto LocalYawLast = LocalPlayer->GetPrevRotationYaw(Env);
	const auto LocalPitchLast = LocalPlayer->GetPrevRotationPitch(Env);

	auto Warp180 = [&](float Angle) -> float
	{
		Angle = std::fmod(Angle, 360.0F);

		if (Angle >= 180.0F) {
			Angle -= 360.0F;
		}

		if (Angle < -180.0F) {
			Angle += 360.0F;
		}

		return Angle;
	};

	auto FixRotations = [&](std::pair<float, float> Rotation, std::pair<float, float> LastRotation)-> std::pair<float, float>
	{
		float yaw = Rotation.first;
		float pitch = Rotation.second;

		float lastYaw = LastRotation.first;
		float lastPitch = LastRotation.second;

		float f = /*GameSettings->MouseSensitivity(Env)*//*(1.F - ((200.F - 5.F) / 200.F))*/GameSettings->MouseSensitivity(Env) * 0.6F + 0.2F;
		float gcd = f * f * f * 1.2F;

		float deltaYaw = yaw - lastYaw;
		float deltaPitch = pitch - lastPitch;

		float fixedDeltaYaw = deltaYaw - std::fmod(deltaYaw, gcd);
		float fixedDeltaPitch = deltaPitch - std::fmod(deltaPitch, gcd);

		float fixedYaw = lastYaw + fixedDeltaYaw;
		float fixedPitch = lastPitch + fixedDeltaPitch;

		return {fixedYaw, fixedPitch};
	};

	auto RotationsToTarget = [&]() -> std::pair<float, float>
	{
		const auto MultiPointDifferenceScaled = MultipointDifference * MultiPointOffset;
		const auto LastTickPos = TargetPlayer->GetLastTickPos(Env) + MultiPointDifferenceScaled;
		const auto LocalLastTickPos = LocalPlayer->GetLastTickPos(Env);
		const auto RenderPartialTicks = Timer->GetRenderPartialTicks(Env);

		const Vector3<float> InterpolatedTargetPos =
		{
			static_cast<float>(LastTickPos.X + ((ClosestPoint.X - LastTickPos.X) * RenderPartialTicks)),
			static_cast<float>(LastTickPos.Y + ((ClosestPoint.Y - LastTickPos.Y) * RenderPartialTicks)),
			static_cast<float>(LastTickPos.Z + ((ClosestPoint.Z - LastTickPos.Z) * RenderPartialTicks)),
		};

		const Vector3<float> InterpolatedLocalPos =
		{
			static_cast<float>(LocalLastTickPos.X + ((LocalPlayerPosition.X - LocalLastTickPos.X) * RenderPartialTicks)),
			static_cast<float>(LocalLastTickPos.Y + ((LocalPlayerPosition.Y - LocalLastTickPos.Y) * RenderPartialTicks)),
			static_cast<float>(LocalLastTickPos.Z + ((LocalPlayerPosition.Z - LocalLastTickPos.Z) * RenderPartialTicks)),
		};

		//(ClosestPoint.X /*- (LastTickPos.X - ClosestPoint.X) + 0.01*/ - LocalPlayerPosition.X);
		//(ClosestPoint.Z /*- (LastTickPos.Z - ClosestPoint.Z)*/ - LocalPlayerPosition.Z);
		double DistanceX = InterpolatedTargetPos.X - InterpolatedLocalPos.X;
		double DistanceZ = InterpolatedTargetPos.Z - InterpolatedLocalPos.Z;
		double DistanceY = VerticalPoint == CORNER ? (InterpolatedTargetPos.Y + 0.4 + TargetPlayer->GetEyeHeight(Env) / 1.3 - (InterpolatedLocalPos.Y + LocalPlayer->GetEyeHeight(Env))) : (InterpolatedTargetPos.Y - InterpolatedLocalPos.Y);;

		double Hyopt = sqrt(DistanceX * DistanceX + DistanceZ * DistanceZ);

		float Yaw = (float)(atan2(DistanceZ, DistanceX) * 180.0 / std::numbers::pi) - 90.0F;
		float Pitch = (float)-(atan2(DistanceY, Hyopt) * 180.0 / std::numbers::pi);

		Yaw = LocalPlayer->GetRotationYaw(Env) + Warp180(Yaw - LocalPlayer->GetRotationYaw(Env));
		Pitch = LocalPlayer->GetRotationPitch(Env) + Warp180(Pitch - LocalPlayer->GetRotationPitch(Env));

		auto FixedRotations = FixRotations({ Yaw, Pitch }, { LocalPlayer->GetPrevRotationYaw(Env), LocalPlayer->GetPrevRotationPitch(Env) });
		
		return { FixedRotations.first, std::clamp(FixedRotations.second, -90.F, 90.F) };
	};

	auto SmoothenAngles = [&](std::pair<float, float> Angles) -> std::pair<float, float>
	{
		std::pair<float, float> TempAngles = Angles;

		const auto HorizontalScale = std::clamp(HorizontalSpeed, 10.5F, 30.F) / 30.F;
		const auto VerticalScale = std::clamp(VerticalSpeed, 10.5F, 30.F) / 30.F;

		if (Smoothing == LINEAR)
		{
			TempAngles.first *= Random.GenerateFloatNormal(0.35F * HorizontalScale, 0.75F * HorizontalScale, RandomizationScale);
			TempAngles.second *= Random.GenerateFloatNormal(0.35F * VerticalScale, 0.75F * VerticalScale, RandomizationScale);

			TempAngles.first *= Random.GenerateFloatNormal(0.8f, 0.3f, RandomizationScale);
			TempAngles.second *= Random.GenerateFloatNormal(0.8f, 0.3f, RandomizationScale);
		}

		return TempAngles;
	};
		/*switch (Smoothing)
		{
		case LINEAR:
			

			break;

		case CONSTANT:
			Yaw = YawDiff * (1.F - cos((0.2F * (HorizontalSpeed / 30.F) * PI) / 2.F));
			Pitch = PitchDiff * (1.F - cos((0.2F * (VerticalSpeed / 30.F) * PI) / 2.F));

			break;

		case EASE_OUT_CUBIC:
			Yaw = YawDiff * (1.F - pow(1.F - (0.01F * (HorizontalSpeed / 30.F)), 3.F));
			Pitch = PitchDiff * (1.F - pow(1.F - (0.01F * (VerticalSpeed / 30.F)), 3.F));

			break;

		case EASE_IN_QUART:
			Yaw = YawDiff * pow(2.F, 10.F * ((HorizontalSpeed / 2.F) / 30.F) - 10.F);
			Pitch = PitchDiff * pow(2.F, 10.F * ((VerticalSpeed / 2.F) / 30.F) - 10.F);

			break;
		}*/

	/*auto SmoothAngles = [&]() -> std::pair<Vector3<double>, Vector3<double>>
	{
		float Yaw = 0.F;
		float Pitch = 0.F;

		float PrevYaw = 0.F;
		float PrevPitch = 0.F;

		auto YawDiff = DeltaYaw;
		auto PitchDiff = DeltaPitch;

		const auto Scale = RandomizationScale / 100.F;

		switch (Smoothing)
		{
		case LINEAR:
			YawDiff *= 0.9F;
			PitchDiff *= 0.9F;

			Yaw = YawDiff * 0.8F;
			Pitch = PitchDiff * 0.8F;

			PrevYaw = YawDiff * 0.8F;
			PrevPitch = PitchDiff * 0.8F;

			break;

		case CONSTANT:
			Yaw = YawDiff * (1.F - cos((0.2F * (HorizontalSpeed / 30.F) * PI) / 2.F));
			Pitch = PitchDiff * (1.F - cos((0.2F * (VerticalSpeed / 30.F) * PI) / 2.F));

			PrevYaw = YawDiff * (1.F - cos((0.2F * (HorizontalSpeed / 30.F) * PI) / 2.F));
			PrevPitch = PitchDiff * (1.F - cos((0.2F * (VerticalSpeed / 30.F) * PI) / 2.F));
			break;

		case EASE_OUT_CUBIC:
			Yaw = YawDiff * (1.F - pow(1.F - (0.01F * (HorizontalSpeed / 30.F)), 3.F));
			Pitch = PitchDiff * (1.F - pow(1.F - (0.01F * (VerticalSpeed / 30.F)), 3.F));

			PrevYaw = YawDiff * (1.F - pow(1.F - (0.01F * (HorizontalSpeed / 30.F)), 3.F));
			PrevPitch = PitchDiff * (1.F - pow(1.F - (0.01F * (VerticalSpeed / 30.F)), 3.F));

			break;

		case EASE_IN_QUART:
			Yaw = YawDiff * pow(2.F, 10.F * ((HorizontalSpeed / 2.F) / 30.F) - 10.F);
			Pitch = PitchDiff * pow(2.F, 10.F * ((VerticalSpeed / 2.F) / 30.F) - 10.F);

			PrevYaw = YawDiff * pow(2.F, 10.F * ((HorizontalSpeed / 2.F) / 30.F) - 10.F);
			PrevPitch = PitchDiff * pow(2.F, 10.F * ((VerticalSpeed / 2.F) / 30.F) - 10.F);

			break;
		}

		return std::make_pair<Vector3<double>, Vector3<double>>(Vector3<double>(Yaw, Pitch, 0.F), Vector3<double>(PrevYaw, PrevPitch, 0.F));
	};*/

	const auto Scale = 0.1F * (RandomizationScale / 100.F);

	auto rotations = RotationsToTarget();


	float targetYaw = (float)(rotations.first /*+ Math.random()*/);
	float targetPitch = (float)(rotations.second /*+ Math.random()*/);

	const std::pair<float, float> Smooth = SmoothenAngles({rotations.first - LocalYaw, rotations.second - LocalPitch});

	float niggaYaw = Smooth.first;
	float niggaPitch = Smooth.second;

	float f = /*GameSettings->MouseSensitivity(Env)*//*(1.F - ((200.F - 5.F) / 200.F))*/GameSettings->MouseSensitivity(Env) * 0.6F + 0.2F;
	float gcd = f * f * f * 8.0F;

	YawChange = std::round(niggaYaw / gcd);

	if (Vertical) PitchChange = std::round(niggaPitch / gcd);
	else PitchChange = 0;

	/*LocalPlayer->SetRotationYaw(Env, LocalPlayer->GetRotationYaw(Env) + YawFinal);
	LocalPlayer->SetRotationPitch(Env, LocalPlayer->GetRotationPitch(Env) + PitchFinal);*/



	//if (fabs(DeltaSmooth.X) != 0.F)
	//{
	//	constexpr float Minimums = 0.01796885407F;

	//	const auto YawChange = DeltaSmooth.X;
	//	float PitchAdd = Minimums * Random.GenerateInt(-5 * Scale, 5 * Scale);

	//	DeltaSmooth.Y += PitchAdd;

	//	std::cout << PitchAdd << std::endl;
	//}

	//Clamp90(&DeltaSmooth.Y);

	//LocalPlayer->SetRotationYaw(Env, LocalPlayer->GetRotationYaw(Env) + RoundRotation(DeltaSmooth.X));
	//LocalPlayer->SetRotationPitch(Env, LocalPlayer->GetRotationPitch(Env) + RoundRotation(DeltaSmooth.Y));
	////LocalPlayer->SetPrevRotationYaw(Env, LocalPlayer->GetPrevRotationYaw(Env) + RoundRotation(PrevAngles.X));

	////LocalPlayer->SetPrevRotationPitch(Env, LocalPlayer->GetPrevRotationPitch(Env) + RoundRotation(PrevAngles.Y));

	//LastPitch = LocalPlayer->GetRotationPitch(Env);
	//LastYaw = LocalPlayer->GetPrevRotationYaw(Env);
}



void AimAssist::OnSetAngles(JNIEnv* Env, float Yaw, float Pitch, bool* ShouldCancel)
{
	if (YawChange == 0.F && PitchChange == 0.F) {
		return;
	}

	GameClasses ThreadClasses;
	if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses)) {
		return;
	}

	const auto& LocalPlayer = ThreadClasses.Player;
	const auto& World = ThreadClasses.World;
	const auto& Minecraft = ThreadClasses.Minecraft;
	const auto& MovingObjectPosition = std::make_shared<::MovingObjectPosition>(Env, Minecraft->GetObjectMouseOver(Env));
	const auto& GameSettings = ThreadClasses.Minecraft->GetGameSettingsPtr(Env);
	const auto& MouseHelper = Minecraft->GetMouseHelperPtr(Env);

	const auto MouseDelta = MouseHelper->MouseDelta(Env);

	float f = /*GameSettings->MouseSensitivity(Env)*//*(1.F - ((200.F - 5.F) / 200.F))*/GameSettings->MouseSensitivity(Env) * 0.6F + 0.2F;
	float f1 = f * f * f * 8.0F;

	/*float f2 = (float)(MouseDelta.X + (MouseDelta.X != 0 ? YawChange.load() : 0)) * f1;
	float f3 = (float)(MouseDelta.Y - (MouseDelta.Y != 0 ? PitchChange.load() : 0)) * f1;*/

	//smoothen when mouse move dtc
	float f2 = Yaw == 0 ? YawChange * f1 : Yaw;
	float f3 = Pitch == 0 ? -PitchChange * f1 : Pitch;

	int i = 1;

	ModuleManager::SkipSetAnglesEvent = true;
	LocalPlayer->SetAngles(Env, f2, f3);
	*ShouldCancel = true;
	ModuleManager::SkipSetAnglesEvent = false;
}