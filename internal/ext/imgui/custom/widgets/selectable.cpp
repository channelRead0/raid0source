#include <map>

#include "../custom.hpp"
#include <vector>

using namespace Schemes;
using namespace ImGui;

const char* Items_SingleStringGetter(void* data, int idx)
{
    const char* items_separated_by_zeros = (const char*)data;
    int items_count = 0;
    const char* p = items_separated_by_zeros;
    while (*p)
    {
        if (idx == items_count)
            break;
        p += strlen(p) + 1;
        items_count++;
    }
    return *p ? p : NULL;
}

const char* Items_ArrayGetter(void* data, int idx)
{
    const char* const* items = (const char* const*)data;
    return items[idx];
}

bool Widgets::Combo(const char* label, int* current_item, const char* items_separated_by_zeros, int height_in_items)
{
    int items_count = 0;
    const char* p = items_separated_by_zeros;       // FIXME-OPT: Avoid computing this, or at least only when combo is open
    while (*p)
    {
        p += strlen(p) + 1;
        items_count++;
    }
    bool value_changed = Combo(label, current_item, Items_SingleStringGetter, (void*)items_separated_by_zeros, items_count, height_in_items);
    return value_changed;
}

bool Widgets::Combo(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items)
{
    const bool value_changed = Combo(label, current_item, Items_ArrayGetter, (void*)items, items_count, height_in_items);
    return value_changed;
}

//bool Widgets::Combo(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items)
//{
//    const bool value_changed = Combo(label, current_item, Items_ArrayGetter, (void*)items, items_count, height_in_items);
//    return value_changed;
//}

float CalcMaxPopupHeightFromItemCount(int items_count)
{
    ImGuiContext& g = *GImGui;
    if (items_count <= 0)
        return FLT_MAX;
    return (g.FontSize + g.Style.ItemSpacing.y) * items_count - g.Style.ItemSpacing.y + (g.Style.WindowPadding.y * 2);
}


bool Widgets::Combo(const char* label, int* current_item, const char* (*getter)(void* user_data, int idx), void* user_data, int items_count, int popup_max_height_in_items)
{
    ImGuiContext& g = *GImGui;

    // Call the getter to obtain the preview string which is a parameter to BeginCombo()
    const char* preview_value = NULL;
    if (*current_item >= 0 && *current_item < items_count)
        preview_value = getter(user_data, *current_item);

    // The old Combo() API exposed "popup_max_height_in_items". The new more general BeginCombo() API doesn't have/need it, but we emulate it here.
    if (popup_max_height_in_items != -1 && !(g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSizeConstraint))
        SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, CalcMaxPopupHeightFromItemCount(popup_max_height_in_items)));

    if (!BeginCombo(label, preview_value, ImGuiComboFlags_None))
        return false;

    // Display items
    // FIXME-OPT: Use clipper (but we need to disable it on the appearing frame to make sure our call to SetItemDefaultFocus() is processed)
    bool value_changed = false;
    for (int i = 0; i < items_count; i++)
    {
        const char* item_text = getter(user_data, i);
        if (item_text == NULL)
            item_text = "*Unknown item*";

        PushID(i);
        const bool item_selected = (i == *current_item);
        if (Selectable(item_text, item_selected) && *current_item != i)
        {
            value_changed = true;
            *current_item = i;
        }
        if (item_selected)
            SetItemDefaultFocus();
        PopID();
    }

    EndCombo();

    if (value_changed)
        MarkItemEdited(g.LastItemData.ID);

    return value_changed;
}

inline void RotatePoint(float& x, float& y, float centerX, float centerY, float angle)
{
    float originalX = x - centerX;
    float originalY = y - centerY;
    x = centerX + originalX * std::cos(angle) - originalY * std::sin(angle);
    y = centerY + originalX * std::sin(angle) + originalY * std::cos(angle);
}

