#pragma once

#include "player.hpp"
#include "src/sdk/internal/templates/classbase.hpp"

class Packet : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> ChatMessagePacketMapper = nullptr;
	inline static std::shared_ptr<GameClass> CustomPayloadPacketMapper = nullptr;
	inline static std::shared_ptr<GameClass> BlockPlacementPacketMapper = nullptr;

	inline static std::shared_ptr<GameClass> PacketMapper = nullptr;
	inline static std::shared_ptr<GameClass> PacketUseEntityMapper = nullptr;
	inline static std::shared_ptr<GameClass> VelocityPacketMapper = nullptr;

	inline static std::shared_ptr<GameClass> AttackPacketMapper = nullptr;
	inline static std::shared_ptr<GameClass> PlayerDiggingPacketMapper = nullptr;
	inline static std::shared_ptr<GameClass> EntityPacketMapper = nullptr;

	inline static std::shared_ptr<GameClass> PacketLoginStartMapper = nullptr;
	inline static std::shared_ptr<GameClass> EntityPacketEntityRelMovePacketMapper = nullptr;
	inline static std::shared_ptr<GameClass> EntityPacketEntityLookPacketMapper = nullptr;

	inline static std::shared_ptr<GameClass> EntityPacketEntityLookMoveMapper = nullptr;
	inline static std::shared_ptr<GameClass> EntityPacketTeleportMapper = nullptr;
	inline static std::shared_ptr<GameClass> TeamsPacketMapper = nullptr;

	inline static std::shared_ptr<GameClass> SpawnPlayerPacketMapper = nullptr;
	inline static std::shared_ptr<GameClass> ScoreBoardObjectivePacketMapper = nullptr;
	inline static std::shared_ptr<GameClass> INetHandlerPlayClientMapper = nullptr;

	inline static std::shared_ptr<GameClass> PacketRespawnMapper = nullptr;
	inline static std::shared_ptr<GameClass> PacketPlayerMapper = nullptr;
public:
	Packet(JNIEnv* Env);
	Packet(JNIEnv* Env, jobject InstanceObject);

	void ProcessPacket(JNIEnv* Env, jobject INetHandlerObject) const;

	bool IsRespawnPacket(JNIEnv* Env) const;
	bool IsPlayerPacket(JNIEnv* Env) const;
	bool IsPlayerServerPacket(JNIEnv* Env) const;
	bool IsPlayClientPacket(JNIEnv* Env) const;
	bool IsTeamsPacket(JNIEnv* Env) const;
	bool IsSpawnPlayerPacket(JNIEnv* Env) const;
	bool IsScoreboardObjectivePacket(JNIEnv* Env) const;
	bool IsLoginStartPacket(JNIEnv* Env) const;
	bool IsChatMessagePacket(JNIEnv* Env) const;
	bool IsCustomPayloadPacket(JNIEnv* Env) const;
	bool IsBlockPlacementPacket(JNIEnv* Env) const;
	bool IsUseEntityPacket(JNIEnv* Env) const;
	bool IsVelocityPacket(JNIEnv* Env) const;
	bool IsStatusPacket(JNIEnv* Env) const;
	bool IsAttackPacket(JNIEnv* Env, int SelfEntityID = -999) const;
	bool IsPlayerDiggingPacket(JNIEnv* Env) const;
	bool IsEntityPacket(JNIEnv* Env) const;
	bool IsEntityRelMovePacket(JNIEnv* Env) const;
	bool IsEntityLookPacket(JNIEnv* Env) const;
	bool IsEntityLookMovePacket(JNIEnv* Env) const;
	bool IsEntityTeleportPacket(JNIEnv* Env) const;
	bool IsInstanceOfINetHandlerPlayClient(JNIEnv* Env) const;

	~Packet();
};

class BlockPlacementPacket : public ClassBase
{
private:
	std::shared_ptr<GameClass> BlockPlacementPacketMapper = nullptr;
public:
	BlockPlacementPacket(JNIEnv* Env);
	BlockPlacementPacket(JNIEnv* Env, jobject InstanceObject);

	std::shared_ptr<ItemStack> GetStack(JNIEnv* Env) const;

	~BlockPlacementPacket();
};

class PacketUseEntity : public ClassBase {
private:
	std::shared_ptr<GameClass> PacketUseEntityMapper = nullptr;
	std::shared_ptr<GameClass> PacketUseEntityActionMapper = nullptr;

public:
	PacketUseEntity(JNIEnv* Env);
	PacketUseEntity(JNIEnv* Env, jobject InstanceObject);

	bool IsAttack(JNIEnv* Env) const;
	int GetEntityID(JNIEnv* Env) const;

	~PacketUseEntity();
};

class AttackPacket : public ClassBase {
private:
	std::shared_ptr<GameClass> AttackPacketMapper = nullptr;

public:
	AttackPacket(JNIEnv* Env);
	AttackPacket(JNIEnv* Env, jobject InstanceObject);

	int GetEntityID(JNIEnv* Env) const;
	jbyte GetLogicOpCode(JNIEnv* Env) const;

