#include "pch.h"
#include "player.hpp"

#include "sdk/internal/register/envregister.hpp"

Player::Player(JNIEnv* Env)
{
	if (this->EntityLivingBaseMapper == nullptr) {
		this->EntityLivingBaseMapper = std::make_shared<GameClass>(Env, "net/minecraft/entity/EntityLivingBase");
	}

	if (this->EntityPlayerMapper == nullptr) {
		this->EntityPlayerMapper = std::make_shared<GameClass>(Env, "net/minecraft/entity/player/EntityPlayer");
	}

	if (this->EntityPlayerSPMapper == nullptr) {
		this->EntityPlayerSPMapper = std::make_shared<GameClass>(Env, "net/minecraft/client/entity/EntityPlayerSP");
	}

	if (this->EntityItemMapper == nullptr) {
		this->EntityItemMapper = std::make_shared<GameClass>(Env, "net/minecraft/entity/item/EntityItem");
	}

	if (this->EntityMapper == nullptr) {
		this->EntityMapper = std::make_shared<GameClass>(Env, "net/minecraft/entity/Entity");
	}

	if (this->RenderPlayerMapper == nullptr) {
		this->RenderPlayerMapper = std::make_shared<GameClass>(Env, "net/minecraft/client/renderer/entity/RenderPlayer");
	}

	if (this->RendererLivingEntityMapper == nullptr) {
		this->RendererLivingEntityMapper = std::make_shared<GameClass>(Env, "net/minecraft/client/renderer/entity/RendererLivingEntity");
	}
}

Player::Player(JNIEnv* Env, jobject InstanceObject)
{
	if (this->EntityLivingBaseMapper == nullptr) {
		this->EntityLivingBaseMapper = std::make_shared<GameClass>(Env, "net/minecraft/entity/EntityLivingBase");
	}

	if (this->EntityPlayerMapper == nullptr) {
		this->EntityPlayerMapper = std::make_shared<GameClass>(Env, "net/minecraft/entity/player/EntityPlayer");
	}

	if (this->EntityPlayerSPMapper == nullptr) {
		this->EntityPlayerSPMapper = std::make_shared<GameClass>(Env, "net/minecraft/client/entity/EntityPlayerSP");
	}

	if (this->EntityItemMapper == nullptr) {
		this->EntityItemMapper = std::make_shared<GameClass>(Env, "net/minecraft/entity/item/EntityItem");
	}

	if (this->EntityMapper == nullptr) {
		this->EntityMapper = std::make_shared<GameClass>(Env, "net/minecraft/entity/Entity");
	}

	if (this->RenderPlayerMapper == nullptr) {
		this->RenderPlayerMapper = std::make_shared<GameClass>(Env, "net/minecraft/client/renderer/entity/RenderPlayer");
	}

	if (this->RendererLivingEntityMapper == nullptr) {
		this->RendererLivingEntityMapper = std::make_shared<GameClass>(Env, "net/minecraft/client/renderer/entity/RendererLivingEntity");
	}

	this->InstanceObject = InstanceObject;
}

jobject Player::GetBoundingBox(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityMapper->GetObjectField(Env, InstanceObject, "boundingBox");
}

jobject Player::GetInventory(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityPlayerMapper->GetObjectField(Env, InstanceObject, "inventoryContainer");
}

jobject Player::GetInventoryPlayer(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityPlayerMapper->GetObjectField(Env, InstanceObject, "inventory");
}

jobject Player::GetHeldItem(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityPlayerMapper->CallObjectMethod(Env, InstanceObject, "getHeldItem");
}

jobject Player::GetCurrentEquippedItem(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityPlayerMapper->CallObjectMethod(Env, InstanceObject, "getCurrentEquippedItem");
}

jobject Player::GetMainModel(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return RenderPlayerMapper->CallObjectMethod(Env, InstanceObject, "getMainModel");
}

jobject Player::GetActivePotionEffects(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityLivingBaseMapper->CallObjectMethod(Env, InstanceObject, "getActivePotionEffects");
}