inline void DrawRotatedTriangle(ImDrawList* drawList, ImVec2 center, float width, float height, float angle, ImU32 color)
{
    ImVec2 p1 = ImVec2(center.x - width / 2, center.y - height / 2);
    ImVec2 p2 = ImVec2(center.x + width / 2, center.y - height / 2);
    ImVec2 p3 = ImVec2(center.x, center.y + height / 2);

    // Rotate each point around the center
    RotatePoint(p1.x, p1.y, center.x, center.y, angle);
    RotatePoint(p2.x, p2.y, center.x, center.y, angle);
    RotatePoint(p3.x, p3.y, center.x, center.y, angle);

    ImVec2 trianglePoints[] = { p1, p2, p3 };

    drawList->AddConvexPolyFilled(trianglePoints, 3, color);

    //drawList->AddPolyline(trianglePoints, 3, color, false, 1.0f);
}

struct ComboAnimationsG
{
    float PopupAlpha;
    float HeightOffset;
};


static std::map< ImGuiID, ComboAnimationsG > ComboAnimations;

bool Widgets::BeginCombo(const char* label, const char* preview_value, ImGuiComboFlags flags)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow();

    ImGuiNextWindowDataFlags backup_next_window_data_flags = g.NextWindowData.Flags;
    g.NextWindowData.ClearFlags(); // We behave like Begin() and need to consume those values
    if (window->SkipItems)
        return false;

    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    IM_ASSERT((flags & (ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_NoPreview)) != (ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_NoPreview)); // Can't use both flags together
    if (flags & ImGuiComboFlags_WidthFitPreview)
        IM_ASSERT((flags & (ImGuiComboFlags_NoPreview | (ImGuiComboFlags)ImGuiComboFlags_CustomPreview)) == 0);

    const float arrow_size = (flags & ImGuiComboFlags_NoArrowButton) ? 0.0f : GetFrameHeight();
    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const float preview_width = ((flags & ImGuiComboFlags_WidthFitPreview) && (preview_value != NULL)) ? CalcTextSize(preview_value, NULL, true).x : 0.0f;
    const float w = (flags & ImGuiComboFlags_NoPreview) ? arrow_size : ((flags & ImGuiComboFlags_WidthFitPreview) ? (arrow_size + preview_width + style.FramePadding.x * 2.0f) : CalcItemWidth());
    const ImRect bb(window->DC.CursorPos + ImVec2(9.F, 21.F), window->DC.CursorPos + ImVec2(245.F, 40.F));
    const ImRect total_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(253.F, 43));
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id, &bb))
        return false;

    // Open on click
    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);
    const ImGuiID popup_id = ImHashStr("##ComboPopup", 0, id);
    bool popup_open = IsPopupOpen(popup_id, ImGuiPopupFlags_None);


    auto AnimationMapG = ComboAnimations.find(popup_id);
    if (AnimationMapG == ComboAnimations.end())
    {
        ComboAnimations.insert({ popup_id, ComboAnimationsG() });
        AnimationMapG = ComboAnimations.find(popup_id);
    }

    if (pressed && !popup_open)
    {
        OpenPopupEx(popup_id, ImGuiPopupFlags_None);
        popup_open = true;
    }

    AnimationMapG->second.PopupAlpha = ImLerp(AnimationMapG->second.PopupAlpha, popup_open ? 1.F : 0.F, ImGui::GetIO().DeltaTime * Parameters::AnimationSpeed);

    if (!popup_open) {
        AnimationMapG->second.HeightOffset = 0.F;
    }

    struct ComboAnimations
    {
        float TriangleRotation;
        float YOffset;
        float PopupAlpha;
    };

    const int AnimationID = reinterpret_cast<int>(label);

    static std::map< ImGuiID, ComboAnimations > Animations;
    auto AnimationMap = Animations.find(AnimationID);
    if (AnimationMap == Animations.end())
    {
        Animations.insert({ AnimationID, ComboAnimations() });
        AnimationMap = Animations.find(AnimationID);
    }

    // Render shape
    const ImU32 frame_col = GetColorU32(hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
    const float value_x2 = ImMax(bb.Min.x, bb.Max.x - arrow_size);
    RenderNavHighlight(bb, id);
    if (!(flags & ImGuiComboFlags_NoPreview)) {
        window->DrawList->AddRectFilled(bb.Min, bb.Max, ColorScheme.WindowBackground, 1.F);
        window->DrawList->AddRect(bb.Min, bb.Max, ColorScheme.Border, 1.F);
    }
    if (!(flags & ImGuiComboFlags_NoArrowButton))
    {
        AnimationMap->second.YOffset = ImLerp(AnimationMap->second.YOffset, popup_open ? 0.F : 1.F, ImGui::GetIO().DeltaTime * 1.f * Parameters::AnimationSpeed);
        AnimationMap->second.TriangleRotation = ImLerp(AnimationMap->second.TriangleRotation, popup_open ? 180.F : 0.F, ImGui::GetIO().DeltaTime * 1.f * Parameters::AnimationSpeed);
        ImVec2 center2 = ImVec2(bb.Max.x - 4.78F - (11.34F / 2.F) - 1.F, bb.GetCenter().y + AnimationMap->second.YOffset);

        float width = 10.0f;
        float height = 7.f;
        float angleInRadians = AnimationMap->second.TriangleRotation * 3.14159265f / 180.0f;
        DrawRotatedTriangle(window->DrawList, center2, width, height, angleInRadians, ColorScheme.Accent);
       /* ImU32 bg_col = GetColorU32((popup_open || hovered) ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
        ImU32 text_col = GetColorU32(ImGuiCol_Text);
        window->DrawList->AddRectFilled(ImVec2(value_x2, bb.Min.y), bb.Max, bg_col, style.FrameRounding, (w <= arrow_size) ? ImDrawFlags_RoundCornersAll : ImDrawFlags_RoundCornersRight);
        if (value_x2 + arrow_size - style.FramePadding.x <= bb.Max.x)
            RenderArrow(window->DrawList, ImVec2(value_x2 + style.FramePadding.y, bb.Min.y + style.FramePadding.y), text_col, ImGuiDir_Down, 1.0f);*/
    }
  //  RenderFrameBorder(bb.Min, bb.Max, style.FrameRounding);

    // Custom preview
    if (flags & ImGuiComboFlags_CustomPreview)
    {
        g.ComboPreviewData.PreviewRect = ImRect(bb.Min.x, bb.Min.y, value_x2, bb.Max.y);
        IM_ASSERT(preview_value == NULL || preview_value[0] == 0);
        preview_value = NULL;
    }

    // Render preview and label
    if (preview_value != NULL && !(flags & ImGuiComboFlags_NoPreview))
    {
        if (g.LogEnabled)
            LogSetNextTextDecoration("{", "}");
        RenderText(ImVec2((bb.Min + ImVec2(5.F, 0)).x, bb.GetCenter().y - ImGui::CalcTextSize(preview_value).y / 2.F), preview_value, NULL, NULL);
    }
    if (label_size.x > 0)
        RenderText(total_bb.Min + ImVec2(9.F, 3.F), label);

   // window->DrawList->AddRect(total_bb.Min, total_bb.Max, ColorScheme.Accent);
    if (!popup_open)
        return false;

    g.NextWindowData.Flags = backup_next_window_data_flags;
    return BeginComboPopup(popup_id, bb, flags);
}


