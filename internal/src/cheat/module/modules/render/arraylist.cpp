#include "pch.h"
#include "arraylist.hpp"
#include <src/cheat/module/module.hpp>
#include <map>

#include <src/cheat/module/modules/combat/aimassist.hpp>
#include <src/cheat/module/modules/combat/autoclicker.hpp>
#include <src/cheat/module/modules/combat/reach.hpp>
#include <src/cheat/module/modules/combat/velocity.hpp>
#include <src/cheat/module/modules/combat/blockhit.hpp>
#include <src/cheat/module/modules/combat/hitselect.hpp>
#include <src/cheat/module/modules/movement/blink.hpp>
#include <src/cheat/module/modules/movement/sprint.hpp>
#include <src/cheat/module/modules/movement/wtap.hpp>
#include <src/cheat/module/modules/movement/stap.hpp>
#include <src/cheat/module/modules/movement/jumpreset.hpp>
#include <src/cheat/module/modules/movement/nojumpdelay.hpp>
#include <src/cheat/module/modules/render/arraylist.hpp>
#include <src/cheat/module/modules/render/render.hpp>
#include <src/cheat/module/modules/utils/backtrack.hpp>
#include <src/cheat/module/modules/utils/bridgeassist.hpp>
#include <src/cheat/module/modules/utils/autotool.hpp>
#include <src/cheat/module/modules/utils/cheststealer.hpp>
#include <src/cheat/module/modules/utils/fastplace.hpp>
#include <src/cheat/module/modules/utils/refill.hpp>
#include <src/cheat/module/modules/utils/lagrange.hpp>
#include <src/cheat/module/modules/utils/rightclicker.hpp>
#include <src/cheat/module/modules/utils/throwpot.hpp>
#include <cheat/module/modules/world/antibot.hpp>
#include <Windows.h>


struct ArrayData
{
	std::string Name;
	std::string Flags;
	ImVec2 RenderPos = ImVec2(-100.F, -100.F);
	float BoundingBoxMin = -100.F;
	float BoundingBoxMax = -100.F;
	float BoundingBoxMinY = -100.F;
	float BoundingBoxMaxY = -100.F;
	float Alpha = 0.F;
};

ImVec4 oscillateColor(const ImVec4& colorA, const ImVec4& colorB, float oscillationSpeed) 
{
	static auto startTime = std::chrono::steady_clock::now();
	auto currentTime = std::chrono::steady_clock::now();
	float elapsedTime = std::chrono::duration<float>(currentTime - startTime).count();

	float oscillationValue = sin(oscillationSpeed * elapsedTime);
	float t = (oscillationValue + 1.0f) / 2.0f;

	return ImLerp(colorA, colorB, t);
}


//ImVec4 oscillateColor(const ImVec4& colorA, const ImVec4& colorB, float oscillationSpeed, int index, int totalItems) {
//	// Get current time
//	static auto startTime = std::chrono::steady_clock::now();
//	auto currentTime = std::chrono::steady_clock::now();
//	float elapsedTime = std::chrono::duration<float>(currentTime - startTime).count();
//
//	// Calculate offset based on index and total items
//	float offset = (2 * ImGui::GetTime() + index * 0.1f) / totalItems; // You can adjust the multiplier (0.1f) to change the offset spread
//
//	// Calculate oscillation value based on sine function, time, and offset
//	float oscillationValue = sin(oscillationSpeed * elapsedTime + offset);
//
//	// Map sine value (-1 to 1) to range [0, 1]
//	float t = (oscillationValue + 1.0f) / 2.0f;
//
//	// Interpolate between colorA and colorB using the oscillation value
//	return lerpColor(colorA, colorB, t);
//}



inline std::string FormatI(const char* Fmt, int Data)
{
	char buffer[50];

	std::sprintf(buffer, Fmt, Data);

	return std::string(buffer);
}


Color convert(Color color, Color color2, double n) {
	double n2 = 1.0 - n;
	return Color((int)(color.R * n + color2.R * n2), (int)(color.G * n + color2.G * n2), (int)(color.B * n + color2.B * n2));
}
Color getGradient(double delay) {
	return convert(ArrayList::InverseColorWave ? ArrayList::ModuleColorB: ArrayList::ModuleColor, ArrayList::InverseColorWave ? ArrayList::ModuleColor : ArrayList::ModuleColorB, ((std::sin(GetTickCount64() / 1.0E8 * 0.5 * 400000.0 + delay * 0.550000011920929) + 1.0) * 0.5));
}


