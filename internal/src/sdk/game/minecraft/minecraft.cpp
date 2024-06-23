#include "pch.h"
#include "minecraft.hpp"

#include "sdk/internal/register/envregister.hpp"

Minecraft::Minecraft(JNIEnv* Env)
{
	this->IsGlobal = true;
	this->ClassInterface = this->ClassInterface ? this->ClassInterface : std::make_shared<GameClass>(Env, "net/minecraft/client/Minecraft");
	this->InstanceObject = Env->NewGlobalRef(ClassInterface->CallObjectMethod(Env, ClassInterface->GetClass(), "getMinecraft"));
}

jobject Minecraft::GetThePlayer(JNIEnv* Env) const
{
	//auto start = std::chrono::high_resolution_clock::now();

	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	//auto ret = ClassInterface->GetObjectField(Env, InstanceObject, "thePlayer");
	

	return ClassInterface->GetObjectField(Env, InstanceObject, "thePlayer");
}

jobject Minecraft::GetTheWorld(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return  ClassInterface->GetObjectField(Env, InstanceObject, "theWorld");
}

jobject Minecraft::GetCurrentScreen(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return ClassInterface->GetObjectField(Env, InstanceObject, "currentScreen");
}

jobject Minecraft::GetRenderManager(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return ClassInterface->CallObjectMethod(Env, InstanceObject, "getRenderManager");
}

jobject Minecraft::GetPlayerController(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return ClassInterface->GetObjectField(Env, InstanceObject, "playerController");
}

jobject Minecraft::GetTimer(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return ClassInterface->GetObjectField(Env, InstanceObject, "timer");
}

jobject Minecraft::GetEntityRenderer(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return ClassInterface->GetObjectField(Env, InstanceObject, "entityRenderer");
}

jobject Minecraft::GetFontRenderer(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return ClassInterface->GetObjectField(Env, InstanceObject, "fontRendererObj");
}

jobject Minecraft::GetObjectMouseOver(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return ClassInterface->GetObjectField(Env, InstanceObject, "objectMouseOver");
}

jobject Minecraft::GetGameSettings(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return ClassInterface->GetObjectField(Env, InstanceObject, "gameSettings");
}

jobject Minecraft::GetCurrentServerData(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return ClassInterface->CallObjectMethod(Env, InstanceObject, "getCurrentServerData");
}

jobject Minecraft::GetCurrentSession(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return ClassInterface->CallObjectMethod(Env, InstanceObject, "getSession");
}

std::shared_ptr<Player> Minecraft::GetThePlayerPtr(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<Player>(Env, jobject());
	}

	if (const auto ThePlayerObject = ClassInterface->GetObjectField(Env, InstanceObject, "thePlayer")) {
		return std::make_shared<Player>(Env, Env->NewGlobalRef(ThePlayerObject));
	}

	return std::make_shared<Player>(Env, jobject());
}

std::shared_ptr<Player> Minecraft::GetRenderViewEntityPtr(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<Player>(Env, jobject());
	}

	if (const auto RenderViewEntityObject = ClassInterface->CallObjectMethod(Env, InstanceObject, "getRenderViewEntity")) {
		return std::make_shared<Player>(Env, Env->NewLocalRef(RenderViewEntityObject));
	}

	return std::make_shared<Player>(Env, jobject());
}

std::shared_ptr<World> Minecraft::GetTheWorldPtr(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<World>(Env, jobject());
	}

	if (const auto TheWorldObject = ClassInterface->GetObjectField(Env, InstanceObject, "theWorld")) {
		return std::make_shared<World>(Env, Env->NewGlobalRef(TheWorldObject));
	}

	return std::make_shared<World>(Env, jobject());
}

std::shared_ptr<GuiScreen> Minecraft::GetCurrentScreenPtr(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<GuiScreen>(Env, jobject());
	}

	if (const auto CurrentScreenObject = ClassInterface->GetObjectField(Env, InstanceObject, "currentScreen")) {
		return std::make_shared<GuiScreen>(Env, Env->NewGlobalRef(CurrentScreenObject));
	}

	return std::make_shared<GuiScreen>(Env, jobject());
}

