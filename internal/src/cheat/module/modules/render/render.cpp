#include "pch.h"
#include "render.hpp"
#define IMGUI_DEFINE_MATH_OPERATORS
#include <map>
#include <shared_mutex>
#include <GL/gl.h>
#include "cheat/module/modules/utils/data.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "sdk/game/minecraft/activerenderinfo.hpp"
#include "cheat/module/modules/world/antibot.hpp"
#include "cheat/module/modules/world/teams.hpp"
#include "cheat/module/modules/world/friends.hpp"

#include <ppl.h>
#include <concurrent_vector.h>

std::shared_mutex Mutex;
float LocalPlayerYaw = 0.F;
Vector3<double> LocalPlayerCoordinates;
std::vector<float> ProjectionMatrix;
std::vector<float> ModelViewMatrix;
concurrency::concurrent_vector<RenderData::ItemRenderInfo> ItemRenderQueue;
concurrency::concurrent_vector<RenderData::PlayerRenderInfo> PlayerRenderQueue;
concurrency::concurrent_vector<RenderData::RadarRenderInfo> RadarRenderQueue;

void Render::Run(JNIEnv* Env)
{
	std::unique_lock<std::shared_mutex> Lock(Mutex);
	Lock.unlock();
	//SEPERATED NAMETAGS MODULE/// TDOOOO;::::
	if (!ModuleManager::GetModuleByName("PlayerESP")->GetState() && !ModuleManager::GetModuleByName("Radar")->GetState() && !ModuleManager::GetModuleByName("NameTags")->GetState())
	{
		Lock.lock();
		PlayerRenderQueue.clear();
		RadarRenderQueue.clear();
		Lock.unlock();

		Sleep(100);
		return;
	}

	GameClasses ThreadClasses;
	if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses)) 
	{
		Lock.lock();
		PlayerRenderQueue.clear();
		RadarRenderQueue.clear();
		Lock.unlock();

		Sleep(5);
		return;
	}

	const auto& Minecraft = ThreadClasses.Minecraft;
	const auto& World = ThreadClasses.World;
	const auto& LocalPlayer = ThreadClasses.Player;

	const auto& Timer = Minecraft->GetTimerPtr(Env);
	const auto& RenderManager = Minecraft->GetRenderManagerPtr(Env);
	const auto& LocalPlayerPosition = LocalPlayer->GetPos(Env);

	const auto& PlayerEntityList = World->GetPlayerEntitiesWithinAABBVector(Env, ThreadClasses.Player.get(), ThreadClasses.Player->GetPos(Env), Render::RenderDistance);

	if (PlayerEntityList.empty())
	{
		Lock.lock();
		PlayerRenderQueue.clear();
		RadarRenderQueue.clear();
		Lock.unlock();

		Sleep(5);
		return;
	}

	concurrency::concurrent_vector<RenderData::PlayerRenderInfo> PlayerRenderQueueInfo;
	concurrency::concurrent_vector<RenderData::RadarRenderInfo> RadarRenderQueueInfo;

	for (const auto& Entity : PlayerEntityList)
	{
		const Vector3<float> EntityPosition = Entity->GetPos(Env);
		const Vector3<float> EntityLastTickPosition = Entity->GetLastTickPos(Env);

		const auto RenderPartialTicks = Timer->GetRenderPartialTicks(Env);
		const Vector3<float> RenderPosition = RenderManager->GetRenderPos(Env);
		const auto EntityDistanceToLocal = Entity->GetPos(Env).Distance(LocalPlayer->GetPos(Env));

		if (Env->IsSameObject(LocalPlayer->GetInstanceObject(), Entity->GetInstanceObject())) {
			continue;
		}

		if (!LocalPlayer->IsPlayer(Env)) {
			continue;
		}

		if (LocalPlayer->IsPlayer(Env) && AntiBot::IsBot(Env, World.get(), LocalPlayer.get(), Entity.get())) {
			continue;
		}

		if (ExcludeTeamMates && Teams::IsOnSameTeam(Env, LocalPlayer.get(), Entity.get())) {
			continue;
		}

		if (ExcludeFriends && Friends::IsFriend(Entity->GetEntityID(Env))) {
			continue;
		}

		RenderData::RadarRenderInfo RadarRenderInfo = 
		{
			EntityPosition
		};

		RadarRenderQueueInfo.push_back(RadarRenderInfo);

		auto AxisAlignedBoundingBoxNative = Entity->GetAxisAlignedBoundingBoxPtr(Env)->GetAxisAlignedBoundingBoxNative(Env);

		AxisAlignedBoundingBoxNative.MinX -= 0.2;
		AxisAlignedBoundingBoxNative.MinY -= -0.030;
		AxisAlignedBoundingBoxNative.MinZ -= 0.2;
		AxisAlignedBoundingBoxNative.MaxX += 0.2;
		AxisAlignedBoundingBoxNative.MaxY += 0.128;
		AxisAlignedBoundingBoxNative.MaxZ += 0.2;

		const auto AxisAlignedBoundingBoxInterpolated = AxisAlignedBoundingBoxNative.GetBoundingBox3D(EntityPosition, EntityLastTickPosition, RenderPosition, RenderPartialTicks);
		const auto EntityHealth = Entity->GetHealth(Env);
		
		RenderData::PlayerRenderInfo PlayerRenderInfo
		{
			Entity->GetName(Env),
			EntityDistanceToLocal,
			(double)EntityHealth,
			AxisAlignedBoundingBoxInterpolated,
		};

		PlayerRenderQueueInfo.push_back(PlayerRenderInfo);
	}


	const auto ActiveRenderInfo = ::ActiveRenderInfo(Env);

	Lock.lock();
	LocalPlayerCoordinates = LocalPlayerPosition;
	LocalPlayerYaw = LocalPlayer->GetRotationYaw(Env);

	ProjectionMatrix.clear();
	ModelViewMatrix.clear();

	ProjectionMatrix = ActiveRenderInfo.GetProjection(Env);
	ModelViewMatrix = ActiveRenderInfo.GetModelView(Env);

	PlayerRenderQueue.clear();
	RadarRenderQueue.clear();

	PlayerRenderQueue = PlayerRenderQueueInfo;
	RadarRenderQueue = RadarRenderQueueInfo;
	Lock.unlock();
}

