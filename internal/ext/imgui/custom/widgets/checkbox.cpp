#include <map>
#include <Windows.h>
#include "../custom.hpp"

using namespace Schemes;
using namespace ImGui;

bool Widgets::Checkbox(const char* label, bool* v)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    const float square_sz = 13.F;
    const ImVec2 pos = window->DC.CursorPos;
    const ImRect total_bb(pos, pos + ImVec2(253.F, 21.F));

    struct CheckboxAnimations
    {
        ImColor FillColor;
        ImColor TextColor;
    };

    const int AnimationID = id;

    static std::map< ImGuiID, CheckboxAnimations > Animations;
    auto AnimationMap = Animations.find(AnimationID);
    if (AnimationMap == Animations.end())
    {
        Animations.insert({ AnimationID, CheckboxAnimations() });
        AnimationMap = Animations.find(AnimationID);
    }

    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id))
    {
        IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
        return false;
    }

    bool hovered, held;
    bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed)
    {
        *v = !(*v);
        MarkItemEdited(id);
    }

    const ImRect check_bb(pos + ImVec2(232.F, 4.F), pos + ImVec2(245.F, 17.F));
    RenderNavHighlight(total_bb, id);

    AnimationMap->second.FillColor.Value = ImLerp(AnimationMap->second.FillColor.Value, *v ? ColorScheme.Accent : ColorScheme.WindowBackground, ImGui::GetIO().DeltaTime * Parameters::AnimationSpeed);
    AnimationMap->second.TextColor.Value = ImLerp(AnimationMap->second.TextColor.Value, *v ? ColorScheme.TextDisabled : ColorScheme.TextDisabled, ImGui::GetIO().DeltaTime * Parameters::AnimationSpeed);
   
    window->DrawList->AddRectFilled(check_bb.Min + ImVec2(1.F, 1.F), check_bb.Max - ImVec2(1.F, 1.F), AnimationMap->second.FillColor, 0.F);
    window->DrawList->AddRect(check_bb.Min, check_bb.Max, ColorScheme.Border, 2.F, 0, 1.F);
   // RenderFrame(check_bb.Min, check_bb.Max, GetColorU32((held && hovered) ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg), true, style.FrameRounding);
    //ImU32 check_col = GetColorU32(ImGuiCol_CheckMark);
    //bool mixed_value = (g.LastItemData.InFlags & ImGuiItemFlags_MixedValue) != 0;
    //if (mixed_value)
    //{
    //    // Undocumented tristate/mixed/indeterminate checkbox (#2644)
    //    // This may seem awkwardly designed because the aim is to make ImGuiItemFlags_MixedValue supported by all widgets (not just checkbox)
    //    ImVec2 pad(ImMax(1.0f, IM_TRUNC(square_sz / 3.6f)), ImMax(1.0f, IM_TRUNC(square_sz / 3.6f)));
    //    //window->DrawList->AddRectFilled(check_bb.Min + pad, check_bb.Max - pad, check_col, style.FrameRounding);
    //}
    //else if (*v)
    //{
    //    const float pad = ImMax(1.0f, IM_TRUNC(square_sz / 6.0f));
    //    //RenderCheckMark(window->DrawList, check_bb.Min + ImVec2(pad, pad), check_col, square_sz - pad * 2.0f);
    //}

    ImVec2 label_pos = ImVec2(total_bb.Min.x + 9.F, total_bb.GetCenter().y - (label_size.y / 2.F));
   /* if (g.LogEnabled)
        LogRenderedText(&label_pos, mixed_value ? "[~]" : *v ? "[x]" : "[ ]");*/

    ImGui::PushStyleColor(ImGuiCol_Text, AnimationMap->second.TextColor.Value);
    if (label_size.x > 0.0f)
        RenderText(label_pos, label);
    ImGui::PopStyleColor();

   // window->DrawList->AddRect(total_bb.Min, total_bb.Max, ColorScheme.Accent);
    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
    return pressed;
}