std::shared_ptr<RenderManager> Minecraft::GetRenderManagerPtr(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<RenderManager>(Env, jobject());
	}

	if (const auto RenderManagerObject = ClassInterface->CallObjectMethod(Env, InstanceObject, "getRenderManager")) {
		return std::make_shared<RenderManager>(Env, Env->NewGlobalRef(RenderManagerObject));
	}

	return std::make_shared<RenderManager>(Env, jobject());
}

std::shared_ptr<PlayerControllerMP> Minecraft::GetPlayerControllerPtr(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<PlayerControllerMP>(Env, jobject());
	}

	if (const auto PlayerControllerObject = ClassInterface->GetObjectField(Env, InstanceObject, "playerController")) {
		return std::make_shared<PlayerControllerMP>(Env, Env->NewGlobalRef(PlayerControllerObject));
	}

	return std::make_shared<PlayerControllerMP>(Env, jobject());
}

std::shared_ptr<Timer> Minecraft::GetTimerPtr(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<Timer>(Env, jobject());
	}

	if (const auto TimerObject = ClassInterface->GetObjectField(Env, InstanceObject, "timer")) {
		return std::make_shared<Timer>(Env, Env->NewGlobalRef(TimerObject));
	}

	return std::make_shared<Timer>(Env, jobject());
}

std::shared_ptr<EntityRenderer> Minecraft::GetEntityRendererPtr(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<EntityRenderer>(Env, jobject());
	}

	if (const auto EntityRendererObject = ClassInterface->GetObjectField(Env, InstanceObject, "entityRenderer")) {
		return std::make_shared<EntityRenderer>(Env, Env->NewGlobalRef(EntityRendererObject));
	}

	return std::make_shared<EntityRenderer>(Env, jobject());
}

std::shared_ptr<FontRenderer> Minecraft::GetFontRendererPtr(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<FontRenderer>(Env, jobject());
	}

	if (const auto FontRendererObject = ClassInterface->GetObjectField(Env, InstanceObject, "fontRendererObj")) {
		return std::make_shared<FontRenderer>(Env, Env->NewGlobalRef(FontRendererObject));
	}

	return std::make_shared<FontRenderer>(Env, jobject());
}

std::shared_ptr<GameSettings> Minecraft::GetGameSettingsPtr(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<GameSettings>(Env, jobject());
	}

	if (const auto FontRendererObject = ClassInterface->GetObjectField(Env, InstanceObject, "gameSettings")) {
		return std::make_shared<GameSettings>(Env, Env->NewGlobalRef(FontRendererObject));
	}

	return std::make_shared<GameSettings>(Env, jobject());
}

std::shared_ptr<ServerData> Minecraft::GetCurrentServerDataPtr(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<ServerData>(Env, jobject());
	}

	if (const auto CurrentServerDataObject = ClassInterface->CallObjectMethod(Env, InstanceObject, "getCurrentServerData")) {
		return std::make_shared<ServerData>(Env, Env->NewGlobalRef(CurrentServerDataObject));
	}

	return std::make_shared<ServerData>(Env, jobject());
}

std::shared_ptr<MovingObjectPosition> Minecraft::GetObjectMouseOverPtr(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<MovingObjectPosition>(Env, jobject());
	}

	if (const auto ObjectMouseOverObject = ClassInterface->GetObjectField(Env, InstanceObject, "objectMouseOver")) {
		return std::make_shared<MovingObjectPosition>(Env, Env->NewGlobalRef(ObjectMouseOverObject));
	}

	return std::make_shared<MovingObjectPosition>(Env, jobject());
}

std::shared_ptr<Framebuffer> Minecraft::GetFrameBufferPtr(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<Framebuffer>(Env, jobject());
	}

	if (const auto FrameBufferObject = ClassInterface->CallObjectMethod(Env, InstanceObject, "getFramebuffer")) {
		return std::make_shared<Framebuffer>(Env, Env->NewLocalRef(FrameBufferObject));
	}

	return std::make_shared<Framebuffer>(Env, jobject());
}