void Render::RenderName(Frame* Frame, Thread* Thread, bool* ShouldCancel)
{
	if (!ModuleManager::GetModuleByName("NameTags")->GetState()) {
		return;
	}

	const auto Env = Thread->GetEnv();

	const auto EntityObject = Env->NewGlobalRef(HookManager->GetObjectParam(Frame, 1));
	const auto& Entity = std::make_shared<::Player>(Env, EntityObject);

	GameClasses ThreadClasses;
	if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses)) {
		return;
	}

	const auto& Minecraft = ThreadClasses.Minecraft;
	const auto& World = ThreadClasses.World;
	const auto& LocalPlayer = ThreadClasses.Player;

	if (!Entity->IsPlayer(Env)) {
		return;
	}

	if (AntiBot::IsBot(Env, World.get(), LocalPlayer.get(), Entity.get())) {
		return;
	}

	if (ExcludeTeamMates && Teams::IsOnSameTeam(Env, LocalPlayer.get(), Entity.get())) {
		return;
	}

	if (ExcludeFriends && Friends::IsFriend(Entity->GetEntityID(Env))) {
		return;
	}

	*ShouldCancel = true;
}

#include <iomanip> 

std::string RoundAndTrim(float num) {
	std::stringstream stream;
	stream << std::fixed << std::setprecision(2) << num;
	std::string roundedString = stream.str();

	while (roundedString.back() == '0' && roundedString.find('.') != std::string::npos) {
		roundedString.pop_back();
	}

	if (roundedString.back() == '.') {
		roundedString.pop_back();
	}

	return roundedString;
}