const char* const KeyNames[] =
{
    "Unknown",
    "VK_LBUTTON",
    "VK_RBUTTON",
    "VK_CANCEL",
    "VK_MBUTTON",
    "VK_XBUTTON1",
    "VK_XBUTTON2",
    "Unknown",
    "VK_BACK",
    "VK_TAB",
    "Unknown",
    "Unknown",
    "VK_CLEAR",
    "VK_RETURN",
    "Unknown",
    "Unknown",
    "VK_SHIFT",
    "VK_CONTROL",
    "VK_MENU",
    "VK_PAUSE",
    "VK_CAPITAL",
    "VK_KANA",
    "Unknown",
    "VK_JUNJA",
    "VK_FINAL",
    "VK_KANJI",
    "Unknown",
    "VK_ESCAPE",
    "VK_CONVERT",
    "VK_NONCONVERT",
    "VK_ACCEPT",
    "VK_MODECHANGE",
    "VK_SPACE",
    "VK_PRIOR",
    "VK_NEXT",
    "VK_END",
    "VK_HOME",
    "VK_LEFT",
    "VK_UP",
    "VK_RIGHT",
    "VK_DOWN",
    "VK_SELECT",
    "VK_PRINT",
    "VK_EXECUTE",
    "VK_SNAPSHOT",
    "VK_INSERT",
    "VK_DELETE",
    "VK_HELP",
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J",
    "K",
    "L",
    "M",
    "N",
    "O",
    "P",
    "Q",
    "R",
    "S",
    "T",
    "U",
    "V",
    "W",
    "X",
    "Y",
    "Z",
    "VK_LWIN",
    "VK_RWIN",
    "VK_APPS",
    "Unknown",
    "VK_SLEEP",
    "VK_NUMPAD0",
    "VK_NUMPAD1",
    "VK_NUMPAD2",
    "VK_NUMPAD3",
    "VK_NUMPAD4",
    "VK_NUMPAD5",
    "VK_NUMPAD6",
    "VK_NUMPAD7",
    "VK_NUMPAD8",
    "VK_NUMPAD9",
    "VK_MULTIPLY",
    "VK_ADD",
    "VK_SEPARATOR",
    "VK_SUBTRACT",
    "VK_DECIMAL",
    "VK_DIVIDE",
    "VK_F1",
    "VK_F2",
    "VK_F3",
    "VK_F4",
    "VK_F5",
    "VK_F6",
    "VK_F7",
    "VK_F8",
    "VK_F9",
    "VK_F10",
    "VK_F11",
    "VK_F12",
    "VK_F13",
    "VK_F14",
    "VK_F15",
    "VK_F16",
    "VK_F17",
    "VK_F18",
    "VK_F19",
    "VK_F20",
    "VK_F21",
    "VK_F22",
    "VK_F23",
    "VK_F24",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "VK_NUMLOCK",
    "VK_SCROLL",
    "VK_OEM_NEC_EQUAL",
    "VK_OEM_FJ_MASSHOU",
    "VK_OEM_FJ_TOUROKU",
    "VK_OEM_FJ_LOYA",
    "VK_OEM_FJ_ROYA",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "VK_LSHIFT",
    "VK_RSHIFT",
    "VK_LCONTROL",
    "VK_RCONTROL",
    "VK_LMENU",
    "VK_RMENU"
};


