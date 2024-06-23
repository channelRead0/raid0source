#include "pch.h"

#include "packet.hpp"

#include "sdk/internal/register/envregister.hpp"

Packet::Packet(JNIEnv* Env)
{

	this->ChatMessagePacketMapper = this->ChatMessagePacketMapper ? this->ChatMessagePacketMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/client/C01PacketChatMessage");
	this->CustomPayloadPacketMapper = this->CustomPayloadPacketMapper ? this->CustomPayloadPacketMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/client/C17PacketCustomPayload");
	this->BlockPlacementPacketMapper = this->BlockPlacementPacketMapper ? this->BlockPlacementPacketMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/client/C08PacketPlayerBlockPlacement");

	this->PacketMapper = this->PacketMapper ? this->PacketMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/Packet");
	this->PacketUseEntityMapper = this->PacketUseEntityMapper ? this->PacketUseEntityMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/client/C02PacketUseEntity");
	this->VelocityPacketMapper = this->VelocityPacketMapper ? this->VelocityPacketMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/server/S12PacketEntityVelocity");

	this->AttackPacketMapper = this->AttackPacketMapper ? this->AttackPacketMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/server/S19PacketEntityStatus");
	this->PlayerDiggingPacketMapper = this->PlayerDiggingPacketMapper ? this->PlayerDiggingPacketMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/client/C07PacketPlayerDigging");
	this->EntityPacketMapper = this->EntityPacketMapper ? this->EntityPacketMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/server/S14PacketEntity");

	this->PacketLoginStartMapper = this->PacketLoginStartMapper ? this->PacketLoginStartMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/login/client/C00PacketLoginStart");
	this->EntityPacketEntityRelMovePacketMapper = this->EntityPacketEntityRelMovePacketMapper ? this->EntityPacketEntityRelMovePacketMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/server/S14PacketEntity$S15PacketEntityRelMove");
	this->EntityPacketEntityLookPacketMapper = this->EntityPacketEntityLookPacketMapper ? this->EntityPacketEntityLookPacketMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/server/S14PacketEntity$S16PacketEntityLook");

	this->EntityPacketEntityLookMoveMapper = this->EntityPacketEntityLookMoveMapper ? this->EntityPacketEntityLookMoveMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/server/S14PacketEntity$S17PacketEntityLookMove");
	this->EntityPacketTeleportMapper = this->EntityPacketTeleportMapper ? this->EntityPacketTeleportMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/server/S18PacketEntityTeleport");
	this->TeamsPacketMapper = this->TeamsPacketMapper ? this->TeamsPacketMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/server/S3EPacketTeams");

	this->SpawnPlayerPacketMapper = this->SpawnPlayerPacketMapper ? this->SpawnPlayerPacketMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/server/S0CPacketSpawnPlayer");
	this->ScoreBoardObjectivePacketMapper = this->ScoreBoardObjectivePacketMapper ? this->ScoreBoardObjectivePacketMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/server/S3BPacketScoreboardObjective");
	this->INetHandlerPlayClientMapper = this->INetHandlerPlayClientMapper ? this->INetHandlerPlayClientMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/INetHandlerPlayClient");

	this->PacketRespawnMapper = this->PacketRespawnMapper ? this->PacketRespawnMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/server/S07PacketRespawn"); 
	this->PacketPlayerMapper = this->PacketPlayerMapper ? this->PacketPlayerMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/client/C03PacketPlayer");
}

Packet::Packet(JNIEnv* Env, const jobject InstanceObject)
{
	this->ChatMessagePacketMapper = this->ChatMessagePacketMapper ? this->ChatMessagePacketMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/client/C01PacketChatMessage");
	this->CustomPayloadPacketMapper = this->CustomPayloadPacketMapper ? this->CustomPayloadPacketMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/client/C17PacketCustomPayload");
	this->BlockPlacementPacketMapper = this->BlockPlacementPacketMapper ? this->BlockPlacementPacketMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/client/C08PacketPlayerBlockPlacement");

	this->PacketMapper = this->PacketMapper ? this->PacketMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/Packet");
	this->PacketUseEntityMapper = this->PacketUseEntityMapper ? this->PacketUseEntityMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/client/C02PacketUseEntity");
	this->VelocityPacketMapper = this->VelocityPacketMapper ? this->VelocityPacketMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/server/S12PacketEntityVelocity");

	this->AttackPacketMapper = this->AttackPacketMapper ? this->AttackPacketMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/server/S19PacketEntityStatus");
	this->PlayerDiggingPacketMapper = this->PlayerDiggingPacketMapper ? this->PlayerDiggingPacketMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/client/C07PacketPlayerDigging");
	this->EntityPacketMapper = this->EntityPacketMapper ? this->EntityPacketMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/server/S14PacketEntity");

	this->PacketLoginStartMapper = this->PacketLoginStartMapper ? this->PacketLoginStartMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/login/client/C00PacketLoginStart");
	this->EntityPacketEntityRelMovePacketMapper = this->EntityPacketEntityRelMovePacketMapper ? this->EntityPacketEntityRelMovePacketMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/server/S14PacketEntity$S15PacketEntityRelMove");
	this->EntityPacketEntityLookPacketMapper = this->EntityPacketEntityLookPacketMapper ? this->EntityPacketEntityLookPacketMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/server/S14PacketEntity$S16PacketEntityLook");

	this->EntityPacketEntityLookMoveMapper = this->EntityPacketEntityLookMoveMapper ? this->EntityPacketEntityLookMoveMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/server/S14PacketEntity$S17PacketEntityLookMove");
	this->EntityPacketTeleportMapper = this->EntityPacketTeleportMapper ? this->EntityPacketTeleportMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/server/S18PacketEntityTeleport");
	this->TeamsPacketMapper = this->TeamsPacketMapper ? this->TeamsPacketMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/server/S3EPacketTeams");

	this->SpawnPlayerPacketMapper = this->SpawnPlayerPacketMapper ? this->SpawnPlayerPacketMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/server/S0CPacketSpawnPlayer");
	this->ScoreBoardObjectivePacketMapper = this->ScoreBoardObjectivePacketMapper ? this->ScoreBoardObjectivePacketMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/server/S3BPacketScoreboardObjective");
	this->INetHandlerPlayClientMapper = this->INetHandlerPlayClientMapper ? this->INetHandlerPlayClientMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/INetHandlerPlayClient");

	this->PacketRespawnMapper = this->PacketRespawnMapper ? this->PacketRespawnMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/server/S07PacketRespawn");
	this->PacketPlayerMapper = this->PacketPlayerMapper ? this->PacketPlayerMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/client/C03PacketPlayer");

	this->InstanceObject = InstanceObject;
}