bool Widgets::BeginComboPopup(ImGuiID popup_id, const ImRect& bb, ImGuiComboFlags flags)
{
    ImGuiContext& g = *GImGui;
    if (!IsPopupOpen(popup_id, ImGuiPopupFlags_None))
    {
        g.NextWindowData.ClearFlags();
        return false;
    }

    auto AnimationMapG = ComboAnimations.find(popup_id);
    if (AnimationMapG == ComboAnimations.end())
    {
        ComboAnimations.insert({ popup_id, ComboAnimationsG() });
        AnimationMapG = ComboAnimations.find(popup_id);
    }

    // Set popup size
    float w = bb.GetWidth();
    if (g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSizeConstraint)
    {
        g.NextWindowData.SizeConstraintRect.Min.x = ImMax(g.NextWindowData.SizeConstraintRect.Min.x, w);
    }
    else
    {
        if ((flags & ImGuiComboFlags_HeightMask_) == 0)
            flags |= ImGuiComboFlags_HeightRegular;
        IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiComboFlags_HeightMask_)); // Only one
        int popup_max_height_in_items = -1;
        if (flags & ImGuiComboFlags_HeightRegular)     popup_max_height_in_items = 8;
        else if (flags & ImGuiComboFlags_HeightSmall)  popup_max_height_in_items = 4;
        else if (flags & ImGuiComboFlags_HeightLarge)  popup_max_height_in_items = 20;
        ImVec2 constraint_min(0.0f, 0.0f), constraint_max(FLT_MAX, FLT_MAX);
        if ((g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSize) == 0 || g.NextWindowData.SizeVal.x <= 0.0f) // Don't apply constraints if user specified a size
            constraint_min.x = w;
        if ((g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSize) == 0 || g.NextWindowData.SizeVal.y <= 0.0f)
            constraint_max.y = CalcMaxPopupHeightFromItemCount(popup_max_height_in_items);


        AnimationMapG->second.HeightOffset = ImLerp(AnimationMapG->second.HeightOffset, constraint_max.y, ImGui::GetIO().DeltaTime * Parameters::AnimationSpeed);
        SetNextWindowSizeConstraints(constraint_min, ImVec2(constraint_max.x, constraint_max.y/*AnimationMapG->second.HeightOffset*/));
    }

    // This is essentially a specialized version of BeginPopupEx()
    char name[16];
    ImFormatString(name, IM_ARRAYSIZE(name), "##Combo_%02d", g.BeginComboDepth); // Recycle windows based on depth

    // Set position given a custom constraint (peak into expected window size so we can position it)
    // FIXME: This might be easier to express with an hypothetical SetNextWindowPosConstraints() function?
    // FIXME: This might be moved to Begin() or at least around the same spot where Tooltips and other Popups are calling FindBestWindowPosForPopupEx()?
    if (ImGuiWindow* popup_window = FindWindowByName(name))
        if (popup_window->WasActive)
        {
            // Always override 'AutoPosLastDirection' to not leave a chance for a past value to affect us.
            ImVec2 size_expected = CalcWindowNextAutoFitSize(popup_window);
            popup_window->AutoPosLastDirection = (flags & ImGuiComboFlags_PopupAlignLeft) ? ImGuiDir_Left : ImGuiDir_Down; // Left = "Below, Toward Left", Down = "Below, Toward Right (default)"
            ImRect r_outer = GetPopupAllowedExtentRect(popup_window);
            ImVec2 pos = FindBestWindowPosForPopupEx(bb.GetBL(), size_expected, &popup_window->AutoPosLastDirection, r_outer, bb, ImGuiPopupPositionPolicy_ComboBox);
            SetNextWindowPos(pos + ImVec2(0.F, 3.F));
        }

    // We don't use BeginPopupEx() solely because we have a custom name string, which we could make an argument to BeginPopupEx()
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Popup | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar;
    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.F, 5.F)); // Horizontally align ourselves with the framed text
    PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.F);
    PushStyleVar(ImGuiStyleVar_PopupRounding, 1.F);
    PushStyleVar(ImGuiStyleVar_Alpha, AnimationMapG->second.PopupAlpha);
    PushStyleColor(ImGuiCol_Border, ColorScheme.Border.GetColorU32());
    PushStyleColor(ImGuiCol_PopupBg, ColorScheme.WindowBackground.GetColorU32());
    bool ret = Begin(name, NULL, window_flags);
    PopStyleVar(4);
    PopStyleColor(2);
    if (!ret)
    {
        EndPopup();
        IM_ASSERT(0);   // This should never happen as we tested for IsPopupOpen() above
        return false;
    }
    g.BeginComboDepth++;
    return true;
}

