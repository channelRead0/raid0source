#include "pch.h"
#include "backtrack.hpp"

#include "cheat/hook/detours/java/channelread0.hpp"
#include <src/sdk/game/minecraft/activerenderinfo.hpp>
#include <GL/gl.h>
#include <algorithm> 

void BackTrack::Run(JNIEnv* Env)
{
	try
	{
		if (!this->GetState())
		{
			PacketQueueHandlerPtr->ProcessAndClearQueue(Env);
			TargetHandlerPtr->ClearTarget();

			return;
		}

		GameClasses ThreadClasses;
		if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses))
		{
			PacketQueueHandlerPtr->ClearQueue(Env);
			TargetHandlerPtr->ClearTarget();

			return;
		}

		const auto& LocalPlayer = ThreadClasses.Player;

		if (LocalPlayer->GetHurtTime(Env) != 0) {
			ComboCount = 0;
		}

		if (GetTickCount64() - HitTime > 1500) {
			ComboCount = 0;
		}

		Sleep(50);
	}
	catch (...)
	{

	}
}

void BackTrack::OnTick(JNIEnv* Env)
{
	
}

void BackTrack::OnPacketReceive(JNIEnv* Env, const std::shared_ptr<PacketReceiveData>& PacketData, bool* ShouldCancel)
{
	try
	{
		if (!this->GetState())
		{
			TargetHandlerPtr->ClearTarget();
			PacketQueueHandlerPtr->ProcessAndClearQueue(Env);

			return;
		}

		GameClasses ThreadClasses;
		if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses))
		{
			TargetHandlerPtr->ClearTarget();
			PacketQueueHandlerPtr->ClearQueue(Env);

			return;
		}

		const auto& LocalPlayer = ThreadClasses.Player;
		const auto& World = ThreadClasses.World;
		const auto& Minecraft = ThreadClasses.Minecraft;

		const auto& Packet = PacketData->PacketInstance;
		const auto& NetworkManager = PacketData->NetworkManagerInstance;
		const auto& ChannelHandlerContext = PacketData->ChannelHandlerContextInstance;

		if (!Packet->IsPlayClientPacket(Env)) {
			return;
		}

		static int PreviousMode = Mode;
		if (PreviousMode != Mode)
		{
			PreviousMode = Mode;

			TargetHandlerPtr->ClearTarget();
			PacketQueueHandlerPtr->ProcessAndClearQueue(Env);

			return;
		}

		if (Packet->IsTeamsPacket(Env)) {
			return;
		}

		const auto& Target = TargetHandlerPtr->GetTarget();

		if (Target != nullptr)
		{
			if (Packet->IsEntityTeleportPacket(Env))
			{
				const auto TeleportPacket = std::make_shared<EntityPacketTeleport>(Env, Packet->GetInstanceObject());
				if (TeleportPacket->GetEntityID(Env) == LocalPlayer->GetEntityID(Env))
				{
					TargetHandlerPtr->ClearTarget();
					PacketQueueHandlerPtr->ProcessAndClearQueue(Env);

					return;
				}

				if (TeleportPacket->GetEntityID(Env) == Target->GetEntityID())
				{
					const auto Position = Vector3<double>
						(
							TeleportPacket->GetPosition(Env).X / 32.0,
							TeleportPacket->GetPosition(Env).Y / 32.0,
							TeleportPacket->GetPosition(Env).Z / 32.0
						);

					Target->UpdatePosition(Env, Position, LerpOffset);
				}
			}

			if (Packet->IsEntityPacket(Env))
			{
				const auto EntityPacket = std::make_shared<::EntityPacket>(Env, Packet->GetInstanceObject());
				if (EntityPacket->GetEntityID(Env) == Target->GetEntityID())
				{
					const auto Position = Vector3<double>
						(
							EntityPacket->GetPosition(Env).X / 32.0,
							EntityPacket->GetPosition(Env).Y / 32.0,
							EntityPacket->GetPosition(Env).Z / 32.0
						);

					Target->UpdatePosition(Env, Position, LerpOffset, true);
				}
			}
		}

		if (Mode == BACKTRACK_A)
		{
			if (!TargetHandlerPtr->IsValid(Env))
			{
				PacketQueueHandlerPtr->ProcessAndClearQueue(Env);
				return;
			}

			if (ComboCount < ComboThreshold)
			{
				TargetHandlerPtr->ClearTarget();
				PacketQueueHandlerPtr->ProcessAndClearQueue(Env);

				return;
			}

			if (GetTickCount64() > static_cast<unsigned long long>(TargetHandlerPtr->GetTargetTime()) + Delay)
			{
				TargetHandlerPtr->ClearTarget();
				PacketQueueHandlerPtr->ProcessAndClearQueue(Env);

				return;
			}

			if (DisableOnHit && Packet->IsAttackPacket(Env, LocalPlayer->GetEntityID(Env)))
			{
				TargetHandlerPtr->ClearTarget();
				PacketQueueHandlerPtr->ProcessAndClearQueue(Env);

				return;
			}

			const auto DistanceReal = LocalPlayer->GetPos(Env).Distance(Target->GetPosition());
			const auto DistanceLocal = LocalPlayer->GetPos(Env).Distance(Target->GetTarget()->GetPos(Env));

			if (!Packet->IsAttackPacket(Env, Target->GetEntityID()))
			{
				PacketQueueHandlerPtr->QueuePacket(Env, PacketData);
				*ShouldCancel = true;
				return;
			}
		}

		if (Mode == BACKTRACK_B)
		{
			PacketQueueHandlerPtr->UpdateQueueTick(Env, Delay);

			/*const auto DistanceReal = LocalPlayer->GetPos(Env).Distance(Target->GetPosition());
			const auto DistanceLocal = LocalPlayer->GetPos(Env).Distance(Target->GetTarget()->GetPos(Env));*/

			if (Target != nullptr && !Packet->IsAttackPacket(Env, Target->GetEntityID()))
			{
				PacketQueueHandlerPtr->QueuePacket(Env, PacketData);
				*ShouldCancel = true;
				return;
			}
		}
	}
	catch (...)
	{
		PacketQueueHandlerPtr->ProcessAndClearQueue(Env);
	}
}

