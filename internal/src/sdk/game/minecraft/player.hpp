#pragma once

#include "axisalignedboundingbox.hpp"
#include "container.hpp"
#include "damagesource.hpp"
#include "sdk/internal/templates/classbase.hpp"
#include "itemstack.hpp"
#include "potion.hpp"
#include "inventoryplayer.hpp"
#include "team.hpp"
#include "nethandlerplayclient.hpp"

class Player : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> EntityMapper = nullptr;
	inline static std::shared_ptr<GameClass> EntityItemMapper = nullptr;
	inline static std::shared_ptr<GameClass> EntityPlayerMapper = nullptr;
    inline static std::shared_ptr<GameClass> EntityPlayerSPMapper = nullptr;
	inline static std::shared_ptr<GameClass> EntityLivingBaseMapper = nullptr;
	inline static std::shared_ptr<GameClass> RenderPlayerMapper = nullptr;
    inline static std::shared_ptr<GameClass> RendererLivingEntityMapper = nullptr;
    inline static std::shared_ptr<GameClass> CollectionClass = nullptr;
public:
    Player(JNIEnv* Env);
    Player(JNIEnv* Env, jobject InstanceObject);
   // Player(JNIEnv* Env, jobject InstanceObject, bool IsGlobal);

    jobject GetBoundingBox(JNIEnv* Env) const;
    jobject GetInventory(JNIEnv* Env) const;
    jobject GetInventoryPlayer(JNIEnv* Env) const;
    jobject GetHeldItem(JNIEnv* Env) const;
    jobject GetCurrentEquippedItem(JNIEnv* Env) const;
    jobject GetMainModel(JNIEnv* Env) const;
    jobject GetActivePotionEffects(JNIEnv* Env) const;


    //ONLY FOR ITEM ENTITIES
    std::shared_ptr<::InventoryPlayer> GetInventoryPlayerPtr(JNIEnv* Env) const;
    std::shared_ptr<::ItemStack> GetEntityItemPtr(JNIEnv* Env) const;
    std::shared_ptr<::ItemStack> GetHeldItemPtr(JNIEnv* Env) const;
    std::shared_ptr<::Container> GetOpenContainerPtr(JNIEnv* Env) const;
    std::shared_ptr<::Container> GetInventoryContainerPtr(JNIEnv* Env) const;
    std::shared_ptr<::AxisAlignedBoundingBox> GetAxisAlignedBoundingBoxPtr(JNIEnv* Env) const;
    std::shared_ptr<::CTeam> GetTeamPtr(JNIEnv* Env) const;
    std::shared_ptr<::NetHandlerPlayClient> GetSendQueuePtr(JNIEnv* Env) const;

   // std::shared_ptr<IChatComponent> GetDisplayName() const;
    std::string GetName(JNIEnv* Env) const;

    Vector3<double> GetPos(JNIEnv* Env) const;
    Vector3<double> GetPreviousPos(JNIEnv* Env) const;
    Vector3<double> GetLastTickPos(JNIEnv* Env) const;
    Vector3<int> GetServerPos(JNIEnv* Env) const;

    void SetRotationPitch(JNIEnv* Env, float Pitch) const;
    void SetRotationYaw(JNIEnv* Env, float Yaw) const;
    void SetPrevRotationPitch(JNIEnv* Env, float Pitch) const;
    void SetPrevRotationYaw(JNIEnv* Env, float Yaw) const;
    void SetSprinting(JNIEnv* Env, bool Sprinting) const;
    void CloseScreen(JNIEnv* Env) const;
    void HandleStatusUpdate(JNIEnv* Env, jbyte ID) const;

    void SetAngles(JNIEnv* Env, float Yaw, float Pitch) const;
    void SetRotationYawHead(JNIEnv* Env, float Yaw) const;
    void SetRenderOutlines(JNIEnv* Env, bool Draw) const;
    void SetMotionX(JNIEnv* Env, double MotionX) const;
    void SetMotionY(JNIEnv* Env, double MotionY) const;
    void SetMotionZ(JNIEnv* Env, double MotionZ) const;
    void SetJumpTicks(JNIEnv* Env, int Ticks) const;
    void Jump(JNIEnv* Env) const;
    void SwingItem(JNIEnv* Env) const;
    void DoSwingItemAnimation(JNIEnv* Env) const;
    void SetSwingInProgress(JNIEnv* Env, bool State) const;
    void SetSwingProgressInt(JNIEnv* Env, int Progress) const;

    bool IsPlayer(JNIEnv* Env) const;
    bool IsItem(JNIEnv* Env) const;
    bool IsMob(JNIEnv* Env) const;
    bool IsEntitySeen(JNIEnv* Env, const Player* Other) const;
    bool IsInvisibleToPlayer(JNIEnv* Env, const Player* Other) const;
    bool IsOnSameTeam(JNIEnv* Env, Player* Other) const;
    bool IsOnSameTeamHypixel(JNIEnv* Env, Player* Other) const;
    bool IsOnSameTeamCheckAll(JNIEnv* Env, Player* Other) const;
    bool IsNPC(JNIEnv* Env) const;
    bool IsBlocking(JNIEnv* Env) const;
    bool IsSpectator(JNIEnv* Env) const;
    bool IsDead(JNIEnv* Env) const;
    bool IsUsingItem(JNIEnv* Env) const;
    bool IsSprinting(JNIEnv* Env) const;
    bool IsInWater(JNIEnv* Env) const;
	bool IsInLava(JNIEnv* Env) const;
	bool IsInvisible(JNIEnv* Env) const;
    bool IsOnGround(JNIEnv* Env) const;
    bool IsBurning(JNIEnv* Env) const;
    bool IsSneaking(JNIEnv* Env) const;
    bool IsEntityInvulnerable(JNIEnv* Env, const DamageSource* DamageSource) const;
    bool IsPotionActive(JNIEnv* Env, const Potion* Potion) const;
    bool IsPotionActive(JNIEnv* Env, const int PotionID) const;
    bool IsImmuneToFire(JNIEnv* Env) const;
    bool IsSwingInProgress(JNIEnv* Env) const;

    int GetHurtTime(JNIEnv* Env) const;
    int GetHurtResistantTime(JNIEnv* Env) const;
    int GetMaxHurtResistantTime(JNIEnv* Env) const;
    int GetEntityID(JNIEnv* Env) const;
    int GetUniqueID(JNIEnv* Env) const;
    int GetFire(JNIEnv* Env) const;
    int GetTicksExisted(JNIEnv* Env) const;
    int GetArmSwingAnimationEnd(JNIEnv* Env) const;
    int GetSwingProgressInt(JNIEnv* Env) const;

    float GetFovTo(JNIEnv* Env, const Player* Other) const;
    float GetHealth(JNIEnv* Env) const;
    float GetMaxHealth(JNIEnv* Env) const;
    float GetHeight(JNIEnv* Env) const;
    float GetEyeHeight(JNIEnv* Env) const;
    float GetRotationPitch(JNIEnv* Env) const;
    float GetRotationYaw(JNIEnv* Env) const;
    float GetRotationYawHead(JNIEnv* Env) const;
    float GetPrevRotationPitch(JNIEnv* Env) const;
    float GetRenderYawOffset(JNIEnv* Env) const;
    float GetPrevRenderYawOffset(JNIEnv* Env) const;
    float GetPrevRotationYawHead(JNIEnv* Env) const;
    float GetPrevRotationYaw(JNIEnv* Env) const;
    float GetMoveForward(JNIEnv* Env) const;
    float GetMoveStrafe(JNIEnv* Env) const;
    float GetDistance(JNIEnv* Env, const Player* Other) const;

    double GetMotionX(JNIEnv* Env) const;
    double GetMotionY(JNIEnv* Env) const;
    double GetMotionZ(JNIEnv* Env) const;

    ~Player();
};

//TODO: check differences and define them in Player1_7 derivation, 1.8 should be fully working
//use respective class version derivation on declaration, methods with differences will be added here
class Player1_7 : public Player
{
public:
};

class Player1_8 : public Player
{
public:
};