std::shared_ptr<InventoryPlayer> Player::GetInventoryPlayerPtr(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<InventoryPlayer>(Env, jobject());
	}

	if (const auto InventoryPlayerObject = EntityPlayerMapper->GetObjectField(Env, InstanceObject, "inventory")) {
		return std::make_shared<InventoryPlayer>(Env, InventoryPlayerObject);
	}

	return std::make_shared<InventoryPlayer>(Env, jobject());
}

std::shared_ptr<ItemStack> Player::GetEntityItemPtr(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<ItemStack>(Env, jobject());
	}

	if (const auto EntityItemObject = EntityPlayerMapper->CallObjectMethod(Env, InstanceObject, "getEntityItem")) {
		return std::make_shared<ItemStack>(Env, EntityItemObject);
	}

	return std::make_shared<ItemStack>(Env, jobject());
}

std::shared_ptr<ItemStack> Player::GetHeldItemPtr(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<ItemStack>(Env, jobject());
	}

	if (const auto HeldItemObject = EntityPlayerMapper->CallObjectMethod(Env, InstanceObject, "getHeldItem")) {
		return std::make_shared<ItemStack>(Env, HeldItemObject);
	}

	return std::make_shared<ItemStack>(Env, jobject());
}

std::shared_ptr<::Container> Player::GetOpenContainerPtr(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<Container>(Env, jobject());
	}

	if (const auto OpenContainerObject = EntityPlayerMapper->GetObjectField(Env, InstanceObject, "openContainer")) {
		return std::make_shared<Container>(Env, OpenContainerObject);
	}

	return std::make_shared<Container>(Env, jobject());
}

std::shared_ptr<Container> Player::GetInventoryContainerPtr(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<Container>(Env, jobject());
	}

	if (const auto InventoryContainerObject = EntityPlayerMapper->GetObjectField(Env, InstanceObject, "inventoryContainer")) {
		return std::make_shared<Container>(Env, InventoryContainerObject);
	}

	return std::make_shared<Container>(Env, jobject());
}

std::shared_ptr<AxisAlignedBoundingBox> Player::GetAxisAlignedBoundingBoxPtr(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<AxisAlignedBoundingBox>(Env, jobject());
	}

	if (const auto BoundingBoxObject = EntityMapper->GetObjectField(Env, InstanceObject, "boundingBox")) {
		return std::make_shared<AxisAlignedBoundingBox>(Env, BoundingBoxObject);
	}

	return std::make_shared<AxisAlignedBoundingBox>(Env, jobject());
}

std::shared_ptr<::CTeam> Player::GetTeamPtr(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<CTeam>(Env, jobject());
	}

	if (const auto TeamObject = EntityMapper->GetObjectField(Env, InstanceObject, "getTeam")) {
		return std::make_shared<CTeam>(Env, TeamObject);
	}

	return std::make_shared<CTeam>(Env, jobject());
}

std::shared_ptr<::NetHandlerPlayClient> Player::GetSendQueuePtr(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<NetHandlerPlayClient>(Env, jobject());
	}

	if (const auto SendQueueObject = EntityPlayerSPMapper->GetObjectField(Env, InstanceObject, "sendQueue")) {
		return std::make_shared<NetHandlerPlayClient>(Env, SendQueueObject);
	}

	return std::make_shared<NetHandlerPlayClient>(Env, jobject());
}

std::string Player::GetName(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityMapper->CallStringMethod(Env, InstanceObject, "getName");
}

Vector3<double> Player::GetPos(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return
	{
		EntityMapper->GetDoubleField(Env, InstanceObject, "posX"),
		EntityMapper->GetDoubleField(Env, InstanceObject, "posY"),
		EntityMapper->GetDoubleField(Env, InstanceObject, "posZ"),
	};
}

Vector3<double> Player::GetPreviousPos(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return
	{
		EntityMapper->GetDoubleField(Env, InstanceObject, "prevPosX"),
		EntityMapper->GetDoubleField(Env, InstanceObject, "prevPosY"),
		EntityMapper->GetDoubleField(Env, InstanceObject, "prevPosZ"),
	};
}

