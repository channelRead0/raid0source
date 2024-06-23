#include "pch.h"
#include <map>

#include "../custom.hpp"

using namespace Schemes;
using namespace ImGui;

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

