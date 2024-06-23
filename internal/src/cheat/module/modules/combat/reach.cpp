#include "pch.h"
#include "reach.hpp"
#include <src/globals/utilities/maths/raytrace.hpp>

void Reach::OnStrictMathAtan2(JNIEnv* Env)
{
	if (!this->GetState()) {
		return;
	}

	GameClasses ThreadClasses;
	if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses)) {
		return;
	}

	#pragma region GameClasses

	const auto Minecraft = ThreadClasses.Minecraft;
	const auto LocalPlayer = ThreadClasses.Player;
	const auto World = ThreadClasses.World;

	const auto GameSettings = Minecraft->GetGameSettingsPtr(Env);
	const auto PlayerController = Minecraft->GetPlayerControllerPtr(Env);
	const auto MovingObjectPosition = Minecraft->GetObjectMouseOverPtr(Env);

	#pragma endregion


	if (OnlySprinting && !LocalPlayer->IsSprinting(Env)) {
		return;
	}

	if (LiquidCheck && LocalPlayer->IsInWater(Env)) {
		return;
	}

	if (!HitThroughWalls && MovingObjectPosition->IsAimingBlock(Env)) {
		return;
	}

	if (Random.GenerateInt(0, 100) > Chance) {
		return;
	}

	float TargetRange = Random.GenerateFloat(DistanceMin, DistanceMax);

	const auto LocalPlayerPos = LocalPlayer->GetPos(Env);
	const auto LocalPLayerEyeHeight = LocalPlayer->GetEyeHeight(Env);

	const auto RaytraceOrigin = Vector3<double>(LocalPlayerPos.X, LocalPlayerPos.Y + LocalPLayerEyeHeight, LocalPlayerPos.Z);
	const auto LocalAngles = Vector3<double>(LocalPlayer->GetRotationPitch(Env), LocalPlayer->GetRotationYaw(Env), 0.F);

	Vector3<float> ForwardVector;
	AngleVectors(LocalAngles, &ForwardVector);

	Vector3<double> TargetPosition{};

	std::shared_ptr<Player> TargetPlayer = nullptr;

	const auto PlayerList = World->GetPlayers(Env);

	for (auto& Player : PlayerList)
	{
		if (Env->IsSameObject(Player->GetInstanceObject(), LocalPlayer->GetInstanceObject())) {
			continue;
		}

		/*if (!LocalPlayer->IsEntitySeen(Env, Player.get()) && AimAtInvisible) {
			continue;
		}*/

		const auto PlayerPos = Player->GetPos(Env);
		const auto PlayerBoundingBoxObject = Player->GetAxisAlignedBoundingBoxPtr(Env);
		const auto PlayerNativeBoundingBox = PlayerBoundingBoxObject->GetAxisAlignedBoundingBoxNative(Env);

		if (fabs(PlayerPos.Y - LocalPlayerPos.Y) > 1.f && YCheck) {
			continue;
		}

		Vector3<double> PlayerMinimums(PlayerNativeBoundingBox.MinX, PlayerNativeBoundingBox.MinY, PlayerNativeBoundingBox.MinZ);
		Vector3<double> PlayerMaximums(PlayerNativeBoundingBox.MaxX, PlayerNativeBoundingBox.MaxY, PlayerNativeBoundingBox.MaxZ);

		PlayerMinimums = PlayerMinimums - PlayerPos;
		PlayerMaximums = PlayerMaximums - PlayerPos;

		if (ExpandHitbox)
		{
			float HitboxScale = Random.GenerateFloat(HitboxScaleMin, HitboxScaleMax);

			PlayerMinimums = PlayerMinimums * HitboxScale;
			PlayerMaximums = PlayerMaximums * HitboxScale;
			PlayerMaximums.Y = std::copysignf(1.5f, PlayerMaximums.Y) + (.3f * HitboxScale);
		}

		PlayerMinimums = PlayerMinimums + PlayerPos;
		PlayerMaximums = PlayerMaximums + PlayerPos;

		RayTrace RayTrace(RaytraceOrigin, ForwardVector);

		float TraceDistance = 0.f;
		Vector3<double> TracePosition{};

		if (!RayTrace.Trace(PlayerMinimums, PlayerMaximums, TraceDistance, TracePosition)) {
			continue;
		}

		if (TraceDistance < TargetRange)
		{
			TargetPlayer = Player;
			TargetRange = TraceDistance;
			TargetPosition = TracePosition;
		}
	}

	if (TargetPlayer == nullptr) {
		return;
	}

	Minecraft->SetObjectMouseOver(Env, TargetPlayer.get(), TargetPosition);
	Minecraft->SetPointedEntity(Env, TargetPlayer.get());
}