void Packet::ProcessPacket(JNIEnv* Env, jobject INetHandlerObject) const
{
	if (!Env || !InstanceObject) {
		return;
	}

	PacketMapper->CallVoidMethod(Env, InstanceObject, "processPacket", INetHandlerObject);
}

bool Packet::IsRespawnPacket(JNIEnv* Env) const
{
	if (!Env || !InstanceObject) {
		return false;
	}

	return Env->IsInstanceOf(InstanceObject, PacketRespawnMapper->GetClass());
}

bool Packet::IsPlayerPacket(JNIEnv* Env) const
{
	if (!Env || !InstanceObject) {
		return false;
	}

	return Env->IsInstanceOf(InstanceObject, PacketPlayerMapper->GetClass());
}

bool Packet::IsPlayerServerPacket(JNIEnv* Env) const
{
	if (!Env || !InstanceObject) {
		return false;
	}

	const auto ClassName = GetJavaClassName(Env, Env->GetObjectClass(InstanceObject));

	return
	(
		ClassName.contains("C0EPacketClickWindow") || ClassName.contains("C0FPacketConfirmTransaction") || ClassName.contains("C10PacketCreativeInventoryAction") ||
		ClassName.contains("C11PacketEnchantItem") || ClassName.contains("C12PacketUpdateSign") || ClassName.contains("C13PacketPlayerAbilities") ||
		ClassName.contains("C14PacketTabComplete") || ClassName.contains("C15PacketClientSettings") || ClassName.contains("C16PacketClientStatus") ||
		ClassName.contains("C17PacketCustomPayload") || ClassName.contains("C18PacketSpectate") || ClassName.contains("C19PacketResourcePackStatus") ||
		ClassName.contains("C00PacketKeepAlive") || ClassName.contains("C0APacketAnimation") || ClassName.contains("C0BPacketEntityAction") ||
		ClassName.contains("C0CPacketInput") || ClassName.contains("C0DPacketCloseWindow") || ClassName.contains("C01PacketChatMessage") || 
		ClassName.contains("C02PacketUseEntity") || ClassName.contains("C03PacketPlayer") || ClassName.contains("C07PacketPlayerDigging") ||
		ClassName.contains("C08PacketPlayerBlockPlacement") || ClassName.contains("C09PacketHeldItemChange")
	);
}

