#include "pch.h"
#include "lagrange.hpp"
#include "../movement/blink.hpp"
#include <src/sdk/game/minecraft/activerenderinfo.hpp>
#include <GL/gl.h>

int TicksSinceRespawn = 0;

void LagRange::OnTick(JNIEnv* Env) 
{
	TicksSinceRespawn++;
}

void LagRange::OnPacketSend(JNIEnv* Env, const std::shared_ptr<PacketSendData>& PacketData, bool* ShouldCancel)
{
	const auto& Packet = PacketData->PacketInstance;
	const auto& NetHandler = PacketData->NetHandlerInstance;

	if (!this->GetState())
	{
		PacketReceiveQueueHandlerPtr->ProcessAndClearQueue(Env, NetHandler);
		return;
	}

	GameClasses ThreadClasses;
	if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses))
	{
		PacketReceiveQueueHandlerPtr->ProcessAndClearQueue(Env, NetHandler);
		return;
	}

	if (TicksSinceRespawn < 150)
	{
		PacketReceiveQueueHandlerPtr->ProcessAndClearQueue(Env, NetHandler);
		return;
	}

	auto IsBlockPlacementPacket = [Env, PacketData, ShouldCancel, this]() -> bool
	{
		const auto Packet = PacketData->PacketInstance;

		if (!Packet->IsBlockPlacementPacket(Env)) {
			return false;
		}

		const auto BlockPlacementPacket = std::make_shared<::BlockPlacementPacket>(Env, Packet->GetInstanceObject());
		const auto PacketItemStack = BlockPlacementPacket->GetStack(Env);

		if (!PacketItemStack->IsBlock(Env)) {
			return false;
		}

		return true;
	};

	const auto& LocalPlayer = ThreadClasses.Player;
	const auto& World = ThreadClasses.World;
	const auto& Minecraft = ThreadClasses.Minecraft;
	const auto& GuiScreen = Minecraft->GetCurrentScreenPtr(Env);

	if (GuiScreen->IsInMenu())
	{
		PacketReceiveQueueHandlerPtr->ProcessAndClearQueue(Env, NetHandler);
		return;
	}

	if (LocalPlayer->GetTicksExisted(Env) < 150)
	{
		return;
	}

	if (LocalPlayer->IsDead(Env))
	{
		PacketReceiveQueueHandlerPtr->ProcessAndClearQueue(Env, NetHandler);
		return;
	}

	if (LocalPlayer->IsSpectator(Env))
	{
		PacketReceiveQueueHandlerPtr->ProcessAndClearQueue(Env, NetHandler);
		return;
	}

	const auto ClosestPlayer = World->GetClosestPlayerWithinFovRadius(Env, LocalPlayer.get(), ActivationDistance, 90.F);
	//std::cout << LocalPlayer->GetFovTo(Env, ClosestPlayer.get()) << std::endl;
	if (ClosestPlayer == nullptr || Env->IsSameObject(ClosestPlayer->GetInstanceObject(), NULL))
	{
		PacketReceiveQueueHandlerPtr->ProcessAndClearQueue(Env, NetHandler);
		return;
	}

	if (IsBlockPlacementPacket())
	{
		PacketReceiveQueueHandlerPtr->ProcessAndClearQueue(Env, NetHandler);
		return;
	}

	if (Packet->IsUseEntityPacket(Env))
	{
		PacketReceiveQueueHandlerPtr->ProcessAndClearQueue(Env, NetHandler);
		return;
	}

	static int NextCooldown = RandomNumberGenerator().GenerateInt(Cooldown, Cooldown + 25);
	if (PacketReceiveQueueHandlerPtr->GetLastFlushTime() != 0 && GetTickCount64() - PacketReceiveQueueHandlerPtr->GetLastFlushTime() < NextCooldown)
	{
		NextCooldown = RandomNumberGenerator().GenerateInt(Cooldown, Cooldown + 25);
		PacketReceiveQueueHandlerPtr->ProcessAndClearQueue(Env, NetHandler);
		return;
	}

	static int NextDelay = RandomNumberGenerator().GenerateInt(Delay, Delay + 50);
	if (Packet->IsPlayerPacket(Env) && PacketReceiveQueueHandlerPtr->GetLastQueueTime() != 0 && GetTickCount64() - PacketReceiveQueueHandlerPtr->GetLastQueueTime() > NextDelay)
	{
		NextDelay = RandomNumberGenerator().GenerateInt(Delay, Delay + 50);
		PacketReceiveQueueHandlerPtr->ProcessAndClearQueue(Env, NetHandler);
		return;
	}

	const auto ServerDistance = ClosestPlayer->GetPos(Env).Distance(PacketReceiveQueueHandlerPtr->GetLastServerPosition());
	const auto LocalDistance = ClosestPlayer->GetPos(Env).Distance(LocalPlayer->GetPos(Env));

	if (ServerDistance < LocalDistance)
	{
		NextDelay = RandomNumberGenerator().GenerateInt(Delay, Delay + 50);
		PacketReceiveQueueHandlerPtr->ProcessAndClearQueue(Env, NetHandler);
		return;
	}
	
	PacketReceiveQueueHandlerPtr->QueuePacket(Env, PacketData);
	*ShouldCancel = true;
	return;
}