std::shared_ptr<MouseHelper> Minecraft::GetMouseHelperPtr(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<MouseHelper>(Env, jobject());
	}

	if (const auto MouseHelperObject = ClassInterface->GetObjectField(Env, InstanceObject, "mouseHelper")) {
		return std::make_shared<MouseHelper>(Env, Env->NewLocalRef(MouseHelperObject));
	}

	return std::make_shared<MouseHelper>(Env, jobject());
}

int Minecraft::GetDisplayWidth(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return ClassInterface->GetIntField(Env, InstanceObject, "displayWidth");
}

int Minecraft::GetDisplayHeight(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return ClassInterface->GetIntField(Env, InstanceObject, "displayHeight");
}

int Minecraft::GetDebugFPS(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return  ClassInterface->GetIntField(Env, InstanceObject, "debugFPS");
}

void Minecraft::SetRightClickDelayTimer(JNIEnv* Env, int Ticks) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	ClassInterface->SetIntField(Env, InstanceObject, "rightClickDelayTimer", Ticks);
}

void Minecraft::SetLeftClickCount(JNIEnv* Env, int Count) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	ClassInterface->SetIntField(Env, InstanceObject, "leftClickCounter", Count);
}

void Minecraft::SetPointedEntity(JNIEnv* Env, const Player* Object) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	ClassInterface->SetObjectField(Env, InstanceObject, "pointedEntity", Object->GetInstanceObject());
}
//todo: put all this shit in classes lol
void Minecraft::SetObjectMouseOver(JNIEnv* Env, const Player* Object, const Vector3<double>& HitPosition) const
{
	const auto MovingObjectPositionClass = std::make_shared<GameClass>(Env, "net/minecraft/util/MovingObjectPosition");
	const auto MovingObjectPositionConstructor_MethodID = Env->GetMethodID(MovingObjectPositionClass->GetClass(), "<init>", "(Lnet/minecraft/entity/Entity;Lnet/minecraft/util/Vec3;)V");

	const auto Vec3Class = std::make_shared<GameClass>(Env, "net/minecraft/util/Vec3");
	const auto Vec3Constructor_MethodID = Env->GetMethodID(Vec3Class->GetClass(), "<init>", "(DDD)V");

	const auto MovingObjectPositionVec3 = Env->NewObject(Vec3Class->GetClass(), Vec3Constructor_MethodID, HitPosition.X, HitPosition.Y, HitPosition.Z);
	const auto MovingObjectPositionObject = Env->NewObject(MovingObjectPositionClass->GetClass(), MovingObjectPositionConstructor_MethodID, Object->GetInstanceObject(), MovingObjectPositionVec3);

	ClassInterface->SetObjectField(Env, InstanceObject, "objectMouseOver", MovingObjectPositionObject);

	Env->DeleteLocalRef(MovingObjectPositionVec3);
	Env->DeleteLocalRef(MovingObjectPositionObject);
}

void Minecraft::SetDebugFPS(JNIEnv* Env, int FPS) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	ClassInterface->SetIntField(Env, InstanceObject, "debugFPS", FPS);
}

void Minecraft::DisplayGuiScreen(JNIEnv* Env, GuiScreen* Screen) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	ClassInterface->CallVoidMethod(Env, InstanceObject, "displayGuiScreen", Screen->GetInstanceObject());
}

void Minecraft::SetSession(JNIEnv* Env, jobject Session) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	ClassInterface->SetObjectField(Env, InstanceObject, "session", Session);
}

bool Minecraft::InGameHasFocus(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return false;
	}

	return ClassInterface->GetBooleanField(Env, InstanceObject, "inGameHasFocus");
}


Minecraft::~Minecraft()
{
}
//getClosestPlayerToEntity //()Lnet/minecraft/client/multiplayer/ServerData;