bool Packet::IsPlayClientPacket(JNIEnv* Env) const
{
	if (!Env || !InstanceObject) {
		return false;
	}

	const auto ClassName = GetJavaClassName(Env, Env->GetObjectClass(InstanceObject));

	return
	(
		ClassName.contains("S0EPacketSpawnObject") || ClassName.contains("S11PacketSpawnExperienceOrb") || ClassName.contains("S2CPacketSpawnGlobalEntity") ||
		ClassName.contains("S0FPacketSpawnMob") || ClassName.contains("S3BPacketScoreboardObjective") || ClassName.contains("S10PacketSpawnPainting") ||
		ClassName.contains("S0CPacketSpawnPlayer") || ClassName.contains("S0BPacketAnimation") || ClassName.contains("S37PacketStatistics") ||
		ClassName.contains("S25PacketBlockBreakAnim") || ClassName.contains("S36PacketSignEditorOpen") || ClassName.contains("S35PacketUpdateTileEntity") ||
		ClassName.contains("S24PacketBlockAction") || ClassName.contains("S23PacketBlockChange") || ClassName.contains("S02PacketChat") ||
		ClassName.contains("S3APacketTabComplete") || ClassName.contains("S22PacketMultiBlockChange") || ClassName.contains("S34PacketMaps") ||
		ClassName.contains("S32PacketConfirmTransaction") || ClassName.contains("S2EPacketCloseWindow") || ClassName.contains("S30PacketWindowItems") ||
		ClassName.contains("S2DPacketOpenWindow") || ClassName.contains("S31PacketWindowProperty") || ClassName.contains("S2FPacketSetSlot") ||
		ClassName.contains("S3FPacketCustomPayload") || ClassName.contains("S0APacketUseBed") || ClassName.contains("S19PacketEntityStatus") ||
		ClassName.contains("S1BPacketEntityAttach") || ClassName.contains("S27PacketExplosion") || ClassName.contains("S2BPacketChangeGameState") ||
		ClassName.contains("S00PacketKeepAlive") || ClassName.contains("S21PacketChunkData") || ClassName.contains("S26PacketMapChunkBulk") ||
		ClassName.contains("S28PacketEffect") || ClassName.contains("S14PacketEntity") || ClassName.contains("S08PacketPlayerPosLook") ||
		ClassName.contains("S2APacketParticles") || ClassName.contains("S39PacketPlayerAbilities") || ClassName.contains("S38PacketPlayerListItem") ||
		ClassName.contains("S13PacketDestroyEntities") || ClassName.contains("S1EPacketRemoveEntityEffect") || ClassName.contains("S07PacketRespawn") ||
		ClassName.contains("S19PacketEntityHeadLook") || ClassName.contains("S09PacketHeldItemChange") || ClassName.contains("S3DPacketDisplayScoreboard") ||
		ClassName.contains("S1CPacketEntityMetadata") || ClassName.contains("S12PacketEntityVelocity") || ClassName.contains("S04PacketEntityEquipment") ||
		ClassName.contains("S1FPacketSetExperience") || ClassName.contains("S06PacketUpdateHealth") || ClassName.contains("S3EPacketTeams") ||
		ClassName.contains("S3CPacketUpdateScore") || ClassName.contains("S05PacketSpawnPosition") || ClassName.contains("S03PacketTimeUpdate") ||
		ClassName.contains("S33PacketUpdateSign") || ClassName.contains("S29PacketSoundEffect") || ClassName.contains("S0DPacketCollectItem") ||
		ClassName.contains("S18PacketEntityTeleport") || ClassName.contains("S20PacketEntityProperties") || ClassName.contains("S1DPacketEntityEffect") ||
		ClassName.contains("S42PacketCombatEvent") || ClassName.contains("S41PacketServerDifficulty") || ClassName.contains("S43PacketCamera") ||
		ClassName.contains("S44PacketWorldBorder") || ClassName.contains("S45PacketTitle") || ClassName.contains("S46PacketSetCompressionLevel") ||
		ClassName.contains("S47PacketPlayerListHeaderFooter") || ClassName.contains("S48PacketResourcePackSend") || ClassName.contains("S49PacketUpdateEntityNBT")
	);
}

bool Packet::IsTeamsPacket(JNIEnv* Env) const
{
	if (!Env || !InstanceObject) {
		return false;
	}

	return Env->IsInstanceOf(InstanceObject, TeamsPacketMapper->GetClass());
}

bool Packet::IsSpawnPlayerPacket(JNIEnv* Env) const
{
	if (!Env || !InstanceObject) {
		return false;
	}

	const auto ClassName = GetJavaClassName(Env, Env->GetObjectClass(InstanceObject));
	return ClassName.contains("S3EPacketTeams");//Env->IsInstanceOf(InstanceObject, SpawnPlayerPacketMapper->GetClass());
}

bool Packet::IsScoreboardObjectivePacket(JNIEnv* Env) const
{
	if (!Env || !InstanceObject) {
		return false;
	}

	return Env->IsInstanceOf(InstanceObject, ScoreBoardObjectivePacketMapper->GetClass());
}

bool Packet::IsLoginStartPacket(JNIEnv* Env) const
{
	if (!Env || !InstanceObject) {
		return false;
	}

	return Env->IsInstanceOf(InstanceObject, PacketLoginStartMapper->GetClass());
}

bool Packet::IsChatMessagePacket(JNIEnv* Env) const
{
	if (!Env || !InstanceObject) {
		return {};
	}

	return Env->IsInstanceOf(InstanceObject, ChatMessagePacketMapper->GetClass());
}

bool Packet::IsCustomPayloadPacket(JNIEnv* Env) const
{
	if (!Env || !InstanceObject) {
		return {};
	}

	return Env->IsInstanceOf(InstanceObject, CustomPayloadPacketMapper->GetClass());
}

bool Packet::IsBlockPlacementPacket(JNIEnv* Env) const
{
	if (!Env || !InstanceObject) {
		return {};
	}

	return Env->IsInstanceOf(InstanceObject, BlockPlacementPacketMapper->GetClass());
}

bool Packet::IsUseEntityPacket(JNIEnv* Env) const
{
	if (!Env || !InstanceObject) {
		return false;
	}

	return Env->IsInstanceOf(InstanceObject, PacketUseEntityMapper->GetClass());
}

bool Packet::IsVelocityPacket(JNIEnv* Env) const
{
	if (!Env || !InstanceObject) {
		return false;
	}

	return Env->IsInstanceOf(InstanceObject, VelocityPacketMapper->GetClass());
}

bool Packet::IsStatusPacket(JNIEnv* Env) const
{
	if (!Env || !InstanceObject) {
		return false;
	}

	return Env->IsInstanceOf(InstanceObject, AttackPacketMapper->GetClass());
}


