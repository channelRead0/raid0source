#include "pch.h"
#include "ui.hpp"

#include <ext/imgui/imgui.h>
#include <iostream>

#include "cheat/module/modules/combat/aimassist.hpp"
#include "cheat/module/modules/combat/hitselect.hpp"
#include "cheat/module/modules/combat/autoclicker.hpp"
#include "cheat/module/modules/combat/velocity.hpp"
#include "cheat/module/modules/combat/reach.hpp"
#include "cheat/module/modules/combat/blockhit.hpp"
#include "cheat/module/modules/render/render.hpp"
#include "cheat/module/modules/render/zerohud.hpp"
#include "cheat/module/modules/render/arraylist.hpp"
#include "cheat/module/modules/utils/backtrack.hpp"
#include "cheat/module/modules/utils/throwpot.hpp"
#include "cheat/module/modules/utils/bridgeassist.hpp"
#include "cheat/module/modules/utils/refill.hpp"
#include "cheat/module/modules/utils/autotool.hpp"
#include "cheat/module/modules/utils/cheststealer.hpp"
#include "cheat/module/modules/utils/rightclicker.hpp"
#include "cheat/module/modules/utils/fastplace.hpp"
#include "cheat/module/modules/utils/framespoofer.hpp"
#include "cheat/module/modules/utils/lagrange.hpp"
#include "cheat/module/modules/world/antibot.hpp"
#include "cheat/module/modules/movement/wtap.hpp"
#include "cheat/module/modules/movement/stap.hpp"
#include "cheat/module/modules/movement/sprint.hpp"
#include "cheat/module/modules/movement/blink.hpp"
#include "cheat/module/modules/movement/jumpreset.hpp"
#include "cheat/module/modules/world/teams.hpp"

#include "cheat/module/client/altmanager.hpp"
#include "notifications/notifications.hpp"
#include "spotify/spotify.hpp"

using namespace Schemes;

