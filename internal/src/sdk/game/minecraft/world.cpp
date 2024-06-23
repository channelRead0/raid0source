#include "pch.h"
#include "world.hpp"

#include "blockpos.hpp"

World::World(JNIEnv* Env)
{
	this->WorldMapper = this->WorldMapper ? this->WorldMapper : std::make_shared<GameClass>(Env, "net/minecraft/world/World");
	this->ListMapper = this->ListMapper ? this->ListMapper : std::make_shared<GameClass>(Env, "java/util/List");
	this->BlockPosMapper = this->BlockPosMapper ? this->BlockPosMapper : std::make_shared<GameClass>(Env, "net/minecraft/util/BlockPos");
	this->IBlockStateMapper = this->IBlockStateMapper ? this->IBlockStateMapper : std::make_shared<GameClass>(Env, "net/minecraft/block/state/IBlockState");
}

World::World(JNIEnv* Env, jobject InstanceObject)
{
	this->InstanceObject = InstanceObject;
	this->WorldMapper = this->WorldMapper ? this->WorldMapper : std::make_shared<GameClass>(Env, "net/minecraft/world/World");
	this->ListMapper = this->ListMapper ? this->ListMapper : std::make_shared<GameClass>(Env, "java/util/List");
	this->BlockPosMapper = this->BlockPosMapper ? this->BlockPosMapper : std::make_shared<GameClass>(Env, "net/minecraft/util/BlockPos");
	this->IBlockStateMapper = this->IBlockStateMapper ? this->IBlockStateMapper : std::make_shared<GameClass>(Env, "net/minecraft/block/state/IBlockState");
}

std::vector<std::shared_ptr<Player>> World::GetPlayers(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	std::vector<std::shared_ptr<Player>> PlayerEntities;

	const jobject PlayerEntitiesObject = WorldMapper->GetObjectField(Env, InstanceObject, "playerEntities");

	if (!PlayerEntitiesObject) {
		return{};
	}

	const auto PlayerList = ListMapper->CallArrayMethod(Env, PlayerEntitiesObject, "toArray");

	if (PlayerList.empty()) {
		return{};
	}

	PlayerEntities.reserve(PlayerList.size());
	for (jobject PlayerObject : PlayerList) {
		PlayerEntities.push_back(std::make_shared<Player>(Env, PlayerObject));
	}

	Env->DeleteLocalRef(PlayerEntitiesObject);

	return PlayerEntities;
}

std::vector<std::shared_ptr<Player>> World::GetPlayerVectorGlobal(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	std::vector<std::shared_ptr<Player>> PlayerEntities;

	const jobject PlayerEntitiesObject = WorldMapper->GetObjectField(Env, InstanceObject, "playerEntities");

	if (!PlayerEntitiesObject) {
		return{};
	}

	const auto PlayerList = ListMapper->CallArrayMethod(Env, PlayerEntitiesObject, "toArray");

	if (PlayerList.empty()) {
		return{};
	}

	PlayerEntities.reserve(PlayerList.size());
	for (const jobject PlayerObject : PlayerList) {
		PlayerEntities.push_back(std::make_shared<Player>(Env, Env->NewGlobalRef(PlayerObject)));
	}

	Env->DeleteLocalRef(PlayerEntitiesObject);

	return PlayerEntities;
}

std::vector<std::shared_ptr<Player>> World::GetEntitiesWithinAABB(JNIEnv* Env, const Player* LocalPlayer, const Vector3<double>& Position, const double Threshold) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	const double OriginX = Position.X;
	const double OriginY = Position.Y;
	const double OriginZ = Position.Z;

	const AxisAlignedBB_T<double> SearchAABB =
	{
		OriginX - Threshold,
		OriginY - Threshold,
		OriginZ - Threshold,
		OriginX + Threshold,
		OriginY + Threshold,
		OriginZ + Threshold
	};

	const auto AxisAlignedBoundingBox = std::make_shared<::AxisAlignedBoundingBox>(Env);
	const auto AxisAlignedBoundingBoxObject = AxisAlignedBoundingBox->NewObject(Env, SearchAABB);

	if (!AxisAlignedBoundingBoxObject) {
		return{};
	}

	const auto GetEntitiesMethodID = Env->GetMethodID(WorldMapper->GetClass(), "getEntitiesWithinAABBExcludingEntity", "(Lnet/minecraft/entity/Entity;Lnet/minecraft/util/AxisAlignedBB;)Ljava/util/List;");
	const auto EntityList = Env->CallObjectMethod(InstanceObject, GetEntitiesMethodID, LocalPlayer->GetInstanceObject(), AxisAlignedBoundingBoxObject);
	const auto EntityArray = ListMapper->CallArrayMethod(Env, EntityList, "toArray");

	std::vector<std::shared_ptr<Player>> Entities;

	if (EntityArray.empty()) {
		return{};
	}

	Entities.reserve(EntityArray.size());
	for (jobject PlayerObject : EntityArray) {
		Entities.push_back(std::make_shared<Player>(Env, PlayerObject));
	}

	Env->DeleteLocalRef(EntityList);

	return Entities;
}