bool Packet::IsAttackPacket(JNIEnv* Env, const int SelfEntityID) const
{
	if (!Env || !InstanceObject) {
		return false;
	}
	if (!Env->IsInstanceOf(InstanceObject, AttackPacketMapper->GetClass())) {
		return false;
	}

	const AttackPacket AttackPacket(Env, InstanceObject);

	/*
	if u want it to also check for ur local entity id, put it as parameter

	u can make this look better if u create a player instance here and call GetEntityID, but i was unsure of doing that
	*/

	return AttackPacket.GetEntityID(Env) == SelfEntityID && AttackPacket.GetLogicOpCode(Env) == static_cast<jbyte>(2);
}

bool Packet::IsPlayerDiggingPacket(JNIEnv* Env) const
{
	if (!Env || !InstanceObject) {
		return false;
	}

	if (Env->IsSameObject(InstanceObject, NULL)) {
		return false;
	}

	const auto PlayerDiggingMapper2 = std::make_shared<GameClass>(Env, "net/minecraft/network/play/client/C07PacketPlayerDigging");
	return Env->IsInstanceOf(InstanceObject, PlayerDiggingMapper2->GetClass());
}

bool Packet::IsEntityPacket(JNIEnv* Env) const
{
	if (!Env || !InstanceObject) {
		return false;
	}

	if (Env->IsSameObject(InstanceObject, NULL)) {
		return false;
	}

	return Env->IsInstanceOf(InstanceObject, EntityPacketMapper->GetClass());
}

bool Packet::IsEntityRelMovePacket(JNIEnv* Env) const
{
	if (!Env || !InstanceObject) {
		return false;
	}

	if (Env->IsSameObject(InstanceObject, NULL)) {
		return false;
	}

	return Env->IsInstanceOf(InstanceObject, EntityPacketEntityRelMovePacketMapper->GetClass());
}

bool Packet::IsEntityLookPacket(JNIEnv* Env) const
{
	if (!Env || !InstanceObject) {
		return false;
	}

	if (Env->IsSameObject(InstanceObject, NULL)) {
		return false;
	}

	return Env->IsInstanceOf(InstanceObject, EntityPacketEntityLookPacketMapper->GetClass());
}

bool Packet::IsEntityLookMovePacket(JNIEnv* Env) const
{
	if (!Env || !InstanceObject) {
		return false;
	}

	if (Env->IsSameObject(InstanceObject, NULL)) {
		return false;
	}

	return Env->IsInstanceOf(InstanceObject, EntityPacketEntityLookMoveMapper->GetClass());
}

bool Packet::IsEntityTeleportPacket(JNIEnv* Env) const
{
	if (!Env || !InstanceObject) {
		return false;
	}

	if (Env->IsSameObject(InstanceObject, NULL)) {
		return false;
	}

	return Env->IsInstanceOf(InstanceObject, EntityPacketTeleportMapper->GetClass());
}

bool Packet::IsInstanceOfINetHandlerPlayClient(JNIEnv* Env) const
{
	if (!Env || !InstanceObject) {
		return false;
	}

	if (Env->IsSameObject(InstanceObject, NULL)) {
		return false;
	}

	return Env->IsInstanceOf(InstanceObject, INetHandlerPlayClientMapper->GetClass());
}

Packet::~Packet()
{
	const auto Env = EnvRegister::GetThreadEnv();

	if (Env == nullptr) {
		return;
	}

	if (InstanceObject && !Env->IsSameObject(InstanceObject, NULL))
	{
		const auto ReferenceType = Env->GetObjectRefType(InstanceObject);

		if (ReferenceType == JNIGlobalRefType || ReferenceType == JNIWeakGlobalRefType) { Env->DeleteGlobalRef(InstanceObject); }
		if (ReferenceType == JNILocalRefType) { Env->DeleteLocalRef(InstanceObject); }
		//Env->DeleteGlobalRef(InstanceObject);
	}

	InstanceObject = nullptr;
}

// ATTACK

AttackPacket::AttackPacket(JNIEnv* Env)
{
	this->AttackPacketMapper = std::make_shared<GameClass>(Env, "net/minecraft/network/play/server/S19PacketEntityStatus");
}

AttackPacket::AttackPacket(JNIEnv* Env, const jobject InstanceObject)
{
	this->AttackPacketMapper = std::make_shared<GameClass>(Env, "net/minecraft/network/play/server/S19PacketEntityStatus");
	this->InstanceObject = InstanceObject;
}

int AttackPacket::GetEntityID(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return 0;
	}

	return AttackPacketMapper->GetIntField(Env, InstanceObject, "entityId");
}

jbyte AttackPacket::GetLogicOpCode(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return (jbyte)0;
	}

	return AttackPacketMapper->GetByteField(Env, InstanceObject, "logicOpcode");
}

AttackPacket::~AttackPacket()
{
	const auto Env = EnvRegister::GetThreadEnv();

	if (Env == nullptr) {
		return;
	}

	if (InstanceObject && !Env->IsSameObject(InstanceObject, NULL))
	{
		/*const auto ReferenceType = Env->GetObjectRefType(InstanceObject);

		if (ReferenceType == JNIGlobalRefType || ReferenceType == JNIWeakGlobalRefType) { Env->DeleteGlobalRef(InstanceObject); }
		if (ReferenceType == JNILocalRefType) { Env->DeleteLocalRef(InstanceObject); }*/
	}

	InstanceObject = nullptr;
}