bool Widgets::Selectable(const char* label, bool selected, ImGuiSelectableFlags flags, const ImVec2& size_arg)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    // Submit label or explicit size to ItemSize(), whereas ItemAdd() will submit a larger/spanning rectangle.
    ImGuiID id = window->GetID(label);
    ImVec2 label_size = CalcTextSize(label, NULL, true);
    ImVec2 size(size_arg.x != 0.0f ? size_arg.x : label_size.x, size_arg.y != 0.0f ? size_arg.y : label_size.y);
    ImVec2 pos = window->DC.CursorPos;
    pos.y += window->DC.CurrLineTextBaseOffset;
    ItemSize(size, 0.0f);

    struct SelectableAnimations
    {
        ImColor TextColor;
    };

    const int AnimationID = id;

    static std::map< ImGuiID, SelectableAnimations > Animations;
    auto AnimationMap = Animations.find(AnimationID);
    if (AnimationMap == Animations.end())
    {
        Animations.insert({ AnimationID, SelectableAnimations() });
        AnimationMap = Animations.find(AnimationID);
    }

    // Fill horizontal space
    // We don't support (size < 0.0f) in Selectable() because the ItemSpacing extension would make explicitly right-aligned sizes not visibly match other widgets.
    const bool span_all_columns = (flags & ImGuiSelectableFlags_SpanAllColumns) != 0;
    const float min_x = span_all_columns ? window->ParentWorkRect.Min.x : pos.x;
    const float max_x = span_all_columns ? window->ParentWorkRect.Max.x : window->WorkRect.Max.x;
    if (size_arg.x == 0.0f || (flags & ImGuiSelectableFlags_SpanAvailWidth))
        size.x = ImMax(label_size.x, max_x - min_x);

    // Text stays at the submission position, but bounding box may be extended on both sides
    const ImVec2 text_min = pos;
    const ImVec2 text_max(min_x + size.x, pos.y + size.y);

    // Selectables are meant to be tightly packed together with no click-gap, so we extend their box to cover spacing between selectable.
    ImRect bb(min_x, pos.y, text_max.x, text_max.y);
    if ((flags & ImGuiSelectableFlags_NoPadWithHalfSpacing) == 0)
    {
        const float spacing_x = span_all_columns ? 0.0f : style.ItemSpacing.x;
        const float spacing_y = style.ItemSpacing.y;
        const float spacing_L = IM_TRUNC(spacing_x * 0.50f);
        const float spacing_U = IM_TRUNC(spacing_y * 0.50f);
        bb.Min.x -= spacing_L;
        bb.Min.y -= spacing_U;
        bb.Max.x += (spacing_x - spacing_L);
        bb.Max.y += (spacing_y - spacing_U);
    }
    //if (g.IO.KeyCtrl) { GetForegroundDrawList()->AddRect(bb.Min, bb.Max, IM_COL32(0, 255, 0, 255)); }

    // Modify ClipRect for the ItemAdd(), faster than doing a PushColumnsBackground/PushTableBackgroundChannel for every Selectable..
    const float backup_clip_rect_min_x = window->ClipRect.Min.x;
    const float backup_clip_rect_max_x = window->ClipRect.Max.x;
    if (span_all_columns)
    {
        window->ClipRect.Min.x = window->ParentWorkRect.Min.x;
        window->ClipRect.Max.x = window->ParentWorkRect.Max.x;
    }

    const bool disabled_item = (flags & ImGuiSelectableFlags_Disabled) != 0;
    const bool item_add = ItemAdd(bb, id, NULL, disabled_item ? ImGuiItemFlags_Disabled : ImGuiItemFlags_None);
    if (span_all_columns)
    {
        window->ClipRect.Min.x = backup_clip_rect_min_x;
        window->ClipRect.Max.x = backup_clip_rect_max_x;
    }

    if (!item_add)
        return false;

    const bool disabled_global = (g.CurrentItemFlags & ImGuiItemFlags_Disabled) != 0;
    if (disabled_item && !disabled_global) // Only testing this as an optimization
        BeginDisabled();

    // FIXME: We can standardize the behavior of those two, we could also keep the fast path of override ClipRect + full push on render only,
    // which would be advantageous since most selectable are not selected.
    if (span_all_columns)
    {
        if (g.CurrentTable)
            TablePushBackgroundChannel();
        else if (window->DC.CurrentColumns)
            PushColumnsBackground();
        g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HasClipRect;
        g.LastItemData.ClipRect = window->ClipRect;
    }

    // We use NoHoldingActiveID on menus so user can click and _hold_ on a menu then drag to browse child entries
    ImGuiButtonFlags button_flags = 0;
    if (flags & ImGuiSelectableFlags_NoHoldingActiveID) { button_flags |= ImGuiButtonFlags_NoHoldingActiveId; }
    if (flags & ImGuiSelectableFlags_NoSetKeyOwner) { button_flags |= ImGuiButtonFlags_NoSetKeyOwner; }
    if (flags & ImGuiSelectableFlags_SelectOnClick) { button_flags |= ImGuiButtonFlags_PressedOnClick; }
    if (flags & ImGuiSelectableFlags_SelectOnRelease) { button_flags |= ImGuiButtonFlags_PressedOnRelease; }
    if (flags & ImGuiSelectableFlags_AllowDoubleClick) { button_flags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick; }
    if ((flags & ImGuiSelectableFlags_AllowOverlap) || (g.LastItemData.InFlags & ImGuiItemFlags_AllowOverlap)) { button_flags |= ImGuiButtonFlags_AllowOverlap; }

    const bool was_selected = selected;
    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, button_flags);

    // Auto-select when moved into
    // - This will be more fully fleshed in the range-select branch
    // - This is not exposed as it won't nicely work with some user side handling of shift/control
    // - We cannot do 'if (g.NavJustMovedToId != id) { selected = false; pressed = was_selected; }' for two reasons
    //   - (1) it would require focus scope to be set, need exposing PushFocusScope() or equivalent (e.g. BeginSelection() calling PushFocusScope())
    //   - (2) usage will fail with clipped items
    //   The multi-select API aim to fix those issues, e.g. may be replaced with a BeginSelection() API.
    if ((flags & ImGuiSelectableFlags_SelectOnNav) && g.NavJustMovedToId != 0 && g.NavJustMovedToFocusScopeId == g.CurrentFocusScopeId)
        if (g.NavJustMovedToId == id)
            selected = pressed = true;

    // Update NavId when clicking or when Hovering (this doesn't happen on most widgets), so navigation can be resumed with gamepad/keyboard
    if (pressed || (hovered && (flags & ImGuiSelectableFlags_SetNavIdOnHover)))
    {
        if (!g.NavDisableMouseHover && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent)
        {
            SetNavID(id, window->DC.NavLayerCurrent, g.CurrentFocusScopeId, WindowRectAbsToRel(window, bb)); // (bb == NavRect)
            g.NavDisableHighlight = true;
        }
    }
    if (pressed)
        MarkItemEdited(id);

    // In this branch, Selectable() cannot toggle the selection so this will never trigger.
    if (selected != was_selected) //-V547
        g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_ToggledSelection;


    AnimationMap->second.TextColor.Value = ImLerp(AnimationMap->second.TextColor.Value, selected ?/* ColorScheme.TextActive.GetColorVec4() * ImVec4(0.5F, 0.5F, 0.5F, 1.F)*/ ColorScheme.Accent.GetColorVec4() : ColorScheme.TextDisabled.GetColorVec4(), ImGui::GetIO().DeltaTime * Parameters::AnimationSpeed);
    // Render
    if (hovered || selected)
    {
        const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_HeaderActive : hovered ? ImGuiCol_HeaderHovered : ImGuiCol_Header);
        RenderFrame(bb.Min, bb.Max, ColorScheme.WindowBackground, false, 0.0f);
    }
    if (g.NavId == id)
        RenderNavHighlight(bb, id, ImGuiNavHighlightFlags_Compact | ImGuiNavHighlightFlags_NoRounding);

    if (span_all_columns)
    {
        if (g.CurrentTable)
            TablePopBackgroundChannel();
        else if (window->DC.CurrentColumns)
            PopColumnsBackground();
    }

    ImGui::PushStyleColor(ImGuiCol_Text, AnimationMap->second.TextColor.Value);
    RenderText(ImVec2(bb.Min.x, bb.GetCenter().y - ImGui::CalcTextSize(label).y / 2.F), label);
    ImGui::PopStyleColor();
   // RenderTextClipped(text_min, text_max, label, NULL, &label_size, style.SelectableTextAlign, &bb);

    // Automatically close popups
    if (pressed && (window->Flags & ImGuiWindowFlags_Popup) && !(flags & ImGuiSelectableFlags_DontClosePopups) && !(g.LastItemData.InFlags & ImGuiItemFlags_SelectableDontClosePopup))
        CloseCurrentPopup();

    if (disabled_item && !disabled_global)
        EndDisabled();

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
    return pressed; //-V1020
}