void LagRange::OnPacketReceive(JNIEnv* Env, const std::shared_ptr<PacketReceiveData>& PacketData, bool* ShouldCancel)
{
	const auto& Packet = PacketData->PacketInstance;
	const auto& NetworkManager = PacketData->NetworkManagerInstance;

	GameClasses ThreadClasses;
	if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses)) {
		return;
	}

	const auto& LocalPlayer = ThreadClasses.Player;
	const auto& World = ThreadClasses.World;
	const auto& Minecraft = ThreadClasses.Minecraft;

	if (Packet->IsEntityPacket(Env))
	{
		const auto EntityPacket = std::make_shared<::EntityPacket>(Env, Packet->GetInstanceObject());
		if (EntityPacket->GetEntityID(Env) == LocalPlayer->GetEntityID(Env)) 
		{
			TicksSinceRespawn = 0;
		}
	}

	const auto ClassName = GetJavaClassName(Env, Env->GetObjectClass(Packet->GetInstanceObject()));
	if (ClassName.contains("S01PacketJoinGame"))
	{
		TicksSinceRespawn = 0;
	}

	if (ClassName.contains("S05PacketSpawnPosition"))
	{
		TicksSinceRespawn = 0;
	}

	if (ClassName.contains("S07PacketRespawn"))
	{
		TicksSinceRespawn = 0;
	}

	if (ClassName.contains("S08PacketPlayerPosLook"))
	{
		TicksSinceRespawn = 0;
	}

	//if (Packet->IsAttackPacket(Env, LocalPlayer->GetEntityID(Env)))
	//{
	//	//TicksSinceRespawn = 0;
	//	std::cout << "AttkPkt" << std::endl;
	//}

	if (Packet->IsEntityTeleportPacket(Env))
	{
		const auto TeleportPacket = std::make_shared<EntityPacketTeleport>(Env, Packet->GetInstanceObject());
		if (TeleportPacket->GetEntityID(Env) == LocalPlayer->GetEntityID(Env))
		{
			//PacketReceiveQueueHandlerPtr->ClearQueue(Env);
			TicksSinceRespawn = 0;
		}
	}
}

void LagRange::OnRespawn(JNIEnv* Env)
{
	//TicksSinceRespawn = 0;
}

void LagRange::OnRender(JNIEnv* Env)
{
	try
	{
		GameClasses ThreadClasses;
		if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses)) {
			return;
		}

		static float Alpha = 0.F;
		Alpha = std::lerp(Alpha, PacketReceiveQueueHandlerPtr->ShouldRender() ? 1.F : 0.F, 0.01F);

		if (!PacketReceiveQueueHandlerPtr->ShouldRender())
		{
			Alpha = 0.F;
			return;
		}

		Vector3<double> Position = PacketReceiveQueueHandlerPtr->GetLastPosition();

		AxisAlignedBB_T<double> Bounds = AxisAlignedBB_T<double>
		{
			Position.X - 0.3,
			Position.Y,
			Position.Z - 0.3,
			Position.X + 0.3,
			Position.Y + 1.8,
			Position.Z + 0.3
		};

		const auto& Minecraft = ThreadClasses.Minecraft;
		const auto& Timer = Minecraft->GetTimerPtr(Env);
		const auto& GameSettings = Minecraft->GetGameSettingsPtr(Env);
		const auto& RenderManager = Minecraft->GetRenderManagerPtr(Env);

		if (GameSettings->GetThirdPersonView(Env) != 1)
		{
			Alpha = 0.F;
			return;
		}

		const auto RenderPartialTicks = Timer->GetRenderPartialTicks(Env);
		const auto RenderPosition = RenderManager->GetRenderPos(Env);

		Bounds = Bounds.GetBoundingBox3D(Position, Position, RenderPosition, RenderPartialTicks);

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

std::shared_ptr<PacketReceiveQueueHandler> PacketReceiveQueueHandlerPtr = std::make_shared<PacketReceiveQueueHandler>();