std::vector<std::vector<std::shared_ptr<Player>>> World::GetSortedEntitiesWithinAABBVectorGlobal(JNIEnv* Env, const Player* LocalPlayer, const Vector3<double>& Position, const double Threshold) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	const double OriginX = Position.X;
	const double OriginY = Position.Y;
	const double OriginZ = Position.Z;

	const AxisAlignedBB_T<double> SearchAABB =
	{
		OriginX - Threshold,
		OriginY - Threshold,
		OriginZ - Threshold,
		OriginX + Threshold,
		OriginY + Threshold,
		OriginZ + Threshold
	};

	const auto AxisAlignedBoundingBox = std::make_shared<::AxisAlignedBoundingBox>(Env);
	const auto AxisAlignedBoundingBoxObject = AxisAlignedBoundingBox->NewObject(Env, SearchAABB);

	if (!AxisAlignedBoundingBoxObject) {
		return{};
	}

	//const auto GetEntitiesMethodID = Env->GetMethodID(WorldMapper->GetClass(), "getEntitiesWithinAABBExcludingEntity", "(Lnet/minecraft/entity/Entity;Lnet/minecraft/util/AxisAlignedBB;)Ljava/util/List;");
	//const auto EntityList = Env->CallObjectMethod(InstanceObject, GetEntitiesMethodID, LocalPlayer->GetInstanceObject(), AxisAlignedBoundingBoxObject);
	const auto EntityList = WorldMapper->CallObjectMethod(Env, InstanceObject, "getEntitiesWithinAABBExcludingEntity", LocalPlayer->GetInstanceObject(), AxisAlignedBoundingBoxObject);
	const auto EntityArray = ListMapper->CallArrayMethod(Env, EntityList, "toArray");

	std::vector<std::shared_ptr<Player>> PlayerEntities;
	std::vector<std::shared_ptr<Player>> OtherEntities;

	if (EntityArray.empty()) {
		return{};
	}

	PlayerEntities.reserve(EntityArray.size());
	for (const jobject EntityObject : EntityArray)
	{
		auto Entity = std::make_shared<::Player>(Env, Env->NewGlobalRef(EntityObject));

		if (Entity->IsPlayer(Env)) 
		{
			PlayerEntities.push_back(Entity);
			continue;
		}

		OtherEntities.push_back(Entity);
	}

	Env->DeleteLocalRef(EntityList);

	return {PlayerEntities, OtherEntities};
}

std::vector<std::shared_ptr<Player>> World::GetEntitiesWithinAABBVectorGlobal(JNIEnv* Env, const Player* LocalPlayer, const Vector3<double>& Position, const double Threshold) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	const double OriginX = Position.X;
	const double OriginY = Position.Y;
	const double OriginZ = Position.Z;

	const AxisAlignedBB_T<double> SearchAABB =
	{
		OriginX - Threshold,
		OriginY - Threshold,
		OriginZ - Threshold,
		OriginX + Threshold,
		OriginY + Threshold,
		OriginZ + Threshold
	};

	const auto AxisAlignedBoundingBox = std::make_shared<::AxisAlignedBoundingBox>(Env);
	const auto AxisAlignedBoundingBoxObject = AxisAlignedBoundingBox->NewObject(Env, SearchAABB);

	if (!AxisAlignedBoundingBoxObject) {
		return{};
	}

	const auto GetEntitiesMethodID = Env->GetMethodID(WorldMapper->GetClass(), "getEntitiesWithinAABBExcludingEntity", "(Lnet/minecraft/entity/Entity;Lnet/minecraft/util/AxisAlignedBB;)Ljava/util/List;");
	const auto EntityList = Env->CallObjectMethod(InstanceObject, GetEntitiesMethodID, LocalPlayer->GetInstanceObject(), AxisAlignedBoundingBoxObject);
	const auto EntityArray = ListMapper->CallArrayMethod(Env, EntityList, "toArray");

	std::vector<std::shared_ptr<Player>> Entities;

	if (EntityArray.empty()) {
		return{};
	}

	Entities.reserve(EntityArray.size());
	for (const jobject PlayerObject : EntityArray) {
		Entities.push_back(std::make_shared<Player>(Env, Env->NewGlobalRef(PlayerObject)));
	}

	Env->DeleteLocalRef(EntityList);

	return Entities;
}

