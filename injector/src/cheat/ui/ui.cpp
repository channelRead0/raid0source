#include "pch.h"
#include <src/cheat/raid0.hpp>
#include <ext/imgui/imgui.h>
#include <ext/imgui/imgui_internal.h>
#include <ext/imgui/custom/custom.hpp>

void Raid0::Render() noexcept
{
	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::SetNextWindowSize({ (float)WIDTH, (float)HEIGHT });

	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.F);

	ImGui::PushStyleColor(ImGuiCol_Border, Schemes::ColorSchemeUI.Border.GetColorU32());
	ImGui::PushStyleColor(ImGuiCol_WindowBg, Schemes::ColorSchemeUI.WindowBackground.GetColorU32());

	ImGui::Begin("Loader", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

	struct WindowAnimations
	{
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

	const auto AnimatedText = Widgets::TextAnimation();
	const auto DrawList = ImGui::GetWindowDrawList();

	ImRect Bounds = ImRect(ImVec2(0.F, 0.F), ImVec2(WIDTH, HEIGHT));

	static char KeyBuffer[33] = "";
	static bool RenderStages = false;
	if (!RenderStages)
	{
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);

		const auto TextSize = ImGui::CalcTextSize(AnimatedText.data());
		auto TextPos = ImVec2(Bounds.GetCenter().x - TextSize.x / 2.F, 133.F);

		DrawList->AddText(TextPos, Schemes::ColorSchemeUI.TextActive.GetColorU32(), AnimatedText.data());

		ImGui::PopFont();

		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);

		const auto FooterTextSize = ImGui::CalcTextSize("\xC2\xA9 raid0");
		auto FooterTextPos = ImVec2(Bounds.GetCenter().x - FooterTextSize.x / 2.F, 368.F);

		DrawList->AddText(FooterTextPos, Color("444751").GetColorU32(), "\xC2\xA9 raid0");

		ImGui::PopFont();

		auto TextBoxPos = ImVec2(Bounds.GetCenter().x - (245.F / 2.F), 193.F);

		ImGui::SetCursorPos(TextBoxPos);
		ImGui::InputText("Key", KeyBuffer, IM_ARRAYSIZE(KeyBuffer), ImGuiInputTextFlags_Password);

		ImGui::SetCursorPos(ImVec2(225.F, 237.F));
		if (ImGui::Button("Login", ImVec2(55.F, 19.F)) && std::string(KeyBuffer).size() > 0)
		{
			//AUTH HERE
		}
	}

	if (RenderStages)
	{

	}

	ImGui::End();

	ImGui::PopStyleVar();
	ImGui::PopStyleColor(2);
}