bool Widgets::BindCheckbox(const char* label, bool* v, int* bind)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    const float square_sz = 13.F;
    const ImVec2 pos = window->DC.CursorPos;
    const ImRect total_bb(pos, pos + ImVec2(253.F, 21.F));

    struct CheckboxAnimations
    {
        ImColor FillColor;
        ImColor TextColor;
        ImColor TextColorBind;
        int KeyBind = 0;
        bool BindPressed = false;
    };

    const int AnimationID = id;

    static std::map< ImGuiID, CheckboxAnimations > Animations;
    auto AnimationMap = Animations.find(AnimationID);
    if (AnimationMap == Animations.end())
    {
        Animations.insert({ AnimationID, CheckboxAnimations() });
        AnimationMap = Animations.find(AnimationID);
    }

    auto BindBuffer = std::string("...");

    if (AnimationMap->second.BindPressed)
    {
        BindBuffer = ". . .";
    }

    const auto IO = ImGui::GetIO();
    bool BindChanged = false;

    if (AnimationMap->second.BindPressed)
    {
        for (auto i = 0; i < 5; i++)
        {
            if (IO.MouseDown[i])
            {
                switch (i)
                {
                    /*case 0:
                        KeyBind = VK_LBUTTON;
                        break;*/
                case 1:
                    AnimationMap->second.KeyBind = VK_RBUTTON;
                    BindChanged = true;
                    break;
                case 2:
                    AnimationMap->second.KeyBind = VK_MBUTTON;
                    BindChanged = true;
                    break;
                case 3:
                    AnimationMap->second.KeyBind = VK_XBUTTON1;
                    BindChanged = true;
                    break;
                case 4:
                    AnimationMap->second.KeyBind = VK_XBUTTON2;
                    BindChanged = true;
                    break;
                }
            }
        }
        if (!BindChanged)
        {
            for (auto KeyIndex = VK_BACK; KeyIndex <= VK_RMENU; KeyIndex++)
            {
                if (KeyIndex == VK_ESCAPE) {
                    continue;
                }

                if (IO.KeysDown[KeyIndex])
                {
                    AnimationMap->second.KeyBind = KeyIndex;
                    BindChanged = true;
                }
            }
        }
    }

    if (BindChanged)
    {
        AnimationMap->second.BindPressed = false;
    }

    if (AnimationMap->second.KeyBind != 0 && !AnimationMap->second.BindPressed)
    {
        char DisplayBuffer[64];
        strcpy_s(DisplayBuffer, KeyNames[AnimationMap->second.KeyBind]);

        BindBuffer = DisplayBuffer;
    }

    *bind = AnimationMap->second.KeyBind;

    const auto bindtextsize = ImGui::CalcTextSize(BindBuffer.data());
    const auto BindBoundingBox = ImRect(pos + ImVec2((228.F - bindtextsize.x) - 12.F, 1.F), pos + ImVec2(228.F, 20.F));

    bool BindHovered = false; bool SkipLmb = false;
    BindHovered = (Alignment::PointInsideRect(ImGui::GetMousePos(), BindBoundingBox));

    if (Alignment::PointInsideRect(GetMousePos(), BindBoundingBox) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        AnimationMap->second.BindPressed = true;
        SkipLmb = true;
    }

    if (!Alignment::PointInsideRect(GetMousePos(), BindBoundingBox) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        AnimationMap->second.BindPressed = false;
    }

    if (AnimationMap->second.BindPressed && GetAsyncKeyState(VK_ESCAPE) & 0x8000)
    {
        AnimationMap->second.KeyBind = 0;
        AnimationMap->second.BindPressed = false;
    }

    const ImRect check_bb(pos + ImVec2(232.F, 4.F), pos + ImVec2(245.F, 17.F));
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id))
    {
        IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
        return false;
    }

    bool hovered, held;
    bool pressed = ButtonBehavior(check_bb, id, &hovered, &held);
    if (pressed)
    {
        *v = !(*v);
        MarkItemEdited(id);
    }

    static bool set = false;
    if (!AnimationMap->second.BindPressed && IO.KeysDown[*bind] && !set)
    {
        set = true;
        //*v = !(*v);
        MarkItemEdited(id);
    }

    if (!IO.KeysDown[*bind] && set)
    {
        set = false;
    }

    RenderNavHighlight(total_bb, id);

    AnimationMap->second.FillColor.Value = ImLerp(AnimationMap->second.FillColor.Value, *v ? ColorScheme.Accent : ColorScheme.WindowBackground, ImGui::GetIO().DeltaTime * Parameters::AnimationSpeed);
    AnimationMap->second.TextColor.Value = ImLerp(AnimationMap->second.TextColor.Value, *v ? ColorScheme.TextDisabled : ColorScheme.TextDisabled, ImGui::GetIO().DeltaTime * Parameters::AnimationSpeed);
    AnimationMap->second.TextColorBind.Value = ImLerp(AnimationMap->second.TextColorBind.Value, AnimationMap->second.BindPressed ? ColorScheme.TextDisabled : ColorScheme.TextDisabled, ImGui::GetIO().DeltaTime * Parameters::AnimationSpeed);

    window->DrawList->AddRectFilled(BindBoundingBox.Min, BindBoundingBox.Max, ColorScheme.WindowBackground, 2.F);
    window->DrawList->AddRect(BindBoundingBox.Min, BindBoundingBox.Max, ColorScheme.Border, 2.F);
    window->DrawList->AddText(ImVec2(BindBoundingBox.GetCenter() - bindtextsize / 2.F), AnimationMap->second.TextColorBind, BindBuffer.data());

    window->DrawList->AddRectFilled(check_bb.Min + ImVec2(1.F, 1.F), check_bb.Max - ImVec2(1.F, 1.F), AnimationMap->second.FillColor, 1.F);
    window->DrawList->AddRect(check_bb.Min, check_bb.Max, ColorScheme.Border, 1.F, 0, 1.F);
    // RenderFrame(check_bb.Min, check_bb.Max, GetColorU32((held && hovered) ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg), true, style.FrameRounding);
     //ImU32 check_col = GetColorU32(ImGuiCol_CheckMark);
     //bool mixed_value = (g.LastItemData.InFlags & ImGuiItemFlags_MixedValue) != 0;
     //if (mixed_value)
     //{
     //    // Undocumented tristate/mixed/indeterminate checkbox (#2644)
     //    // This may seem awkwardly designed because the aim is to make ImGuiItemFlags_MixedValue supported by all widgets (not just checkbox)
     //    ImVec2 pad(ImMax(1.0f, IM_TRUNC(square_sz / 3.6f)), ImMax(1.0f, IM_TRUNC(square_sz / 3.6f)));
     //    //window->DrawList->AddRectFilled(check_bb.Min + pad, check_bb.Max - pad, check_col, style.FrameRounding);
     //}
     //else if (*v)
     //{
     //    const float pad = ImMax(1.0f, IM_TRUNC(square_sz / 6.0f));
     //    //RenderCheckMark(window->DrawList, check_bb.Min + ImVec2(pad, pad), check_col, square_sz - pad * 2.0f);
     //}

    ImVec2 label_pos = ImVec2(total_bb.Min.x + 9.F, total_bb.GetCenter().y - (label_size.y / 2.F));
    /* if (g.LogEnabled)
         LogRenderedText(&label_pos, mixed_value ? "[~]" : *v ? "[x]" : "[ ]");*/

    ImGui::PushStyleColor(ImGuiCol_Text, AnimationMap->second.TextColor.Value);
    if (label_size.x > 0.0f)
        RenderText(label_pos, label);
    ImGui::PopStyleColor();

    //window->DrawList->AddRect(total_bb.Min, total_bb.Max, ColorScheme.Accent);
    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
    return pressed;
}