void PlayerESP::OnImGuiRender(JNIEnv* Env)
{
	if (!this->GetState()) {
		return;
	}

	if (PlayerESP::RenderMode == RENDER_3D && !ModuleManager::GetModuleByName("NameTags")->GetState()) {
		return;
	}

	std::shared_lock<std::shared_mutex> Lock(Mutex);
	
	if (PlayerRenderQueue.empty()) {
		return;
	}

	if (ProjectionMatrix.size() != 16 || ModelViewMatrix.size() != 16) {
		return;
	}

	struct ESPAnimations
	{
		ImVec4 PrevPos;
	};

	static std::map< const char*, ESPAnimations > Animations;

	for (const auto& PlayerRenderInfo : PlayerRenderQueue)
	{
		auto BoundingBoxData = PlayerRenderInfo.BoundingBoxData;
		const auto DistanceToLocal = PlayerRenderInfo.Distance;

		const char* AnimationID = PlayerRenderInfo.EntityName.c_str();

		auto AnimationMap = Animations.find(AnimationID);
		if (AnimationMap == Animations.end())
		{
			Animations.insert({ AnimationID, ESPAnimations() });
			AnimationMap = Animations.find(AnimationID);
		}

		const auto BoundingBoxScreen = BoundingBoxData.GetScreenPosition(ProjectionMatrix, ModelViewMatrix, static_cast<int>(DisplayHeight), static_cast<int>(DisplayWidth));

		const auto BoundingBox = ImRect(ImVec2(BoundingBoxScreen.X, BoundingBoxScreen.Y), ImVec2(BoundingBoxScreen.Z, BoundingBoxScreen.W));

		if (BoundingBox.GetWidth() < 0.F) {
			continue;
		}

		const auto FadeFactor = DistanceToLocal > Render::FadeDistance ? 1.F : DistanceToLocal / Render::FadeDistance;
		const auto ScaledProjection = std::clamp(BoundingBox.GetHeight() / (0.33038348082F * DisplayHeight), 0.F, 1.F);
		const auto ScaleFactor = Render::UniformScaling ? ( 1.F - ScaledProjection >= 1 ? 0.F : ScaledProjection ) : 1.F;
		auto ScaleFactorA = ScaleFactor;

		if (ScaleFactorA < 0.2F) {
			ScaleFactorA = 0.2F;
		}

		const auto BackgroundDrawList = ImGui::GetBackgroundDrawList();

		if (PlayerESP::RenderMode == RENDER_2D)
		{
			if (DrawESPBackground)
			{
				if (DrawESPShadow) {
					BackgroundDrawList->AddShadowRect(BoundingBox.Min, BoundingBox.Max, Color(0, 0, 0, 255 * FadeFactor), 10.F, ImVec2(0.F, 0.F));
				}

				BackgroundDrawList->AddRectFilled(BoundingBox.Min, BoundingBox.Max, Color(EspBackgroundColor).SetAlpha((255.F * EspBackgroundColor.w) * FadeFactor), 0);
			}

			

			if (DrawESPOutlines) 
			{
				if (RenderStyle == CORNERS)
				{
					const auto Percentage = BoundingBox.GetHeight() * 0.15F;

					//if (DrawOutlines && Render::UniformScaling)
					//{
					//	/*BackgroundDrawList->AddLine(ImVec2(BoundingBox.Min.x, BoundingBox.Min.y), ImVec2(BoundingBox.Min.x + Percentage, BoundingBox.Min.y), Color(0, 0, 0).SetAlpha(255 * FadeFactor), 3 * ScaleFactor);
					//	BackgroundDrawList->AddLine(ImVec2(BoundingBox.Min.x, BoundingBox.Min.y), ImVec2(BoundingBox.Min.x, BoundingBox.Min.y + Percentage), Color(0, 0, 0).SetAlpha(255 * FadeFactor), 3 * ScaleFactor);

					//	BackgroundDrawList->AddLine(ImVec2(BoundingBox.Min.x, BoundingBox.Max.y), ImVec2(BoundingBox.Min.x, BoundingBox.Max.y - Percentage), Color(0, 0, 0).SetAlpha(255 * FadeFactor), 3 * ScaleFactor);
					//	BackgroundDrawList->AddLine(ImVec2(BoundingBox.Min.x, BoundingBox.Max.y), ImVec2(BoundingBox.Min.x + Percentage, BoundingBox.Max.y), Color(0, 0, 0).SetAlpha(255 * FadeFactor), 3 * ScaleFactor);

					//	BackgroundDrawList->AddLine(ImVec2(BoundingBox.Max.x, BoundingBox.Min.y), ImVec2(BoundingBox.Max.x, BoundingBox.Min.y + Percentage), Color(0, 0, 0).SetAlpha(255 * FadeFactor), 3 * ScaleFactor);
					//	BackgroundDrawList->AddLine(ImVec2(BoundingBox.Max.x, BoundingBox.Min.y), ImVec2(BoundingBox.Max.x - Percentage, BoundingBox.Min.y), Color(0, 0, 0).SetAlpha(255 * FadeFactor), 3 * ScaleFactor);

					//	BackgroundDrawList->AddLine(ImVec2(BoundingBox.Max.x, BoundingBox.Max.y), ImVec2(BoundingBox.Max.x - Percentage, BoundingBox.Max.y), Color(0, 0, 0).SetAlpha(255 * FadeFactor), 3 * ScaleFactor);
					//	BackgroundDrawList->AddLine(ImVec2(BoundingBox.Max.x, BoundingBox.Max.y), ImVec2(BoundingBox.Max.x, BoundingBox.Max.y - Percentage), Color(0, 0, 0).SetAlpha(255 * FadeFactor), 3 * ScaleFactor);*/
					//	// Draw lines for the corners of the bounding box
					//	BackgroundDrawList->AddLine(ImVec2(BoundingBox.Min.x, BoundingBox.Min.y), ImVec2(BoundingBox.Min.x + Percentage, BoundingBox.Min.y), Color(0, 0, 0).SetAlpha(255 * FadeFactor), 3 * ScaleFactor);
					//	BackgroundDrawList->AddLine(ImVec2(BoundingBox.Min.x, BoundingBox.Min.y ), ImVec2(BoundingBox.Min.x, BoundingBox.Min.y + Percentage), Color(0, 0, 0).SetAlpha(255 * FadeFactor), 3 * ScaleFactor);

					//	BackgroundDrawList->AddLine(ImVec2(BoundingBox.Min.x, BoundingBox.Max.y), ImVec2(BoundingBox.Min.x + Percentage, BoundingBox.Max.y), Color(0, 0, 0).SetAlpha(255 * FadeFactor), 3 * ScaleFactor);
					//	BackgroundDrawList->AddLine(ImVec2(BoundingBox.Min.x, BoundingBox.Max.y), ImVec2(BoundingBox.Min.x, BoundingBox.Max.y - Percentage), Color(0, 0, 0).SetAlpha(255 * FadeFactor), 3 * ScaleFactor);

					//	BackgroundDrawList->AddLine(ImVec2(BoundingBox.Max.x, BoundingBox.Min.y), ImVec2(BoundingBox.Max.x - Percentage, BoundingBox.Min.y), Color(0, 0, 0).SetAlpha(255 * FadeFactor), 3 * ScaleFactor);
					//	BackgroundDrawList->AddLine(ImVec2(BoundingBox.Max.x, BoundingBox.Min.y), ImVec2(BoundingBox.Max.x, BoundingBox.Min.y + Percentage), Color(0, 0, 0).SetAlpha(255 * FadeFactor), 3 * ScaleFactor);

					//	BackgroundDrawList->AddLine(ImVec2(BoundingBox.Max.x, BoundingBox.Max.y), ImVec2(BoundingBox.Max.x - Percentage, BoundingBox.Max.y), Color(0, 0, 0).SetAlpha(255 * FadeFactor), 3 * ScaleFactor);
					//	BackgroundDrawList->AddLine(ImVec2(BoundingBox.Max.x, BoundingBox.Max.y), ImVec2(BoundingBox.Max.x, BoundingBox.Max.y - Percentage), Color(0, 0, 0).SetAlpha(255 * FadeFactor), 3 * ScaleFactor);
					//}

					//// Draw lines for the corners of the bounding box
					//BackgroundDrawList->AddLine(ImVec2(BoundingBox.Min.x, BoundingBox.Min.y), ImVec2(BoundingBox.Min.x + Percentage, BoundingBox.Min.y), Color(EspOutlineColor).SetAlpha(255 * FadeFactor), 1 * ScaleFactor);
					//BackgroundDrawList->AddLine(ImVec2(BoundingBox.Min.x, BoundingBox.Min.y), ImVec2(BoundingBox.Min.x, BoundingBox.Min.y + Percentage), Color(EspOutlineColor).SetAlpha(255 * FadeFactor), 1 * ScaleFactor);

					//BackgroundDrawList->AddLine(ImVec2(BoundingBox.Min.x, BoundingBox.Max.y), ImVec2(BoundingBox.Min.x + Percentage, BoundingBox.Max.y), Color(EspOutlineColor).SetAlpha(255 * FadeFactor), 1 * ScaleFactor);
					//BackgroundDrawList->AddLine(ImVec2(BoundingBox.Min.x, BoundingBox.Max.y), ImVec2(BoundingBox.Min.x, BoundingBox.Max.y - Percentage), Color(EspOutlineColor).SetAlpha(255 * FadeFactor), 1 * ScaleFactor);

					//BackgroundDrawList->AddLine(ImVec2(BoundingBox.Max.x, BoundingBox.Min.y), ImVec2(BoundingBox.Max.x - Percentage, BoundingBox.Min.y), Color(EspOutlineColor).SetAlpha(255 * FadeFactor), 1 * ScaleFactor);
					//BackgroundDrawList->AddLine(ImVec2(BoundingBox.Max.x, BoundingBox.Min.y), ImVec2(BoundingBox.Max.x, BoundingBox.Min.y + Percentage), Color(EspOutlineColor).SetAlpha(255 * FadeFactor), 1 * ScaleFactor);

					//BackgroundDrawList->AddLine(ImVec2(BoundingBox.Max.x, BoundingBox.Max.y), ImVec2(BoundingBox.Max.x - Percentage, BoundingBox.Max.y), Color(EspOutlineColor).SetAlpha(255 * FadeFactor), 1 * ScaleFactor);
					//BackgroundDrawList->AddLine(ImVec2(BoundingBox.Max.x, BoundingBox.Max.y), ImVec2(BoundingBox.Max.x, BoundingBox.Max.y - Percentage), Color(EspOutlineColor).SetAlpha(255 * FadeFactor), 1 * ScaleFactor);

					float x = BoundingBox.Min.x;
					float y = BoundingBox.Min.y;
					float w = BoundingBox.Max.x - x;
					float h = BoundingBox.Max.y - y;

					float linew = (w / 4.f);
					float lineh = (w / 5.f);

					if (DrawOutlines)
					{
						// top left
						BackgroundDrawList->AddLine({ x - 1.f, y - 1.f }, { x + linew, y - 1.f }, Color(OutlineColor).SetAlpha(255 * FadeFactor), 1 * ScaleFactor);
						BackgroundDrawList->AddLine({ x - 1.f, y - 1.f }, { x - 1.f, y + lineh }, Color(OutlineColor).SetAlpha(255 * FadeFactor), 1 * ScaleFactor);

						// bot left
						BackgroundDrawList->AddLine({ x - 1.f, y + h - lineh }, { x - 1.f, y + h + 2.f }, Color(OutlineColor).SetAlpha(255 * FadeFactor), 1 * ScaleFactor);
						BackgroundDrawList->AddLine({ x - 1.f, y + h + 1.f }, { x + linew, y + h + 1.f }, Color(OutlineColor).SetAlpha(255 * FadeFactor), 1 * ScaleFactor);

						// top right
						BackgroundDrawList->AddLine({ x + w - linew, y - 1.f }, { x + w + 1.f, y - 1.f }, Color(OutlineColor).SetAlpha(255 * FadeFactor), 1 * ScaleFactor);
						BackgroundDrawList->AddLine({ x + w + 1.f, y - 1.f }, { x + w + 1.f, y + lineh }, Color(OutlineColor).SetAlpha(255 * FadeFactor), 1 * ScaleFactor);

						// bot right
						BackgroundDrawList->AddLine({ x + w + 1.f, y + h - lineh }, { x + w + 1.f, y + h + 1.f }, Color(OutlineColor).SetAlpha(255 * FadeFactor), 1 * ScaleFactor);
						BackgroundDrawList->AddLine({ x + w - linew, y + h + 1.f }, { x + w + 1.f, y + h + 1.f }, Color(OutlineColor).SetAlpha(255 * FadeFactor), 1 * ScaleFactor);
					}

					// top left
					BackgroundDrawList->AddLine({ x, y }, { x, y + lineh }, Color(EspOutlineColor).SetAlpha(255 * FadeFactor), 1 * ScaleFactor);
					BackgroundDrawList->AddLine({ x, y }, { x + linew, y }, Color(EspOutlineColor).SetAlpha(255 * FadeFactor), 1 * ScaleFactor);

					// top right
					BackgroundDrawList->AddLine({ x + w - linew, y }, { x + w, y }, Color(EspOutlineColor).SetAlpha(255 * FadeFactor), 1 * ScaleFactor);
					BackgroundDrawList->AddLine({ x + w, y }, { x + w, y + lineh }, Color(EspOutlineColor).SetAlpha(255 * FadeFactor), 1 * ScaleFactor);

					// bot left
					BackgroundDrawList->AddLine({ x, y + h - lineh }, { x, y + h + 1.f }, Color(EspOutlineColor).SetAlpha(255 * FadeFactor), 1 * ScaleFactor);
					BackgroundDrawList->AddLine({ x, y + h }, { x + linew, y + h }, Color(EspOutlineColor).SetAlpha(255 * FadeFactor), 1 * ScaleFactor);

					// bot right
					BackgroundDrawList->AddLine({ x + w - linew, y + h }, { x + w, y + h }, Color(EspOutlineColor).SetAlpha(255 * FadeFactor), 1 * ScaleFactor);
					BackgroundDrawList->AddLine({ x + w, y + h - lineh }, { x + w, y + h }, Color(EspOutlineColor).SetAlpha(255 * FadeFactor), 1 * ScaleFactor);

					//BackgroundDrawList->AddLine(ImVec2(BoundingBox.Min.x, BoundingBox.Min.y), ImVec2(BoundingBox.Min.x + Percentage, BoundingBox.Min.y), Color(EspOutlineColor).SetAlpha(255 * FadeFactor), 1 * ScaleFactor);
					//BackgroundDrawList->AddLine(ImVec2(BoundingBox.Min.x, BoundingBox.Min.y), ImVec2(BoundingBox.Min.x, BoundingBox.Min.y + Percentage), Color(EspOutlineColor).SetAlpha(255 * FadeFactor), 1 * ScaleFactor);

					//BackgroundDrawList->AddLine(ImVec2(BoundingBox.Min.x, BoundingBox.Max.y), ImVec2(BoundingBox.Min.x, BoundingBox.Max.y - Percentage), Color(EspOutlineColor).SetAlpha(255 * FadeFactor), 1 * ScaleFactor);
					//BackgroundDrawList->AddLine(ImVec2(BoundingBox.Min.x, BoundingBox.Max.y), ImVec2(BoundingBox.Min.x + Percentage, BoundingBox.Max.y), Color(EspOutlineColor).SetAlpha(255 * FadeFactor), 1 * ScaleFactor);

					//BackgroundDrawList->AddLine(ImVec2(BoundingBox.Max.x, BoundingBox.Min.y), ImVec2(BoundingBox.Max.x, BoundingBox.Min.y + Percentage), Color(EspOutlineColor).SetAlpha(255 * FadeFactor), 1 * ScaleFactor);
					//BackgroundDrawList->AddLine(ImVec2(BoundingBox.Max.x, BoundingBox.Min.y), ImVec2(BoundingBox.Max.x - Percentage, BoundingBox.Min.y), Color(EspOutlineColor).SetAlpha(255 * FadeFactor), 1 * ScaleFactor);

					//BackgroundDrawList->AddLine(ImVec2(BoundingBox.Max.x, BoundingBox.Max.y), ImVec2(BoundingBox.Max.x - Percentage, BoundingBox.Max.y), Color(EspOutlineColor).SetAlpha(255 * FadeFactor), 1 * ScaleFactor);
					//BackgroundDrawList->AddLine(ImVec2(BoundingBox.Max.x, BoundingBox.Max.y), ImVec2(BoundingBox.Max.x, BoundingBox.Max.y - Percentage), Color(EspOutlineColor).SetAlpha(255 * FadeFactor), 1 * ScaleFactor);
				}

				if (RenderStyle == FULL)
				{
					if (DrawOutlines) {
						BackgroundDrawList->AddRect(BoundingBox.Min, BoundingBox.Max, Color(OutlineColor).SetAlpha(255 * FadeFactor), 0, 0, 3 * ScaleFactor);
					}

					BackgroundDrawList->AddRect(BoundingBox.Min, BoundingBox.Max, Color(EspOutlineColor).SetAlpha(255 * FadeFactor), 0, 0, 1 * ScaleFactor);
				}
				//
			}
		}

		//HEALTH BAR/
		//336 at 1017px

		//157 at 481px

		if (DrawHealthBar)
		{
			const float HealthTextPadding = 2.F /** ScaleFactorA*/;
			const float HealthBarWith = 3.F /** ScaleFactorA*/;
			const float HealthBarPadding = 2.F /** ScaleFactorA*/;
			const auto HealthBarBoundingBox = ImRect(ImVec2(BoundingBox.Min.x - HealthBarPadding - HealthBarWith, BoundingBox.Min.y), ImVec2(BoundingBox.Min.x - HealthBarPadding, BoundingBox.Max.y));
			//336
			const float EntityHealthMax = 20.0F;//todo
			const float EntityHealth = std::clamp((float)PlayerRenderInfo.Health, 0.0F, EntityHealthMax);//todo

			/*EntityHealth = round(EntityHealth * 100) / 100;
			EntityHealthMax = round(EntityHealthMax * 100) / 100;*/

			const auto HealthPercentage = std::clamp(1.0F - (EntityHealth / EntityHealthMax), 0.F, 1.F);
			const auto HealthBarShadedBoundingBox = ImRect(ImVec2(BoundingBox.Min.x - HealthBarPadding - HealthBarWith, BoundingBox.Min.y + ((BoundingBox.Max.y - BoundingBox.Min.y) * HealthPercentage)), ImVec2(BoundingBox.Min.x - HealthBarPadding, BoundingBox.Max.y));

			//BackgroundDrawList->AddShadowRect(HealthBarBoundingBox.Min, HealthBarBoundingBox.Max, Color(0, 0, 0, 255 * FadeFactor), 10.F , ImVec2(0.F, 0.F));
			BackgroundDrawList->AddRectFilled(HealthBarBoundingBox.Min, HealthBarBoundingBox.Max, Color(/*HealthBarBackgroundColor*/EspBackgroundColor).SetAlpha((/*HealthBarBackgroundColor*//*EspBackgroundColor.w * */255) * FadeFactor), 0 * ScaleFactor);
			BackgroundDrawList->AddRectFilled(HealthBarShadedBoundingBox.Min, HealthBarShadedBoundingBox.Max, Color(HealthBarShadedColor).SetAlpha((HealthBarShadedColor.w * 255)* FadeFactor), 0 * ScaleFactor);

			if (DrawOutlines) {
				BackgroundDrawList->AddRect(HealthBarBoundingBox.Min, HealthBarBoundingBox.Max, Color(OutlineColor).SetAlpha(255 * FadeFactor), 0, 0, 0.5 * ScaleFactor);
			}



		}

		if (ModuleManager::GetModuleByName("NameTags")->GetState())
		{
			char DistanceTextC[32];
			std::snprintf(DistanceTextC, sizeof(DistanceTextC), "%.1f", PlayerRenderInfo.Distance);
			std::string DistanceString(DistanceTextC);

			char HealthTextC[32];
			std::snprintf(DistanceTextC, sizeof(DistanceTextC), "%.1f", PlayerRenderInfo.Health);
			std::string HealthString(DistanceTextC);

			std::string NameTagText;

			NameTagText += std::string(PlayerRenderInfo.EntityName);
			NameTagText += std::string(" ") + HealthString;
			NameTagText += std::string(" ") + DistanceString;

			const auto NameTagSize = ImGui::CalcTextSize(NameTagText.c_str());
			const auto Padding = 5.f;
			const auto TopContainerBoundingBox = ImRect(ImVec2(BoundingBox.GetCenter().x - (NameTagSize.x / 2.F) - 5.F, BoundingBox.Min.y - Padding - (NameTagSize.y) - 2.F), ImVec2(BoundingBox.GetCenter().x + (NameTagSize.x / 2.F) + 5.F, BoundingBox.Min.y - Padding + 2.F));
			const auto NamePosition = ImVec2(BoundingBox.GetCenter().x - (NameTagSize.x / 2.F), BoundingBox.Min.y - Padding - (NameTagSize.y));

			//BackgroundDrawList->AddRectFilled(TopContainerBoundingBox.Min, TopContainerBoundingBox.Max, Color(0, 0, 0, 125));

			const auto NameColor = ImGui::GetColorU32(ImVec4(1.F, 1.F, 1.F, 1.F * FadeFactor));
			const auto HealthColor = PlayerRenderInfo.Health >= 12.F ? ImColor(0, 99, 10, (int)(255 * FadeFactor)) : PlayerRenderInfo.Health <= 4.F ? ImColor(171, 0, 0, (int)(255 * FadeFactor)) : ImColor(171, 154, 0, (int)(255 * FadeFactor));

			BackgroundDrawList->AddShadowRect(TopContainerBoundingBox.Min, TopContainerBoundingBox.Max, ImGui::GetColorU32(ImVec4(0.F, 0.F, 0.F, (255.F * FadeFactor) / 255.F)), 10.F, { 0.F, 0.F }, 0, 2.F);
			BackgroundDrawList->AddRectFilled(TopContainerBoundingBox.Min, TopContainerBoundingBox.Max, ImGui::GetColorU32(ImVec4(0.F, 0.F, 0.F, (125.F * FadeFactor) / 255.F)), 2.F);
			BackgroundDrawList->AddRect(TopContainerBoundingBox.Min, TopContainerBoundingBox.Max, Color(0, 0, 0, 255 * FadeFactor));

			float XOffset = 0.F;

			const auto NameSize = ImGui::CalcTextSize(PlayerRenderInfo.EntityName.c_str());
			BackgroundDrawList->AddText(NamePosition, NameColor, PlayerRenderInfo.EntityName.data());

			XOffset += NameSize.x;

			const auto HealthText = std::string(" ") + HealthString + std::string("");
			const auto HealthSize = ImGui::CalcTextSize(HealthText.c_str());

			BackgroundDrawList->AddText(ImVec2(NamePosition.x + XOffset, NamePosition.y), HealthColor, HealthText.c_str());

			XOffset += HealthSize.x;

			const auto DistanceText = std::string(" ") + DistanceString;
			const auto DistanceSize = ImGui::CalcTextSize(DistanceText.c_str());

			BackgroundDrawList->AddText(ImVec2(NamePosition.x + XOffset, NamePosition.y), ImColor(0, 99, 10, (int)(255 * FadeFactor)), DistanceText.c_str());
		}
	}
}