void ArrayList::OnImGuiRender(JNIEnv* Env)
{
	//BOOL success = PlaySound(reinterpret_cast<LPCSTR>(buffer), NULL, SND_MEMORY | SND_ASYNC);

	if (!this->GetState()) {
		return;
	}

	GameClasses ThreadClasses;
	if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses)) {
		return;
	}

	auto Convert = [](Color ColorA, Color ColorB, double Offset) -> Color
	{
		double OffsetNormlaized = 1.0 - Offset;
		return Color(static_cast<int>(ColorA.R * Offset + ColorB.R * OffsetNormlaized), static_cast<int>(ColorA.G * Offset + ColorB.G * OffsetNormlaized), static_cast<int>(ColorA.B * Offset + ColorB.B * OffsetNormlaized));
	};

	auto Rainbow = [](int Offset, float Saturation)
	{
		const auto MiliSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

		const auto State = ceil((MiliSeconds.count() + Offset) / 20);
		const auto Rainbow = FromHSB((fmod(State, 360) / 360.0f), Saturation, 0.8f);
		return ImVec4((int)Rainbow.X / 255.F, (int)Rainbow.Y / 255.F, (int)Rainbow.Z / 255.F, 1.F);
	};

	auto GetModuleColor = [Convert, Rainbow](double Delay, Color ColorA, Color ColorB, bool Inverse = false) -> Color
	{
		return Convert(Inverse ? ColorB : ColorA, Inverse ? ColorA : ColorB, (std::sin(GetTickCount64() / 1.0E8 * ArrayList::WaveSpeed * 400000.0 + Delay * 0.550000011920929) + 1.0) * 0.5);
	};

	auto ToLower = [](const std::string& String) -> std::string
	{
		std::string Result = String;
		std::transform(Result.begin(), Result.end(), Result.begin(), [](unsigned char Char) { return std::tolower(Char); });
		return Result;
	};

	auto SizeComparison = [ToLower](const std::pair<Module*, ArrayData> DataA, const std::pair<Module*, ArrayData> DataB)
	{
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[5 + Font]);

		auto FinalTextA = DataA.second.Name + (DataA.second.Flags.empty() ? "" : " " + DataA.second.Flags);
		auto FinalTextB = DataB.second.Name + (DataB.second.Flags.empty() ? "" : " " + DataB.second.Flags);

		const auto ModuleNameTextA = LowerCase ? ToLower(DataA.second.Name + (DataA.second.Flags.empty() ? "" : " ")) : DataA.second.Name + (DataA.second.Flags.empty() ? "" : " ");
		const auto FlagTextA = LowerCase ? ToLower(DataA.second.Flags) : DataA.second.Flags;

		const auto ModuleNameTextB = LowerCase ? ToLower(DataB.second.Name + (DataB.second.Flags.empty() ? "" : " ")) : DataB.second.Name + (DataB.second.Flags.empty() ? "" : " ");
		const auto FlagTextB = LowerCase ? ToLower(DataB.second.Flags) : DataB.second.Flags;

		const auto CombinedA = ModuleNameTextA + " " + FlagTextA;
		const auto CombinedTextSizeA = ImGui::CalcTextSize(CombinedA.data());

		const auto CombinedB = ModuleNameTextB + " " + FlagTextB;
		const auto CombinedTextSizeB = ImGui::CalcTextSize(CombinedB.data());

		//FinalTextA = LowerCase ? ToLower(FinalTextA) : FinalTextA;
		//FinalTextB = LowerCase ? ToLower(FinalTextB) : FinalTextB;

		bool Ret = CombinedTextSizeA.x > CombinedTextSizeB.x;

		ImGui::PopFont();

		return Ret;
	};

	auto ColorWave = [](const ImVec4& ColorA, const ImVec4& ColorB, float Speed, int Index, int TotalItems)
	{
		static auto StartTime = std::chrono::steady_clock::now();
		auto CurrentTime = std::chrono::steady_clock::now();
		auto ElapsedTime = std::chrono::duration<float>(CurrentTime - StartTime).count();
		auto PhraseShift = fmod(Speed * ElapsedTime, 2 * ImGui::GetTime());
		auto Offset = (2 * ImGui::GetTime() + Index * 0.3f + PhraseShift) / TotalItems;
		auto Oscillation = sin(Speed * ElapsedTime + Offset);
		auto Time = (Oscillation + 1.0f) / 2.0f;

		return ImLerp(ColorA, ColorB, Time);
	};

	const auto WindowBounds = ImRect(ImVec2(0.F, 0.F), ImGui::GetIO().DisplaySize);
	const auto StartingPos = ImVec2(WindowBounds.Max.x, WindowBounds.Min.y - 1.F);

	const auto ModuleList = ModuleManager::GetThreadedModules();

	static std::vector<std::pair<Module*, ArrayData>> ArrayListModules =
	{ 
		//{ModuleManager::GetModuleByName("HitSelect"), ArrayData{"Hit Select", FormatI("%d hits", HitSelect::ComboThreshold)}},
		{ModuleManager::GetModuleByName("AimAssist"), ArrayData{"Aim Assist", AimAssist::AimMode == AimAssist::AIM_MODE::STATIC ? "Static" : "Dynamic" }},
		{ModuleManager::GetModuleByName("AutoClicker"), ArrayData{"Auto Clicker", Format("Normal %.1f cps", AutoClicker::TargetCPS)}},
		{ModuleManager::GetModuleByName("BackTrack"), ArrayData{"Back Track",  BackTrack::Mode == BackTrack::BACKTRACK_MODES::BACKTRACK_A ? "Normal " + FormatI("%d ms", BackTrack::Delay) : "Experimental A " + FormatI("%d ms", BackTrack::Delay)}},
		{ModuleManager::GetModuleByName("Reach"), ArrayData{"Reach", Format("%.2f", Reach::DistanceMin) + "-" + Format("%.2f blocks", Reach::DistanceMax)}},
		{ModuleManager::GetModuleByName("Velocity"), ArrayData{"Velocity", Velocity::VelocityMode == Velocity::VELOCITY_MODE::PACKET ? "Packet" : "Normal" }},
		{ModuleManager::GetModuleByName("ThrowPot"), ArrayData{"Throw Pot", ThrowPot::ThrowMode == ThrowPot::THROW_MODE::DYNAMIC ? "Dynamic" : ThrowPot::THROW_MODE::DOUBLE ? "Double" : "Single" }},
		{ModuleManager::GetModuleByName("ChestStealer"), ArrayData{"Chest Stealer", ""}},
		{ModuleManager::GetModuleByName("Refill"), ArrayData{"Refill", ""}},
		{ModuleManager::GetModuleByName("Radar"), ArrayData{"Radar", ""}},
		{ModuleManager::GetModuleByName("NameTags"), ArrayData{"Name Tags", ""}},
		{ModuleManager::GetModuleByName("BridgeAssist"), ArrayData{"Bridge Assist", FormatI("%dms", BridgeAssist::Delay)}},
		{ModuleManager::GetModuleByName("RightClicker"), ArrayData{"Right Clicker", Format("%.1f cps", RightClicker::TargetCPS)}},
		{ModuleManager::GetModuleByName("FastPlace"), ArrayData{"Fast Place", FormatI("%d ticks", FastPlace::TickDelay)}},
		{ModuleManager::GetModuleByName("AntiBot"), ArrayData{"Anti Bot", AntiBot::Mode == AntiBot::MODE::HYPIXEL ? "Hypixel" : "Custom"}},
		{ModuleManager::GetModuleByName("Teams"), ArrayData{"Teams", ""}},
		{ModuleManager::GetModuleByName("Friends"), ArrayData{"Friends", ""}},
		{ModuleManager::GetModuleByName("Blink"), ArrayData{"Blink", FormatI("%d ms", Blink::Timeout)}},
		{ModuleManager::GetModuleByName("Sprint"), ArrayData{"Sprint", ""}},
		{ModuleManager::GetModuleByName("AutoTool"), ArrayData{"AutoTool", FormatI("%d ms", AutoTool::StartDelay)}},
		{ModuleManager::GetModuleByName("WTap"), ArrayData{"WTap", FormatI("%d ms", WTap::DelayMax)}},
		{ModuleManager::GetModuleByName("STap"), ArrayData{"STap", STap::Mode == STap::AGGRESSIVE ? "Aggressive " + FormatI("%d ms", STap::DelayMax) : "Passive " + FormatI("%d ms", STap::DelayMax)}},
		{ModuleManager::GetModuleByName("PlayerESP"), ArrayData{"Player ESP", PlayerESP::RenderMode == PlayerESP::RENDER_2D ? "2D" : "3D"}},
		{ModuleManager::GetModuleByName("LagRange"), ArrayData{"Lag Range", FormatI("%d ms", LagRange::Delay)}},
		{ModuleManager::GetModuleByName("NoJumpDelay"), ArrayData{"No Jump Delay", ""}},
		{ModuleManager::GetModuleByName("BlockHit"), ArrayData{"Block Hit",  BlockHit::Mode == BlockHit::PASSIVE ? "Passive " + FormatI("%d ms", BlockHit::BlockHitDelayMax) : "Active " + FormatI("%d ms", BlockHit::BlockHitDelayMax)}},
		{ModuleManager::GetModuleByName("JumpReset"), ArrayData{"Jump Reset",  JumpReset::Mode == JumpReset::PRE ? "Pre " + FormatI("%d ms", JumpReset::TickDelay) : "Post " + FormatI("%d ms", JumpReset::TickDelay)}},
	};

	auto GetModuleByName = [](const char* Name) -> ArrayData*
	{
		for (auto& ArrayModule : ArrayListModules)
		{
			if (ArrayModule.second.Name == Name) {
				return &ArrayModule.second;
			}
		}

		return nullptr;
	};

	GetModuleByName("Aim Assist")->Flags = AimAssist::AimMode == AimAssist::AIM_MODE::STATIC ? "Static" : "Dynamic";
	GetModuleByName("Auto Clicker")->Flags = Format("Normal %.1f cps", AutoClicker::TargetCPS);
	GetModuleByName("Back Track")->Flags = BackTrack::Mode == BackTrack::BACKTRACK_MODES::BACKTRACK_A ? "Normal " + FormatI("%d ms", BackTrack::Delay) : "Experimental A " + FormatI("%d ms", BackTrack::Delay);
	GetModuleByName("Reach")->Flags = Format("%.2f", Reach::DistanceMin) + "-" + Format("%.2f blocks", Reach::DistanceMax);
	GetModuleByName("Velocity")->Flags = Velocity::VelocityMode == Velocity::VELOCITY_MODE::PACKET ? "Packet" : "Normal";
	GetModuleByName("Throw Pot")->Flags = ThrowPot::ThrowMode == ThrowPot::THROW_MODE::DYNAMIC ? "Dynamic" : ThrowPot::THROW_MODE::DOUBLE ? "Double" : "Single";
	GetModuleByName("Chest Stealer")->Flags = "";
	GetModuleByName("Refill")->Flags = "";
	GetModuleByName("Radar")->Flags = "";
	GetModuleByName("Name Tags")->Flags = "";
	GetModuleByName("Bridge Assist")->Flags = FormatI("%d ms", BridgeAssist::Delay);
	GetModuleByName("Right Clicker")->Flags = Format("%.1f cps", RightClicker::TargetCPS);
	GetModuleByName("Fast Place")->Flags = FormatI("%d ticks", FastPlace::TickDelay);
	GetModuleByName("Anti Bot")->Flags = AntiBot::Mode == AntiBot::MODE::HYPIXEL ? "Hypixel" : "Custom";
	GetModuleByName("Teams")->Flags = "";
	GetModuleByName("Friends")->Flags = "";
	//GetModuleByName("Hit Select")->Flags = FormatI("%d hits", HitSelect::ComboThreshold);
	GetModuleByName("AutoTool")->Flags = FormatI("%d ms", AutoTool::StartDelay);
	GetModuleByName("Blink")->Flags = FormatI("%d ms", Blink::Timeout);
	GetModuleByName("Sprint")->Flags = "";
	GetModuleByName("WTap")->Flags = FormatI("%d ms", WTap::DelayMax);
	GetModuleByName("STap")->Flags = STap::Mode == STap::AGGRESSIVE ? "Aggressive " + FormatI("%d ms", STap::DelayMax) : "Passive " + FormatI("%d ms", STap::DelayMax);
	GetModuleByName("Player ESP")->Flags = PlayerESP::RenderMode == PlayerESP::RENDER_2D ? "2D" : "3D";
	GetModuleByName("Lag Range")->Flags = FormatI("%d ms", LagRange::Delay);
	GetModuleByName("No Jump Delay")->Flags = "";
	GetModuleByName("Jump Reset")->Flags = JumpReset::Mode == JumpReset::PRE ? "Pre " + FormatI("%d ms", JumpReset::TickDelay) : "Post " + FormatI("%d ms", JumpReset::TickDelay);
	GetModuleByName("Block Hit")->Flags = BlockHit::Mode == BlockHit::PASSIVE ? "Passive " + FormatI("%d ms", BlockHit::BlockHitDelayMax) : "Active " + FormatI("%d ms", BlockHit::BlockHitDelayMax);

	//ArrayListModules[1].second.Flags = Format("%.1f CPS", AutoClicker::TargetCPS);
	//ArrayListModules[2].second.Flags = Format("%.2f", Reach::DistanceMin) + " - " + Format("%.2f", Reach::DistanceMax) + " blocks";
	//ArrayListModules[3].second.Flags = Format("%.1f H", Velocity::VelocityHorizontal) + " " + Format("%.2f V", Velocity::VelocityVertical);
	//ArrayListModules[4].second.Flags = Format("%.1f ms", Blink::Timeout);
	//ArrayListModules[5].second.Flags = "Sprint", "";
	//ArrayListModules[6].second.Flags = Format("%.d", WTap::DelayMin) + " - " + Format("%.d", WTap::DelayMax) + " ms";
	//ArrayListModules[7].second.Flags = PlayerESP::RenderMode == PlayerESP::RENDER_2D ? "2D" : "3D";

	std::sort(ArrayListModules.begin(), ArrayListModules.end(), SizeComparison);

	if (ArrayListModules.empty()) {
		return;
	}

	auto EnabledCount = 0;
	std::string LastEnabledModuleName;
	for (auto& [Module, ArrayData] : ArrayListModules)
	{
		if (Module->GetState()) 
		{
			EnabledCount++;
			LastEnabledModuleName = ArrayData.Name;
		}
	}

	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[5 + Font]);

	ImGui::SetNextWindowPos(StartingPos - ImVec2(500.F, 0.F));
	ImGui::SetNextWindowSize(ImVec2(500.F, 500.F));

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.F, 0.F));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.F, 0.F));
	ImGui::PushStyleColor(ImGuiCol_WindowShadow, Color(0.F, 0.F, 0.F, 0.F).GetColorU32());

	ImGui::Begin("ArrayList", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	{
		ImVec2 Padding = ImVec2(PaddingX, PaddingY);

		const auto DrawList = ImGui::GetWindowDrawList();
		const auto WindowBounds = ImRect(ImGui::GetWindowPos(), ImGui::GetWindowPos() + ImVec2(500.F, ImGui::GetWindowSize().y));
		auto StartingPosLocal = ImVec2(WindowBounds.Max.x - (0.F + StartOffsetX), WindowBounds.Min.y + (0.F + StartOffsetY));

		const auto ImGuiIO = ImGui::GetIO().DeltaTime;

		int Index = 0;
		int DrawnIndex = 0;
		float OffsetY = 0.F;
		float BarWidth = !Bar ? 0 : ArrayList::BarWidth;

		int WaveDelay = 0;
		int FlagWaveDelay = 0;
		for (auto& [Module, ArrayData] : ArrayListModules)
		{
			const auto ModuleNameText = LowerCase ? ToLower(ArrayData.Name + (ArrayData.Flags.empty() ? "" : " ")) : ArrayData.Name + (ArrayData.Flags.empty() ? "" : " ");
			const auto FlagText = LowerCase ? ToLower(ArrayData.Flags) : ArrayData.Flags;

			const auto Combined = ModuleNameText + " " + FlagText;
			const auto ModuleNameTextSize = ImGui::CalcTextSize(ModuleNameText.data());
			const auto FlagTextSize = ImGui::CalcTextSize(FlagText.data());
			const auto CombinedTextSize = ImGui::CalcTextSize(Combined.data());

			const auto BoundingBox = ImRect(ImVec2((Module->GetState() ? StartingPosLocal.x - CombinedTextSize.x - (Padding.x * 2.F) - BarWidth : StartingPosLocal.x), StartingPosLocal.y + OffsetY), ImVec2((Module->GetState() ? StartingPosLocal.x - BarWidth : StartingPosLocal.x + CombinedTextSize.x + (Padding.x * 2.F)), StartingPosLocal.y + 3.F + OffsetY + CombinedTextSize.y + Padding.y * 2.F));
			

			ArrayData.BoundingBoxMax = ImLerp(ArrayData.BoundingBoxMax, BoundingBox.Max.x, ImGuiIO * 7.F);
			ArrayData.BoundingBoxMin = ImLerp(ArrayData.BoundingBoxMin, BoundingBox.Min.x, ImGuiIO * 7.F);

			ArrayData.BoundingBoxMaxY = ImLerp(ArrayData.BoundingBoxMaxY, BoundingBox.Max.y, ImGuiIO * 7.F);
			ArrayData.BoundingBoxMinY = ImLerp(ArrayData.BoundingBoxMinY, BoundingBox.Min.y, ImGuiIO * 7.F);


			ArrayData.Alpha = ImLerp(ArrayData.Alpha, Module->GetState() ? 255.F : 0.F, ImGuiIO * 7.F);

			const auto BoundingBoxFinal = ImRect(ImVec2(ArrayData.BoundingBoxMin, ArrayData.BoundingBoxMinY), ImVec2(ArrayData.BoundingBoxMax, ArrayData.BoundingBoxMaxY));
			//rawList->AddRect(BoundingBoxFinal.Min, BoundingBoxFinal.Max, Color(255, 255, 255));

			const auto TextPos = ImVec2(BoundingBox.GetCenter().x - (CombinedTextSize.x / 2.F) + 1.F + 1.5F, (BoundingBox.GetCenter().y - (CombinedTextSize.y / 2.F)) - 1.F/*(Font == FONT_A ? 1.F : 0.F)*/);

			ArrayData.RenderPos.x = ImLerp(ArrayData.RenderPos.x, TextPos.x, ImGuiIO * 7.F);
			ArrayData.RenderPos.y = ImLerp(ArrayData.RenderPos.y, TextPos.y, ImGuiIO * 7.F);

			const auto RainbowColor = Rainbow(Index * 200, Saturation);
			ImVec4 ModuleNameColor;
			ImVec4 FlagNameColor = FlagColor;
			ImVec4 ModuleBackgroundColor;

			ModuleBackgroundColor = GetModuleColor(WaveDelay, ModuleBoxColor, Color(ModuleBoxColor).Darken(0.2F), InverseColorWave);
			ModuleBackgroundColor.w = ModuleBoxColor.w * (ArrayData.Alpha / 255.F);

			if (ColorMode == STATIC) 
			{
				ModuleNameColor = Color(ModuleColor);
			}

			if (ColorMode == CHROMA) 
			{
				ModuleNameColor = Rainbow(WaveDelay, Saturation);
				WaveDelay -= 120;
			}

			if (ColorMode == STATIC_COLORWAVE) 
			{
				ModuleNameColor = GetModuleColor(WaveDelay, ModuleColor, ModuleColorB, InverseColorWave);
				
				if (!FlagText.empty())
				{
					FlagNameColor = GetModuleColor(FlagWaveDelay, FlagColor, Color(FlagColor).Darken(0.5F), InverseColorWave);
					FlagWaveDelay = InverseColorWaveDirection ? FlagWaveDelay + 12 : FlagWaveDelay - 12;
				}
				//std::cout << getGradient(WaveDelay).R << std::endl;

				WaveDelay = InverseColorWaveDirection ? WaveDelay + 12 : WaveDelay - 12;
			}

			if (DrawBox)
			{
				const auto BoxBoundingBox = BoundingBoxFinal;

				constexpr auto BeginFlags = ImDrawFlags_RoundCornersBottomLeft /*| ImDrawFlags_RoundCornersTop*/;
				constexpr auto MiddleFlags = ImDrawFlags_RoundCornersBottomLeft;
				constexpr auto BottomFlags = /*ImDrawFlags_RoundCornersBottom*/0;

				//DrawList->AddShadowRect(BoxBoundingBox.Min, BoxBoundingBox.Max, Color(ModuleBackgroundColor), 15.F, ImVec2(0.F, 0.F), Index == 0 ? BeginFlags : /*ArrayData.Name == LastEnabledModuleName ? BottomFlags : */MiddleFlags, Rounding);
				DrawList->AddRectFilled(BoxBoundingBox.Min, BoxBoundingBox.Max, Color(ModuleBackgroundColor), Rounding, Index == 0 ? BeginFlags : /*ArrayData.Name == LastEnabledModuleName ? BottomFlags : */MiddleFlags);
			}

			if (TextShadows) {
				DrawList->AddText(ArrayData.RenderPos + ImVec2(1.F, 1.F), Color(0, 0, 0).SetAlpha(200.F * (ArrayData.Alpha / 255.F)), ModuleNameText.data());
			}

			DrawList->AddText(ArrayData.RenderPos, Color(ModuleNameColor).SetAlpha(ArrayData.Alpha), ModuleNameText.data());

			if (TextShadows) {
				DrawList->AddText(ImVec2(ArrayData.RenderPos.x + ModuleNameTextSize.x, ArrayData.RenderPos.y) + ImVec2(1.F, 1.F), Color(0, 0, 0).SetAlpha(200.F * (ArrayData.Alpha / 255.F)), FlagText.data());
			}

			DrawList->AddText(ImVec2(ArrayData.RenderPos.x + ModuleNameTextSize.x, ArrayData.RenderPos.y), Color(FlagNameColor).SetAlpha(ArrayData.Alpha), FlagText.data());

			if (Bar)
			{
				const auto BarBoundingBox = ImRect(ImVec2(BoundingBoxFinal.Max.x, BoundingBoxFinal.Min.y + /*Rounding*/1), ImVec2(BoundingBoxFinal.Max.x + BarWidth, BoundingBoxFinal.Max.y - /*Rounding*/1));
				DrawList->AddRectFilled(BarBoundingBox.Min, BarBoundingBox.Max, Color(ModuleNameColor).SetAlpha(ArrayData.Alpha), 5.F/*, ImDrawFlags_RoundCornersRight*/);
			}

			if (Module->GetState()) {
				DrawnIndex++;
			}

			OffsetY += Module->GetState() ? /*CombinedTextSize.y*/BoundingBoxFinal.GetHeight() : 0.F;
			Index++;
		}

		if (Bar)
		{	
		/*	ImVec4 ColorTop, ColorBottom;

			if (ColorMode == STATIC)
			{
				if (StaticColorWave)
				{
					ColorTop = ModuleColor;
					ColorBottom = ModuleColor;
				}
				else
				{
					ColorTop = Color(ModuleColor);
					ColorBottom = Color(ModuleColor);
				}
			}

			if (ColorMode == CHROMA)
			{
				ColorTop = Rainbow(1 * 200, Saturation);
				ColorBottom = Rainbow(Index > 2.F ? Index : 1.F * 200, Saturation);
			}

			static auto PreviousHeight = 0.F;

			PreviousHeight = ImLerp(PreviousHeight, StartingPosLocal.y + OffsetY, ImGuiIO * 7.F);
			const auto LineBB = ImRect(ImVec2(StartingPosLocal.x, StartingPosLocal.y), ImVec2(StartingPosLocal.x + 3.F, PreviousHeight));
			DrawList->AddRectFilledMultiColor(LineBB.Min, LineBB.Max, Color(ColorTop), Color(ColorTop), Color(ColorBottom), Color(ColorBottom));*/
		}
	}
	ImGui::End();

	ImGui::PopStyleVar(2);
	ImGui::PopStyleColor();

	ImGui::PopFont();
}