void CombatGroup()
{
    ImGui::SetCursorPos(ImVec2(8.F, 80.F));
    Widgets::BeginChild("Aim Assist", false, nullptr);
    {
        const auto Module = ModuleManager::GetModuleByName("AimAssist");

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(0.F, 5.F));
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::BindCheckbox("Toggled", Module->GetTogglePtr(), Module->GetBindPtr());

        if (AimAssist::Vertical)
        {
            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::SliderFloat("Vertical Speed", &AimAssist::VerticalSpeed, 1.F, 30.F, "%.2f");
        }

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderFloat("Horizontal Speed", &AimAssist::HorizontalSpeed, 1.F, 30.F, "%.2f");

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderFloat("Aim FOV", &AimAssist::MaximumFov, 0.F, 180.F, "%.2f degrees");
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderFloat("Aim Distance", &AimAssist::MaximumRange, 3.F, 8.F, "%.2f blocks");

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderInt("Randomization Scale", &AimAssist::RandomizationScale, 10, 100, "%d percent");

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderInt("Randomization Magnitude", &AimAssist::RandomizationMagnitude, 10, 200, "%d percent");

        if (AimAssist::AimMode == AimAssist::AIM_MODE::MULTIPOINT)
        {
            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::SliderFloat("Multi-Point Offset (?)", &AimAssist::MultiPointOffset, 0.F, 1.F, "%.2f percent");
            Widgets::ToolTip("Multi-Point Offset", "Sets how close to the entity's closest point the player should aim at, 1.0 being the closest 0.0 having the same effect as static aim mode. (It is generally recommended to keep it at 1.0 for full effect)", ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary));
        }

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        const char* SmoothingProfiles[] = { "Linear", "Constant", "Ease Out Cubic", "Ease In Quart"};
        Widgets::Combo("Smoothing Mode", &AimAssist::Smoothing, SmoothingProfiles, IM_ARRAYSIZE(SmoothingProfiles));

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        const char* TargetingProfiles[] = { "Closest To Player", "Lowest Health", "Closest Fov" };
        Widgets::Combo("Filter Mode", &AimAssist::FilteringMode, TargetingProfiles, IM_ARRAYSIZE(TargetingProfiles));

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        const char* AimProfiles[] = { "Multi-Point", "Static" };
        Widgets::Combo("Aim Mode (?)", &AimAssist::AimMode, AimProfiles, IM_ARRAYSIZE(AimProfiles));
        Widgets::ToolTip("Aim Modes", "Specification on where in the entity hitbox to aim at.\nStatic: Aims at the center of the hitbox.\nMulti-Point: Dynamically calculates and aims at\nthe closest point to the player on the entity hitbox.", ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary));

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        const char* TargetModes[] = { "Switch", "Single" };
        Widgets::Combo("Targeting Mode", &AimAssist::TargetingMode, TargetModes, IM_ARRAYSIZE(TargetModes));

        if (AimAssist::Vertical)
        {
            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            const char* VerticalProfiles[] = { "Eyes", "Corner" };
            Widgets::Combo("Vertical Aim Point", &AimAssist::VerticalPoint, VerticalProfiles, IM_ARRAYSIZE(VerticalProfiles));
        }

        std::vector<std::tuple<const char*, const char*>> Conditions =
        {
            {"Vertical", "Vertical"},
            {"Only Clicking", "Only Clicking"},
            {"Through Walls", "Through Walls"},
            {"Aim At Invisible", "Aim At Invisible"},
            {"Break Blocks", "Break Blocks"},
            //{"Free Y Axis In Hitbox", "Free Y Axis In Hitbox"},
            {"Whitelist Items", "Whitelist Items"},
        };

        std::vector<std::tuple<const char*, const char*>> WhiteListItems =
        {
            {"Swords", "Swords"},
            {"Axes", "Axes"},
            {"Fists", "Fists"},
            {"Sticks", "Sticks"},
            {"PickAxes", "PickAxes"},
        };

        bool WhiteList[5]
        {
            AimAssist::WhitelistSwords,
            AimAssist::WhitelistAxes,
            AimAssist::WhitelistFist,
            AimAssist::WhitelistSticks,
            AimAssist::WhitelistPickAxe
        };
            
        bool ConditionsB[6]
        {
            AimAssist::Vertical,
            AimAssist::OnlyClicking,
            AimAssist::AimThroughWalls,
            AimAssist::AimAtInvisible,
            AimAssist::BreakBlocks,
            //AimAssist::FreeYAxisInHitbox,
            AimAssist::ItemWhitelist
        };

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::MultiSelectableCombo({ 200.F, 200.F }, Conditions, ConditionsB, 6, "Conditions##AA");

        AimAssist::Vertical = ConditionsB[0];
        AimAssist::OnlyClicking = ConditionsB[1];
        AimAssist::AimThroughWalls = ConditionsB[2];
        AimAssist::AimAtInvisible = ConditionsB[3];
        AimAssist::BreakBlocks = ConditionsB[4];
       // AimAssist::FreeYAxisInHitbox = ConditionsB[5];
        AimAssist::ItemWhitelist = ConditionsB[5];

        if (AimAssist::ItemWhitelist)
        {
            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::MultiSelectableCombo({ 200.F, 200.F }, WhiteListItems, WhiteList, 5, "Whitelist##AA");
        }

        AimAssist::WhitelistSwords = WhiteList[0];
        AimAssist::WhitelistAxes = WhiteList[1];
        AimAssist::WhitelistFist = WhiteList[2];
        AimAssist::WhitelistSticks = WhiteList[3];
        AimAssist::WhitelistPickAxe = WhiteList[4];

        /* ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
         static ImVec4 col; static bool seexxx;
         //Widgets::ColorCheckbox("MyColor##3", &seexxx,(float*)&col*/ /*ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview*///);
    }
    Widgets::EndChild();

    ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(8.F, 7.F));

    Widgets::BeginChild("Velocity", false, nullptr);
    {
        const auto Module = ModuleManager::GetModuleByName("Velocity");

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(0.F, 5.F));
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::BindCheckbox("Toggled", Module->GetTogglePtr(), Module->GetBindPtr());

        const char* VelocityModes[] = { "Normal", "Packet" };
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::Combo("Velocity Mode", &Velocity::VelocityMode, VelocityModes, IM_ARRAYSIZE(VelocityModes));

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderFloat("Vertical", &Velocity::VelocityVertical, 0.F, 100.F);
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderFloat("Horizontal", &Velocity::VelocityHorizontal, 0.F, 100.F);

        /*  ImGui::Checkbox("Check Fire", &Velocity::CheckOnFire);
          ImGui::Checkbox("Check Ground", &Velocity::CheckOnGround);
          ImGui::Checkbox("Check Liquid", &Velocity::LiquidCheck);
          ImGui::Checkbox("Use Delay", &Velocity::UseDelay);*/


        if (Velocity::VelocityMode == Velocity::VELOCITY_MODE::NORMAL && Velocity::UseDelay)
        {
            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::SliderInt("Delay", &Velocity::Delay, 0, 19);
        }

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderInt("Chance", &Velocity::Chance, 0, 100);

        if (Velocity::VelocityMode == Velocity::VELOCITY_MODE::NORMAL)
        {
            std::vector<std::tuple<const char*, const char*>> Conditions =
            {
                {"Use Delay", "Use Delay"},
                { "Disable In Liquids", "Disable In Liquids" },
                { "Disable If On Fire", "Disable If On Fire" },
                { "Targeting Only", "Targeting Only" },
                { "Sprinting Only", "Sprinting Only" },
            };

            bool ConditionsB[5]
            {
                Velocity::UseDelay,
                Velocity::LiquidCheck,
                Velocity::CheckOnFire,
                Velocity::OnlyTargeting,
                Velocity::OnlySprinting,
            };

            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::MultiSelectableCombo(ImVec2(200, 200), Conditions, ConditionsB, 5, "Conditions##VELON");

            Velocity::UseDelay = ConditionsB[0];
            Velocity::LiquidCheck = ConditionsB[1];
            Velocity::CheckOnFire = ConditionsB[2];
            Velocity::OnlyTargeting = ConditionsB[3];
            Velocity::OnlySprinting = ConditionsB[4];
        }

        if (Velocity::VelocityMode == Velocity::VELOCITY_MODE::PACKET)
        {
            std::vector<std::tuple<const char*, const char*>> Conditions =
            {
                { "Disable In Liquids", "Disable In Liquids" },
                { "Disable If On Fire", "Disable If On Fire" },
                { "Targeting Only", "Targeting Only" },
                { "Sprinting Only", "Sprinting Only" },
            };

            bool ConditionsB[4]
            {
                Velocity::LiquidCheck,
                Velocity::CheckOnFire,
                Velocity::OnlyTargeting,
                Velocity::OnlySprinting,
            };

            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::MultiSelectableCombo(ImVec2(200, 200), Conditions, ConditionsB, 4, "Conditions##VELOP");

            Velocity::LiquidCheck = ConditionsB[0];
            Velocity::CheckOnFire = ConditionsB[1];
            Velocity::OnlyTargeting = ConditionsB[2];
            Velocity::OnlySprinting = ConditionsB[3];
        }
    }
    Widgets::EndChild();

    ImGui::SetCursorPos(ImVec2(271.F, 80.F));

    Widgets::BeginChild("Auto Clicker", false, nullptr);
    {
        const auto Module = ModuleManager::GetModuleByName("AutoClicker");

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(0.F, 5.F));
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::BindCheckbox("Toggled", Module->GetTogglePtr(), Module->GetBindPtr());
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderFloat("Target CPS", &AutoClicker::TargetCPS, 1.F, 25.F, "%.2f CPS");

        const char* Config[] = { "Auto", "Custom" };
        static int ConfigMode = 0;

       

        if (ConfigMode == 1)
        {
            static float OutlierChance = 30.F;
            static float SpikeChance = 50.F;
            static float DropChance = 50.F;
            static float DoubleClickChance = 3.F;
            static float RateOfChange = 90;

            AutoClicker::OutlierChance = OutlierChance / 100.F;
            AutoClicker::SpikeChance = SpikeChance / 100.F;
            AutoClicker::DropChance = DropChance / 100.F;
            AutoClicker::DoubleClickChance = DoubleClickChance / 100.F;
            AutoClicker::ChangeChance = RateOfChange / 100.F;

           /* inline static float MinCpsRange = 4.F;
            inline static float MaxCpsRange = 4.F;
            inline static float OutlierChance = 0.03F;
            inline static float SpikeChance = 0.5F;
            inline static float DropChance = 0.5F;
            inline static float DoubleClickChance = 0.03F;
            inline static float ChangeChance = 0.9F;*/

          /*  ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::SliderFloat("Fluctuation Min", &AutoClicker::MinCpsRange, 0.5F, 10.F, "%.1f cps");

            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::SliderFloat("Fluctuation Max", &AutoClicker::MaxCpsRange, 0.5F, 10.F, "%.1f cps");*/

            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::SliderFloat("Outlier Chance", &OutlierChance, 0.F, 100.F, "%.1f%% chance");

            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::SliderFloat("Outlier Range", &AutoClicker::OutlierRange, 0.F, 10.F, "%.1f cps");

            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::SliderFloat("Outlier Cooldown", &AutoClicker::OutlierCoolDown, 0.F, 10.F, "%.1f clicks");

            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::SliderFloat("Outlier Transistion Steps", &AutoClicker::OutlierSteps, 0.F, 10.F, "%.1f clicks");

            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::SliderFloat("Kurtosis", &AutoClicker::Kurtosis, 0.F, 15.F, "%.1f kurtosis");

            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::SliderFloat("Std Dev", &AutoClicker::StdDev, 0.F, 50.F, "%.1f dev");

            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            if (Widgets::SliderFloat("Spike Chance", &SpikeChance, 0.F, 100.F, "%.1f%% chance")) {
                DropChance = 100.F - SpikeChance;
            }

            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            if (Widgets::SliderFloat("Drop Chance", &DropChance, 0.F, 100.F, "%.1f%% chance")) {
                SpikeChance = 100.F - DropChance;
            }

            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::SliderFloat("Double Click Chance", &DoubleClickChance, 0.F, 100.F, "%.1f%% chance");

            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::SliderFloat("Rate Of Change", &RateOfChange, 0.F, 100.F, "%.1f%% chance");
        }

        std::vector<std::tuple<const char*, const char*>> WhiteListItems =
        {
             {"Swords", "Swords"},
             {"Axes", "Axes"},
             {"Fists", "Fists"},
             {"Sticks", "Sticks"},
             {"PickAxes", "PickAxes"},
        };

        std::vector<std::tuple<const char*, const char*>> Conditions =
        {
             {"Only Clicking", "Only Clicking"},
             {"Allow Inventory", "Allow Inventory"},
             {"No Hit Delay", "No Hit Delay"},
             {"Break Blocks", "Break Blocks"},
             {"Prevent Unrefill", "Prevent Unrefill"},
             {"WhiteList", "WhiteList"},
             {"Hit Select", "Hit Select"},
        };

        std::vector<std::tuple<const char*, const char*>> HitSelectConditions =
        {
             {"Fake Swing", "Fake Swing"},
             {"Filter Air", "Filter Air"},
             {"Favor Criticals", "Favor Criticals"},
        };

        bool WhiteList[5]
        {
            AutoClicker::WhitelistSwords,
            AutoClicker::WhitelistAxes,
            AutoClicker::WhitelistFist,
            AutoClicker::WhitelistSticks,
            AutoClicker::WhitelistPickAxe
        };

        bool ConditionsB[7]
        {
            AutoClicker::OnlyClicking,
            AutoClicker::AllowInventory,
            AutoClicker::NoHitDelay,
            AutoClicker::BreakBlocks,
            AutoClicker::PreventUnrefill,
            AutoClicker::ItemWhitelist,
            AutoClicker::HitSelect
        };

        bool HitSelectConditionsB[3]
        {
            AutoClicker::FakeSwing,
            AutoClicker::FilterAir,
            AutoClicker::FavorCriticals
        };

        if (AutoClicker::HitSelect)
        {
            if (AutoClicker::FilterAir)
            {
                ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
                Widgets::SliderInt("Air Hit Cancel Rate", &AutoClicker::CancelRate, 0, 100, "%d percent");
            }

            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::SliderInt("HitSelect Activation Combo Threshold", &AutoClicker::ComboThreshold, 0, 5, "%d hits");

            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::SliderInt("HitSelect Attack Window", &AutoClicker::AttackWindow, 1, 3, "%d ticks");

            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::MultiSelectableCombo(ImVec2(200, 200), HitSelectConditions, HitSelectConditionsB, 3, "HitSelect Conditions##AC");
        }

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::Combo("Configuration Mode", &ConfigMode, Config, IM_ARRAYSIZE(Config));

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::MultiSelectableCombo(ImVec2(200, 200), Conditions, ConditionsB, 7, "Conditions##AC"); 

        AutoClicker::OnlyClicking = ConditionsB[0];
        AutoClicker::AllowInventory = ConditionsB[1];
        AutoClicker::NoHitDelay = ConditionsB[2];
        AutoClicker::BreakBlocks = ConditionsB[3];
        AutoClicker::PreventUnrefill = ConditionsB[4];
        AutoClicker::ItemWhitelist = ConditionsB[5];
        AutoClicker::HitSelect = ConditionsB[6];

        AutoClicker::FakeSwing = HitSelectConditionsB[0];
        AutoClicker::FilterAir = HitSelectConditionsB[1];
        AutoClicker::FavorCriticals = HitSelectConditionsB[2];

        if (AutoClicker::ItemWhitelist)
        {
            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::MultiSelectableCombo({ 200.F, 200.F }, WhiteListItems, WhiteList, 5, "Whitelist##AC");
        }
        
        AutoClicker::WhitelistSwords = WhiteList[0];
        AutoClicker::WhitelistAxes = WhiteList[1];
        AutoClicker::WhitelistFist = WhiteList[2];
        AutoClicker::WhitelistSticks = WhiteList[3];
        AutoClicker::WhitelistPickAxe = WhiteList[4];
    }
    Widgets::EndChild();

    ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(271.F, 7.F));
    Widgets::BeginChild("Block Hit", false, nullptr);
    {
        const auto Module = ModuleManager::GetModuleByName("BlockHit");

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(0.F, 5.F));
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::BindCheckbox("Toggled", Module->GetTogglePtr(), Module->GetBindPtr());

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderFloat("Minimum Duration", &BlockHit::BlockHitDelayMin, 10.F, 500.F, "%.0f ms");
        if (BlockHit::BlockHitDelayMax < BlockHit::BlockHitDelayMin) { BlockHit::BlockHitDelayMax = BlockHit::BlockHitDelayMin; }

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderFloat("Maximum Duration", &BlockHit::BlockHitDelayMax, 10.F, 500.F, "%.0f ms");
        if (BlockHit::BlockHitDelayMax < BlockHit::BlockHitDelayMin) { BlockHit::BlockHitDelayMax = BlockHit::BlockHitDelayMin; }

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderInt("Chance", &BlockHit::Chance, 0, 100, "%d");

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        const char* BlockhitModes[] = { "Passive", "Active" };
        Widgets::Combo("Mode", &BlockHit::Mode, BlockhitModes, IM_ARRAYSIZE(BlockhitModes));
    }
    Widgets::EndChild();

    /*ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(271.F, 7.F));
    Widgets::BeginChild("Hit Select", false, nullptr);
    {
        const auto Module = ModuleManager::GetModuleByName("HitSelect");

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(0.F, 5.F));
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::BindCheckbox("Toggled", Module->GetTogglePtr(), Module->GetBindPtr());
   
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderInt("Attack Window (?)", &HitSelect::AttackWindow, 1, 3, "%d ticks");
        Widgets::ToolTip("Hit Select Window", "Specify how long hit select will be active\nbefore the target entity can be hurt and after the\ntarget is hurt.", ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary));

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderInt("Activation Hit Threshold", &HitSelect::ComboThreshold, 1, 5, "%d hits");

        if (HitSelect::ReduceAir)
        {
            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::SliderInt("Reduction Rate", &HitSelect::CancelRate, 5, 100, "%d");
        }

        std::vector<std::tuple<const char*, const char*>> Conditions =
        {
            {"Fake Swing", "Fake Swing"},
            {"Reduce Air Hits", "Reduce Air Hits"},
        };

        static bool ConditionsB[2]
        {
            HitSelect::FakeSwing,
            HitSelect::ReduceAir,
        };

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::MultiSelectableCombo(ImVec2(200, 200), Conditions, ConditionsB, 2, "Conditions##HITSELECT");

        HitSelect::FakeSwing = ConditionsB[0];
        HitSelect::ReduceAir = ConditionsB[1];
    }
    Widgets::EndChild();*/

    ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(271.F, 7.F));
    Widgets::BeginChild("Reach", false, nullptr);
    {
        const auto Module = ModuleManager::GetModuleByName("Reach");

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(0.F, 5.F));
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::BindCheckbox("Toggled", Module->GetTogglePtr(), Module->GetBindPtr());

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderFloat("Minimum Range", &Reach::DistanceMin, 3.F, 6.F, "%.2f blocks");
        if (Reach::DistanceMax < Reach::DistanceMin) { Reach::DistanceMax = Reach::DistanceMin; }

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderFloat("Maximum Range", &Reach::DistanceMax, 3.F, 6.F, "%.2f blocks");
        if (Reach::DistanceMin > Reach::DistanceMax) { Reach::DistanceMin = Reach::DistanceMax; }

        if (Reach::ExpandHitbox)
        {
            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::SliderFloat("Minimum Scale", &Reach::HitboxScaleMin, 1.F, 5.F, "%.2f percent");
            if (Reach::HitboxScaleMax < Reach::HitboxScaleMin) { Reach::HitboxScaleMax = Reach::HitboxScaleMin; }

            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::SliderFloat("Maximum Scale", &Reach::HitboxScaleMax, 1.F, 5.F, "%.2f percent");  
            if (Reach::HitboxScaleMin > Reach::HitboxScaleMax) { Reach::HitboxScaleMin = Reach::HitboxScaleMax; }
        }

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderInt("Chance", &Reach::Chance, 0, 100, "%d");

        std::vector<std::tuple<const char*, const char*>> Conditions =
        {
            {"Sprinting Only", "Combo Only"},
            {"Disable In Liquids", "Disable In Liquids"},
            {"Hit Through Walls", "Hit Through Walls"},
            {"Check Y", "Check Y"},
            {"Expand Hitboxes", "Expand Hitboxes"},
        };

        static bool ConditionsB[5]
        {
            Reach::OnlySprinting,
            Reach::LiquidCheck,
            Reach::HitThroughWalls,
            Reach::YCheck,
            Reach::ExpandHitbox,
        };

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::MultiSelectableCombo(ImVec2(200, 200), Conditions, ConditionsB, 5, "Conditions##REACH");

        Reach::OnlySprinting = ConditionsB[0];
        Reach::LiquidCheck = ConditionsB[1];
        Reach::HitThroughWalls = ConditionsB[2];
        Reach::YCheck = ConditionsB[3];
        Reach::ExpandHitbox = ConditionsB[4];
    }
    Widgets::EndChild();

    //ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(271.F, 7.F));

    //Widgets::BeginChild("Back Track", false, nullptr);
    //{
    //    const auto Module = ModuleManager::GetModuleByName("BackTrack");

    //    ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(0.F, 5.F));
    //    ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
    //    Widgets::BindCheckbox("Toggled", /*Module->GetTogglePtr()*/&BackTrack::DoBt, Module->GetBindPtr());
    //    ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
    //    Widgets::SliderFloat("Range Min", &BackTrack::DistanceMin, 0.F, 3.F, "%.1f blocks");
    //    if (BackTrack::DistanceMax < BackTrack::DistanceMin) { BackTrack::DistanceMax = BackTrack::DistanceMin; }
    //    ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
    //    Widgets::SliderFloat("Range Max", &BackTrack::DistanceMax, 0.F, 3.F, "%.1f blocks");
    //    if (BackTrack::DistanceMin > BackTrack::DistanceMax) { BackTrack::DistanceMin = BackTrack::DistanceMax; }
    //    ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
    //    Widgets::SliderInt("Delay", &BackTrack::Delay, 10, 1500, "%.d ms");

    //    std::vector<std::tuple<const char*, const char*>> Conditions =
    //    {
    //     {"Disable On Hit", "Disable On Hit"},
    //    };

    //    static bool ConditionsB[1]
    //    {
    //        BackTrack::DisableOnHit,
    //    };

    //    ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
    //    Widgets::MultiSelectableCombo(ImVec2(200, 200), Conditions, ConditionsB, 1, "Conditions##BACKTRACK");

    //    BackTrack::DisableOnHit = ConditionsB[0];
    //}
    //Widgets::EndChild();
}