// VELOCITY

VelocityPacket::VelocityPacket(JNIEnv* Env)
{
	this->VelocityPacketMapper = std::make_shared<GameClass>(Env, "net/minecraft/network/play/server/S12PacketEntityVelocity");
}

VelocityPacket::VelocityPacket(JNIEnv* Env, const jobject InstanceObject)
{
	this->VelocityPacketMapper = std::make_shared<GameClass>(Env, "net/minecraft/network/play/server/S12PacketEntityVelocity");
	this->InstanceObject = InstanceObject;
}

int VelocityPacket::GetEntityID(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return 0;
	}

	return VelocityPacketMapper->GetIntField(Env, InstanceObject, "entityID");
}

int VelocityPacket::GetMotionX(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return 0;
	}

	return VelocityPacketMapper->GetIntField(Env, InstanceObject, "motionX");
}

int VelocityPacket::GetMotionY(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return 0;
	}

	return VelocityPacketMapper->GetIntField(Env, InstanceObject, "motionY");
}

int VelocityPacket::GetMotionZ(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return 0;
	}

	return VelocityPacketMapper->GetIntField(Env, InstanceObject, "motionZ");
}

void VelocityPacket::SetMotionX(JNIEnv* Env, const int MotionX) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	return VelocityPacketMapper->SetIntField(Env, InstanceObject, "motionX", MotionX);
}

void VelocityPacket::SetMotionY(JNIEnv* Env, const int MotionY) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	return VelocityPacketMapper->SetIntField(Env, InstanceObject, "motionY", MotionY);
}

void VelocityPacket::SetMotionZ(JNIEnv* Env, const int MotionZ) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	return VelocityPacketMapper->SetIntField(Env, InstanceObject, "motionZ", MotionZ);
}

VelocityPacket::~VelocityPacket()
{
	const auto Env = EnvRegister::GetThreadEnv();

	if (Env == nullptr) {
		return;
	}

	if (InstanceObject && !Env->IsSameObject(InstanceObject, NULL))
	{
		/*const auto ReferenceType = Env->GetObjectRefType(InstanceObject);

		if (ReferenceType == JNIGlobalRefType || ReferenceType == JNIWeakGlobalRefType) { Env->DeleteGlobalRef(InstanceObject); }
		if (ReferenceType == JNILocalRefType) { Env->DeleteLocalRef(InstanceObject); }*/
	}

	InstanceObject = nullptr;
}

EntityPacket::EntityPacket(JNIEnv* Env)
{
	this->EntityPacketMapper = this->EntityPacketMapper ? this->EntityPacketMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/server/S14PacketEntity");
}

EntityPacket::EntityPacket(JNIEnv* Env, jobject InstanceObject)
{
	this->EntityPacketMapper = this->EntityPacketMapper ? this->EntityPacketMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/server/S14PacketEntity");
	this->InstanceObject = InstanceObject;
}

int EntityPacket::GetEntityID(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return 0;
	}

	return EntityPacketMapper->GetIntField(Env, InstanceObject, "entityId");
}

Vector3<jbyte> EntityPacket::GetPosition(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return {};
	}

	return
	{
		EntityPacketMapper->GetByteField(Env, InstanceObject, "posX"),
		EntityPacketMapper->GetByteField(Env, InstanceObject, "posY"),
		EntityPacketMapper->GetByteField(Env, InstanceObject, "posZ"),
	};
}

EntityPacket::~EntityPacket()
{
	const auto Env = EnvRegister::GetThreadEnv();

	if (Env == nullptr) {
		return;
	}

	if (InstanceObject && !Env->IsSameObject(InstanceObject, NULL))
	{
		/*const auto ReferenceType = Env->GetObjectRefType(InstanceObject);

		if (ReferenceType == JNIGlobalRefType || ReferenceType == JNIWeakGlobalRefType) { Env->DeleteGlobalRef(InstanceObject); }
		if (ReferenceType == JNILocalRefType) { Env->DeleteLocalRef(InstanceObject); }*/
	}

	InstanceObject = nullptr;
}

EntityPacketEntityRelMove::EntityPacketEntityRelMove(JNIEnv* Env): EntityPacket(Env) { }

EntityPacketEntityRelMove::EntityPacketEntityRelMove(JNIEnv* Env, jobject InstanceObject): EntityPacket(Env, InstanceObject) { }

Vector3<double> EntityPacketEntityRelMove::GetDeltaPosition(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return {};
	}

	constexpr auto Divisor = static_cast<jbyte>(32);

	const auto PosX = static_cast<double>(EntityPacketMapper->GetByteField(Env, InstanceObject, "posX")) / Divisor;
	const auto PosY = static_cast<double>(EntityPacketMapper->GetByteField(Env, InstanceObject, "posY")) / Divisor;
	const auto PosZ = static_cast<double>(EntityPacketMapper->GetByteField(Env, InstanceObject, "posZ")) / Divisor;

	return Vector3<double>
	{
		static_cast<double>(PosX),
		static_cast<double>(PosY),
		static_cast<double>(PosZ)
	};
}

