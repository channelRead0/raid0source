#include "pch.h"
#include "custom.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS

#include <ext/imgui/imgui.h>
#include <ext/imgui/imgui_internal.h>
#include <globals/structures/color.hpp>

void RenderProgress(int Progress, const char* StageDescription, bool DoTextAnim = true)
{
	const auto Pos = ImGui::GetCursorPos();
	const auto Size = ImVec2(296.F, 11.F);
	const auto BoundingBox = ImRect(Pos, Pos + Size);
	const auto CircleSize = 11.F;
	const auto CircleCount = 16;
	const auto window = ImGui::GetCurrentWindow();

	struct ProgreesAnimation
	{
		ImColor Color;
		float GlowOffset = 0.F;
		DWORD LastWaveTime;
		bool Switch = false;
	};


	static std::map< ImGuiID, ProgreesAnimation > Animations;

	float OffsetX = Pos.x + (11.F / 2.F);
	float CenterX = Pos.y + (11.F / 2.F);
	for (int i = 0; i < 16; i++)
	{
		const int AnimationID = i;
		auto AnimationMap = Animations.find(AnimationID);
		if (AnimationMap == Animations.end())
		{
			Animations.insert({ AnimationID, ProgreesAnimation() });
			AnimationMap = Animations.find(AnimationID);
		}

		AnimationMap->second.Color.Value = ImLerp(AnimationMap->second.Color.Value, (i < Progress) ? Schemes::ColorSchemeUI.Accent : Schemes::ColorSchemeUI.Border, ImGui::GetIO().DeltaTime * 7.F);

		if ((i < Progress))
		{
			if (GetTickCount64() - AnimationMap->second.LastWaveTime > 1500) {
				AnimationMap->second.Switch = !AnimationMap->second.Switch; AnimationMap->second.LastWaveTime = GetTickCount64();
			}
			AnimationMap->second.GlowOffset = ImLerp(AnimationMap->second.GlowOffset, AnimationMap->second.Switch ? 15.F : 0.F, ImGui::GetIO().DeltaTime * 3.F);
		}

		window->DrawList->AddShadowCircle(ImVec2(OffsetX, CenterX), (11.F / 2.F), AnimationMap->second.Color, AnimationMap->second.GlowOffset, { 0.F, 0.F });
		window->DrawList->AddCircleFilled(ImVec2(OffsetX, CenterX), (11.F / 2.F), AnimationMap->second.Color);
		OffsetX += ((11.F / 2.F) * 2.F) + 8.F;
	}

	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);

	std::string RenderText = { StageDescription };

	static DWORD PrevStageChange = GetTickCount64();
	static int PrevStage = Progress;

	if (Progress != PrevStage)
	{
		PrevStage = Progress;
		PrevStageChange = GetTickCount64();
	}

	static std::string PrevRender = { StageDescription };
	static std::string EndString = "";
	static DWORD LastAdd = GetTickCount64();

	if (DoTextAnim && GetTickCount64() - PrevStageChange > 2500)
	{
		if (GetTickCount64() - LastAdd > 500)
		{
			if (PrevRender != std::string(StageDescription))
			{
				EndString = "";
			}

			if (EndString != "...") {
				EndString += ".";
			}
			else {
				EndString = "";
			}

			LastAdd = GetTickCount64();
			PrevRender = StageDescription;
		}

		RenderText += EndString;
	}

	const auto TextSize = ImGui::CalcTextSize(RenderText.data());
	auto TextPos = ImVec2((BoundingBox.GetCenter() - (TextSize / 2.F)).x, 212.F);

	window->DrawList->AddText(TextPos, Schemes::ColorSchemeUI.TextDisabled, RenderText.data());

	ImGui::PopFont();
}

std::string Widgets::TextAnimation()
{
	struct WindowAnimations
	{
		std::string Name = "raid0";
		float CoverHeight = 0.F;
		float CoverHeightMin = 0.F;
		float CoverAlpha = 255.F;
	};

	const int AnimationID = 821892;

	static std::map< ImGuiID, WindowAnimations > Animations;
	auto AnimationMap = Animations.find(AnimationID);
	if (AnimationMap == Animations.end())
	{
		Animations.insert({ AnimationID, WindowAnimations() });
		AnimationMap = Animations.find(AnimationID);
	}

	auto DoTextAnimation = [AnimationMap]() -> void
		{
			static int Index = 0;
			static int CharIndex = 1;
			static DWORD LastUpdate = GetTickCount64();
			const auto CharSet = "$*&^@%";
			const auto Original = "raid0";
			static auto Name = /*AnimationMap->second.Name*/std::string("*****");

			if (GetTickCount64() - LastUpdate < 10000)
			{
				//Name = "raid0";
				AnimationMap->second.Name = "raid0";
				return;
			}

			CharIndex++;;
			if (CharIndex >= 6)
			{
				Name[Index] = Original[Index];

				CharIndex = 0;
				Index++;

				if (Index >= Name.size())
				{
					Index = 0;
					AnimationMap->second.Name = Name;
					LastUpdate = GetTickCount64();
					Name = "*****";
					return;
				}
			}

			const auto ReplaceChar = CharSet[CharIndex];

			Name[Index] = ReplaceChar;

			AnimationMap->second.Name = Name;
		};

	static DWORD LastUpdate = GetTickCount64();

	if (GetTickCount64() - LastUpdate > 100)
	{
		DoTextAnimation();
		LastUpdate = GetTickCount64();
	}

	return AnimationMap->second.Name;
}