void PlayerGroup()
{
    ImGui::SetCursorPos(ImVec2(8.F, 80.F));

    Widgets::BeginChild("Back Track", false, nullptr);
    {
        const auto Module = ModuleManager::GetModuleByName("BackTrack");

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(0.F, 5.F));
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::BindCheckbox("Toggled", Module->GetTogglePtr(), Module->GetBindPtr());

        if (BackTrack::RenderServerRecord)
        {
            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::ColorSelector("Box Background", (float*)&BackTrack::BoxColor);
            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::ColorSelector("Box Outlines", (float*)&BackTrack::OutlineColor);
            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::SliderFloat("Lerp Divisor (can crash debug only)", &BackTrack::LerpOffset, 0.01F, 1.F, "%.2f div");
        }

        if (BackTrack::Mode == BackTrack::BACKTRACK_B)
        {
           /* ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::SliderFloat("Distance Max", &BackTrack::DistanceMax, 0.F, 6.F, "%.1f block");*/
        }

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderInt("Delay", &BackTrack::Delay, 10, 1500, "%.d ms");

        if (BackTrack::Mode == BackTrack::BACKTRACK_A)
        {
            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::SliderInt("Combo Threshold", &BackTrack::ComboThreshold, 1, 5, "%.d hits");
        }

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        const char* BackTrackMode[] = { "Normal", "Experimental A" };
        Widgets::Combo("BackTrack Mode", &BackTrack::Mode, BackTrackMode, IM_ARRAYSIZE(BackTrackMode));

        if (BackTrack::Mode == BackTrack::BACKTRACK_A)
        {
            std::vector<std::tuple<const char*, const char*>> Conditions =
            {
                {"Stop On Local Damage", "Stop Local Damage"},
                {"Render Server Record", "Render Server Record"},
            };

            static bool ConditionsB[2]
            {
                BackTrack::DisableOnHit,
                BackTrack::RenderServerRecord,
            };

            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::MultiSelectableCombo(ImVec2(200, 200), Conditions, ConditionsB, 2, "Conditions##BACKTRACKB");

            BackTrack::DisableOnHit = ConditionsB[0];
            BackTrack::RenderServerRecord = ConditionsB[1];
        }

        if (BackTrack::Mode == BackTrack::BACKTRACK_B)
        {
            std::vector<std::tuple<const char*, const char*>> Conditions =
            {
                {"Render Server Record", "Render Server Record"},
            };

            static bool ConditionsB[1]
            {
                BackTrack::RenderServerRecord,
            };

            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::MultiSelectableCombo(ImVec2(200, 200), Conditions, ConditionsB, 1, "Conditions##BACKTRACK");

            BackTrack::RenderServerRecord = ConditionsB[0];
        }
      
       /* ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderFloat("Range Min", &BackTrack::DistanceMin, 0.F, 8.F, "%.1f blocks");*/
       /* if (BackTrack::DistanceMax < BackTrack::DistanceMin) { BackTrack::DistanceMax = BackTrack::DistanceMin; }
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderFloat("Range Max", &BackTrack::DistanceMax, 0.F, 8.F, "%.1f blocks");
        if (BackTrack::DistanceMin > BackTrack::DistanceMax) { BackTrack::DistanceMin = BackTrack::DistanceMax; }
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderInt("Delay", &BackTrack::TickDelay, 10, 1500, "%.d ms");

        std::vector<std::tuple<const char*, const char*>> Conditions =
        {
         {"Disable On Hit", "Disable On Hit"},
        };

        static bool ConditionsB[1]
        {
            BackTrack::DisableOnHit,
        };

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::MultiSelectableCombo(ImVec2(200, 200), Conditions, ConditionsB, 1, "Conditions##BACKTRACK");

        BackTrack::DisableOnHit = ConditionsB[0];*/
    }
    Widgets::EndChild();

    ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(8.F, 7.F));

    Widgets::BeginChild("Lag Range", false, nullptr);
    {
        const auto Module = ModuleManager::GetModuleByName("LagRange");

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(0.F, 5.F));
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::BindCheckbox("Toggled", Module->GetTogglePtr(), Module->GetBindPtr()); 
        
        if (LagRange::RenderServerRecord)
        {
            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::ColorSelector("Box Background", (float*)&LagRange::BoxColor);
            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::ColorSelector("Box Outlines", (float*)&LagRange::OutlineColor);
        }

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderInt("Delay", &LagRange::Delay, 10, 1000, "%.d ms");
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderInt("Cooldown", &LagRange::Cooldown, 100, 5000, "%.d ms");
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderFloat("Activation Distance", &LagRange::ActivationDistance, 0.5F, 10.F, "%.1f blocks");

        std::vector<std::tuple<const char*, const char*>> Conditions =
        {
            {"Render Server Record", "Render Server Record"},
        };

        static bool ConditionsB[1]
        {
            LagRange::RenderServerRecord,
        };

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::MultiSelectableCombo(ImVec2(200, 200), Conditions, ConditionsB, 1, "Conditions##LAGRANGE");

        LagRange::RenderServerRecord = ConditionsB[0];
    }
    Widgets::EndChild();

    ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(8.F, 7.F));

    Widgets::BeginChild("WTap", false, nullptr);
    {
        const auto Module = ModuleManager::GetModuleByName("WTap");

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(0.F, 5.F));
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::BindCheckbox("Toggled", Module->GetTogglePtr(), Module->GetBindPtr());

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderFloat("Minimum Duration", &WTap::DelayMin, 10.F, 500.F, "%.0f ms");
        if (WTap::DelayMax < WTap::DelayMin) { WTap::DelayMax = WTap::DelayMin; }

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderFloat("Maximum Duration", &WTap::DelayMax, 10.F, 500.F, "%.0f ms");
        if (WTap::DelayMax < WTap::DelayMin) { WTap::DelayMax = WTap::DelayMin; }

        /*std::vector<std::tuple<const char*, const char*>> Conditions =
        {
         {"Combo Only", "Combo Only"},
        };

        static bool ConditionsB[1]
        {
            WTap::InCombo,
        };

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::MultiSelectableCombo(ImVec2(200, 200), Conditions, ConditionsB, 1, "Conditions##WTAP");

        WTap::InCombo = ConditionsB[0];*/
    }
    Widgets::EndChild();

    ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(8.F, 7.F));
    Widgets::BeginChild("Jump Reset", false, nullptr);
    {
        const auto Module = ModuleManager::GetModuleByName("JumpReset");

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(0.F, 5.F));
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::BindCheckbox("Toggled", Module->GetTogglePtr(), Module->GetBindPtr());
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderInt("Jump Delay (?)", &JumpReset::TickDelay, 0, 20, "%d ms");
        Widgets::ToolTip("Jump Delay", "This allows to delay the jump after getting hit\nit is recommended to keep this at 10 under normal circumstances.", ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary));
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderInt("Chance (?)", &JumpReset::Chance, 0, 100, "%d chance");
        Widgets::ToolTip("Chance", "Using chance mitigates the risk\nof getting detected on advanced anticheats.", ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary));
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        const char* JumpMode[] = { "Pre", "Post" };
        Widgets::Combo("Jump Mode", &JumpReset::Mode, JumpMode, IM_ARRAYSIZE(JumpMode));
    }
    Widgets::EndChild();

    ImGui::Dummy(ImVec2(534.F, 22.F));

    ImGui::SetCursorPos(ImVec2(271.F, 80.F));

    Widgets::BeginChild("Sprint", false, nullptr);
    {
        const auto Module = ModuleManager::GetModuleByName("Sprint");

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(0.F, 5.F));
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::BindCheckbox("Toggled", Module->GetTogglePtr(), Module->GetBindPtr()); 

        std::vector<std::tuple<const char*, const char*>> Conditions =
        {
            {"Disable If Invisible", "Disable If Invisible"},
        };

        static bool ConditionsB[1]
        {
            Sprint::DisableOnInvisible,
        };

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::MultiSelectableCombo(ImVec2(200, 200), Conditions, ConditionsB, 1, "Conditions##SPRINT");

        Sprint::DisableOnInvisible = ConditionsB[0];
    }
    Widgets::EndChild();

    ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(271.F, 7.F));

    Widgets::BeginChild("STap", false, nullptr);
    {
        const auto Module = ModuleManager::GetModuleByName("STap");

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(0.F, 5.F));
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::BindCheckbox("Toggled", Module->GetTogglePtr(), Module->GetBindPtr());

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderFloat("Minimum Duration", &STap::DelayMin, 10.F, 500.F, "%.0f ms");
        if (STap::DelayMax < STap::DelayMin) { STap::DelayMax = STap::DelayMin; }

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderFloat("Maximum Duration", &STap::DelayMax, 10.F, 500.F, "%.0f ms");
        if (STap::DelayMax < STap::DelayMin) { STap::DelayMax = STap::DelayMin; }

        if (STap::Mode == STap::PASSIVE)
        {
            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::SliderFloat("Maintain Distance", &STap::MaintainDistance, 2.F, 3.F, "%.1f blocks");
        }

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        const char* BackTrackMode[] = { "Passive", "Aggressive" };
        Widgets::Combo("STap Mode", &STap::Mode, BackTrackMode, IM_ARRAYSIZE(BackTrackMode));

        /*std::vector<std::tuple<const char*, const char*>> Conditions =
        {
         {"Combo Only", "Combo Only"},
        };

        static bool ConditionsB[1]
        {
            WTap::InCombo,
        };

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::MultiSelectableCombo(ImVec2(200, 200), Conditions, ConditionsB, 1, "Conditions##WTAP");

        WTap::InCombo = ConditionsB[0];*/
    }
    Widgets::EndChild();

    ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(271.F, 7.F));

    Widgets::BeginChild("Blink", false, nullptr);
    {
        const auto Module = ModuleManager::GetModuleByName("Blink");

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(0.F, 5.F));
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::BindCheckbox("Toggled", Module->GetTogglePtr(), Module->GetBindPtr());

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        const char* DirectionModes[] = { "Send", "Receive", "Both"};
        Widgets::Combo("Direction", &Blink::Direction, DirectionModes, IM_ARRAYSIZE(DirectionModes));

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        const char* OnTimeOut[] = { "Disable", "Auto Send" };
        Widgets::Combo("On Timeout", &Blink::TimeoutMode, OnTimeOut, IM_ARRAYSIZE(OnTimeOut));

        if (Blink::TimeoutMode == Blink::ON_TIMEOUT::AUTOSEND)
        {
            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::SliderInt("Re-Toggle Interval", &Blink::TimeBeforeAutoSend, 10, 2000, "%d ms");
        }

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderInt("Timeout", &Blink::Timeout, 10, 10000, "%d ms");

        std::vector<std::tuple<const char*, const char*>> Conditions =
        {
            {"Disable On Local Hit", "Disable On Local Hit"},
            {"Disable On Target Interact", "Disable On Target Interact"},
        };

        static bool ConditionsB[2]
        {
            Blink::DisableOnLocalDamage,
            Blink::DisableOnTargetDamage
        };

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::MultiSelectableCombo(ImVec2(200, 200), Conditions, ConditionsB, 2, "Conditions##BLINK");

        Blink::DisableOnLocalDamage = ConditionsB[0];
        Blink::DisableOnTargetDamage = ConditionsB[1];
    }
    Widgets::EndChild();
}