Vector3<double> Player::GetLastTickPos(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return
	{
		EntityMapper->GetDoubleField(Env, InstanceObject, "lastTickPosX"),
		EntityMapper->GetDoubleField(Env, InstanceObject, "lastTickPosY"),
		EntityMapper->GetDoubleField(Env, InstanceObject, "lastTickPosZ"),
	};
}

Vector3<int> Player::GetServerPos(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return
	{
		EntityMapper->GetIntField(Env, InstanceObject, "serverPosX"),
		EntityMapper->GetIntField(Env, InstanceObject, "serverPosY"),
		EntityMapper->GetIntField(Env, InstanceObject, "serverPosZ"),
	};
}

void Player::SetRotationPitch(JNIEnv* Env, float Pitch) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	EntityMapper->SetFloatField(Env, InstanceObject, "rotationPitch", Pitch);
}

void Player::SetRotationYaw(JNIEnv* Env, float Yaw) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	EntityMapper->SetFloatField(Env, InstanceObject, "rotationYaw", Yaw);
}

void Player::SetPrevRotationPitch(JNIEnv* Env, float Pitch) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	EntityMapper->SetFloatField(Env, InstanceObject, "prevRotationPitch", Pitch);
}

void Player::SetPrevRotationYaw(JNIEnv* Env, float Yaw) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	EntityMapper->SetFloatField(Env, InstanceObject, "prevRotationYaw", Yaw);
}

void Player::SetSprinting(JNIEnv* Env, bool Sprinting) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	EntityMapper->CallBooleanMethod(Env, InstanceObject, "setSprinting", Sprinting);
}

void Player::CloseScreen(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	EntityPlayerMapper->CallVoidMethod(Env, InstanceObject, "closeScreen");
}

void Player::HandleStatusUpdate(JNIEnv* Env, jbyte ID) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	EntityLivingBaseMapper->CallVoidMethod(Env, InstanceObject, "handleStatusUpdate", ID);
}

void Player::SetAngles(JNIEnv* Env, float Yaw, float Pitch) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	EntityMapper->CallVoidMethod(Env, InstanceObject, "setAngles", Yaw, Pitch);
}

void Player::SetRotationYawHead(JNIEnv* Env, float Yaw) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	EntityLivingBaseMapper->SetFloatField(Env, InstanceObject, "rotationYawHead", Yaw);
}

void Player::SetRenderOutlines(JNIEnv* Env, bool Draw) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	RendererLivingEntityMapper->SetBooleanField(Env, InstanceObject, "renderOutlines", Draw);
}

void Player::SetMotionX(JNIEnv* Env, double MotionX) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	EntityMapper->SetDoubleField(Env, InstanceObject, "motionX", MotionX);
}

void Player::SetMotionY(JNIEnv* Env, double MotionY) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	EntityMapper->SetDoubleField(Env, InstanceObject, "motionY", MotionY);
}

void Player::SetMotionZ(JNIEnv* Env, double MotionZ) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	EntityMapper->SetDoubleField(Env, InstanceObject, "motionZ", MotionZ);
}

void Player::SetJumpTicks(JNIEnv* Env, int Ticks) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	EntityLivingBaseMapper->SetIntField(Env, InstanceObject, "jumpTicks", Ticks);
}

void Player::Jump(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	EntityPlayerMapper->CallVoidMethod(Env, InstanceObject, "jump");
}

void Player::SwingItem(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	EntityPlayerSPMapper->CallVoidMethod(Env, InstanceObject, "swingItem");
}

void Player::DoSwingItemAnimation(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	EntityLivingBaseMapper->CallVoidMethod(Env, InstanceObject, "swingItem");
}

void Player::SetSwingInProgress(JNIEnv* Env, bool State) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	EntityLivingBaseMapper->SetBooleanField(Env, InstanceObject, "isSwingInProgress", State);
}

void Player::SetSwingProgressInt(JNIEnv* Env, int Progress) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	EntityLivingBaseMapper->SetIntField(Env, InstanceObject, "swingProgressInt", Progress);
}

bool Player::IsPlayer(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return {};
	}

	return Env->IsInstanceOf(InstanceObject, EntityPlayerMapper->GetClass());
}

