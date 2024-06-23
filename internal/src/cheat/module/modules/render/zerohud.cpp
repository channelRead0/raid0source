#include "pch.h"
#include "zerohud.hpp"

#define POPPINS_MEDIUM 8;
#define POPPINS_REGULAR 9;

void ZeroHud::OnImGuiRender(JNIEnv* Env)
{
	if (!this->GetState()) {
		return;
	}

	const auto BackgroundDrawList = ImGui::GetBackgroundDrawList();

	static auto Pos = ImVec2(40.F, 40.F);
	const auto BoundingBox = ImRect(Pos, Pos + ImVec2(261.F, 19.F));
	const auto BoundingBoxColorBar = ImRect(ImVec2(Pos.x, Pos.y + 20.F), ImVec2(Pos.x + 261.F, Pos.y + 22.F));
	const auto TotalBoundingBox = ImRect(Pos, Pos + ImVec2(261.F, 22.F));

	if (Alignment::PointInsideRect(ImGui::GetCursorPos(), TotalBoundingBox) && ImGui::IsMouseDown(ImGuiMouseButton_Left))
	{
		const auto CursorPos = ImGui::GetCursorPos();
		const auto DeltaX = CursorPos.x - TotalBoundingBox.GetWidth();
		const auto DeltaY = CursorPos.y - TotalBoundingBox.GetHeight();

		Pos.x += DeltaX;
		Pos.y += DeltaY;
	}

	BackgroundDrawList->AddRectFilled(BoundingBox.Min, BoundingBox.Max, Color("0F0F0F"));
	BackgroundDrawList->AddRectFilledMultiColor(BoundingBoxColorBar.Min, BoundingBoxColorBar.Max, Color("7A79A8"), Color("7A79A8").Darken(0.5F), Color("7A79A8").Darken(0.5F), Color("7A79A8"));
}