bool EntityPacketEntityRelMove::OnGround(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return {};
	}

	return EntityPacketMapper->GetBooleanField(Env, InstanceObject, "onGround");
}

EntityPacketEntityLook::EntityPacketEntityLook(JNIEnv* Env) : EntityPacket(Env) { }

EntityPacketEntityLook::EntityPacketEntityLook(JNIEnv* Env, jobject InstanceObject) : EntityPacket(Env, InstanceObject) { }

float EntityPacketEntityLook::Yaw(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return {};
	}

	constexpr auto Divisor = static_cast<jbyte>(256);
	const auto Yaw = (static_cast<float>(EntityPacketMapper->GetByteField(Env, InstanceObject, "yaw")) / Divisor) * 360.F;

	return Yaw;
}

float EntityPacketEntityLook::Pitch(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return {};
	}

	constexpr auto Divisor = static_cast<jbyte>(256);
	const auto Pitch = (static_cast<float>(EntityPacketMapper->GetByteField(Env, InstanceObject, "pitch")) / Divisor) * 360.F;

	return Pitch;
}

EntityPacketEntityLookMove::EntityPacketEntityLookMove(JNIEnv* Env) : EntityPacket(Env) { }

EntityPacketEntityLookMove::EntityPacketEntityLookMove(JNIEnv* Env, jobject InstanceObject) : EntityPacket(Env, InstanceObject) { }

Vector3<double> EntityPacketEntityLookMove::GetDeltaPosition(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return {};
	}

	constexpr auto Divisor = static_cast<jbyte>(32);

	const auto PosX = static_cast<double>(EntityPacketMapper->GetByteField(Env, InstanceObject, "posX")) / Divisor;
	const auto PosY = static_cast<double>(EntityPacketMapper->GetByteField(Env, InstanceObject, "posY")) / Divisor;
	const auto PosZ = static_cast<double>(EntityPacketMapper->GetByteField(Env, InstanceObject, "posZ")) / Divisor;

	return Vector3<double>
	{
		static_cast<double>(PosX),
			static_cast<double>(PosY),
			static_cast<double>(PosZ)
	};
}

bool EntityPacketEntityLookMove::OnGround(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return {};
	}

	return EntityPacketMapper->GetBooleanField(Env, InstanceObject, "onGround");
}

float EntityPacketEntityLookMove::Yaw(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return {};
	}

	constexpr auto Divisor = static_cast<jbyte>(256);
	const auto Yaw = (static_cast<float>(EntityPacketMapper->GetByteField(Env, InstanceObject, "yaw")) / Divisor) * 360.F;

	return Yaw;
}

float EntityPacketEntityLookMove::Pitch(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return {};
	}

	constexpr auto Divisor = static_cast<jbyte>(256);
	const auto Pitch = (static_cast<float>(EntityPacketMapper->GetByteField(Env, InstanceObject, "pitch")) / Divisor) * 360.F;

	return Pitch;
}

EntityPacketTeleport::EntityPacketTeleport(JNIEnv* Env)
{
	this->EntityPacketTeleportMapper = this->EntityPacketTeleportMapper ? this->EntityPacketTeleportMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/server/S18PacketEntityTeleport");
}

EntityPacketTeleport::EntityPacketTeleport(JNIEnv* Env, jobject InstanceObject)
{
	this->EntityPacketTeleportMapper = this->EntityPacketTeleportMapper ? this->EntityPacketTeleportMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/server/S18PacketEntityTeleport");
	this->InstanceObject = InstanceObject;
}

Vector3<int> EntityPacketTeleport::GetPosition(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return {};
	}

	return Vector3<int>
	{
		EntityPacketTeleportMapper->GetIntField(Env, InstanceObject, "posX"),
		EntityPacketTeleportMapper->GetIntField(Env, InstanceObject, "posY"),
		EntityPacketTeleportMapper->GetIntField(Env, InstanceObject, "posZ"),
	};
}

int EntityPacketTeleport::GetEntityID(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return {};
	}

	return EntityPacketTeleportMapper->GetIntField(Env, InstanceObject, "entityId");
}

bool EntityPacketTeleport::OnGround(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return {};
	}

	return EntityPacketTeleportMapper->GetBooleanField(Env, InstanceObject, "onGround");
}

float EntityPacketTeleport::Yaw(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return {};
	}

	constexpr auto Divisor = static_cast<jbyte>(256);
	const auto Yaw = (static_cast<float>(EntityPacketTeleportMapper->GetByteField(Env, InstanceObject, "yaw")) / Divisor) * 360.F;

	return Yaw;
}

float EntityPacketTeleport::Pitch(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return {};
	}

	constexpr auto Divisor = static_cast<jbyte>(256);
	const auto Pitch = (static_cast<float>(EntityPacketTeleportMapper->GetByteField(Env, InstanceObject, "pitch")) / Divisor) * 360.F;

	return Pitch;
}