void VisualsGroup()
{
    ImGui::SetCursorPos(ImVec2(8.F, 80.F));
    Widgets::BeginChild("ESP", false, nullptr);
    {
        const auto Module = ModuleManager::GetModuleByName("PlayerESP");

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(0.F, 5.F));
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::BindCheckbox("Toggled", Module->GetTogglePtr(), Module->GetBindPtr());


        const auto NameTags = ModuleManager::GetModuleByName("NameTags");

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::BindCheckbox("NameTags", NameTags->GetTogglePtr(), NameTags->GetBindPtr());
        /*ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::ColorCheckbox("Outlines", &PlayerESP::DrawOutlines, (float*)&PlayerESP::OutlineColor);*/

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::ColorCheckbox("Box Outlines", &PlayerESP::DrawESPOutlines, (float*)&PlayerESP::EspOutlineColor);
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::ColorCheckbox("Box Background", &PlayerESP::DrawESPBackground, (float*)&PlayerESP::EspBackgroundColor);

        if (PlayerESP::RenderMode == PlayerESP::RENDER_2D)
        {
            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::ColorCheckbox("Health Bar", &PlayerESP::DrawHealthBar, (float*)&PlayerESP::HealthBarShadedColor);
        }

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderFloat("Render Distance", &Render::RenderDistance, 5.F, 160.F, "%.0f blocks");

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderFloat("Fade Distance", &Render::FadeDistance, 1.F, 10.F, "%.0f blocks");

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        const char* RenderModes[] = { "2D", "3D" };
        Widgets::Combo("Render Mode", &PlayerESP::RenderMode, RenderModes, IM_ARRAYSIZE(RenderModes));

        if (PlayerESP::RenderMode == PlayerESP::RENDER_2D)
        {
            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            const char* RenderStyle[] = { "Full", "Corners" };
            Widgets::Combo("Render Style", &PlayerESP::RenderStyle, RenderStyle, IM_ARRAYSIZE(RenderStyle));
        }

        if (PlayerESP::RenderMode == PlayerESP::RENDER_3D)
        {
            PlayerESP::DrawHealthBar = false;
        }

        std::vector<std::tuple<const char*, const char*>> Conditions =
        {
            {"Exclude Team Mates", "Exclude Team Mates"},
            {"Exclude Friends", "Exclude Friends"},
        };

        static bool ConditionsB[2]
        {
            Render::ExcludeTeamMates,
            Render::ExcludeFriends,
        };

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::MultiSelectableCombo(ImVec2(200, 200), Conditions, ConditionsB, 2, "Conditions##RENDER");

        Render::ExcludeTeamMates = ConditionsB[0];
        Render::ExcludeFriends = ConditionsB[0];
    }
    Widgets::EndChild();

    ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(8.F, 7.F));

    Widgets::BeginChild("Radar", false, nullptr);
    {
        const auto Module = ModuleManager::GetModuleByName("Radar");

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(0.F, 5.F));
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::BindCheckbox("Toggled", Module->GetTogglePtr(), Module->GetBindPtr());
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::ColorSelector("Blip Color", (float*)&Radar::BlipColor);
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderFloat("Zoom", &Radar::RadarZoom, 1.F, 10.F, "%.1f pp/s");
    }
    Widgets::EndChild();


    /*Widgets::BeginChild("NameTags", false, nullptr);
    {
        const auto Module = ModuleManager::GetModuleByName("NameTags");

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(0.F, 5.F));
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::BindCheckbox("Toggled", Module->GetTogglePtr(), Module->GetBindPtr());
    }
    Widgets::EndChild();*/

    ImGui::SetCursorPos(ImVec2(271.F, 80.F));

    Widgets::BeginChild("ArrayList", false, nullptr);
    {
        const auto Module = ModuleManager::GetModuleByName("ArrayList");

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(0.F, 5.F));
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::BindCheckbox("Toggled", Module->GetTogglePtr(), Module->GetBindPtr());

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::ColorCheckbox("Module Background Color", &ArrayList::DrawBox, (float*)&ArrayList::ModuleBoxColor);

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::ColorSelector("Info Text Color", (float*)&ArrayList::FlagColor);


        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::ColorSelector("Module Color", (float*)&ArrayList::ModuleColor);

        if (ArrayList::ColorMode == ArrayList::COLOR_MODE::STATIC_COLORWAVE)
        {
            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::ColorSelector("Module Color B", (float*)&ArrayList::ModuleColorB);
        }

        if (ArrayList::ColorMode == ArrayList::CHROMA)
        {
            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::SliderFloat("Saturation", &ArrayList::Saturation, 0.F, 1.F, "%.1f");
        }

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderFloat("Position Offset X", &ArrayList::StartOffsetX, -5.F, 30.F, "%.1f pixels");

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderFloat("Position Offset Y", &ArrayList::StartOffsetY, -5.F, 30.F, "%.1f pixels");

        static float PaddingY = 0;
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderFloat("Padding X", &ArrayList::PaddingX, 0.F, 5.F, "%.1f pixels");
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderFloat("Padding Y", &PaddingY, 0.F, 1.F, "%.1f pixels");

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderFloat("Rounding", &ArrayList::Rounding, 0.F, 5.F);

        ArrayList::PaddingY = -1 + PaddingY;

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));

        if (ArrayList::ColorMode == ArrayList::COLOR_MODE::STATIC_COLORWAVE)
        {
            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::SliderFloat("Wave Speed", &ArrayList::WaveSpeed, 0.1F, 1.F);
        }

        static float Barwidth = 1.F;
        if (ArrayList::Bar)
        {
            Widgets::SliderFloat("Bar Width", &ArrayList::BarWidth, 1.F, 3.F, "%.1f pixels");
            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        }

        const char* RenderStyle[] = { "Static", "Chroma", "Static Colorwave"};
        Widgets::Combo("Render Style##ARRAYLIS", &ArrayList::ColorMode, RenderStyle, IM_ARRAYSIZE(RenderStyle));

        const char* FontStyle[] = { "Font A", "Font B", "Font C" };
        Widgets::Combo("Font Style##ARRAYLIS", &ArrayList::Font, FontStyle, IM_ARRAYSIZE(FontStyle));

       /* if (ArrayList::ColorMode == 0)
        {
            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::Checkbox("Color Wave", &ArrayList::StaticColorWave);
        }*/

        if (ArrayList::ColorMode != ArrayList::COLOR_MODE::STATIC_COLORWAVE)
        {
            std::vector<std::tuple<const char*, const char*>> Conditions =
            {
                {"Text Shadows", "Text Shadows" },
                {"Array Bar", "Array Bar"},
                {"Lower Case", "Lower Case"}
            };

            static bool ConditionsB[3]
            {
                ArrayList::TextShadows,
                ArrayList::Bar,
                ArrayList::LowerCase
            };

            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::MultiSelectableCombo(ImVec2(200, 200), Conditions, ConditionsB, 3, "Conditions##ARRAY");

            ArrayList::TextShadows = ConditionsB[0];
            ArrayList::Bar = ConditionsB[1];
            ArrayList::LowerCase = ConditionsB[2];
        }
        else
        {
            std::vector<std::tuple<const char*, const char*>> Conditions =
            {
                {"Text Shadows", "Text Shadows" },
                {"Array Bar", "Array Bar"},
                {"Lower Case", "Lower Case"},
                {"Inverse Color Wave", "Inverse Color Wave"},
                {"Inverse Color Wave Flow", "Inverse Color Wave Flow"}
            };

            static bool ConditionsB[5]
            {
                ArrayList::TextShadows,
                ArrayList::Bar,
                ArrayList::LowerCase,
                ArrayList::InverseColorWave,
                ArrayList::InverseColorWaveDirection
            };

            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::MultiSelectableCombo(ImVec2(200, 200), Conditions, ConditionsB, 5, "Conditions##ARRAYCW");

            ArrayList::TextShadows = ConditionsB[0];
            ArrayList::Bar = ConditionsB[1];
            ArrayList::LowerCase = ConditionsB[2];
            ArrayList::InverseColorWave = ConditionsB[3];
            ArrayList::InverseColorWaveDirection = ConditionsB[4];
        }
        
    }
    Widgets::EndChild();
}