void ItemESP::OnImGuiRender(JNIEnv* Env) 
{
}
double normalize(double value) {
	// Map the range [-180, 180] to [0, 360]
	return (value + 180.0) * (360.0 / 360.0);
}

void Radar::OnImGuiRender(JNIEnv* Env)
{
	if (!this->GetState()) {
		return;
	}

	std::shared_lock<std::shared_mutex> Lock(Mutex);

	struct RadarAnimations
	{
		float Size;
		float Alpha;
		float AlphaOffset;
	};

	static std::map< const char*, RadarAnimations > Animations;

	const char* AnimationID = "sda2sads";

	auto AnimationMap = Animations.find(AnimationID);
	if (AnimationMap == Animations.end())
	{
		Animations.insert({ AnimationID, RadarAnimations() });
		AnimationMap = Animations.find(AnimationID);
	}

	AnimationMap->second.Size = 222.F;
	AnimationMap->second.Alpha = ImLerp(AnimationMap->second.Alpha, RadarRenderQueue.empty() ? 0.F : 125.F, ImGui::GetIO().DeltaTime * 1 * 4.F);
	AnimationMap->second.AlphaOffset = ImLerp(AnimationMap->second.AlphaOffset, RadarRenderQueue.empty() ? 0.F : 1.F, ImGui::GetIO().DeltaTime * 1 * 4.F);

	const auto Style = ImGui::GetStyle();

	ImGui::GetStyle().WindowBorderSize = 0.1F;
	ImGui::GetStyle().WindowRounding = 6.F;

	ImGui::GetStyle().WindowShadowSize = 33.F;
	ImGui::GetStyle().WindowPadding = ImVec2(0.F, 0.F);
	ImGui::GetStyle().FramePadding = ImVec2(0.F, 0.F);
	ImGui::GetStyle().Colors[ImGuiCol_WindowShadow] = Color(0.F, 0.F, 0.F, 255 * AnimationMap->second.AlphaOffset).GetColorVec4();
	ImGui::GetStyle().Colors[ImGuiCol_Border] = Color(0.F, 0.F, 0.F, 255 * AnimationMap->second.AlphaOffset).GetColorVec4();

	ImGui::SetNextWindowBgAlpha(AnimationMap->second.Alpha / 255.F);
	ImGui::SetNextWindowSize(ImVec2(AnimationMap->second.Size, AnimationMap->second.Size));
	ImGui::Begin("Radar", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
	{
		const auto WindowPos = ImGui::GetWindowPos();
		const auto DrawList = ImGui::GetWindowDrawList();
		const auto OverlayBounds = ImRect(WindowPos, ImVec2(WindowPos.x + 222.F, WindowPos.y + 222.F));

		if (!RadarRenderQueue.empty())
		{
			for (const auto& RadarRenderInfo : RadarRenderQueue)
			{
				auto EntityCoordinates = Vector2<double>(RadarRenderInfo.PlayerCoordinates.X, RadarRenderInfo.PlayerCoordinates.Z);
				auto LocalCoordinates = Vector2<double>(LocalPlayerCoordinates.X, LocalPlayerCoordinates.Z);

				auto OverlayCenter = Vector2<double>(OverlayBounds.GetCenter().x, OverlayBounds.GetCenter().y);
				auto DistanceDelta = EntityCoordinates - LocalCoordinates;
				auto Scale = EntityCoordinates.Distance(LocalCoordinates) * (0.02 * RadarZoom);
				auto RenderPreRotation = OverlayCenter + Vector2(DistanceDelta.X * Scale, DistanceDelta.Y * Scale);
				auto RenderPos = RenderPreRotation.Rotate(normalize(-LocalPlayerYaw), OverlayCenter);

				DrawList->AddCircleFilled(ImVec2(RenderPos.X, RenderPos.Y), 5.F, Color(BlipColor));
				DrawList->AddShadowCircle(ImVec2(RenderPos.X, RenderPos.Y), 5.F, Color(BlipColor), 12.F, ImVec2(0.F, 0.F));

			}
		}

		const auto YAxis = ImRect(ImVec2(OverlayBounds.GetCenter().x, OverlayBounds.Min.y), ImVec2(OverlayBounds.GetCenter().x, OverlayBounds.Max.y));
		const auto XAxis = ImRect(ImVec2(OverlayBounds.Min.x, OverlayBounds.GetCenter().y), ImVec2(OverlayBounds.Max.x, OverlayBounds.GetCenter().y));

		DrawList->AddShadowRect(ImVec2(YAxis.Min.x - 0.5F, YAxis.Min.y), ImVec2(YAxis.Max.x + 0.5F, YAxis.Max.y), Color(70.F, 70.F, 70.F, 125 * AnimationMap->second.AlphaOffset), 7.F, ImVec2(0.F, 0.F));
		DrawList->AddShadowRect(ImVec2(XAxis.Min.x, XAxis.Min.y - 0.5F), ImVec2(XAxis.Max.x, XAxis.Max.y + 0.5F), Color(70.F, 70.F, 70.F, 125 * AnimationMap->second.AlphaOffset), 7.F, ImVec2(0.F, 0.F));

		DrawList->AddLine(YAxis.Min, YAxis.Max, Color(70.F, 70.F, 70.F, 125 * AnimationMap->second.AlphaOffset), 1.F);
		DrawList->AddLine(XAxis.Min, XAxis.Max, Color(70.F, 70.F, 70.F, 125 * AnimationMap->second.AlphaOffset), 1.F);

		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[4]);

		const auto RadarText = std::to_string(RadarRenderQueue.size()) + (RadarRenderQueue.size() == 1 ? " entity found" : " entities found");
		const auto RadarTextSize = ImGui::CalcTextSize(RadarText.data());
		const auto RadarTextBoundingBox = ImRect(ImVec2(OverlayBounds.GetCenter().x - RadarTextSize.x / 2.F - 3.F , OverlayBounds.Max.y + 1.5F), ImVec2(OverlayBounds.GetCenter().x + RadarTextSize.x / 2.F + 3.F , OverlayBounds.Max.y + RadarTextSize.y + 3.F + 1.5F));

		ImGui::GetBackgroundDrawList()->AddShadowRect(RadarTextBoundingBox.Min, RadarTextBoundingBox.Max, Color(0.F, 0.F, 0.F, 125 * AnimationMap->second.AlphaOffset), 7.F, ImVec2(0.F, 0.F), 2.F);
		ImGui::GetBackgroundDrawList()->AddRectFilled(RadarTextBoundingBox.Min, RadarTextBoundingBox.Max, Color(0.F, 0.F, 0.F, 125 * AnimationMap->second.AlphaOffset), 2.F);

		ImGui::GetBackgroundDrawList()->AddText(ImVec2(OverlayBounds.GetCenter().x - RadarTextSize.x / 2.F, OverlayBounds.Max.y + 3.F ), Color(255.F, 255.F, 255.F, 255 * AnimationMap->second.AlphaOffset), RadarText.data());
		ImGui::PopFont();
	}
	ImGui::End();

	ImGui::GetStyle() = Style;
}