void BackTrack::OnHandleStatusUpdate(JNIEnv* Env, const std::shared_ptr<Player>& TargetInstance, jbyte Byte, bool* ShouldCancel)
{
	try
	{
		if (!this->GetState()) {
			return;
		}

		if (TargetHandlerPtr->IsValid(Env) && TargetInstance->GetEntityID(Env) == TargetHandlerPtr->GetTarget()->GetEntityID())
		{
			ComboCount++;
			HitTime = GetTickCount64();

			return;
		}

		GameClasses ThreadClasses;
		if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses)) {
			return;
		}

		const auto& LocalPlayer = ThreadClasses.Player;
		const auto& Minecraft = ThreadClasses.Minecraft;
		const auto& World = ThreadClasses.World;
		const auto& EntityRenderer = Minecraft->GetEntityRendererPtr(Env);
		const auto& EntityPointed = EntityRenderer->GetPointedEntityPtr(Env);

		if (TargetInstance->GetEntityID(Env) != EntityPointed->GetEntityID(Env)) {
			return;
		}

		ComboCount++;
		HitTime = GetTickCount64();

		if (Mode == BACKTRACK_A && ComboCount < ComboThreshold) {
			return;
		}

		if (GetTickCount64() < static_cast<unsigned long long>(TargetHandlerPtr->GetClearTime()) + Cooldown) {
			return;
		}

		TargetHandlerPtr->SetTarget(Env, TargetInstance);
	}
	catch (...)
	{

	}
}