EntityPacketTeleport::~EntityPacketTeleport()
{
	const auto Env = EnvRegister::GetThreadEnv();

	if (Env == nullptr) {
		return;
	}

	if (InstanceObject && !Env->IsSameObject(InstanceObject, NULL))
	{
		/*const auto ReferenceType = Env->GetObjectRefType(InstanceObject);

		if (ReferenceType == JNIGlobalRefType || ReferenceType == JNIWeakGlobalRefType) { Env->DeleteGlobalRef(InstanceObject); }
		if (ReferenceType == JNILocalRefType) { Env->DeleteLocalRef(InstanceObject); }*/
	}

	InstanceObject = nullptr;
}

PacketUseEntity::PacketUseEntity(JNIEnv* Env)
{
	this->PacketUseEntityMapper = this->PacketUseEntityMapper ? this->PacketUseEntityMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/client/C02PacketUseEntity");
	this->PacketUseEntityActionMapper = this->PacketUseEntityActionMapper ? this->PacketUseEntityActionMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/client/C02PacketUseEntity$Action");
}

PacketUseEntity::PacketUseEntity(JNIEnv* Env, jobject InstanceObject)
{
	this->PacketUseEntityMapper = this->PacketUseEntityMapper ? this->PacketUseEntityMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/client/C02PacketUseEntity");
	this->PacketUseEntityActionMapper = this->PacketUseEntityActionMapper ? this->PacketUseEntityActionMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/client/C02PacketUseEntity$Action");
	this->InstanceObject = InstanceObject;
}

bool PacketUseEntity::IsAttack(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return {};
	}

	const auto ATTACKObject = PacketUseEntityActionMapper->GetObjectField(Env, InstanceObject, "ATTACK");
	const auto TypeObject = PacketUseEntityMapper->CallObjectMethod(Env, InstanceObject, "getAction");

	return Env->IsSameObject(ATTACKObject, TypeObject);
}

int PacketUseEntity::GetEntityID(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return {};
	}

	return PacketUseEntityMapper->GetIntField(Env, InstanceObject, "entityId");
}



PacketUseEntity::~PacketUseEntity()
{
}

BlockPlacementPacket::BlockPlacementPacket(JNIEnv* Env)
{
	this->BlockPlacementPacketMapper = this->BlockPlacementPacketMapper ? this->BlockPlacementPacketMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/client/C08PacketPlayerBlockPlacement");
}

BlockPlacementPacket::BlockPlacementPacket(JNIEnv* Env, jobject InstanceObject)
{
	this->BlockPlacementPacketMapper = this->BlockPlacementPacketMapper ? this->BlockPlacementPacketMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/client/C08PacketPlayerBlockPlacement");
	this->InstanceObject = InstanceObject;
}

std::shared_ptr<ItemStack> BlockPlacementPacket::GetStack(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<ItemStack>(Env, jobject());
	}

	if (const auto ItemStackObject = BlockPlacementPacketMapper->CallObjectMethod(Env, InstanceObject, "getStack")) {
		return std::make_shared<ItemStack>(Env, ItemStackObject);
	}

	return std::make_shared<ItemStack>(Env, jobject());
}

BlockPlacementPacket::~BlockPlacementPacket()
{
}

LoginStartPacket::LoginStartPacket(JNIEnv* Env)
{
	this->PacketLoginStartMapper = this->PacketLoginStartMapper ? this->PacketLoginStartMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/login/client/C00PacketLoginStart");
}

LoginStartPacket::LoginStartPacket(JNIEnv* Env, jobject InstanceObject)
{
	this->PacketLoginStartMapper = this->PacketLoginStartMapper ? this->PacketLoginStartMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/login/client/C00PacketLoginStart");
}

LoginStartPacket::LoginStartPacket(JNIEnv* Env, jobject GameProfile, bool GameProfileInitializer)
{
	this->PacketLoginStartMapper = this->PacketLoginStartMapper ? this->PacketLoginStartMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/login/client/C00PacketLoginStart");

	const auto InitializerMethodID = Env->GetMethodID(PacketLoginStartMapper->GetClass(), "<init>", "(Lcom/mojang/authlib/GameProfile;)V");
	this->InstanceObject = Env->NewObject(PacketLoginStartMapper->GetClass(), InitializerMethodID, GameProfile);
}

LoginStartPacket::~LoginStartPacket()
{
}

TeamsPacket::TeamsPacket(JNIEnv* Env)
{
	this->TeamsPacketMapper = this->TeamsPacketMapper ? this->TeamsPacketMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/server/S3EPacketTeams");
	//this->SpawnPlayerPacketMapper = this->SpawnPlayerPacketMapper ? this->SpawnPlayerPacketMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/server/S0CPacketSpawnPlayer");
	//this->ScoreBoardObjectivePacketMapper = this->ScoreBoardObjectivePacketMapper ? this->ScoreBoardObjectivePacketMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/server/S3BPacketScoreboardObjective");
}

TeamsPacket::TeamsPacket(JNIEnv* Env, jobject InstanceObject)
{
	this->TeamsPacketMapper = this->TeamsPacketMapper ? this->TeamsPacketMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/server/S3EPacketTeams");
	this->InstanceObject = InstanceObject;
}

jobject TeamsPacket::GetPlayers(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return {};
	}

	return TeamsPacketMapper->CallObjectMethod(Env, InstanceObject, "getPlayers");
}

