#include <map>

#include "../custom.hpp"

using namespace Schemes;
using namespace ImGui;


void Widgets::ToolTip(const char* Text, const char* Description, bool Open)
{
    struct ToolTipAnimations
    {
        float Alpha = 0;
    };

    const int AnimationID = (int)Text;

    static std::map< ImGuiID, ToolTipAnimations > Animations;
    auto AnimationMap = Animations.find(AnimationID);
    if (AnimationMap == Animations.end())
    {
        Animations.insert({ AnimationID, ToolTipAnimations() });
        AnimationMap = Animations.find(AnimationID);
    }

    AnimationMap->second.Alpha = ImLerp(AnimationMap->second.Alpha, Open ? 1.F : 0.F, ImGui::GetIO().DeltaTime * (Parameters::AnimationSpeed * Open ? 5.F : 7.F));

    if (AnimationMap->second.Alpha <= 0.15F)
    {
        return;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.F, 5.F));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 1.F);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.F);
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, AnimationMap->second.Alpha);
    ImGui::PushStyleColor(ImGuiCol_Border, ColorScheme.Border.GetColorU32());
    ImGui::PushStyleColor(ImGuiCol_PopupBg, ColorScheme.WindowBackground.GetColorU32());

    ImGui::BeginTooltip();
    ImGui::PushTextWrapPos(450.0f);

    ImGui::PushStyleColor(ImGuiCol_Text, ColorScheme.Accent.GetColorU32());
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
    ImGui::TextUnformatted("U");
    ImGui::PopFont();
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0.F));

    ImGui::PushStyleColor(ImGuiCol_Text, ColorScheme.TextActive.GetColorU32());
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
    ImGui::TextUnformatted(Text);
    ImGui::PopFont();
    ImGui::PopStyleColor();

    ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(0.F, 5.F));
    ImGui::Text(Description);

    ImGui::PopTextWrapPos();
    ImGui::EndTooltip();

    ImGui::PopStyleColor(2);
    ImGui::PopStyleVar(4);
}
