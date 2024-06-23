#pragma once

#include "block.hpp"
#include "player.hpp"
#include "worldinfo.hpp"
#include "src/sdk/internal/templates/classbase.hpp"
#include "iblockstate.hpp"

class World : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> WorldMapper = nullptr;
	inline static std::shared_ptr<GameClass> ListMapper = nullptr;
	inline static std::shared_ptr<GameClass> BlockPosMapper = nullptr;
	inline static std::shared_ptr<GameClass> IBlockStateMapper = nullptr;
public:
	World(JNIEnv* Env);
	World(JNIEnv* Env, jobject InstanceObject);

	std::vector<std::shared_ptr<Player>> GetPlayers(JNIEnv* Env) const;
	std::vector<std::shared_ptr<Player>> GetPlayerVectorGlobal(JNIEnv* Env) const;
	std::vector<std::shared_ptr<Player>> GetEntitiesWithinAABB(JNIEnv* Env, const Player* LocalPlayer, const Vector3<double>& Position, const double Threshold) const;
	std::vector<std::vector<std::shared_ptr<Player>>> GetSortedEntitiesWithinAABBVectorGlobal(JNIEnv* Env, const Player* LocalPlayer, const Vector3<double>& Position, const double Threshold) const;
	std::vector<std::shared_ptr<Player>> GetEntitiesWithinAABBVectorGlobal(JNIEnv* Env, const Player* LocalPlayer, const Vector3<double>& Position, const double Threshold) const;
	std::vector<std::shared_ptr<Player>> GetPlayerEntitiesWithinAABBVectorGlobal(JNIEnv* Env, const Player* LocalPlayer, const Vector3<double>& Position, const double Threshold) const;
	std::vector<std::shared_ptr<Player>> GetPlayerEntitiesWithinAABBVector(JNIEnv* Env, const Player* LocalPlayer, const Vector3<double>& Position, double Threshold) const;
	std::shared_ptr<Player> GetPlayer(JNIEnv* Env, const char* Name) const;	
	std::shared_ptr<Player> GetEntityByID(JNIEnv* Env, int ID) const;
	std::shared_ptr<Player> GetClosestPlayerToEntity(JNIEnv* Env, const Player* Other, double Distance) const;
	std::shared_ptr<Player> GetClosestPlayerWithinRadius(JNIEnv* Env, Player* Local, float Radius) const;
	std::shared_ptr<Player> GetClosestPlayerWithinFovRadius(JNIEnv* Env, Player* Local, float Radius, float Fov) const;
	std::shared_ptr<Block> GetBlock(JNIEnv* Env, const Vector3<double>& Position) const;

	std::shared_ptr<IBlockState> GetBlockState(JNIEnv* Env, jobject BlockPos) const;

	std::shared_ptr<WorldInfo> GetWorldInfo(JNIEnv* Env) const;
};

//TODO: check differences and define them in World1_7 derivation, 1.8 should be fully working
//use respective class version derivation on declaration, methods with differences will be added here
class World1_7 : public World
{
public:
};

class World1_8 : public World
{
public:
};