TeamsPacket::~TeamsPacket()
{
	const auto Env = EnvRegister::GetThreadEnv();

	if (Env == nullptr) {
		return;
	}

	if (InstanceObject && !Env->IsSameObject(InstanceObject, NULL))
	{
		/*const auto ReferenceType = Env->GetObjectRefType(InstanceObject);

		if (ReferenceType == JNIGlobalRefType || ReferenceType == JNIWeakGlobalRefType) { Env->DeleteGlobalRef(InstanceObject); }
		if (ReferenceType == JNILocalRefType) { Env->DeleteLocalRef(InstanceObject); }*/
	}

	InstanceObject = nullptr;
}

SpawnPlayerPacket::SpawnPlayerPacket(JNIEnv* Env)
{
	//this->TeamsPacketMapper = this->TeamsPacketMapper ? this->TeamsPacketMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/server/S3EPacketTeams");
	this->SpawnPlayerPacketMapper = this->SpawnPlayerPacketMapper ? this->SpawnPlayerPacketMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/server/S0CPacketSpawnPlayer");
	//this->ScoreBoardObjectivePacketMapper = this->ScoreBoardObjectivePacketMapper ? this->ScoreBoardObjectivePacketMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/server/S3BPacketScoreboardObjective");
}

SpawnPlayerPacket::SpawnPlayerPacket(JNIEnv* Env, jobject InstanceObject)
{
	this->SpawnPlayerPacketMapper = this->SpawnPlayerPacketMapper ? this->SpawnPlayerPacketMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/server/S0CPacketSpawnPlayer");
	this->InstanceObject = InstanceObject;
}

jobject SpawnPlayerPacket::GetPlayer(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return {};
	}

	return SpawnPlayerPacketMapper->CallObjectMethod(Env, InstanceObject, "getPlayer");
}

SpawnPlayerPacket::~SpawnPlayerPacket()
{
	const auto Env = EnvRegister::GetThreadEnv();

	if (Env == nullptr) {
		return;
	}

	if (InstanceObject && !Env->IsSameObject(InstanceObject, NULL))
	{
		/*const auto ReferenceType = Env->GetObjectRefType(InstanceObject);

		if (ReferenceType == JNIGlobalRefType || ReferenceType == JNIWeakGlobalRefType) { Env->DeleteGlobalRef(InstanceObject); }
		if (ReferenceType == JNILocalRefType) { Env->DeleteLocalRef(InstanceObject); }*/
	}

	InstanceObject = nullptr;
}

ScoreboardObjectivePacket::ScoreboardObjectivePacket(JNIEnv* Env)
{
	//this->TeamsPacketMapper = this->TeamsPacketMapper ? this->TeamsPacketMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/server/S3EPacketTeams");
	//this->SpawnPlayerPacketMapper = this->SpawnPlayerPacketMapper ? this->SpawnPlayerPacketMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/server/S0CPacketSpawnPlayer");
	this->ScoreboardObjectivePacketMapper = this->ScoreboardObjectivePacketMapper ? this->ScoreboardObjectivePacketMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/server/S3BPacketScoreboardObjective");
}

ScoreboardObjectivePacket::ScoreboardObjectivePacket(JNIEnv* Env, jobject InstanceObject)
{
	this->ScoreboardObjectivePacketMapper = this->ScoreboardObjectivePacketMapper ? this->ScoreboardObjectivePacketMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/server/S3BPacketScoreboardObjective");
	this->InstanceObject = InstanceObject;
}

jobject ScoreboardObjectivePacket::func_149337_d(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return {};
	}

	return ScoreboardObjectivePacketMapper->CallObjectMethod(Env, InstanceObject, "func_149337_d");
}

ScoreboardObjectivePacket::~ScoreboardObjectivePacket()
{
	const auto Env = EnvRegister::GetThreadEnv();

	if (Env == nullptr) {
		return;
	}

	if (InstanceObject && !Env->IsSameObject(InstanceObject, NULL))
	{
		/*const auto ReferenceType = Env->GetObjectRefType(InstanceObject);

		if (ReferenceType == JNIGlobalRefType || ReferenceType == JNIWeakGlobalRefType) { Env->DeleteGlobalRef(InstanceObject); }
		if (ReferenceType == JNILocalRefType) { Env->DeleteLocalRef(InstanceObject); }*/
	}

	InstanceObject = nullptr;
}

PacketPlayer::PacketPlayer(JNIEnv* Env)
{
	this->PacketPlayerMapper = this->PacketPlayerMapper ? this->PacketPlayerMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/client/C03PacketPlayer");
}

PacketPlayer::PacketPlayer(JNIEnv* Env, jobject InstanceObject)
{
	this->PacketPlayerMapper = this->PacketPlayerMapper ? this->PacketPlayerMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/play/client/C03PacketPlayer");
	this->InstanceObject = InstanceObject;
}

Vector3<double> PacketPlayer::GetPosition(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return {};
	}

	return Vector3<double>
	(
		PacketPlayerMapper->CallDoubleMethod(Env, InstanceObject, "getPositionX"),
		PacketPlayerMapper->CallDoubleMethod(Env, InstanceObject, "getPositionY"),
		PacketPlayerMapper->CallDoubleMethod(Env, InstanceObject, "getPositionZ")
	);
}

PacketPlayer::~PacketPlayer()
{
}