bool Widgets::Selectable(const char* label, bool* p_selected, ImGuiSelectableFlags flags, const ImVec2& size_arg)
{
    if (Selectable(label, *p_selected, flags, size_arg))
    {
        *p_selected = !*p_selected;
        return true;
    }
    return false;
}

IMGUI_API bool Widgets::ColorSelector(const char* label, float col[4])
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
    const ImRect total_bbA(pos, pos + ImVec2(/*209.F*/253.F - (27.F), 21.F));
    const ImRect total_bb(pos, pos + ImVec2(253.F, 21.F));
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
    window->DC.CursorPos = window->DC.CursorPos + ImVec2(0.F, 0.F);
    Widgets::ColorEdit4(label, col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaBar /*| ImGuiColorEditFlags_AlphaPreview*/);

    const ImRect check_bb(pos + ImVec2(232.F, 4.F), pos + ImVec2(245.F, 17.F));
    bool hovered, held;
    bool pressed = ButtonBehavior(total_bbA, id, &hovered, &held);

   /* if (Alignment::PointInsideRect(GetMousePos(), check_bb) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        pressed = true;
    }*/

    if (pressed)
    {
        //*v = !(*v);
        MarkItemEdited(id);
    }

    RenderNavHighlight(total_bb, id);

    //AnimationMap->second.FillColor.Value = ImLerp(AnimationMap->second.FillColor.Value, *v ? ColorScheme.Accent : ColorScheme.WindowBackground, ImGui::GetIO().DeltaTime * Parameters::AnimationSpeed);
    //AnimationMap->second.TextColor.Value = ImLerp(AnimationMap->second.TextColor.Value, *v ? ColorScheme.TextDisabled : ColorScheme.TextDisabled, ImGui::GetIO().DeltaTime * Parameters::AnimationSpeed);

    /*window->DrawList->AddRectFilled(check_bb.Min + ImVec2(1.F, 1.F), check_bb.Max - ImVec2(1.F, 1.F), AnimationMap->second.FillColor, 0.F);
    window->DrawList->AddRect(check_bb.Min, check_bb.Max, ColorScheme.Border, 2.F, 0, 1.F);*/
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

    ImGui::PushStyleColor(ImGuiCol_Text, ColorScheme.TextDisabled.GetColorU32());
    if (label_size.x > 0.0f)
        RenderText(label_pos, label);
    ImGui::PopStyleColor();

    /* window->DrawList->AddRect(total_bb.Min, total_bb.Max, ColorScheme.Accent);
     window->DrawList->AddRect(total_bbA.Min, total_bbA.Max, ColorScheme.Accent);
     window->DrawList->AddRect(total_bbB.Min, total_bbB.Max, ColorScheme.Accent);*/
    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
    return pressed;
}


