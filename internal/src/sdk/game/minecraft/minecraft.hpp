#pragma once

#include "entityrenderer.hpp"
#include "fontrenderer.hpp"
#include "gamesettings.hpp"
#include "guiscreen.hpp"
#include "movingobjectposition.hpp"
#include "player.hpp"
#include "playercontrollermp.hpp"
#include "rendermanager.hpp"
#include "serverdata.hpp"
#include "timer.hpp"
#include "framebuffer.hpp"
#include "world.hpp"
#include "mcsession.hpp"
#include "mousehelper.hpp"

#include "src/sdk/internal/templates/classbase.hpp"

class Minecraft : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> ClassInterface = nullptr;
public:
	Minecraft(JNIEnv* Env);

	//make this point to a shared ptr of the playerclass
	jobject GetThePlayer(JNIEnv* Env) const;
	jobject GetTheWorld(JNIEnv* Env) const;
	jobject GetCurrentScreen(JNIEnv* Env) const;
	jobject GetRenderManager(JNIEnv* Env) const;
	jobject GetPlayerController(JNIEnv* Env) const;
	jobject GetTimer(JNIEnv* Env) const;
	jobject GetEntityRenderer(JNIEnv* Env) const;
	jobject GetFontRenderer(JNIEnv* Env) const;
	jobject GetObjectMouseOver(JNIEnv* Env) const;
	jobject GetGameSettings(JNIEnv* Env) const;
	jobject GetCurrentServerData(JNIEnv* Env) const;
	jobject GetCurrentSession(JNIEnv* Env) const;

	std::shared_ptr<Player> GetThePlayerPtr(JNIEnv* Env) const;
	std::shared_ptr<Player> GetRenderViewEntityPtr(JNIEnv* Env) const;
	std::shared_ptr<World> GetTheWorldPtr(JNIEnv* Env) const;
	std::shared_ptr<GuiScreen> GetCurrentScreenPtr(JNIEnv* Env) const;
	std::shared_ptr<RenderManager> GetRenderManagerPtr(JNIEnv* Env) const;
	std::shared_ptr<PlayerControllerMP> GetPlayerControllerPtr(JNIEnv* Env) const;
	std::shared_ptr<Timer> GetTimerPtr(JNIEnv* Env) const;
	std::shared_ptr<EntityRenderer> GetEntityRendererPtr(JNIEnv* Env) const;
	std::shared_ptr<FontRenderer> GetFontRendererPtr(JNIEnv* Env) const;
	std::shared_ptr<GameSettings> GetGameSettingsPtr(JNIEnv* Env) const;
	std::shared_ptr<ServerData> GetCurrentServerDataPtr(JNIEnv* Env) const;
	std::shared_ptr<MovingObjectPosition> GetObjectMouseOverPtr(JNIEnv* Env) const;
	std::shared_ptr<Framebuffer> GetFrameBufferPtr(JNIEnv* Env) const;
	std::shared_ptr<MouseHelper> GetMouseHelperPtr(JNIEnv* Env) const;

	int GetDisplayWidth(JNIEnv* Env) const;
	int GetDisplayHeight(JNIEnv* Env) const;
	int GetDebugFPS(JNIEnv* Env) const;

	bool InGameHasFocus(JNIEnv* Env) const;

	void SetRightClickDelayTimer(JNIEnv* Env, int Ticks) const;
	void SetLeftClickCount(JNIEnv* Env, int Count) const;
	void SetPointedEntity(JNIEnv* Env, const Player* Object) const;
	void SetObjectMouseOver(JNIEnv* Env, const Player* Object, const Vector3<double>& HitPosition) const;
	void SetDebugFPS(JNIEnv* Env, int FPS) const;
	void DisplayGuiScreen(JNIEnv* Env, GuiScreen* Screen) const;
	void SetSession(JNIEnv* Env, jobject Session) const;

	~Minecraft();
};

//TODO: check differences and define them in Minecraft1_7 derivation, 1.8 should be fully working
//use respective class version derivation on declaration, methods with differences will be added here
class Minecraft1_7 : public Minecraft
{
public:
};

class Minecraft1_8 : public Minecraft
{
public:
};