IMGUI_API bool Widgets::ColorCheckbox(const char* label, bool* v, float col[4])
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    const float square_sz = 13.F;
    const ImVec2 pos = window->DC.CursorPos;
    const ImRect total_bb(pos, pos + ImVec2(253.F, 21.F));
    const ImRect total_bbA(pos, pos + ImVec2(209.F, 21.F));
    const ImRect total_bbB(pos + ImVec2(232.F, 4.F), pos + ImVec2(245.F, 17.F));

    struct CheckboxAnimations
    {
        ImColor FillColor;
        ImColor TextColor;
    };

    const int AnimationID = id;

    static std::map< ImGuiID, CheckboxAnimations > Animations;
    auto AnimationMap = Animations.find(AnimationID);
    if (AnimationMap == Animations.end())
    {
        Animations.insert({ AnimationID, CheckboxAnimations() });
        AnimationMap = Animations.find(AnimationID);
    }

    ItemSize(total_bbA, 0.f);
    if (!ItemAdd(total_bbA, id))
    {
        IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
        return false;
    }

    ImGui::SameLine();
    window->DC.CursorPos = window->DC.CursorPos + ImVec2(0.F, 1.F);
    Widgets::ColorEdit4(label, col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaBar /*| ImGuiColorEditFlags_AlphaPreview*/);

    const ImRect check_bb(pos + ImVec2(232.F, 4.F), pos + ImVec2(245.F, 17.F));
    bool hovered, held;
    bool pressed = ButtonBehavior(total_bbA, id, &hovered, &held);

    if (Alignment::PointInsideRect(GetMousePos(), check_bb) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        pressed = true;
    }

    if (pressed)
    {
        *v = !(*v);
        MarkItemEdited(id);
    }

    RenderNavHighlight(total_bb, id);

    AnimationMap->second.FillColor.Value = ImLerp(AnimationMap->second.FillColor.Value, *v ? ColorScheme.Accent : ColorScheme.WindowBackground, ImGui::GetIO().DeltaTime * Parameters::AnimationSpeed);
    AnimationMap->second.TextColor.Value = ImLerp(AnimationMap->second.TextColor.Value, *v ? ColorScheme.TextDisabled : ColorScheme.TextDisabled, ImGui::GetIO().DeltaTime * Parameters::AnimationSpeed);

    window->DrawList->AddRectFilled(check_bb.Min + ImVec2(1.F, 1.F), check_bb.Max - ImVec2(1.F, 1.F), AnimationMap->second.FillColor, 0.F);
    window->DrawList->AddRect(check_bb.Min, check_bb.Max, ColorScheme.Border, 2.F, 0, 1.F);
    // RenderFrame(check_bb.Min, check_bb.Max, GetColorU32((held && hovered) ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg), true, style.FrameRounding);
     //ImU32 check_col = GetColorU32(ImGuiCol_CheckMark);
     //bool mixed_value = (g.LastItemData.InFlags & ImGuiItemFlags_MixedValue) != 0;
     //if (mixed_value)
     //{
     //    // Undocumented tristate/mixed/indeterminate checkbox (#2644)
     //    // This may seem awkwardly designed because the aim is to make ImGuiItemFlags_MixedValue supported by all widgets (not just checkbox)
     //    ImVec2 pad(ImMax(1.0f, IM_TRUNC(square_sz / 3.6f)), ImMax(1.0f, IM_TRUNC(square_sz / 3.6f)));
     //    //window->DrawList->AddRectFilled(check_bb.Min + pad, check_bb.Max - pad, check_col, style.FrameRounding);
     //}
     //else if (*v)
     //{
     //    const float pad = ImMax(1.0f, IM_TRUNC(square_sz / 6.0f));
     //    //RenderCheckMark(window->DrawList, check_bb.Min + ImVec2(pad, pad), check_col, square_sz - pad * 2.0f);
     //}

    ImVec2 label_pos = ImVec2(total_bb.Min.x + 9.F, total_bb.GetCenter().y - (label_size.y / 2.F));
    /* if (g.LogEnabled)
         LogRenderedText(&label_pos, mixed_value ? "[~]" : *v ? "[x]" : "[ ]");*/

    ImGui::PushStyleColor(ImGuiCol_Text, AnimationMap->second.TextColor.Value);
    if (label_size.x > 0.0f)
        RenderText(label_pos, label);
    ImGui::PopStyleColor();

   /* window->DrawList->AddRect(total_bb.Min, total_bb.Max, ColorScheme.Accent);
    window->DrawList->AddRect(total_bbA.Min, total_bbA.Max, ColorScheme.Accent);
    window->DrawList->AddRect(total_bbB.Min, total_bbB.Max, ColorScheme.Accent);*/
    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
    return pressed;
}