void BackTrack::OnRespawn(JNIEnv* Env)
{
	try
	{
		TargetHandlerPtr->ClearTarget();
		//PacketQueueHandlerPtr->ClearQueue(Env);
		PacketQueueHandlerPtr->ProcessAndClearQueue(Env);
	}
	catch (...)
	{
		PacketQueueHandlerPtr->ProcessAndClearQueue(Env);
	}
}

void BackTrack::OnRender(JNIEnv* Env)
{
	try
	{
		GameClasses ThreadClasses;
		if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses)) {
			return;
		}

		static float Alpha = 0.F;
		Alpha = std::lerp(Alpha, TargetHandlerPtr->IsValid(Env) ? 1.F : 0.F, 0.01F);

		if (!RenderServerRecord)
		{
			Alpha = 0.F;
			return;
		}

		static AxisAlignedBB_T<double> Bounds;
		static Vector3<double> Position;

		if (TargetHandlerPtr->IsValid(Env))
		{
			const auto Target = TargetHandlerPtr->GetTarget();
			Position = Target->GetPosition();
			Target->UpdatePositionTick(Env, LerpOffset);
			Bounds = Target->GetLatestTargetBounds();
		}
		else
		{
			const auto& Minecraft = ThreadClasses.Minecraft;
			const auto& Timer = Minecraft->GetTimerPtr(Env);
			const auto& RenderManager = Minecraft->GetRenderManagerPtr(Env);

			const auto RenderPartialTicks = Timer->GetRenderPartialTicks(Env);
			const auto RenderPosition = RenderManager->GetRenderPos(Env);

			Bounds = Bounds.GetBoundingBox3D(Position, Position, RenderPosition, RenderPartialTicks);
		}

		auto DrawOutlinedBox = [](const AxisAlignedBB_T<double>& BoundingBox)
			{
				glBegin(3);
				glVertex3f(BoundingBox.MinX, BoundingBox.MinY, BoundingBox.MinZ);
				glVertex3f(BoundingBox.MaxX, BoundingBox.MinY, BoundingBox.MinZ);
				glVertex3f(BoundingBox.MaxX, BoundingBox.MinY, BoundingBox.MaxZ);
				glVertex3f(BoundingBox.MinX, BoundingBox.MinY, BoundingBox.MaxZ);
				glVertex3f(BoundingBox.MinX, BoundingBox.MinY, BoundingBox.MinZ);
				glEnd();

				glBegin(3);
				glVertex3f(BoundingBox.MinX, BoundingBox.MaxY, BoundingBox.MinZ);
				glVertex3f(BoundingBox.MaxX, BoundingBox.MaxY, BoundingBox.MinZ);
				glVertex3f(BoundingBox.MaxX, BoundingBox.MaxY, BoundingBox.MaxZ);
				glVertex3f(BoundingBox.MinX, BoundingBox.MaxY, BoundingBox.MaxZ);
				glVertex3f(BoundingBox.MinX, BoundingBox.MaxY, BoundingBox.MinZ);
				glEnd();

				glBegin(1);
				glVertex3f(BoundingBox.MinX, BoundingBox.MinY, BoundingBox.MinZ);
				glVertex3f(BoundingBox.MinX, BoundingBox.MaxY, BoundingBox.MinZ);
				glVertex3f(BoundingBox.MaxX, BoundingBox.MinY, BoundingBox.MinZ);
				glVertex3f(BoundingBox.MaxX, BoundingBox.MaxY, BoundingBox.MinZ);
				glVertex3f(BoundingBox.MaxX, BoundingBox.MinY, BoundingBox.MaxZ);
				glVertex3f(BoundingBox.MaxX, BoundingBox.MaxY, BoundingBox.MaxZ);
				glVertex3f(BoundingBox.MinX, BoundingBox.MinY, BoundingBox.MaxZ);
				glVertex3f(BoundingBox.MinX, BoundingBox.MaxY, BoundingBox.MaxZ);
				glEnd();
			};

		auto DrawFilledBox = [](const AxisAlignedBB_T<double>& BoundingBox)
			{
				glBegin(GL_QUADS);

				glVertex3f(BoundingBox.MinX, BoundingBox.MinY, BoundingBox.MinZ);
				glVertex3f(BoundingBox.MinX, BoundingBox.MaxY, BoundingBox.MinZ);
				glVertex3f(BoundingBox.MaxX, BoundingBox.MaxY, BoundingBox.MinZ);
				glVertex3f(BoundingBox.MaxX, BoundingBox.MinY, BoundingBox.MinZ);

				glVertex3f(BoundingBox.MinX, BoundingBox.MinY, BoundingBox.MaxZ);
				glVertex3f(BoundingBox.MaxX, BoundingBox.MinY, BoundingBox.MaxZ);
				glVertex3f(BoundingBox.MaxX, BoundingBox.MaxY, BoundingBox.MaxZ);
				glVertex3f(BoundingBox.MinX, BoundingBox.MaxY, BoundingBox.MaxZ);

				glVertex3f(BoundingBox.MinX, BoundingBox.MinY, BoundingBox.MinZ);
				glVertex3f(BoundingBox.MinX, BoundingBox.MinY, BoundingBox.MaxZ);
				glVertex3f(BoundingBox.MinX, BoundingBox.MaxY, BoundingBox.MaxZ);
				glVertex3f(BoundingBox.MinX, BoundingBox.MaxY, BoundingBox.MinZ);

				glVertex3f(BoundingBox.MaxX, BoundingBox.MinY, BoundingBox.MinZ);
				glVertex3f(BoundingBox.MaxX, BoundingBox.MaxY, BoundingBox.MinZ);
				glVertex3f(BoundingBox.MaxX, BoundingBox.MaxY, BoundingBox.MaxZ);
				glVertex3f(BoundingBox.MaxX, BoundingBox.MinY, BoundingBox.MaxZ);

				glVertex3f(BoundingBox.MinX, BoundingBox.MaxY, BoundingBox.MinZ);
				glVertex3f(BoundingBox.MinX, BoundingBox.MaxY, BoundingBox.MaxZ);
				glVertex3f(BoundingBox.MaxX, BoundingBox.MaxY, BoundingBox.MaxZ);
				glVertex3f(BoundingBox.MaxX, BoundingBox.MaxY, BoundingBox.MinZ);

				glVertex3f(BoundingBox.MinX, BoundingBox.MinY, BoundingBox.MinZ);
				glVertex3f(BoundingBox.MaxX, BoundingBox.MinY, BoundingBox.MinZ);
				glVertex3f(BoundingBox.MaxX, BoundingBox.MinY, BoundingBox.MaxZ);
				glVertex3f(BoundingBox.MinX, BoundingBox.MinY, BoundingBox.MaxZ);

				glEnd();
			};

		const auto ActiveRenderInfo = ::ActiveRenderInfo(Env);

		glPushMatrix();
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(ActiveRenderInfo.GetProjection(Env).data());
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(ActiveRenderInfo.GetModelView(Env).data());

		glPushMatrix();
		glEnable(GL_LINE_SMOOTH);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);
		glDepthMask(false);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glLineWidth(2.f);


		glColor4f(OutlineColor.x, OutlineColor.y, OutlineColor.z, OutlineColor.w * Alpha);
		DrawOutlinedBox(Bounds);
		glColor4f(BoxColor.x, BoxColor.y, BoxColor.z, BoxColor.w * Alpha);
		DrawFilledBox(Bounds);

		glDisable(GL_BLEND);
		glDepthMask(true);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_LINE_SMOOTH);
		glPopMatrix();

		glPopMatrix();
	}
	catch (...)
	{

	}
}

std::shared_ptr<PacketQueueHandler> PacketQueueHandlerPtr = std::make_shared<PacketQueueHandler>();
std::shared_ptr<TargetHandler> TargetHandlerPtr = std::make_shared<TargetHandler>();