void Widgets::MultiSelectableCombo(ImVec2 size, const std::vector<std::tuple<const char*, const char*>> items, bool* selected, size_t size_, const char* cusid)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();

    size = { 144.F, 18.F };


    struct DropdownAnimations
    {
        std::string label = "None Selected";
        ImColor ToolTip;
        ImColor ToolTipBrder;
        ImColor ToolTipTxt;
        float Alpha = 0;
        bool Callonnce = false;
    };

    const int AnimationID = (int)cusid;

    static std::map< ImGuiID, DropdownAnimations > Animations;
    auto AnimationMap = Animations.find(AnimationID);
    if (AnimationMap == Animations.end())
    {
        Animations.insert({ AnimationID, DropdownAnimations() });
        AnimationMap = Animations.find(AnimationID);
    }

    std::string label = AnimationMap->second.label;
    std::vector<std::string> vec;

    if (!AnimationMap->second.Callonnce)
    {
        for (size_t i = 0; i < size_; i++)
        {
            if (selected[i])
                vec.push_back(std::get<0>(items[i]));
        }

        if (!vec.empty())
        {
            label = "";
        }

        int index = 0;
        std::vector<std::string> shittowrite;
        for (auto Vec : vec)
        {
            auto text = Vec;
            if (index != 0) {
                text = std::string(", ") + text;
            }
            index++;
            shittowrite.push_back(text);
        }

        ImVec2 textsize{};
        for (auto text : shittowrite)
        {
            textsize.x += ImGui::CalcTextSize(text.data()).x;
        }

        if (textsize.x > 213.F)
        {
            shittowrite.push_back({ ", ..." });
            textsize.x += ImGui::CalcTextSize(", ...").x;
        }

        while (textsize.x > 213.F)
        {
            shittowrite.erase(shittowrite.end() - 2);

            textsize.x = 0.F;
            for (auto text : shittowrite)
            {
                textsize.x += ImGui::CalcTextSize(text.data()).x;
            }
        }

        for (auto text : shittowrite)
        {
            label += text;
        }

        AnimationMap->second.label = label;
        AnimationMap->second.Callonnce = true;
    }

    if (Widgets::BeginCombo(cusid, label.c_str()))
    {
        label = "";

        vec.clear();
        for (size_t i = 0; i < size_; i++)
        {
            Widgets::Selectable(std::get<0>(items[i]), &selected[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups, ImVec2(size.x, 18.F));

            if (selected[i])
                vec.push_back(std::get<0>(items[i]));

           // auto AnimationMap2 = Animations.find((int)std::get<1>(items[i]));
           // if (AnimationMap2 == Animations.end())
           // {
           //     Animations.insert({ (int)std::get<1>(items[i]), DropdownAnimations() });
           //     AnimationMap2 = Animations.find((int)std::get<1>(items[i]));
           // }

           // AnimationMap2->second.Alpha = ImLerp(AnimationMap2->second.Alpha, (ImGui::IsItemHovered() && strlen(std::get<1>(items[i])) > 0) ? 1.F : 0.F, ImGui::GetIO().DeltaTime * Parameters::AnimationSpeed);
           // AnimationMap2->second.ToolTip.Value = ImLerp(AnimationMap2->second.ToolTip.Value, (ImGui::IsItemHovered() && strlen(std::get<1>(items[i])) > 0) ? Color(17, 17, 18).GetColorVec4() : Color(17, 17, 18, 0).GetColorVec4(), ImGui::GetIO().DeltaTime * Parameters::AnimationSpeed);
           //// AnimationMap2->second.ToolTipTxt.Value = ImLerp(AnimationMap2->second.ToolTipTxt.Value, (ImGui::IsItemHovered() && strlen(std::get<1>(items[i])) > 0) ? ColorScheme.TextDisabled.GetColorVec4() : ColorScheme.TextDisabled.SetAlpha(0).GetColorVec4(), ImGui::GetIO().DeltaTime * Parameters::AnimationSpeed);

           // if (ImGui::IsItemHovered() && strlen(std::get<1>(items[i])) > 0)
           // {

           //     ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.F, 5.F));
           //     ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 1.F);
           //     ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.F);
           //     ImGui::PushStyleVar(ImGuiStyleVar_Alpha, AnimationMap2->second.Alpha);
           //     ImGui::PushStyleColor(ImGuiCol_Border, ColorScheme.Border.GetColorU32());
           //     //ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetColorU32(AnimationMap2->second.ToolTipTxt.Value));
           //     ImGui::PushStyleColor(ImGuiCol_PopupBg, ImGui::GetColorU32(AnimationMap2->second.ToolTip.Value));
           //     ImGui::BeginTooltip();
           //     ImGui::PushTextWrapPos(450.0f);
           //     ImGui::TextUnformatted(std::get<1>(items[i]));
           //     ImGui::PopTextWrapPos();
           //     ImGui::EndTooltip();
           //     ImGui::PopStyleColor(2);
           //     ImGui::PopStyleVar(4);
           // }
        }

        if (vec.empty())
        {
            label = "None Selected";
        }

        int index = 0;
        std::vector<std::string> shittowrite;
        for (auto Vec : vec)
        {
            auto text = Vec;
            if (index != 0) {
                text = std::string(", ") + text;
            }
            index++;
            shittowrite.push_back(text);
        }

        ImVec2 textsize{};
        for (auto text : shittowrite)
        {
            textsize.x += ImGui::CalcTextSize(text.data()).x;
        }

        if (textsize.x > 213.F)
        {
            shittowrite.push_back({ ", ..." });
            textsize.x += ImGui::CalcTextSize(", ...").x;
        }

        while (textsize.x > 213.F)
        {
            shittowrite.erase(shittowrite.end() - 2);

            textsize.x = 0.F;
            for (auto text : shittowrite)
            {
                textsize.x += ImGui::CalcTextSize(text.data()).x;
            }
        }

        for (auto text : shittowrite)
        {
            label += text;
        }

        AnimationMap->second.label = label;

        ImGui::EndCombo();

    }
    else {

    }
}