void UtilityGroup()
{
    ImGui::SetCursorPos(ImVec2(8.F, 80.F));
    Widgets::BeginChild("Bridge Assist", false, nullptr);
    {
        const auto Module = ModuleManager::GetModuleByName("BridgeAssist");

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(0.F, 5.F));
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::BindCheckbox("Toggled", Module->GetTogglePtr(), Module->GetBindPtr());
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderInt("Delay", &BridgeAssist::Delay, 20, 250, "%d ms");

        std::vector<std::tuple<const char*, const char*>> Conditions =
        {
            {"Rmb Only", "Rmb Only"},
            {"Check Pitch", "Check Pitch"},
            {"Blocks Only", "Blocks Only"},
            {"Moving Backward Only", "Moving Backward Only"},
        };

        static bool ConditionsB[4]
        {
            BridgeAssist::RmbOnly,
            BridgeAssist::CheckPitch,
            BridgeAssist::BlocksOnly,
            BridgeAssist::BackwardOnly,
        };

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::MultiSelectableCombo(ImVec2(200, 200), Conditions, ConditionsB, 4, "Conditions##BRIDGEASSIST");

        BridgeAssist::RmbOnly = ConditionsB[0];
        BridgeAssist::CheckPitch = ConditionsB[1];
        BridgeAssist::BlocksOnly = ConditionsB[2];
        BridgeAssist::BackwardOnly = ConditionsB[3];
    }
    Widgets::EndChild();

    ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(8.F, 7.F));

    Widgets::BeginChild("Right Clicker", false, nullptr);
    {
        const auto Module = ModuleManager::GetModuleByName("RightClicker");

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(0.F, 5.F));
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::BindCheckbox("Toggled", Module->GetTogglePtr(), Module->GetBindPtr());
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderFloat("Target CPS", &RightClicker::TargetCPS, 5.F, 30.F, "%.2f CPS");

        std::vector<std::tuple<const char*, const char*>> Conditions =
        {
            {"Rmb Only", "Rmb Only"},
            {"Blocks Only", "Blocks Only"},
        };

        static bool ConditionsB[2]
        {
            RightClicker::OnRmb,
            RightClicker::BlocksOnly,
        };

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::MultiSelectableCombo(ImVec2(200, 200), Conditions, ConditionsB, 2, "Conditions##RIGHTCLICKER");

        RightClicker::OnRmb = ConditionsB[0];
        RightClicker::BlocksOnly = ConditionsB[1];
    }
    Widgets::EndChild();

    ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(8.F, 7.F));

    Widgets::BeginChild("Auto Tool", false, nullptr);
    {
        const auto Module = ModuleManager::GetModuleByName("AutoTool");

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(0.F, 5.F));
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::BindCheckbox("Toggled", Module->GetTogglePtr(), Module->GetBindPtr());
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderInt("Switch Delay", &AutoTool::SwitchDelay, 10, 150, "%.d ms");
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderInt("Start Delay", &AutoTool::StartDelay, 10, 2000, "%.d ms");

        std::vector<std::tuple<const char*, const char*>> Conditions =
        {
            {"Sneaking Only", "Sneaking Only"},
        };

        static bool ConditionsB[1]
        {
            AutoTool::SneakingOnly,
        };

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::MultiSelectableCombo(ImVec2(200, 200), Conditions, ConditionsB, 1, "Conditions##AUTOTOOL");

        AutoTool::SneakingOnly = ConditionsB[0];
    }
    Widgets::EndChild();

    ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(8.F, 7.F));
    Widgets::BeginChild("Throw Pot", false, nullptr);
    {
        const auto Module = ModuleManager::GetModuleByName("ThrowPot");

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(0.F, 5.F));
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::BindCheckbox("Toggled (?)", Module->GetTogglePtr(), &ThrowPot::ThrowBind);
        Widgets::ToolTip("ThowPot", "A bind is required for this module to function!\nto throw pots, hold the keybind.", ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary));
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderInt("Throw Delay", &ThrowPot::ThrowDelay, 10, 150, "%.d ms");
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderInt("Switch Delay", &ThrowPot::SwitchDelay, 10, 150, "%.d ms");
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderInt("Target Health Percentage", &ThrowPot::TargetHealth, 1, 100, "%.d percent");
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        const char* ThrowMode[] = { "Dynamic", "Single", "Double" };
        Widgets::Combo("Throw Mode", &ThrowPot::ThrowMode, ThrowMode, IM_ARRAYSIZE(ThrowMode));

        std::vector<std::tuple<const char*, const char*>> Conditions =
        {
         {"Random Slot", "Random Slot"},
        };

        static bool ConditionsB[1]
        {
            ThrowPot::RandomSlots,
        };

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::MultiSelectableCombo(ImVec2(200, 200), Conditions, ConditionsB, 1, "Conditions##THROWPOT");

        ThrowPot::RandomSlots = ConditionsB[0];
    }
    Widgets::EndChild();

    ImGui::Dummy(ImVec2(534.F, 22.F));

    ImGui::SetCursorPos(ImVec2(271.F, 80.F));
  /*  Widgets::BeginChild("Frame Spoofer", false, nullptr);
    {
        const auto Module = ModuleManager::GetModuleByName("FrameSpoofer");

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(0.F, 5.F));
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::BindCheckbox("Toggled", Module->GetTogglePtr(), Module->GetBindPtr());
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderFloat("Multiplier", &FrameSpoofer::Multiplier, 1.F, 10.F, "%.1f percent");
    }
    Widgets::EndChild();

    ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(271.F, 7.F));*/

    Widgets::BeginChild("Fast Place", false, nullptr);
    {
        const auto Module = ModuleManager::GetModuleByName("FastPlace");

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(0.F, 5.F));
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::BindCheckbox("Toggled", Module->GetTogglePtr(), Module->GetBindPtr());
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderInt("Delay", &FastPlace::TickDelay, 0, 3, "%d ticks");

        std::vector<std::tuple<const char*, const char*>> Conditions =
        {
            {"Randomize Delay", "Randomize Delay"},
            {"Blocks Only", "Blocks Only"},
        };

        /* inline static bool CheckNoMove = true;
         inline static bool CheckTicksExisted = true;
         inline static bool CheckTicksVisible = true;
         inline static bool CheckName = true;
         inline static bool CheckID = true;*/

        static bool ConditionsB[3]
        {
            FastPlace::Randomize,
            FastPlace::BlocksOnly,
        };

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::MultiSelectableCombo(ImVec2(200, 200), Conditions, ConditionsB, 2, "Conditions##FASTPLACE");

        FastPlace::Randomize = ConditionsB[0];
        FastPlace::BlocksOnly = ConditionsB[1];
    }
    Widgets::EndChild();

    ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(271.F, 7.F));

    Widgets::BeginChild("No Jump Delay", false, nullptr);
    {
        const auto Module = ModuleManager::GetModuleByName("NoJumpDelay");

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(0.F, 5.F));
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::BindCheckbox("Toggled", Module->GetTogglePtr(), Module->GetBindPtr());
    }
    Widgets::EndChild();

    ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(271.F, 7.F));

    Widgets::BeginChild("Refill", false, nullptr);
    {
        const auto Module = ModuleManager::GetModuleByName("Refill");

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(0.F, 5.F));
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::BindCheckbox(/*Refill::RefillMode == Refill::MODE::AUTOMATICALLY_OPEN ? "Toggled (?)" : */"Toggled", Module->GetTogglePtr(), Module->GetBindPtr());

        /* if (Refill::RefillMode == Refill::MODE::AUTOMATICALLY_OPEN)
         {
             Widgets::ToolTip("Refill", "Refill in automatic mode can only be\ntoggled by utilizing the bind.", ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary));
         }*/

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderInt("Start Delay", &Refill::StartDelay, 50, 500, "%.d ms");
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::SliderInt("Refill Delay", &Refill::RefillDelay, 10, 250, "%.d ms");
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        const char* RefillMode[] = { "On Open Inventory"/*, "Automatically Open Inventory"*/ };
        Widgets::Combo("Refill Mode", &Refill::RefillMode, RefillMode, IM_ARRAYSIZE(RefillMode));

        /* if (Refill::RefillMode == Refill::MODE::AUTOMATICALLY_OPEN)
         {
             Module->Disable();

             std::vector<std::tuple<const char*, const char*>> ConditionsR =
             {
                 {"Close On Refill", "Close On Refill"},
                 {"Random Slots", "Random Slots"},
             };

             static bool ConditionsF[2]
             {
                 Refill::CloseOnSteal,
                 Refill::RandomSlots,
             };

             ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
             Widgets::MultiSelectableCombo(ImVec2(200, 200), ConditionsR, ConditionsF, 2, "Conditions##REFILLAUTO");

             Refill::CloseOnSteal = ConditionsF[0];
             Refill::RandomSlots = ConditionsF[1];
         }
         else*/
        {
            std::vector<std::tuple<const char*, const char*>> ConditionsR =
            {
                {"Close On Refill", "Close On Refill"},
                {"Disable On Refill", "Disable On Refill"},
                {"Random Slots", "Random Slots"},
            };


            static bool ConditionsF[3]
            {
                Refill::CloseOnSteal,
                Refill::DisableOnSteal,
                Refill::RandomSlots,
            };

            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::MultiSelectableCombo(ImVec2(200, 200), ConditionsR, ConditionsF, 3, "Conditions##REFILL");

            Refill::CloseOnSteal = ConditionsF[0];
            Refill::DisableOnSteal = ConditionsF[1];
            Refill::RandomSlots = ConditionsF[2];
        }
    }
    Widgets::EndChild();
}