std::vector<std::shared_ptr<Player>> World::GetPlayerEntitiesWithinAABBVectorGlobal(JNIEnv* Env, const Player* LocalPlayer, const Vector3<double>& Position, const double Threshold) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	const double OriginX = Position.X;
	const double OriginY = Position.Y;
	const double OriginZ = Position.Z;

	const AxisAlignedBB_T<double> SearchAABB =
	{
		OriginX - Threshold,
		OriginY - Threshold,
		OriginZ - Threshold,
		OriginX + Threshold,
		OriginY + Threshold,
		OriginZ + Threshold
	};

	const auto AxisAlignedBoundingBox = std::make_shared<::AxisAlignedBoundingBox>(Env);
	const auto AxisAlignedBoundingBoxObject = AxisAlignedBoundingBox->NewObject(Env, SearchAABB);

	if (!AxisAlignedBoundingBoxObject) {
		return{};
	}

	const auto GetEntitiesMethodID = Env->GetMethodID(WorldMapper->GetClass(), "getEntitiesWithinAABBExcludingEntity", "(Lnet/minecraft/entity/Entity;Lnet/minecraft/util/AxisAlignedBB;)Ljava/util/List;");
	const auto EntityList = Env->CallObjectMethod(InstanceObject, GetEntitiesMethodID, LocalPlayer->GetInstanceObject(), AxisAlignedBoundingBoxObject);
	const auto EntityArray = ListMapper->CallArrayMethod(Env, EntityList, "toArray");

	std::vector<std::shared_ptr<Player>> Entities;

	if (EntityArray.empty()) {
		return{};
	}

	Entities.reserve(EntityArray.size());
	for (const jobject PlayerObject : EntityArray)
	{
		auto Player = std::make_shared<::Player>(Env, Env->NewLocalRef(PlayerObject));

		if (Player->IsPlayer(Env))
		{
			Player->UpdateInstanceObject(Env->NewGlobalRef(PlayerObject));
			Entities.push_back(Player);

			continue;
		}

		Player.reset();
	}

	Env->DeleteLocalRef(EntityList);

	return Entities;
}

std::vector<std::shared_ptr<Player>> World::GetPlayerEntitiesWithinAABBVector(JNIEnv* Env, const Player* LocalPlayer, const Vector3<double>& Position, const double Threshold) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	const double OriginX = Position.X;
	const double OriginY = Position.Y;
	const double OriginZ = Position.Z;

	const AxisAlignedBB_T<double> SearchAABB =
	{
		OriginX - Threshold,
		OriginY - Threshold,
		OriginZ - Threshold,
		OriginX + Threshold,
		OriginY + Threshold,
		OriginZ + Threshold
	};

	const auto AxisAlignedBoundingBox = std::make_shared<::AxisAlignedBoundingBox>(Env);
	const auto AxisAlignedBoundingBoxObject = AxisAlignedBoundingBox->NewObject(Env, SearchAABB);

	if (!AxisAlignedBoundingBoxObject) {
		return{};
	}

	const auto EntityList = WorldMapper->CallObjectMethod(Env, InstanceObject, "getEntitiesWithinAABBExcludingEntity", LocalPlayer->GetInstanceObject(), AxisAlignedBoundingBoxObject); 
	const auto EntityArray = ListMapper->CallArrayMethod(Env, EntityList, "toArray");

	std::vector<std::shared_ptr<Player>> Entities;

	if (EntityArray.empty()) {
		return{};
	}

	Entities.reserve(EntityArray.size());
	for (const jobject PlayerObject : EntityArray) 
	{
		auto LocalObject = Env->NewLocalRef(PlayerObject);
		auto Player = std::make_shared<::Player>(Env, LocalObject);

		if (Player->IsPlayer(Env)) 
		{
			Entities.push_back(Player);
			continue;
		}

		Env->DeleteLocalRef(LocalObject);
		Player->UpdateInstanceObject(NULL);
	}

	Env->DeleteLocalRef(EntityList);

	return Entities;
}

std::shared_ptr<Player> World::GetPlayer(JNIEnv* Env, const char* Name) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	std::shared_ptr<Player> PlayerEntity = nullptr;

	const jobject PlayerEntitiesObject = WorldMapper->GetObjectField(Env, InstanceObject, "playerEntities");

	if (!PlayerEntitiesObject) {
		return{};
	}

	const auto PlayerList = ListMapper->CallArrayMethod(Env, PlayerEntitiesObject, "toArray");

	if (PlayerList.empty()) {
		return{};
	}

	for (jobject PlayerObject : PlayerList) 
	{
		const auto PlayerPtr = std::make_shared<Player>(Env, PlayerObject);
		if (PlayerPtr->GetName(Env) == std::string(Name)) 
		{
			PlayerEntity = PlayerPtr;
			break;
		}
	}

	Env->DeleteLocalRef(PlayerEntitiesObject);

	return PlayerEntity;
}

