#pragma once
#include <ios>
#include <sstream>
#include <string>
#include <vector>

#define IMGUI_DEFINE_MATH_OPERATORS

#include <ext/imgui/imgui.h>
#include <ext/imgui/imgui_internal.h>
#include <algorithm>

struct Color
{
    float R, G, B, A;

    int HexToDecimal(const std::string& HexColor) const
    {
        std::stringstream StringStream;
        StringStream << std::hex << HexColor;
        int Decimal;
        StringStream >> Decimal;

        return Decimal;
    }

    Color(const int Red, const int Green, const int Blue, const int Alpha = 255) {
        R = Red; G = Green; B = Blue; A = Alpha;
    }

    Color(const float Red, const float Green, const float Blue, const float Alpha = 255) {
        R = Red; G = Green; B = Blue; A = Alpha;
    }

    Color(const char* Hex)
    {
        const auto HexString = std::string(Hex);

        R = HexToDecimal(HexString.substr(0, 2));
        G = HexToDecimal(HexString.substr(2, 2));
        B = HexToDecimal(HexString.substr(4, 2));
        A = 255.F;
    }

    Color(const std::string& Hex)
    {
        R = HexToDecimal(Hex.substr(0, 2));
        G = HexToDecimal(Hex.substr(2, 2));
        B = HexToDecimal(Hex.substr(4, 2));
        A = 255.F;
    }

    Color(const ImVec4 Color)
    {
        R = Color.x * 255.F;
        G = Color.y * 255.F;
        B = Color.z * 255.F;
        A = Color.w * 255.F;
    }

    Color(const ImColor Color)
    {
        R = Color.Value.x * 255.F;
        G = Color.Value.y * 255.F;
        B = Color.Value.z * 255.F;
        A = Color.Value.w * 255.F;
    }

    ImVec4 GetColorVec4() const {
        return { R / 255.F , G / 255.F, B / 255.F, A / 255.F };
    }

    ImU32 GetColorU32() const {
        return ImGui::GetColorU32(GetColorVec4());
    }

    ImColor GetImColor() const {
        return IM_COL32(R, G, B, A);
    }

    operator ImVec4() const {
        return { R / 255.F , G / 255.F, B / 255.F, A / 255.F };
    }

    operator ImU32() const {
        return ImGui::GetColorU32(GetColorVec4());
    }

    Color Darken(float Factor)
    {
        Factor = std::clamp(Factor, 0.01F, 1.F);

        this->R *= (1.0f - Factor);
        this->G *= (1.0f - Factor);
        this->B *= (1.0f - Factor);

        return Color(R, G, B, A);
    }

    Color SetAlpha(const float Alpha)
    {
        this->A = Alpha;
        return Color(R, G, B, A);
    }

    friend std::ostream& operator<<(std::ostream& OutputStream, const Color& Color)
    {
        OutputStream << "ColorU: {R: " << Color.R << ", G: " << Color.G << ", B: " << Color.B << ", A: " << Color.A << "}";
        return OutputStream;
    }
};


namespace Schemes
{
    struct ColorScheme
    {
        Color TextActive;
        Color TextDisabled;

        Color Border;
        Color WindowBackground;
        Color WindowBorder;

        Color ChildBackground;
        Color ChildHeaderColor;

        Color Accent;
    };

    inline ColorScheme ColorScheme =
    {
        "FFFFFF",
        "444751",
        "19191A",
        "0F0F0F",
        "141415",
        "111012",
        "131314",
        "7978A0",//"7978A0",
    };
}

namespace Widgets
{
    bool Navbar(bool* SectionA, bool* SectionB, bool* SectionC, bool* SectionD, bool* SectionE);
    bool TextButton(ImVec2 Position, const char* Name, bool Selected);
    //imgui
    IMGUI_API bool BeginChild(const char* str_id, bool tooltip = false, const char* tooltipdesc = nullptr, const ImVec2& size = ImVec2(0, 0), ImGuiChildFlags child_flags = 0, ImGuiWindowFlags window_flags = 0);
    IMGUI_API bool BeginChildEx(const char* name, ImGuiID id, const ImVec2& size_arg, ImGuiChildFlags child_flags, ImGuiWindowFlags window_flags, bool tooltip = false, const char* tooltipdesc = nullptr);
    IMGUI_API bool Checkbox(const char* label, bool* v);
    IMGUI_API bool BindCheckbox(const char* label, bool* v, int* bind);
    IMGUI_API bool ColorCheckbox(const char* label, bool* v, float col[4]);
    IMGUI_API bool ColorSelector(const char* label, float col[4]);
    IMGUI_API bool SliderInt(const char* label, int* v, int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);
    IMGUI_API bool SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
    IMGUI_API bool SliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format = NULL, ImGuiSliderFlags flags = 0);
    IMGUI_API bool Combo(const char* label, int* current_item, const char* items_separated_by_zeros, int popup_max_height_in_items = -1);      // Separate items with \0 within a string, end item-list with \0\0. e.g. "One\0Two\0Three\0"
    IMGUI_API bool Combo(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items = -1);
    IMGUI_API bool Combo(const char* label, int* current_item, const char* (*getter)(void* user_data, int idx), void* user_data, int items_count, int popup_max_height_in_items = -1);
    IMGUI_API bool BeginCombo(const char* label, const char* preview_value, ImGuiComboFlags flags = 0);
    IMGUI_API bool Selectable(const char* label, bool selected = false, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0)); // "bool selected" carry the selection state (read-only). Selectable() is clicked is returns true so you can modify your selection state. size.x==0.0: use remaining width, size.x>0.0: specify width. size.y==0.0: use label height, size.y>0.0: specify height
    IMGUI_API bool Selectable(const char* label, bool* p_selected, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));
    IMGUI_API bool BeginComboPopup(ImGuiID popup_id, const ImRect& bb, ImGuiComboFlags flags);
    IMGUI_API bool ColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags = 0);
    IMGUI_API bool ColorPicker4(const char* label, float col[4], ImGuiColorEditFlags flags = 0, const float* ref_col = NULL);
    IMGUI_API bool ColorButton(const char* desc_id, const ImVec4& col, ImGuiColorEditFlags flags = 0, const ImVec2& size = ImVec2(0, 0)); // display a color square/button, hover for details, return true when pressed.
    IMGUI_API void EndChild();

    void MultiSelectableCombo(ImVec2 size, const std::vector<std::tuple<const char*, const char*>> items, bool* selected, size_t size_, const char* cusid);
    void ToolTip(const char* Text, const char* Description, bool Open);
    std::string TextAnimation();
}

namespace Alignment
{
    inline bool PointInsideRect(const ImVec2& Point, const ImRect& Rect) {
        return Point.x >= Rect.Min.x && Point.x <= Rect.Max.x && Point.y >= Rect.Min.y && Point.y <= Rect.Max.y;
    }
}

namespace Parameters
{
    inline float AnimationSpeed = 4.F;
    inline float UIScale = 1.F;
}