#include <src/cheat/hook/detours/wglswapbuffers.hpp>
#include <sdk/internal/register/envregister.hpp>

void ClientGroup()
{
    ImGui::SetCursorPos(ImVec2(8.F, 80.F));
    Widgets::BeginChild("Anti Bot", false, nullptr);
    {
        const auto Module = ModuleManager::GetModuleByName("AntiBot");

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(0.F, 5.F));
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::BindCheckbox("Toggled", Module->GetTogglePtr(), Module->GetBindPtr());
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        const char* AntiBotMode[] = { "Custom", "Hypixel" };
        Widgets::Combo("Mode", &AntiBot::Mode, AntiBotMode, IM_ARRAYSIZE(AntiBotMode));

        std::vector<std::tuple<const char*, const char*>> Conditions =
        {
            {"Check Ticks Existed", "Check Ticks Existed"},
            {"Check Name", "Check Name"},
            {"Check ID", "Check ID"},
            {"Check Bedwars Shops", "Check Bedwars Shops"},
        };

       /* inline static bool CheckNoMove = true;
        inline static bool CheckTicksExisted = true;
        inline static bool CheckTicksVisible = true;
        inline static bool CheckName = true;
        inline static bool CheckID = true;*/

        static bool ConditionsB[4]
        {
            AntiBot::CheckTicksExisted,
            AntiBot::CheckName,
            AntiBot::CheckID,
            AntiBot::CheckBedwarsShops
        };
        
        if (AntiBot::Mode == AntiBot::MODE::CUSTOM)
        {
            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            Widgets::MultiSelectableCombo(ImVec2(200, 200), Conditions, ConditionsB, 4, "Conditions##ANTIBOT");
        }

        AntiBot::CheckTicksExisted = ConditionsB[0];
        AntiBot::CheckName = ConditionsB[1];
        AntiBot::CheckID = ConditionsB[2];
        AntiBot::CheckBedwarsShops = ConditionsB[3];
    }
    Widgets::EndChild();

    ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(8.F, 7.F));

    Widgets::BeginChild("Teams", false, nullptr);
    {
        const auto Module = ModuleManager::GetModuleByName("Teams");

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(0.F, 5.F));
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::BindCheckbox("Toggled", Module->GetTogglePtr(), Module->GetBindPtr());
       /* ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        const char* AntiBotMode[] = { "Custom", "Hypixel" };
        Widgets::Combo("Mode", &AntiBot::Mode, AntiBotMode, IM_ARRAYSIZE(AntiBotMode));*/
    }
    Widgets::EndChild();

    ImGui::SetCursorPos(ImVec2(271.F, 80.F));

    Widgets::BeginChild("Friends", false, nullptr);
    {  
        const auto Module = ModuleManager::GetModuleByName("Friends");

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(0.F, 5.F));
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::BindCheckbox("Toggled", Module->GetTogglePtr(), Module->GetBindPtr());
    }
    Widgets::EndChild();


    ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(271.F, 7.F));

    Widgets::BeginChild("Destruct", false, nullptr);
    {
        static bool Destruct = false;
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(0.F, 5.F));
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::Checkbox("Toggled", &Destruct);

        INSTANCE_DESTRUCT = Destruct;
    }
    Widgets::EndChild();

    ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(271.F, 7.F));

    Widgets::BeginChild("Client", false, nullptr);
    {
        static int Bind = VK_INSERT;
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(0.F, 5.F));
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::BindCheckbox("ClickGUI", &Global::UI_OPEN, &Bind);

        if (Bind == 0)
        {
            WglSwapBuffersHook::UI_BIND = VK_INSERT;
        }
        else
        {
            WglSwapBuffersHook::UI_BIND = Bind;
        }


        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::Checkbox("Notifications", &NotificationHandler::Toggled);
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::Checkbox("Play Notification Sound", &NotificationHandler::PlayNotificationSound);

        if (NotificationHandler::PlayNotificationSound)
        {
            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
            const char* NotificationSounds[] = { "Sigma 5.0", "Samsung", "China" };
            Widgets::Combo("Notification Sounds", &AntiBot::Mode, NotificationSounds, IM_ARRAYSIZE(NotificationSounds));
        }
    }
    Widgets::EndChild();

    ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(271.F, 7.F));
    Widgets::BeginChild("Spotify Currently Playing", false, nullptr);
    {  
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(0.F, 5.F));
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));

        Widgets::Checkbox("Toggled", &SpotifyHandler::Toggled);
    }
    Widgets::EndChild();

    ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(271.F, 7.F));
    Widgets::BeginChild("ZeroHud", false, nullptr);
    {
        const auto Module = ModuleManager::GetModuleByName("ZeroHud");

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(0.F, 5.F));
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));
        Widgets::BindCheckbox("Toggled", Module->GetTogglePtr(), Module->GetBindPtr());
    }
    Widgets::EndChild();

    ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(271.F, 7.F));
    Widgets::BeginChild("AltManager", false, nullptr);
    {
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(0.F, 5.F));
        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(1.F, 0));

        static bool AltLogin = false;
        Widgets::Checkbox("Login", &AltManager::UseCrack);
    }
    Widgets::EndChild();
}