std::shared_ptr<Player> World::GetEntityByID(JNIEnv* Env, int ID) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<Player>(Env, jobject());
	}

	if (const auto EntityObject = WorldMapper->CallObjectMethod(Env, InstanceObject, "getEntityByID", ID)) {
		return std::make_shared<Player>(Env, EntityObject);
	}

	return std::make_shared<Player>(Env, jobject());
}

std::shared_ptr<Player> World::GetClosestPlayerToEntity(JNIEnv* Env, const Player* Other, double Distance) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<Player>(Env, jobject());
	}

	if (const auto ClosestPlayerObject = WorldMapper->CallObjectMethod(Env, InstanceObject, "getClosestPlayerToEntity", Other->GetInstanceObject(), Distance)) {
		return std::make_shared<Player>(Env, ClosestPlayerObject);
	}

	return std::make_shared<Player>(Env, jobject());
}

std::shared_ptr<Player> World::GetClosestPlayerWithinRadius(JNIEnv* Env, Player* Local, float Radius) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<Player>(Env, jobject());
	}

	const auto Players = GetPlayerEntitiesWithinAABBVector(Env, Local, Local->GetPos(Env), Radius);
	if (Players.empty()) {
		return std::make_shared<Player>(Env, jobject());
	}

	float DistanceBest = Radius;
	std::shared_ptr<Player> ClosestPlayer = nullptr;
	for (auto& Player : Players)
	{ 
		const auto Distance = Local->GetPos(Env).Distance(Player->GetPos(Env));
		if (Distance < DistanceBest) 
		{
			DistanceBest = Distance;
			ClosestPlayer = Player;
		}
	}

	if (ClosestPlayer == nullptr) {
		return std::make_shared<Player>(Env, jobject());
	}

	return ClosestPlayer;
}

std::shared_ptr<Player> World::GetClosestPlayerWithinFovRadius(JNIEnv* Env, Player* Local, float Radius, float Fov) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<Player>(Env, jobject());
	}

	const auto Players = GetPlayerEntitiesWithinAABBVector(Env, Local, Local->GetPos(Env), Radius);
	if (Players.empty()) {
		return std::make_shared<Player>(Env, jobject());
	}

	float DistanceBest = Radius;
	std::shared_ptr<Player> ClosestPlayer = nullptr;
	for (auto& Player : Players)
	{
		const auto YawFromLocal = Local->GetFovTo(Env, Player.get());

		if (fabs(YawFromLocal) > Fov) {
			continue;
		}

		if (const auto Distance = Local->GetPos(Env).Distance(Player->GetPos(Env)); Distance < DistanceBest)
		{
			DistanceBest = Distance;
			ClosestPlayer = Player;
		}
	}

	if (ClosestPlayer == nullptr) {
		return std::make_shared<Player>(Env, jobject());
	}

	return ClosestPlayer;
}

std::shared_ptr<Block> World::GetBlock(JNIEnv* Env, const Vector3<double>& Position) const
{
	const auto BlockPos = std::make_shared<::BlockPos>(Env);
	const auto BlockPosObject = BlockPos->NewObject(Env, Position);

	if (!BlockPosObject) {
		return std::make_shared<Block>(Env, jobject());
	}

	const auto BlockStateObject = WorldMapper->CallObjectMethod(Env, InstanceObject, "getBlockState", BlockPosObject);//IBLOCKSTATE

	Env->DeleteLocalRef(BlockPosObject);

	if (const auto BlockObject = IBlockStateMapper->CallObjectMethod(Env, BlockStateObject, "getBlock")) {
		return std::make_shared<Block>(Env, BlockObject);
	}

	return std::make_shared<Block>(Env, jobject());
}

std::shared_ptr<IBlockState> World::GetBlockState(JNIEnv* Env, jobject BlockPos) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<IBlockState>(Env, jobject());
	}

	if (const auto BlockState = WorldMapper->CallObjectMethod(Env, InstanceObject, "getBlockState", BlockPos)) {
		return std::make_shared<IBlockState>(Env, BlockState);
	}

	return std::make_shared<IBlockState>(Env, jobject());
}

std::shared_ptr<WorldInfo> World::GetWorldInfo(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	if (const auto WorldInfoObject = WorldMapper->CallObjectMethod(Env, InstanceObject, "getWorldInfo")) {
		return std::make_shared<WorldInfo>(Env, WorldInfoObject);
	}

	return {};
}
