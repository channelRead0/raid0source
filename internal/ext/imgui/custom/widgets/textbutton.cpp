#include <map>

#include "../custom.hpp"

using namespace Schemes;

bool Widgets::TextButton(ImVec2 Position, const char* Name, bool Selected)
{
    const auto IO = ImGui::GetIO();

    const auto TextSize = ImGui::CalcTextSize(Name);
    const auto BoundingBoxButton = ImRect(Position, Position + TextSize);
    const auto DrawList = ImGui::GetWindowDrawList();
    const auto BoundingBoxButton2 = ImRect(Position - ImVec2(10.F, 5.F), Position + TextSize + ImVec2(10.F, 5.F));

    const auto Pressed = Alignment::PointInsideRect(ImGui::GetMousePos(), BoundingBoxButton2) && ImGui::IsMouseClicked(ImGuiMouseButton_Left);

    struct TextButtonColors
    {
        ImColor TextColor;
    };

    const int AnimationID = reinterpret_cast<int>(Name);

    static std::map< ImGuiID, TextButtonColors > Animations;
    auto AnimationMap = Animations.find(AnimationID);
    if (AnimationMap == Animations.end())
    {
        Animations.insert({ AnimationID, TextButtonColors() });
        AnimationMap = Animations.find(AnimationID);
    }

    AnimationMap->second.TextColor.Value = ImLerp(AnimationMap->second.TextColor.Value, Selected ? ColorScheme.Accent : ColorScheme.TextDisabled, IO.DeltaTime * Parameters::AnimationSpeed);

    DrawList->AddText(BoundingBoxButton.Min, AnimationMap->second.TextColor, Name);

    return Pressed;
}