void UserInterface::Render()
{
    static float FadeOffset = 1.F;

    const auto IO = ImGui::GetIO();


   // ImGui::ShowDemoWindow();
    ImGui::SetNextWindowSize(ImVec2(534.F, 571.F));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ColorScheme.WindowBackground.GetColorU32());
    ImGui::PushStyleColor(ImGuiCol_Border, ColorScheme.WindowBorder.GetColorU32());
    ImGui::PushStyleColor(ImGuiCol_Text, ColorScheme.TextDisabled.GetColorU32());
    ImGui::PushStyleColor(ImGuiCol_NavHighlight, Color(0, 0, 0, 0).GetColorU32());

    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.F);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.F);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.F, 0.F));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.F, 1.F));
    ImGui::PushStyleVar(ImGuiStyleVar_ScrollSmooth, 15.F);

    ImGui::Begin("Main", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar/*| *//*ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar*/);
    {
        #pragma region Main Decorations
        const auto WindowPos = ImGui::GetWindowPos();
        const auto WindowBounds = ImRect(WindowPos, WindowPos + ImVec2(534.F, 571.F));
        const auto WindowDrawlist = ImGui::GetWindowDrawList();
        const auto TitleBoundingBox = ImRect(WindowBounds.Min, ImVec2(WindowBounds.Max.x, WindowBounds.Min.y + 27.F));

        ImGui::PushFont(IO.Fonts->Fonts[1]);

        const auto TitleSize = ImGui::CalcTextSize("raid0");
        WindowDrawlist->AddText(TitleBoundingBox.GetCenter() - (TitleSize / 2.F), ColorScheme.TextActive, "raid0");

        ImGui::PopFont();

        WindowDrawlist->AddRectFilled(ImVec2(WindowBounds.Min.x, WindowBounds.Max.y - 15.F), WindowBounds.Max, ColorScheme.WindowBackground, 5.F, ImDrawFlags_RoundCornersBottom);
        //WindowDrawlist->AddRect(ImVec2(WindowBounds.Min.x, WindowBounds.Max.y - 15.F), ImVec2(WindowBounds.Min.x, WindowBounds.Max.y - 14.F), ColorScheme.Border);
        WindowDrawlist->AddLine(ImVec2(WindowBounds.Min.x, WindowBounds.Max.y - 15.F), ImVec2(WindowBounds.Max.x, WindowBounds.Max.y - 15.F), ColorScheme.Border);
        WindowDrawlist->AddRect(WindowBounds.Min, WindowBounds.Max, ColorScheme.WindowBorder, 5.F, 0, 1.F);
        #pragma endregion

        static bool Combat, Player, Visuals, Utility, Client;
        if (Widgets::Navbar(&Combat, &Player, &Visuals, &Utility, &Client))
        {
            ImGui::SetScrollY(-500.F);
            FadeOffset = 1.F;
        }

        ImGui::PushClipRect(WindowPos + ImVec2(0.F, 27.F + 40.F), ImVec2(WindowBounds.Max.x, WindowBounds.Max.y - 15.F), false);

        if (Combat) {
            CombatGroup();
        }

        if (Player) {
            PlayerGroup();
        }

        if (Visuals) {
            VisualsGroup();
        }

        if (Utility) {
            UtilityGroup();
        }

        if (Client) {
            ClientGroup();
        }

        ImGui::Dummy(ImVec2(534.F, 22.F));
        ImGui::PopClipRect();

        FadeOffset = ImLerp(FadeOffset, 0.F, IO.DeltaTime * Parameters::AnimationSpeed);
        ImGui::GetForegroundDrawList()->AddRectFilled(WindowPos + ImVec2(0.F, 27.F + 40.F), ImVec2(WindowBounds.Max.x, WindowBounds.Max.y - 15.F), Color(15.F, 15.F, 15.F, 255.F * FadeOffset));
    }
    ImGui::End();

    ImGui::PopStyleColor(4);
    ImGui::PopStyleVar(5);
}


