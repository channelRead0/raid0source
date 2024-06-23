#pragma once
#include "pch.h"
#include <sstream>

#define IMGUI_DEFINE_MATH_OPERATORS

#include <ext/imgui/imgui.h>

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

    inline ColorScheme ColorSchemeUI =
    {
        "FFFFFF",
        "444751",
        "19191A",
        "0F0F0F",
        "141415",
        "111012",
        "131314",
        "7978A0",//"7978A0 78A09B",
    };
}