	~AttackPacket();
};

class VelocityPacket : public ClassBase {
private:
	std::shared_ptr<GameClass> VelocityPacketMapper = nullptr;

public:
	VelocityPacket(JNIEnv* Env);
	VelocityPacket(JNIEnv* Env, jobject InstanceObject);

	int GetEntityID(JNIEnv* Env) const;

	int GetMotionX(JNIEnv* Env) const;
	int GetMotionY(JNIEnv* Env) const;
	int GetMotionZ(JNIEnv* Env) const;
	void SetMotionX(JNIEnv* Env, int MotionX) const;
	void SetMotionY(JNIEnv* Env, int MotionY) const;
	void SetMotionZ(JNIEnv* Env, int MotionZ) const;

	~VelocityPacket();
};

class EntityPacket : public ClassBase
{
protected:
	inline static std::shared_ptr<GameClass> EntityPacketMapper = nullptr;
public:
	EntityPacket(JNIEnv* Env);
	EntityPacket(JNIEnv* Env, jobject InstanceObject);

	int GetEntityID(JNIEnv* Env) const;
	Vector3<jbyte> GetPosition(JNIEnv* Env) const;

	~EntityPacket();
};

class EntityPacketEntityRelMove : public EntityPacket
{
public:
	explicit EntityPacketEntityRelMove(JNIEnv* Env);
	EntityPacketEntityRelMove(JNIEnv* Env, jobject InstanceObject);

	Vector3<double> GetDeltaPosition(JNIEnv* Env) const;
	bool OnGround(JNIEnv* Env) const;
};

class EntityPacketEntityLook : public EntityPacket
{
public:
	explicit EntityPacketEntityLook(JNIEnv* Env);
	EntityPacketEntityLook(JNIEnv* Env, jobject InstanceObject);

	float Yaw(JNIEnv* Env) const;
	float Pitch(JNIEnv* Env) const;
};

class EntityPacketEntityLookMove : public EntityPacket
{
public:
	explicit EntityPacketEntityLookMove(JNIEnv* Env);
	EntityPacketEntityLookMove(JNIEnv* Env, jobject InstanceObject);

	Vector3<double> GetDeltaPosition(JNIEnv* Env) const;
	bool OnGround(JNIEnv* Env) const;

	float Yaw(JNIEnv* Env) const;
	float Pitch(JNIEnv* Env) const;
};

class EntityPacketTeleport : public ClassBase
{
private:
	std::shared_ptr<GameClass> EntityPacketTeleportMapper = nullptr;

public:
	EntityPacketTeleport(JNIEnv* Env);
	EntityPacketTeleport(JNIEnv* Env, jobject InstanceObject);

	Vector3<int> GetPosition(JNIEnv* Env) const;
	int GetEntityID(JNIEnv* Env) const;
	bool OnGround(JNIEnv* Env) const;
	float Yaw(JNIEnv* Env) const;
	float Pitch(JNIEnv* Env) const;

	~EntityPacketTeleport();
};

class LoginStartPacket : public ClassBase
{
private:
	std::shared_ptr<GameClass> PacketLoginStartMapper = nullptr;

public:
	LoginStartPacket(JNIEnv* Env);
	LoginStartPacket(JNIEnv* Env, jobject InstanceObject);
	LoginStartPacket(JNIEnv* Env, jobject GameProfile, bool GameProfileInitializer = true);

	~LoginStartPacket();
};

class TeamsPacket : public ClassBase
{
private:
	std::shared_ptr<GameClass> TeamsPacketMapper = nullptr;

public:
	TeamsPacket(JNIEnv* Env);
	TeamsPacket(JNIEnv* Env, jobject InstanceObject);

	jobject GetPlayers(JNIEnv* Env) const;

	~TeamsPacket();
};

class SpawnPlayerPacket : public ClassBase
{
private:
	std::shared_ptr<GameClass> SpawnPlayerPacketMapper = nullptr;

public:
	SpawnPlayerPacket(JNIEnv* Env);
	SpawnPlayerPacket(JNIEnv* Env, jobject InstanceObject);

	jobject GetPlayer(JNIEnv* Env) const;

	~SpawnPlayerPacket();
};

class ScoreboardObjectivePacket : public ClassBase
{
private:
	std::shared_ptr<GameClass> ScoreboardObjectivePacketMapper = nullptr;

public:
	ScoreboardObjectivePacket(JNIEnv* Env);
	ScoreboardObjectivePacket(JNIEnv* Env, jobject InstanceObject);

	jobject func_149337_d(JNIEnv* Env) const;

	~ScoreboardObjectivePacket();
};

class PacketPlayer : public ClassBase
{
private:
	std::shared_ptr<GameClass> PacketPlayerMapper = nullptr;

public:
	PacketPlayer(JNIEnv* Env);
	PacketPlayer(JNIEnv* Env, jobject InstanceObject);

	Vector3<double> GetPosition(JNIEnv* Env) const;

	~PacketPlayer();
};