bool Player::IsItem(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return {};
	}

	return Env->IsInstanceOf(InstanceObject, EntityItemMapper->GetClass());
}

bool Player::IsMob(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return {};
	}

	return (!IsPlayer(Env) && !IsItem(Env));
}

//CRASHES
bool Player::IsEntitySeen(JNIEnv* Env, const Player* Other) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityLivingBaseMapper->CallBooleanMethod(Env, InstanceObject, "canEntityBeSeen", Other->GetInstanceObject());
}

bool Player::IsInvisibleToPlayer(JNIEnv* Env, const Player* Other) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityMapper->CallBooleanMethod(Env, InstanceObject, "isInvisibleToPlayer", Other->GetInstanceObject());
}

bool Player::IsOnSameTeam(JNIEnv* Env, Player* Other) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityLivingBaseMapper->CallBooleanMethod(Env, InstanceObject, "isOnSameTeam", Other->GetInstanceObject());
}

bool Player::IsOnSameTeamHypixel(JNIEnv* Env, Player* Other) const
{
	return {};
}

bool Player::IsOnSameTeamCheckAll(JNIEnv* Env, Player* Other) const
{
	return {};
}

bool Player::IsNPC(JNIEnv* Env) const
{
	return {};
}

bool Player::IsBlocking(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityPlayerMapper->CallBooleanMethod(Env, InstanceObject, "isBlocking");
}

bool Player::IsSpectator(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityPlayerMapper->CallBooleanMethod(Env, InstanceObject, "isSpectator");
}

bool Player::IsDead(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityMapper->GetBooleanField(Env, InstanceObject, "isDead");
}

bool Player::IsUsingItem(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityPlayerMapper->CallBooleanMethod(Env, InstanceObject, "isUsingItem");
}

bool Player::IsSprinting(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityMapper->CallBooleanMethod(Env, InstanceObject, "isSprinting");
}

bool Player::IsInWater(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityMapper->GetBooleanField(Env, InstanceObject, "inWater");
}

bool Player::IsInLava(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityMapper->CallBooleanMethod(Env, InstanceObject, "isInLava");
}

bool Player::IsInvisible(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityMapper->CallBooleanMethod(Env, InstanceObject, "isInvisible");
}

bool Player::IsOnGround(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityMapper->GetBooleanField(Env, InstanceObject, "onGround");
}

bool Player::IsBurning(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityMapper->CallBooleanMethod(Env, InstanceObject, "isBurning");
}

bool Player::IsSneaking(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityMapper->CallBooleanMethod(Env, InstanceObject, "isSneaking");
}

bool Player::IsEntityInvulnerable(JNIEnv* Env, const DamageSource* DamageSource) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityMapper->CallBooleanMethod(Env, InstanceObject, "isEntityInvulnerable", DamageSource->GetInstanceObject());
}

bool Player::IsPotionActive(JNIEnv* Env, const Potion* Potion) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	const auto IsPotionActiveMethodID = Env->GetMethodID(EntityLivingBaseMapper->GetClass(), "isPotionActive", "(Lnet/minecraft/potion/Potion;)Z");

	return Env->CallBooleanMethod(InstanceObject, IsPotionActiveMethodID, Potion->GetInstanceObject());
}

bool Player::IsPotionActive(JNIEnv* Env, const int PotionID) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	const auto IsPotionActiveMethodID = Env->GetMethodID(EntityLivingBaseMapper->GetClass(), "isPotionActive", "(I)Z");

	return Env->CallBooleanMethod(InstanceObject, IsPotionActiveMethodID, PotionID);
}

bool Player::IsImmuneToFire(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityMapper->CallBooleanMethod(Env, InstanceObject, "isImmuneToFire");
}

bool Player::IsSwingInProgress(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityLivingBaseMapper->GetBooleanField(Env, InstanceObject, "isSwingInProgress");
}

int Player::GetHurtTime(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityLivingBaseMapper->GetIntField(Env, InstanceObject, "hurtTime");
}