//void UserInterface::Render()
//{
//    UI::Render(); 
//    /*ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(0.f, 0.F), ImVec2(ImGui::GetIO().DisplaySize), ColorU(150.F, 150.F, 150.F, 100.F).GetColorU32());
//
//    ImGui::ShowDemoWindow();
//
//    ImGui::Begin("Aim Assist Main", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
//    ImGui::BeginChild("Aim Assist", ImVec2(293.F, 309.F));
//    {
//        ImGui::Checkbox("Toggled", &AimAssist::Toggled);
//
//        const char* TargetingProfiles[] = { "Switch", "Single" };
//        ImGui::Combo("Targeting Mode", &AimAssist::TargetingMode, TargetingProfiles, IM_ARRAYSIZE(TargetingProfiles));
//
//        const char* AimProfiles[] = { "MultiPoint", "Static" };
//        ImGui::Combo("Aim Mode", &AimAssist::AimMode, AimProfiles, IM_ARRAYSIZE(AimProfiles));
//
//        const char* FilteringProfiles[] = { "Closest", "Lowest Health" };
//        ImGui::Combo("Target Filter Mode", &AimAssist::FilteringMode, FilteringProfiles, IM_ARRAYSIZE(FilteringProfiles));
//
//        const char* AimDirections[] = { "Vertical", "Horizontal", "Both" };
//        ImGui::Combo("Aim Direction", &AimAssist::AimDirection, AimDirections, IM_ARRAYSIZE(AimDirections));
//
//        ImGui::SliderFloat("Vertical", &AimAssist::VerticalSpeed, 1.F, 100.F);
//        ImGui::SliderFloat("Horizontal", &AimAssist::HorizontalSpeed, 1.F, 100.F);
//
//        ImGui::SliderFloat("Fov Min", &AimAssist::MinimumFov, 1.F, 180.F);
//        if (AimAssist::MinimumFov > AimAssist::MaximumFov) { AimAssist::MinimumFov = AimAssist::MaximumFov; }
//
//        ImGui::SliderFloat("Fov Max", &AimAssist::MaximumFov, 1.F, 180.F);
//        if (AimAssist::MaximumFov < AimAssist::MinimumFov) { AimAssist::MaximumFov = AimAssist::MinimumFov; }
//
//        ImGui::SliderFloat("Range Min", &AimAssist::MinimumRange, 0.F, 6.F);
//        if (AimAssist::MinimumRange > AimAssist::MaximumRange) { AimAssist::MinimumRange = AimAssist::MaximumRange; }
//
//        ImGui::SliderFloat("Range Max", &AimAssist::MaximumRange, 0.F, 6.F);
//        if (AimAssist::MaximumRange < AimAssist::MinimumRange) { AimAssist::MaximumRange = AimAssist::MinimumRange; }
//
//        if (AimAssist::AimMode == 1) {
//            ImGui::SliderFloat("Multipoint", &AimAssist::MultiPointOffset, 0.F, 1.F);
//        }
//
//        std::vector<std::tuple<const char*, const char*>> WhiteListItems =
//        {
//         {"Swords", "Swords"},
//         {"Axes", "Axes"},
//         {"Fists", "Fists"},
//         {"Sticks", "Sticks"},
//         {"PickAxes", "PickAxes"},
//        };
//
//    	bool WhiteList[5]
//        {
//            AimAssist::WhitelistSwords,
//            AimAssist::WhitelistAxes,
//            AimAssist::WhitelistFist,
//            AimAssist::WhitelistSticks,
//            AimAssist::WhitelistPickAxe
//        };
//
//        if (AimAssist::ItemWhitelist) {
//            Elements::MultiSelectableCombo(ImVec2(200, 200), WhiteListItems, WhiteList, 5, "#AA Whitelisted Items");
//        }
//
//        AimAssist::WhitelistSwords = WhiteList[0];
//        AimAssist::WhitelistAxes = WhiteList[1];
//        AimAssist::WhitelistFist = WhiteList[2];
//        AimAssist::WhitelistSticks = WhiteList[3];
//        AimAssist::WhitelistPickAxe = WhiteList[4];
//
//        std::vector<std::tuple<const char*, const char*>> Conditions =
//        {
//         {"Only Clicking", "Only Clicking"},
//         {"Through Walls", "Through Walls"},
//         {"Aim At Invisible", "Aim At Invisible"},
//         {"Break Blocks", "Break Blocks"},
//         {"Free Y Axis In Hitbox", "Free Y Axis In Hitbox"},
//         {"Whitelist Items", "Whitelist Items"},
//        };
//
//        bool ConditionsB[6]
//        {
//            AimAssist::OnlyClicking,
//            AimAssist::AimThroughWalls,
//            AimAssist::AimAtInvisible,
//            AimAssist::BreakBlocks,
//            AimAssist::FreeYAxisInHitbox,
//            AimAssist::ItemWhitelist
//        };
//
//        Elements::MultiSelectableCombo(ImVec2(200, 200), Conditions, ConditionsB, 6, "#AA Conditions");
//
//        AimAssist::OnlyClicking = ConditionsB[0];
//        AimAssist::AimThroughWalls = ConditionsB[1];
//        AimAssist::AimAtInvisible = ConditionsB[2];
//        AimAssist::BreakBlocks = ConditionsB[3];
//        AimAssist::FreeYAxisInHitbox = ConditionsB[4];
//        AimAssist::ItemWhitelist = ConditionsB[5];
//    }
//    ImGui::EndChild();
//    ImGui::End();
//
//    static std::vector<float> ClickVectorA;
//
//    ImGui::Begin("Auto Clicker Main", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
//    ImGui::BeginChild("Auto Clicker", ImVec2(293.F, 309.F));
//    {
//    	ImGui::Checkbox("Toggled", &AutoClicker::Toggled);
//
//        ImGui::SliderFloat("CPS", &AutoClicker::TargetCPS, 5.F, 30.F);
//
//        std::vector<std::tuple<const char*, const char*>> WhiteListItems =
//        {
//         {"Swords", "Swords"},
//         {"Axes", "Axes"},
//         {"Fists", "Fists"},
//         {"Sticks", "Sticks"},
//         {"PickAxes", "PickAxes"},
//        };
//
//        bool WhiteList[5]
//        {
//            AutoClicker::WhitelistSwords,
//            AutoClicker::WhitelistAxes,
//            AutoClicker::WhitelistFist,
//            AutoClicker::WhitelistSticks,
//            AutoClicker::WhitelistPickAxe
//        };
//
//        ImGui::Checkbox("BlockHit", &AutoClicker::BlockHit);
//        if (AutoClicker::BlockHit)
//        {
//            ImGui::Checkbox("BlockHit In Combo", &AutoClicker::BlockHitInCombo);
//            if (AutoClicker::BlockHitInCombo) {
//                ImGui::SliderInt("BlockHit Combo Count", &AutoClicker::BlockHitComboCount, 1.F, 10.F);
//            }
//
//            ImGui::SliderFloat("BlockHit Distance (Min)", &AutoClicker::BlockHitDistanceMin, 2.F, 3.F);
//            if (AutoClicker::BlockHitDistanceMin > AutoClicker::BlockHitDistanceMax) { AutoClicker::BlockHitDistanceMin = AutoClicker::BlockHitDistanceMax; }
//            
//            ImGui::SliderFloat("BlockHit Distance (Max)", &AutoClicker::BlockHitDistanceMax, 2.F, 3.F);
//        	if (AutoClicker::BlockHitDistanceMax < AutoClicker::BlockHitDistanceMin) { AutoClicker::BlockHitDistanceMax = AutoClicker::BlockHitDistanceMin; }
//
//            ImGui::SliderFloat("BlockHit Duration (Min)", &AutoClicker::BlockHitDelayMin, 10.F, 500.F);
//            if (AutoClicker::BlockHitDelayMin > AutoClicker::BlockHitDelayMax) { AutoClicker::BlockHitDelayMin = AutoClicker::BlockHitDelayMax; }
//
//            ImGui::SliderFloat("BlockHit Duration (Max)", &AutoClicker::BlockHitDelayMax, 10.F, 500.F);
//            if (AutoClicker::BlockHitDelayMax < AutoClicker::BlockHitDelayMin) { AutoClicker::BlockHitDelayMax = AutoClicker::BlockHitDelayMin; }
//        }
//
//        ImGui::Checkbox("HitSelect", &AutoClicker::HitSelect);
//
//        if (AutoClicker::HitSelect)
//        {
//            const char* HitSelectModes[] = { "Burst", "Single" };
//            ImGui::Combo("HitSelect Mode", &AutoClicker::HitSelectMode, HitSelectModes, IM_ARRAYSIZE(HitSelectModes));
//
//            if (AutoClicker::HitSelectMode == 0)
//            {
//                ImGui::SliderInt("Burst Duration", &AutoClicker::BurstDuration, 1.F, 7.F);
//            }
//            ImGui::SliderInt("HitSelect Window", &AutoClicker::HitSelectWindow, 0, 7);
//        }
//
//        std::vector<std::tuple<const char*, const char*>> Conditions =
//        {
//         {"Only Clicking", "Only Clicking"},
//         {"Allow Inventory", "Allow Inventory"},
//         {"No Hit Delay", "No Hit Delay"},
//         {"Break Blocks", "Break Blocks"},
//         {"Prevent Unrefill", "Prevent Unrefill"},
//         {"Favor Criticals", "Favor Criticals"},
//         {"WhiteList", "WhiteList"},
//        };
//
//        bool ConditionsB[7]
//        {
//            AutoClicker::OnlyClicking,
//            AutoClicker::AllowInventory,
//            AutoClicker::NoHitDelay,
//            AutoClicker::BreakBlocks,
//            AutoClicker::PreventUnrefill,
//            AutoClicker::FavorCriticals,
//            AutoClicker::ItemWhitelist
//        };
//
//        Elements::MultiSelectableCombo(ImVec2(200, 200), Conditions, ConditionsB, 7, "Conditions");
//
//        AutoClicker::OnlyClicking = ConditionsB[0];
//        AutoClicker::AllowInventory = ConditionsB[1];
//        AutoClicker::NoHitDelay = ConditionsB[2];
//        AutoClicker::BreakBlocks = ConditionsB[3];
//        AutoClicker::PreventUnrefill = ConditionsB[4];
//        AutoClicker::FavorCriticals = ConditionsB[5];
//        AutoClicker::ItemWhitelist = ConditionsB[6];
//
//        if (AutoClicker::ItemWhitelist) {
//            Elements::MultiSelectableCombo(ImVec2(200, 200), WhiteListItems, WhiteList, 5, "Whitelisted Items");
//        }
//
//        AutoClicker::WhitelistSwords = WhiteList[0];
//        AutoClicker::WhitelistAxes = WhiteList[1];
//        AutoClicker::WhitelistFist = WhiteList[2];
//        AutoClicker::WhitelistSticks = WhiteList[3];
//        AutoClicker::WhitelistPickAxe = WhiteList[4];
//    }
//    ImGui::EndChild();
//    ImGui::End();
//
//    ImGui::Begin("Render Main", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
//    ImGui::BeginChild("Render", ImVec2(293.F, 509.F));
//    {
//        ImGui::Checkbox("Radar Enabled", &Radar::Toggled);
//        ImGui::Checkbox("ESP Enabled", &PlayerESP::Toggled);
//        ImGui::Checkbox("NameTags Enabled", &NameTags::Toggled);
//
//        ImGui::Checkbox("Uniform Scaling", &Render::UniformScaling);
//        ImGui::SliderFloat("Fade Distance", &Render::FadeDistance, 1.F, 10.F);
//        ImGui::SliderFloat("Render Distance", &Render::RenderDistance, 1.F, 100.F);
//
//        if (NameTags::Toggled)
//        {
//	        
//        }
//
//        if (Radar::Toggled)
//        {
//            ImGui::SliderFloat("Radar Zoom", &Radar::RadarZoom, 1.F, 10.F);
//
//            ImGui::ColorEdit4("Blip Color", reinterpret_cast<float*>(&Radar::BlipColor), ImGuiColorEditFlags_Float);
//        }
//
//        if (PlayerESP::Toggled)
//        {
//            const char* PlayerESPRenderModes[] = { "Render 2D", "Render 3D" };
//            ImGui::Combo("PlayerESP Mode", &PlayerESP::RenderMode, PlayerESPRenderModes, IM_ARRAYSIZE(PlayerESPRenderModes));
//
//            if (PlayerESP::RenderMode == PlayerESP::RENDER_2D)
//            {
//                const char* RenderStyles[] = { "Filled", "Corners" };
//                ImGui::Combo("Render Style", &PlayerESP::RenderStyle, RenderStyles, IM_ARRAYSIZE(RenderStyles));
//            }
//
//            ImGui::Checkbox("HealthBar", &PlayerESP::DrawHealthBar);
//            ImGui::Checkbox("Outlines", &PlayerESP::DrawOutlines);
//            ImGui::Checkbox("Draw ESP Outlines", &PlayerESP::DrawESPOutlines);
//            ImGui::Checkbox("Draw ESP Background", &PlayerESP::DrawESPBackground);
//            ImGui::Checkbox("Draw ESP Shadows", &PlayerESP::DrawESPShadow);
//
//            ImGui::ColorEdit4("Outline Color", reinterpret_cast<float*>(&PlayerESP::EspOutlineColor), ImGuiColorEditFlags_Float);
//            ImGui::ColorEdit4("Background Color", reinterpret_cast<float*>(&PlayerESP::EspBackgroundColor), ImGuiColorEditFlags_Float);
//            ImGui::ColorEdit4("Healthbar Background Color", reinterpret_cast<float*>(&PlayerESP::HealthBarBackgroundColor), ImGuiColorEditFlags_Float);
//            ImGui::ColorEdit4("Healthbar Shaded Color", reinterpret_cast<float*>(&PlayerESP::HealthBarShadedColor), ImGuiColorEditFlags_Float);
//        }
//    }
//    ImGui::EndChild();
//    ImGui::End();
//
//    ImGui::Begin("Exclusions Main", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
//    ImGui::BeginChild("Exclusions", ImVec2(293.F, 309.F));
//    {
//
//        ImGui::Checkbox("AntiBot", &AntiBot::Enabled);
//        ImGui::Checkbox("Teams", &Teams::Enabled);
//
//    }
//    ImGui::EndChild();
//    ImGui::End();
//
//    ImGui::Begin("RightClicker Main", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
//    ImGui::BeginChild("RightClicker", ImVec2(293.F, 309.F));
//    {
//        ImGui::Checkbox("Toggled", &RightClicker::Toggled);
//        ImGui::Checkbox("Rmb Only", &RightClicker::OnRmb);
//        ImGui::Checkbox("Blocks Only", &RightClicker::BlocksOnly);
//
//        ImGui::SliderFloat("Target CPS", &RightClicker::TargetCPS, 5.F, 30.F);
//    }
//    ImGui::EndChild();
//    ImGui::End();
//
//    ImGui::Begin("BridgeAssist Main", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
//    ImGui::BeginChild("BridgeAssist", ImVec2(293.F, 309.F));
//    {
//        ImGui::Checkbox("Toggled", &BridgeAssist::Toggled);
//        ImGui::Checkbox("Rmb Only", &BridgeAssist::RmbOnly);
//        ImGui::Checkbox("Check Pitch", &BridgeAssist::CheckPitch);
//        ImGui::Checkbox("Blocks Only", &BridgeAssist::BlocksOnly);
//
//        ImGui::SliderInt("Delay", &BridgeAssist::Delay, 10, 100);
//    }
//    ImGui::EndChild();
//    ImGui::End();
//
//    ImGui::Begin("BackTrack Main", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
//    ImGui::BeginChild("BackTrack", ImVec2(293.F, 309.F));
//    {
//        ImGui::Checkbox("Toggled", &BackTrack::Toggled);
//        ImGui::SliderInt("Delay BackTrack", &BackTrack::Delay, 0, 5000);
//    }
//    ImGui::EndChild();
//    ImGui::End();
//
//    ImGui::Begin("Velocity Main", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
//    ImGui::BeginChild("Velocity", ImVec2(293.F, 309.F));
//    {
//        ImGui::Checkbox("Toggled", &Velocity::Toggled);
//
//        const char* VelocityModes[] = { "Jump", "Normal", "Packet" };
//        ImGui::Combo("Velocity Mode", &Velocity::VelocityMode, VelocityModes, IM_ARRAYSIZE(VelocityModes));
//
//        if (Velocity::VelocityMode != Velocity::JUMP)
//        {
//            ImGui::SliderFloat("Vertical", &Velocity::VelocityVertical, 0.F, 100.F);
//            ImGui::SliderFloat("Horizontal", &Velocity::VelocityHorizontal, 0.F, 100.F);
//            ImGui::Checkbox("Check Fire", &Velocity::CheckOnFire);
//        }
//
//        ImGui::Checkbox("Check Ground", &Velocity::CheckOnGround);
//        ImGui::Checkbox("Check Liquid", &Velocity::LiquidCheck);
//
//        if (Velocity::VelocityMode == Velocity::NORMAL)
//        {
//            ImGui::Checkbox("Use Delay", &Velocity::UseDelay);
//
//            if (Velocity::UseDelay) {
//                ImGui::SliderInt("Delay", &Velocity::Delay, 0, 19);
//            }
//        }
//
//        if (Velocity::VelocityMode == Velocity::JUMP)
//        {
//            ImGui::SliderInt("Hurt Time", &Velocity::HurtTimeWindow, 1, 10);
//        }
//    }
//    ImGui::EndChild();
//    ImGui::End();*/
//}
//
////getNameTagVisibility
