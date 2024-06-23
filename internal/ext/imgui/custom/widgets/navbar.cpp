#include <map>

#include "../custom.hpp"

using namespace Schemes;

bool Widgets::Navbar(bool* SectionA, bool* SectionB, bool* SectionC, bool* SectionD, bool* SectionE)
{
    struct NavAnimations
    {
        ImColor FrameColor;
        float FrameOffset;
    };

    constexpr int AnimationID = 000;

    static std::map< ImGuiID, NavAnimations > Animations;
    auto AnimationMap = Animations.find(AnimationID);
    if (AnimationMap == Animations.end())
    {
        Animations.insert({ AnimationID, NavAnimations() });
        AnimationMap = Animations.find(AnimationID);
    }

    const auto WindowPos = ImGui::GetWindowPos();
    const auto WindowBounds = ImRect(WindowPos, WindowPos + ImVec2(534.F, 571.F));
    const auto WindowDrawlist = ImGui::GetWindowDrawList();

    const auto NavbarBoundingBox = ImRect(ImVec2(WindowBounds.Min.x, WindowPos.y + 27.F), ImVec2(WindowBounds.Max.x, WindowPos.y + 27.F + 40.F));
    WindowDrawlist->AddRectFilled(NavbarBoundingBox.Min, NavbarBoundingBox.Max, Color(17, 17, 18, 255));
    WindowDrawlist->AddRect(NavbarBoundingBox.Min, NavbarBoundingBox.Max, ColorScheme.Border);

    bool ValueChanged = false;
    static int ModulePageIndex = 0;
    if (Widgets::TextButton(ImVec2(NavbarBoundingBox.Min.x + 53.F, NavbarBoundingBox.GetCenter().y - ImGui::CalcTextSize("Combat").y / 2.F), "Combat", ModulePageIndex == 0)) { ModulePageIndex = 0; ValueChanged = true; }
    if (Widgets::TextButton(ImVec2(NavbarBoundingBox.Min.x + 157.F, NavbarBoundingBox.GetCenter().y - ImGui::CalcTextSize("Player").y / 2.F), "Player", ModulePageIndex == 1)) { ModulePageIndex = 1; ValueChanged = true; }
    if (Widgets::TextButton(ImVec2(NavbarBoundingBox.Min.x + 249.F, NavbarBoundingBox.GetCenter().y - ImGui::CalcTextSize("Visuals").y / 2.F), "Visuals", ModulePageIndex == 2)) { ModulePageIndex = 2; ValueChanged = true; }
    if (Widgets::TextButton(ImVec2(NavbarBoundingBox.Min.x + 354.F, NavbarBoundingBox.GetCenter().y - ImGui::CalcTextSize("Utility").y / 2.F), "Utility", ModulePageIndex == 3)) { ModulePageIndex = 3; ValueChanged = true; }
    if (Widgets::TextButton(ImVec2(NavbarBoundingBox.Min.x + 451.F, NavbarBoundingBox.GetCenter().y - ImGui::CalcTextSize("Client").y / 2.F), "Client", ModulePageIndex == 4)) { ModulePageIndex = 4; ValueChanged = true; }

    *SectionA = (ModulePageIndex == 0);
    *SectionB = (ModulePageIndex == 1);
    *SectionC = (ModulePageIndex == 2);
    *SectionD = (ModulePageIndex == 3);
    *SectionE = (ModulePageIndex == 4);

    return ValueChanged;
}