void PlayerESP::OnRender(JNIEnv* Env)
{
	if (!this->GetState()) {
		return;
	}

	std::shared_lock<std::shared_mutex> Lock(Mutex);

	if (PlayerRenderQueue.empty() || RenderMode != RENDER_3D) {
		return;
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

			// Front face
			glVertex3f(BoundingBox.MinX, BoundingBox.MinY, BoundingBox.MinZ);
			glVertex3f(BoundingBox.MinX, BoundingBox.MaxY, BoundingBox.MinZ);
			glVertex3f(BoundingBox.MaxX, BoundingBox.MaxY, BoundingBox.MinZ);
			glVertex3f(BoundingBox.MaxX, BoundingBox.MinY, BoundingBox.MinZ);

			// Back face
			glVertex3f(BoundingBox.MinX, BoundingBox.MinY, BoundingBox.MaxZ);
			glVertex3f(BoundingBox.MaxX, BoundingBox.MinY, BoundingBox.MaxZ);
			glVertex3f(BoundingBox.MaxX, BoundingBox.MaxY, BoundingBox.MaxZ);
			glVertex3f(BoundingBox.MinX, BoundingBox.MaxY, BoundingBox.MaxZ);

			// Left face
			glVertex3f(BoundingBox.MinX, BoundingBox.MinY, BoundingBox.MinZ);
			glVertex3f(BoundingBox.MinX, BoundingBox.MinY, BoundingBox.MaxZ);
			glVertex3f(BoundingBox.MinX, BoundingBox.MaxY, BoundingBox.MaxZ);
			glVertex3f(BoundingBox.MinX, BoundingBox.MaxY, BoundingBox.MinZ);

			// Right face
			glVertex3f(BoundingBox.MaxX, BoundingBox.MinY, BoundingBox.MinZ);
			glVertex3f(BoundingBox.MaxX, BoundingBox.MaxY, BoundingBox.MinZ);
			glVertex3f(BoundingBox.MaxX, BoundingBox.MaxY, BoundingBox.MaxZ);
			glVertex3f(BoundingBox.MaxX, BoundingBox.MinY, BoundingBox.MaxZ);

			// Top face
			glVertex3f(BoundingBox.MinX, BoundingBox.MaxY, BoundingBox.MinZ);
			glVertex3f(BoundingBox.MinX, BoundingBox.MaxY, BoundingBox.MaxZ);
			glVertex3f(BoundingBox.MaxX, BoundingBox.MaxY, BoundingBox.MaxZ);
			glVertex3f(BoundingBox.MaxX, BoundingBox.MaxY, BoundingBox.MinZ);

			// Bottom face
			glVertex3f(BoundingBox.MinX, BoundingBox.MinY, BoundingBox.MinZ);
			glVertex3f(BoundingBox.MaxX, BoundingBox.MinY, BoundingBox.MinZ);
			glVertex3f(BoundingBox.MaxX, BoundingBox.MinY, BoundingBox.MaxZ);
			glVertex3f(BoundingBox.MinX, BoundingBox.MinY, BoundingBox.MaxZ);

			glEnd();
		};


	auto DrawBoundingBoxCorners = [](const AxisAlignedBB_T<double>& BoundingBox, double percentage)
		{
			// Calculate the lengths of the corners based on the percentage
			double cornerLengthX = (BoundingBox.MaxX - BoundingBox.MinX) * percentage;
			double cornerLengthY = (BoundingBox.MaxY - BoundingBox.MinY) * percentage;
			double cornerLengthZ = (BoundingBox.MaxZ - BoundingBox.MinZ) * percentage;

			glBegin(GL_LINES);

			// Front face corners
			glVertex3d(BoundingBox.MinX, BoundingBox.MinY, BoundingBox.MinZ);
			glVertex3d(BoundingBox.MinX + cornerLengthX, BoundingBox.MinY, BoundingBox.MinZ);

			glVertex3d(BoundingBox.MinX, BoundingBox.MaxY, BoundingBox.MinZ);
			glVertex3d(BoundingBox.MinX, BoundingBox.MaxY - cornerLengthY, BoundingBox.MinZ);

			glVertex3d(BoundingBox.MaxX, BoundingBox.MaxY, BoundingBox.MinZ);
			glVertex3d(BoundingBox.MaxX - cornerLengthX, BoundingBox.MaxY, BoundingBox.MinZ);

			glVertex3d(BoundingBox.MaxX, BoundingBox.MinY, BoundingBox.MinZ);
			glVertex3d(BoundingBox.MaxX, BoundingBox.MinY + cornerLengthY, BoundingBox.MinZ);

			// Back face corners
			glVertex3d(BoundingBox.MinX, BoundingBox.MinY, BoundingBox.MaxZ);
			glVertex3d(BoundingBox.MinX + cornerLengthX, BoundingBox.MinY, BoundingBox.MaxZ);

			glVertex3d(BoundingBox.MinX, BoundingBox.MaxY, BoundingBox.MaxZ);
			glVertex3d(BoundingBox.MinX, BoundingBox.MaxY - cornerLengthY, BoundingBox.MaxZ);

			glVertex3d(BoundingBox.MaxX, BoundingBox.MaxY, BoundingBox.MaxZ);
			glVertex3d(BoundingBox.MaxX - cornerLengthX, BoundingBox.MaxY, BoundingBox.MaxZ);

			glVertex3d(BoundingBox.MaxX, BoundingBox.MinY, BoundingBox.MaxZ);
			glVertex3d(BoundingBox.MaxX, BoundingBox.MinY + cornerLengthY, BoundingBox.MaxZ);

			glEnd();
		};

	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(ProjectionMatrix.data());
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(ModelViewMatrix.data());

	glPushMatrix();
	glEnable(GL_LINE_SMOOTH);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glDepthMask(false);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glLineWidth(1.f);

	for (const auto& PlayerRenderData : PlayerRenderQueue)
	{
		const auto FadeFactor = PlayerRenderData.Distance > Render::FadeDistance ? 1.F : PlayerRenderData.Distance / Render::FadeDistance;

		const auto BackgroundColorC = Color(EspBackgroundColor).SetAlpha((255.F * EspBackgroundColor.w) * FadeFactor).GetColorVec4();
		const auto OutlineColorC = Color(EspOutlineColor).SetAlpha(255.F * FadeFactor).GetColorVec4();

		if (DrawESPBackground)
		{
			glColor4f(BackgroundColorC.x, BackgroundColorC.y, BackgroundColorC.z, BackgroundColorC.w);
			DrawFilledBox(PlayerRenderData.BoundingBoxData);
		}

		if (DrawESPOutlines)
		{
			glColor4f(OutlineColorC.x, OutlineColorC.y, OutlineColorC.z, OutlineColorC.w);
			DrawOutlinedBox(PlayerRenderData.BoundingBoxData);
		}
	}

	glDisable(GL_BLEND);
	glDepthMask(true);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LINE_SMOOTH);
	glPopMatrix();

	glPopMatrix();
}