int Player::GetHurtResistantTime(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityMapper->GetIntField(Env, InstanceObject, "hurtResistantTime");
}

int Player::GetMaxHurtResistantTime(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityLivingBaseMapper->GetIntField(Env, InstanceObject, "maxHurtResistantTime");
}

int Player::GetEntityID(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return 0;
	}

	return EntityMapper->GetIntField(Env, InstanceObject, "entityId");
}

int Player::GetUniqueID(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return 0;
	}

	return EntityMapper->CallIntMethod(Env, InstanceObject, "getUniqueID");
}

int Player::GetFire(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return 0;
	}

	return EntityMapper->GetIntField(Env, InstanceObject, "fire");
}

int Player::GetTicksExisted(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return 0;
	}

	return EntityMapper->GetIntField(Env, InstanceObject, "ticksExisted");
}

int Player::GetArmSwingAnimationEnd(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return 0;
	}

	return EntityLivingBaseMapper->CallIntMethod(Env, InstanceObject, "getArmSwingAnimationEnd");
}

int Player::GetSwingProgressInt(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return 0;
	}

	return EntityLivingBaseMapper->GetIntField(Env, InstanceObject, "swingProgressInt");
}

float Player::GetFovTo(JNIEnv* Env, const Player* Other) const
{
	const auto LocalPlayerPosition = this->GetPos(Env);
	const auto TargetPosition = Other->GetPos(Env);

	auto TargetDistanceX = TargetPosition.X - LocalPlayerPosition.X;
	auto TargetDistanceZ = TargetPosition.Z - LocalPlayerPosition.Z;

	return CalculateDeltaYaw(TargetDistanceX, TargetDistanceZ, this->GetRotationYaw(Env));
}

float Player::GetHealth(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityLivingBaseMapper->CallFloatMethod(Env, InstanceObject, "getHealth");
}

float Player::GetMaxHealth(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityLivingBaseMapper->CallFloatMethod(Env, InstanceObject, "getMaxHealth");
}

float Player::GetHeight(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityMapper->GetBooleanField(Env, InstanceObject, "height");
}

float Player::GetEyeHeight(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityMapper->CallBooleanMethod(Env, InstanceObject, "getEyeHeight");
}

float Player::GetRotationPitch(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityMapper->GetFloatField(Env, InstanceObject, "rotationPitch");
}

float Player::GetRotationYaw(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityMapper->GetFloatField(Env, InstanceObject, "rotationYaw");
}

float Player::GetRotationYawHead(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityLivingBaseMapper->GetFloatField(Env, InstanceObject, "rotationYawHead");
}

float Player::GetPrevRotationPitch(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityMapper->GetFloatField(Env, InstanceObject, "prevRotationPitch");
}

float Player::GetRenderYawOffset(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityLivingBaseMapper->GetFloatField(Env, InstanceObject, "renderYawOffset");
}

float Player::GetPrevRenderYawOffset(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityLivingBaseMapper->GetFloatField(Env, InstanceObject, "prevRenderYawOffset");
}

float Player::GetPrevRotationYawHead(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityLivingBaseMapper->GetFloatField(Env, InstanceObject, "prevRotationYawHead");
}

float Player::GetPrevRotationYaw(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityMapper->GetFloatField(Env, InstanceObject, "prevRotationYaw");
}

float Player::GetMoveForward(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityLivingBaseMapper->GetFloatField(Env, InstanceObject, "moveForward");
}

float Player::GetMoveStrafe(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityLivingBaseMapper->GetFloatField(Env, InstanceObject, "moveStrafing");
}

float Player::GetDistance(JNIEnv* Env, const Player* Other) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return this->GetPos(Env).Distance(Other->GetPos(Env));
}

double Player::GetMotionX(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityMapper->GetDoubleField(Env, InstanceObject, "motionX");
}

double Player::GetMotionY(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityMapper->GetDoubleField(Env, InstanceObject, "motionY");
}

double Player::GetMotionZ(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityMapper->GetDoubleField(Env, InstanceObject, "motionZ");
}

Player::~Player()
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
	}

	InstanceObject = nullptr;
}
