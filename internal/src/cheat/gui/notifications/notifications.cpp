#include "pch.h"
#include "notifications.hpp"
#include <map>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <windows.h>
#include <playsoundapi.h>


#include "data/samsung.hpp"
#include <ext/imgui/custom/custom.hpp>

void NotificationHandler::Render()
{
	if (!Toggled) {
		return;
	}

    std::lock_guard<std::mutex> Lock(Mutex);

	RECT WindowRect;
	GetClientRect(WINDOW_HANDLE, &WindowRect);

	const int WindowWidth = WindowRect.right - WindowRect.left;
	const int WindowHeight = WindowRect.bottom - WindowRect.top;

	struct NotificationAnimation
	{
		float PositionX = DisplayWidth;
		float PositionY;
		float Alpha = 0.F;
		bool SoundPlayed = false;
	};

	static std::map<ImGuiID, NotificationAnimation> AnimationMap;

	if (NotificationQueue.empty()) {
		return;
	}

	auto ToLower = [](const std::string& String) -> std::string
	{
		std::string Result = String;
		std::transform(Result.begin(), Result.end(), Result.begin(), [](unsigned char Char) { return std::tolower(Char); });
		return Result;
	};

	const auto WindowSize = ImVec2(WindowWidth, WindowHeight);
	const auto WindowBounds = ImRect(ImVec2(0.F, 0.F), WindowSize);
	const auto BackgroundDrawList = ImGui::GetBackgroundDrawList();
	
	const auto StartingPosition = ImVec2(WindowBounds.Max.x - 249.F, WindowBounds.Max.y - 70.F);

	int Index = 0;
	float OffsetY = StartingPosition.y;
	std::vector<int> Discard;
	for (auto& Notification : NotificationQueue)
	{
		auto AnimationHandle = AnimationMap.find((int)Notification.Description.data());

		if (AnimationHandle == AnimationMap.end())
		{
			AnimationMap.insert({ (int)Notification.Description.data(), NotificationAnimation( DisplayWidth, OffsetY, 0.F, false ) });
			AnimationHandle = AnimationMap.find((int)Notification.Description.data());
		}

		bool IsDrawn = GetTickCount64() < Notification.ExpireTime;
		bool PerformEaseOut = GetTickCount64() >= Notification.ExpireTime - 250;

		if (!IsDrawn)
		{
			Discard.push_back(Index);

			Index++;
			continue;
		}

		if (IsDrawn)
		{
			if (PlayNotificationSound && !AnimationHandle->second.SoundPlayed)
			{
				PlaySound(reinterpret_cast<LPCSTR>(samsung_togglesound), NULL, SND_MEMORY | SND_ASYNC);
				AnimationHandle->second.SoundPlayed = true;
			}

			AnimationHandle->second.Alpha = ImLerp(AnimationHandle->second.Alpha, PerformEaseOut ? 0.F : 255.F, ImGui::GetIO().DeltaTime * 7.F);
			AnimationHandle->second.PositionX = ImLerp(AnimationHandle->second.PositionX, PerformEaseOut ? WindowWidth : StartingPosition.x, ImGui::GetIO().DeltaTime * 7.F);
			AnimationHandle->second.PositionY = ImLerp(AnimationHandle->second.PositionY, OffsetY, ImGui::GetIO().DeltaTime * 7.F);

			const auto BoundingBox = ImRect(ImVec2(AnimationHandle->second.PositionX, AnimationHandle->second.PositionY), ImVec2(AnimationHandle->second.PositionX + 234.F, AnimationHandle->second.PositionY + 49.F));
			const auto TimerBoundingBox = ImRect(ImVec2(BoundingBox.Min.x, BoundingBox.Min.y + 52.F), ImVec2(BoundingBox.Max.x, BoundingBox.Min.y + 56.F));

			const auto Percentage =/* 1.F -*/ float(float(Notification.ExpireTime - GetTickCount64()) / float(Notification.ExpireTime - Notification.RegisterTime));

			const auto TimerBoundingShadedBox = ImRect(ImVec2(BoundingBox.Min.x , BoundingBox.Min.y + 52.F), ImVec2(BoundingBox.Max.x - (TimerBoundingBox.GetWidth() * Percentage), BoundingBox.Min.y + 56.F));

			BackgroundDrawList->AddShadowRect(BoundingBox.Min, BoundingBox.Max, Color("0F0F0F").SetAlpha(AnimationHandle->second.Alpha), 20.F, { 0.F, 0.F }, 0, 4.F);
			BackgroundDrawList->AddRectFilled(BoundingBox.Min, BoundingBox.Max, Color("0F0F0F").SetAlpha(AnimationHandle->second.Alpha), 4.F);
			BackgroundDrawList->AddRect(BoundingBox.Min, BoundingBox.Max, Color("141415").SetAlpha(AnimationHandle->second.Alpha), 4.F);

			//BackgroundDrawList->AddShadowRect(TimerBoundingBox.Min, TimerBoundingBox.Max, Color("0F0F0F").SetAlpha(AnimationHandle->second.Alpha), 10.F, { 0.F, 0.F }, 0, 12.F);
			BackgroundDrawList->AddRectFilled(TimerBoundingBox.Min, TimerBoundingBox.Max, Color("0F0F0F").SetAlpha(AnimationHandle->second.Alpha), 12.F);
			BackgroundDrawList->AddRectFilled(TimerBoundingShadedBox.Min, TimerBoundingShadedBox.Max, Color("7978A0").SetAlpha(AnimationHandle->second.Alpha), 12.F);

			ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[8]);
			const auto TitlePosition = BoundingBox.Min + ImVec2(14.F, 9.F);
			BackgroundDrawList->AddText(TitlePosition, Color("FFFFFF").SetAlpha(AnimationHandle->second.Alpha), Notification.Title.data());
			ImGui::PopFont();

			ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[9]);
			const auto DescriptionPosition = BoundingBox.Min + ImVec2(14.F, 25.F);
			BackgroundDrawList->AddText(DescriptionPosition, Color("444751").SetAlpha(AnimationHandle->second.Alpha), ToLower(Notification.Description).data());
			ImGui::PopFont();

			ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[10]);
			const auto IconSize = ImGui::CalcTextSize("A");
			const auto IconPosition = ImVec2(BoundingBox.Max.x - (12.F + IconSize.x), BoundingBox.GetCenter().y - (IconSize.y / 2.F));
			BackgroundDrawList->AddText(IconPosition, Color("444751").SetAlpha(AnimationHandle->second.Alpha), "A");
			ImGui::PopFont();


			OffsetY -= 70.F;
		}

		Index++;
	}

	for (auto& Expired : Discard)
	{
		try {
			NotificationQueue.erase(NotificationQueue.begin() + Expired);
		}
		catch (std::out_of_range& Exception) {
			std::cerr << "Exception: " << Exception.what() << std::endl;
		}
	}

	//const auto TimerPercentage = float(GetTickCount64() - AnimationHandle->second.StartTime) / float(2000.F);

	//const auto NotificationBoundingBox = ImRect({ (WindowSize.x + AnimationHandle->second.Position * (12.F + 234.F)) - 12.F - 234.F, WindowSize.y - 16.F - 55.8F }, { (WindowSize.x + AnimationHandle->second.Position * (12.F + 234.F)) - 12.F,  WindowSize.y - 16.F });

	//const auto MessageBoxBoundingBox = ImRect(NotificationBoundingBox.Min, { NotificationBoundingBox.Max.x , NotificationBoundingBox.Min.y + 48.6F });
	//const auto NotificationTimerBoundingBox = ImRect({ NotificationBoundingBox.Min.x , NotificationBoundingBox.Max.y - 3.6F }, { NotificationBoundingBox.Max.x , NotificationBoundingBox.Max.y });
	//const auto TimerBoundingBox = ImRect({ NotificationBoundingBox.Min.x , NotificationBoundingBox.Max.y - 3.6F }, { NotificationBoundingBox.Min.x + (234.F * TimerPercentage), NotificationBoundingBox.Max.y });

	//AnimationHandle->second.Position = ImLerp(AnimationHandle->second.Position, (!Toggled || !BlinkActive) ? 1.F : 0.F, ImGui::GetIO().DeltaTime * 5.F * 1.F);/*

	//NotificationBoundingBox.Min += ImVec2((12.F - 234.F) * AnimationHandle->second.Position, 0.F);
	//NotificationBoundingBox.Max += ImVec2((12.F - 234.F) * AnimationHandle->second.Position, 0.F);*/


	//BackgroundDrawList->AddRectFilled(NotificationBoundingBox.Min, NotificationBoundingBox.Max, ImGui::GetColorU32(ImVec4(1.F, 1.F, 1.F, 0.F)), 3.6F);
	//BackgroundDrawList->AddRectFilled(MessageBoxBoundingBox.Min, MessageBoxBoundingBox.Max, ImGui::GetColorU32(ImVec4(20.F / 255.F, 18.F / 255.F, 22.F / 255.F, 1.F)), 3.6F);
	//BackgroundDrawList->AddRectFilled(NotificationTimerBoundingBox.Min, NotificationTimerBoundingBox.Max, ImGui::GetColorU32(ImVec4(1.F / 20.F, 1.F / 18.F, 1.F / 22.F, 1.F)), 3.6F);
	//BackgroundDrawList->AddRectFilled(TimerBoundingBox.Min, TimerBoundingBox.Max, ImGui::GetColorU32(ImVec4(95.f / 255.F, 102.F / 255.F, 193.F / 255.F, 1.F)), 3.6F);


	//ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);

	//const auto MessageBoxSize = ImGui::CalcTextSize("Blink Active");
	////BackgroundDrawList->AddText({ MessageBoxBoundingBox.Min.x + 12.6F,  MessageBoxBoundingBox.GetCenter().y - MessageBoxSize.y / 2.F }, ImGui::GetColorU32(ImVec4(89.F / 255.F, 87.F / 255.F, 88.F / 255.F, 1.F)), "Blink Active");
	//BackgroundDrawList->AddText({ MessageBoxBoundingBox.Min.x + 12.6F,  MessageBoxBoundingBox.GetCenter().y - MessageBoxSize.y / 2.F }, ImGui::GetColorU32(ImVec4(89.F / 255.F, 87.F / 255.F, 88.F / 255.F, 1.F)), "Blink Active");

	//ImGui::PopFont();

	//ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);

	//const auto IconUpSize = ImGui::CalcTextSize("A");
	//const auto IconDownSize = ImGui::CalcTextSize("B");

	//BackgroundDrawList->AddText({ MessageBoxBoundingBox.Min.x + 142.2F,  (MessageBoxBoundingBox.GetCenter().y /*- TickCountSize.y / 2.F*/) - 12.5f }, ImGui::GetColorU32(ImVec4(54.F / 255.F, 55.F / 255.F, 57.F / 255.F, 1.F)), "A");
	//BackgroundDrawList->AddText({ MessageBoxBoundingBox.Min.x + 142.2F,  (MessageBoxBoundingBox.GetCenter().y /*- TickCountSize.y / 2.F*/) + 3.5f }, ImGui::GetColorU32(ImVec4(54.F / 255.F, 55.F / 255.F, 57.F / 255.F, 1.F)), "B");

	//ImGui::PopFont();

	//ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);

	//std::string SentPacketCount = std::to_string(SentPackets / 100) + " packets";
	//std::string ReceivedPacketCount = std::to_string(ReceivedPackets / 100) + " packets";

	////BackgroundDrawList->AddText({ MessageBoxBoundingBox.Min.x + 12.6F,  MessageBoxBoundingBox.GetCenter().y - MessageBoxSize.y / 2.F }, ImGui::GetColorU32(ImVec4(89.F / 255.F, 87.F / 255.F, 88.F / 255.F, 1.F)), "Blink Active");
	//BackgroundDrawList->AddText({ MessageBoxBoundingBox.Min.x + 156.6F,  (MessageBoxBoundingBox.GetCenter().y /*- TickCountSize.y / 2.F*/) - 15.3F }, ImGui::GetColorU32(ImVec4(54.F / 255.F, 55.F / 255.F, 57.F / 255.F, 1.F)), SentPacketCount.c_str());
	//BackgroundDrawList->AddText({ MessageBoxBoundingBox.Min.x + 156.6F,  (MessageBoxBoundingBox.GetCenter().y /*- TickCountSize.y / 2.F*/) - 0.9F }, ImGui::GetColorU32(ImVec4(54.F / 255.F, 55.F / 255.F, 57.F / 255.F, 1.F)), ReceivedPacketCount.c_str());

	//ImGui::PopFont();
}


NotificationHandler* NotificationHandlerPtr = new NotificationHandler();