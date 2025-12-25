#include "../Minimal-D3D12-Hook-ImGui-1.0.0/Main/mdx12_api.h" 
#include "../CppSDK/SDK.hpp"
#include "ESP.h"
#include "Configs.h"
#include "DrawESP.h"
#include "DrawImGui.h"

// 根据HTML主题色定义ImGui颜色
namespace ThemeColors {
    // 基础颜色
    const ImVec4 BG = ImVec4(7.0f / 255.0f, 8.0f / 255.0f, 10.0f / 255.0f, 1.0f);            // #07080a
    const ImVec4 MUTED = ImVec4(154.0f / 255.0f, 163.0f / 255.0f, 178.0f / 255.0f, 1.0f);    // #9aa3b2
    const ImVec4 TEXT = ImVec4(215.0f / 255.0f, 225.0f / 255.0f, 234.0f / 255.0f, 1.0f);     // #d7e1ea
    const ImVec4 ACCENT = ImVec4(110.0f / 255.0f, 231.0f / 255.0f, 183.0f / 255.0f, 1.0f);   // #6ee7b7
    const ImVec4 ACCENT2 = ImVec4(79.0f / 255.0f, 214.0f / 255.0f, 166.0f / 255.0f, 1.0f);   // #4fd6a6
    const ImVec4 GLASS_BORDER = ImVec4(1.0f, 1.0f, 1.0f, 0.04f);                        // rgba(255,255,255,0.04)

    // 衍生颜色
    const ImVec4 DARK_BG = ImVec4(0.02f, 0.02f, 0.03f, 0.95f);                           // 深色背景
    const ImVec4 LIGHT_BG = ImVec4(1.0f, 1.0f, 1.0f, 0.03f);                           // 浅色背景
    const ImVec4 HOVER_BG = ImVec4(1.0f, 1.0f, 1.0f, 0.1f);                            // 悬停背景
    const ImVec4 ACTIVE_BG = ImVec4(0.43f, 0.91f, 0.72f, 0.2f);                        // 激活背景
    const ImVec4 BORDER = ImVec4(1.0f, 1.0f, 1.0f, 0.06f);                             // 边框颜色
    const ImVec4 SHADOW = ImVec4(0.01f, 0.02f, 0.09f, 0.7f);                           // 阴影颜色
}

// 设置ImGui自定义风格
void SetupCustomImGuiStyle()
{
    ImGuiStyle& style = ImGui::GetStyle();

    // 圆角设置
    style.WindowRounding = 12.0f;
    style.ChildRounding = 12.0f;
    style.FrameRounding = 10.0f;
    style.PopupRounding = 10.0f;
    style.ScrollbarRounding = 5.0f;
    style.GrabRounding = 8.0f;
    style.TabRounding = 10.0f;
    style.WindowBorderSize = 1.0f;
    style.ChildBorderSize = 1.0f;
    style.PopupBorderSize = 1.0f;
    style.FrameBorderSize = 1.0f;
    style.TabBorderSize = 1.0f;

    // 内边距设置
    style.WindowPadding = ImVec2(20.0f, 20.0f);
    style.FramePadding = ImVec2(14.0f, 10.0f);
    style.CellPadding = ImVec2(8.0f, 8.0f);
    style.ItemSpacing = ImVec2(12.0f, 8.0f);
    style.ItemInnerSpacing = ImVec2(8.0f, 8.0f);
    style.TouchExtraPadding = ImVec2(0.0f, 0.0f);
    style.IndentSpacing = 20.0f;
    style.ScrollbarSize = 10.0f;
    style.GrabMinSize = 20.0f;

    // 窗口设置
    style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
    style.WindowMenuButtonPosition = ImGuiDir_None; // 隐藏菜单按钮
    style.ColorButtonPosition = ImGuiDir_Right;
    style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
    style.SelectableTextAlign = ImVec2(0.0f, 0.0f);
    style.DisplaySafeAreaPadding = ImVec2(3.0f, 3.0f);

    // 主颜色设置
    style.Colors[ImGuiCol_Text] = ThemeColors::TEXT;
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(ThemeColors::MUTED.x, ThemeColors::MUTED.y, ThemeColors::MUTED.z, 0.5f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(ThemeColors::BG.x, ThemeColors::BG.y, ThemeColors::BG.z, 0.95f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(ThemeColors::BG.x, ThemeColors::BG.y, ThemeColors::BG.z, 0.8f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(14.0f / 255.0f, 16.0f / 255.0f, 18.0f / 255.0f, 0.96f);
    style.Colors[ImGuiCol_Border] = ThemeColors::GLASS_BORDER;
    style.Colors[ImGuiCol_BorderShadow] = ThemeColors::SHADOW;

    // 框架和按钮
    style.Colors[ImGuiCol_FrameBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.018f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(1.0f, 1.0f, 1.0f, 0.05f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(1.0f, 1.0f, 1.0f, 0.08f);

    // 标题栏
    style.Colors[ImGuiCol_TitleBg] = ImVec4(ThemeColors::BG.x, ThemeColors::BG.y, ThemeColors::BG.z, 0.8f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(ThemeColors::BG.x, ThemeColors::BG.y, ThemeColors::BG.z, 1.0f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(ThemeColors::BG.x, ThemeColors::BG.y, ThemeColors::BG.z, 0.5f);

    // 菜单栏
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.02f, 0.02f, 0.03f, 0.8f);

    // 滚动条
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.02f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(1.0f, 1.0f, 1.0f, 0.1f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(1.0f, 1.0f, 1.0f, 0.15f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.0f, 1.0f, 1.0f, 0.2f);

    // 复选框、单选按钮
    style.Colors[ImGuiCol_CheckMark] = ThemeColors::ACCENT;

    // 滑块
    style.Colors[ImGuiCol_SliderGrab] = ThemeColors::ACCENT;
    style.Colors[ImGuiCol_SliderGrabActive] = ThemeColors::ACCENT2;

    // 按钮
    style.Colors[ImGuiCol_Button] = ImVec4(1.0f, 1.0f, 1.0f, 0.018f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(1.0f, 1.0f, 1.0f, 0.05f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(1.0f, 1.0f, 1.0f, 0.08f);

    // 标题
    style.Colors[ImGuiCol_Header] = ImVec4(0.43f, 0.91f, 0.72f, 0.06f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.43f, 0.91f, 0.72f, 0.12f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.43f, 0.91f, 0.72f, 0.2f);

    // 分隔线
    style.Colors[ImGuiCol_Separator] = ThemeColors::GLASS_BORDER;
    style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(1.0f, 1.0f, 1.0f, 0.1f);
    style.Colors[ImGuiCol_SeparatorActive] = ImVec4(1.0f, 1.0f, 1.0f, 0.2f);

    // 调整大小手柄
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.0f, 1.0f, 1.0f, 0.02f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.43f, 0.91f, 0.72f, 0.3f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.43f, 0.91f, 0.72f, 0.5f);

    // 选项卡
    style.Colors[ImGuiCol_Tab] = ImVec4(1.0f, 1.0f, 1.0f, 0.018f);
    style.Colors[ImGuiCol_TabHovered] = ImVec4(0.43f, 0.91f, 0.72f, 0.06f);
    style.Colors[ImGuiCol_TabActive] = ImVec4(0.43f, 0.91f, 0.72f, 0.12f);
    style.Colors[ImGuiCol_TabUnfocused] = ImVec4(1.0f, 1.0f, 1.0f, 0.018f);
    style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(1.0f, 1.0f, 1.0f, 0.03f);

    // 表格
    style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.014f);
    style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(1.0f, 1.0f, 1.0f, 0.03f);
    style.Colors[ImGuiCol_TableBorderLight] = ImVec4(1.0f, 1.0f, 1.0f, 0.02f);
    style.Colors[ImGuiCol_TableRowBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.006f);
    style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.002f);

    // 文本输入框
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.43f, 0.91f, 0.72f, 0.3f);

    // 拖放目标
    style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.43f, 0.91f, 0.72f, 0.5f);

    // 导航
    style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.43f, 0.91f, 0.72f, 0.8f);
    style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);
    style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.3f);

    // 模态窗口
    style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.8f);
}

namespace g_DrawImGui {

    // 助手函数：安全地将下一个小部件向右对齐
    void AlignWidgetRight(float widgetWidth)
    {
        // 获取当前行可用的剩余宽度
        float availableWidth = ImGui::GetContentRegionAvail().x;
        // 计算需要的偏移量 (剩余宽度 - 控件宽度)
        // 注意：这里使用 SameLine 来移动光标，而不是 SetCursorPos，这可以防止 "extend window boundaries" 错误
        if (availableWidth > widgetWidth)
        {
            ImGui::SameLine(ImGui::GetCursorPosX() + availableWidth - widgetWidth);
        }
        else
        {
            // 如果没有足够空间，就直接 SameLine，可能会重叠，但不会崩溃
            ImGui::SameLine();
        }
    }

    // 自定义颜色选择器 - 彻底修复版
    void DrawCustomColorPicker(const char* label_id, float* col_ptr, ImGuiColorEditFlags flags = 0)
    {
        // 关键修复 1: PushID 确保每个 Popup 都是独立的
        ImGui::PushID(label_id);

        ImGuiColorEditFlags color_flags = flags | ImGuiColorEditFlags_AlphaBar |
            ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoLabel;

        static float saved_color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

        // 计算颜色选择器的大小
        float button_size = ImGui::GetFrameHeight() - 4.0f;
        ImVec2 button_size_vec = ImVec2(button_size, button_size);

        // 关键修复 2: 移除所有 SetCursorPos 操作。只负责画按钮。
        // 对齐逻辑交给调用者处理 (使用 AlignWidgetRight)。
        if (ImGui::ColorButton("##ColorBtn", ImVec4(col_ptr[0], col_ptr[1], col_ptr[2], col_ptr[3]),
            ImGuiColorEditFlags_NoBorder, button_size_vec))
        {
            ImGui::OpenPopup("##ColorPickerPopup");
        }

        // 弹出式颜色选择器
        if (ImGui::BeginPopup("##ColorPickerPopup"))
        {
            ImGui::Text("Edit Color");
            ImGui::Separator();

            ImGui::ColorPicker4("##Picker", col_ptr, color_flags | ImGuiColorEditFlags_DisplayRGB |
                ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_DisplayHex);

            ImGui::Separator();

            if (ImGui::Button("Copy", ImVec2(60, 0)))
            {
                memcpy(saved_color, col_ptr, sizeof(float) * 4);
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Paste", ImVec2(60, 0)))
            {
                memcpy(col_ptr, saved_color, sizeof(float) * 4);
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Reset", ImVec2(60, 0)))
            {
                col_ptr[0] = 1.0f;
                col_ptr[1] = 1.0f;
                col_ptr[2] = 1.0f;
                col_ptr[3] = 1.0f;
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        ImGui::PopID();
    }

    // 绘制自定义按钮
    bool DrawCustomButton(const char* label, const ImVec2& size = ImVec2(0, 0), bool accent = false)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(14.0f, 10.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);

        if (accent)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.43f, 0.91f, 0.72f, 0.06f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.43f, 0.91f, 0.72f, 0.12f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.43f, 0.91f, 0.72f, 0.18f));
            ImGui::PushStyleColor(ImGuiCol_Text, ThemeColors::ACCENT);
        }

        bool result = ImGui::Button(label, size);

        if (accent)
        {
            ImGui::PopStyleColor(4);
        }

        ImGui::PopStyleVar(2);
        return result;
    }

    // 绘制自定义复选框
    bool DrawCustomCheckbox(const char* label, bool* v)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1.0f, 1.0f, 1.0f, 0.03f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(1.0f, 1.0f, 1.0f, 0.06f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(1.0f, 1.0f, 1.0f, 0.09f));

        bool result = ImGui::Checkbox(label, v);

        ImGui::PopStyleColor(3);
        ImGui::PopStyleVar(1);
        return result;
    }

    // 绘制自定义滑块
    bool DrawCustomSliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = "%.1f")
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1.0f, 1.0f, 1.0f, 0.03f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(1.0f, 1.0f, 1.0f, 0.06f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(1.0f, 1.0f, 1.0f, 0.09f));
        ImGui::PushStyleColor(ImGuiCol_SliderGrab, ThemeColors::ACCENT);
        ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ThemeColors::ACCENT2);

        bool result = ImGui::SliderFloat(label, v, v_min, v_max, format);

        ImGui::PopStyleColor(5);
        ImGui::PopStyleVar(2);
        return result;
    }

    // 绘制带标签的控件组
    void BeginCustomGroup(const char* name)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12.0f, 12.0f));
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 1.0f, 1.0f, 0.012f));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 1.0f, 1.0f, 0.03f));

        // 修复：移除未定义的标识符，使用标准的 Child 窗口参数
        // ImVec2(0, 0) 在宽度上会填满父窗口，高度为 0 会填满剩余空间
        // 如果你需要它“自动缩放高度”，在标准的 ImGui 中通常通过计算内容高度实现，
        // 但为了稳定，我们先给它一个 0（填满剩余）或者根据你的 UI 需求给个固定高度。
        ImGui::BeginChild(name, ImVec2(0, -1), true, ImGuiWindowFlags_None);
    }

    void EndCustomGroup()
    {
        ImGui::EndChild();
        ImGui::PopStyleColor(2);
        ImGui::PopStyleVar(2);
    }

    void MyImGuiDraw(IDXGISwapChain3* pSwapChain, UINT SyncInterval, UINT Flags)
    {
        static bool style_initialized = false;
        if (!style_initialized)
        {
            SetupCustomImGuiStyle();
            style_initialized = true;
        }

        if (g_MDX12::g_MenuState::g_isOpen) {
            ImGui::SetNextWindowSize(ImVec2(600, 600), ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowBgAlpha(0.95f);

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20.0f, 20.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 16.0f);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(7.0f / 255.0f, 8.0f / 255.0f, 10.0f / 255.0f, 0.95f));
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 1.0f, 1.0f, 0.02f));

            if (ImGui::Begin("Begeerte", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar)) {

                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(20.0f, 10.0f));
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 0.0f));
                ImGui::PushStyleColor(ImGuiCol_Tab, ImVec4(1.0f, 1.0f, 1.0f, 0.018f));
                ImGui::PushStyleColor(ImGuiCol_TabHovered, ImVec4(0.43f, 0.91f, 0.72f, 0.06f));
                ImGui::PushStyleColor(ImGuiCol_TabActive, ImVec4(0.43f, 0.91f, 0.72f, 0.12f));
                ImGui::PushStyleColor(ImGuiCol_TabUnfocused, ImVec4(1.0f, 1.0f, 1.0f, 0.018f));
                ImGui::PushStyleColor(ImGuiCol_TabUnfocusedActive, ImVec4(1.0f, 1.0f, 1.0f, 0.03f));

                if (ImGui::BeginTabBar("MainTabBar", ImGuiTabBarFlags_None)) {

                    // --- 视觉选项卡 (敌人 ESP & Flags 合并) ---
                    if (ImGui::BeginTabItem("Visuals")) {
                        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12.0f, 12.0f));

                        // 基础ESP设置
                        BeginCustomGroup("Basic ESP");
                        ImGui::TextColored(ThemeColors::ACCENT, "ESP Settings");
                        ImGui::Separator();

                        // Box
                        DrawCustomCheckbox("Box", &g_Config::bDrawBox);
                        if (g_Config::bDrawBox) {
                            // 使用 AlignWidgetRight 安全地右对齐颜色选择器
                            // 按钮大概大小是 FrameHeight (约20-25)
                            AlignWidgetRight(ImGui::GetFrameHeight());
                            DrawCustomColorPicker("BoxCol##1", g_Config::BoxColor);
                        }

                        // Name
                        DrawCustomCheckbox("Name", &g_Config::bDrawName);
                        if (g_Config::bDrawName) {
                            AlignWidgetRight(ImGui::GetFrameHeight());
                            DrawCustomColorPicker("NameCol##1", g_Config::NameColor);
                        }

                        // Health Bar (之前因为Flags错误导致无法渲染，现在应该修复了)
                        DrawCustomCheckbox("Health Bar", &g_Config::bDrawHealthBar);

                        // --- 原 Flags 内容现在合并到这里 ---
                        DrawCustomCheckbox("Show Species", &g_Config::bDrawSpecies);
                        DrawCustomCheckbox("Show Growth Stage", &g_Config::bDrawGrowth);

                        // Distance
                        DrawCustomCheckbox("Show Distance", &g_Config::bDrawDistance);
                        if (g_Config::bDrawDistance) {
                            AlignWidgetRight(ImGui::GetFrameHeight());
                            DrawCustomColorPicker("DistCol##1", g_Config::DistanceColor);
                        }

                        EndCustomGroup();

                        // 视野外指示器
                        ImGui::Spacing();
                        BeginCustomGroup("Out of FOV Indicator");
                        ImGui::TextColored(ThemeColors::ACCENT, "Out of FOV Settings");
                        ImGui::Separator();

                        DrawCustomCheckbox("Enable Out of FOV", &g_Config::bEnableOOF);
                        if (g_Config::bEnableOOF) {
                            ImGui::Spacing();
                            ImGui::Text("Arrow Color:");
                            AlignWidgetRight(ImGui::GetFrameHeight());
                            DrawCustomColorPicker("OOFCol##1", g_Config::OOFColor);

                            ImGui::Spacing();
                            DrawCustomSliderFloat("Arrow Size", &g_Config::OOFSize, 5.0f, 30.0f, "%.1f");
                            DrawCustomSliderFloat("Screen Radius", &g_Config::OOFRadius, 0.5f, 0.95f, "%.2f");
                            DrawCustomSliderFloat("Breath Speed", &g_Config::OOFBreathSpeed, 0.5f, 5.0f, "%.1f");
                            DrawCustomSliderFloat("Min Alpha", &g_Config::OOFMinAlpha, 0.1f, 0.9f, "%.2f");
                            DrawCustomSliderFloat("Max Alpha", &g_Config::OOFMaxAlpha, 0.2f, 1.0f, "%.2f");
                        }
                        EndCustomGroup();

                        ImGui::PopStyleVar();
                        ImGui::EndTabItem();
                    }

                    // --- 队友 ESP 选项卡 ---
                    if (ImGui::BeginTabItem("Team")) {
                        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12.0f, 12.0f));

                        BeginCustomGroup("Team ESP");
                        ImGui::TextColored(ThemeColors::ACCENT, "Team ESP Settings");
                        ImGui::Separator();

                        DrawCustomCheckbox("Box##Team", &g_Config::bDrawBoxTeam);
                        if (g_Config::bDrawBoxTeam) {
                            AlignWidgetRight(ImGui::GetFrameHeight());
                            DrawCustomColorPicker("BoxCol##Team", g_Config::BoxColorTeam);
                        }

                        DrawCustomCheckbox("Name##Team", &g_Config::bDrawNameTeam);
                        if (g_Config::bDrawNameTeam) {
                            AlignWidgetRight(ImGui::GetFrameHeight());
                            DrawCustomColorPicker("NameCol##Team", g_Config::NameColorTeam);
                        }

                        DrawCustomCheckbox("Health Bar##Team", &g_Config::bDrawHealthBarTeam);
                        DrawCustomCheckbox("Show Species##Team", &g_Config::bDrawSpeciesTeam);
                        DrawCustomCheckbox("Show Growth Stage##Team", &g_Config::bDrawGrowthTeam);

                        DrawCustomCheckbox("Show Distance##Team", &g_Config::bDrawDistanceTeam);
                        if (g_Config::bDrawDistanceTeam) {
                            AlignWidgetRight(ImGui::GetFrameHeight());
                            DrawCustomColorPicker("DistCol##Team", g_Config::DistanceColorTeam);
                        }
                        EndCustomGroup();

                        ImGui::PopStyleVar();
                        ImGui::EndTabItem();
                    }

                    // --- 氏族成员 ESP 选项卡 ---
                    if (ImGui::BeginTabItem("Clan")) {
                        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12.0f, 12.0f));

                        BeginCustomGroup("Clan ESP");
                        ImGui::TextColored(ThemeColors::ACCENT, "Clan ESP Settings");
                        ImGui::Separator();

                        DrawCustomCheckbox("Box##Clan", &g_Config::bDrawBoxClan);
                        if (g_Config::bDrawBoxClan) {
                            AlignWidgetRight(ImGui::GetFrameHeight());
                            DrawCustomColorPicker("BoxCol##Clan", g_Config::BoxColorClan);
                        }

                        DrawCustomCheckbox("Name##Clan", &g_Config::bDrawNameClan);
                        if (g_Config::bDrawNameClan) {
                            AlignWidgetRight(ImGui::GetFrameHeight());
                            DrawCustomColorPicker("NameCol##Clan", g_Config::NameColorClan);
                        }

                        DrawCustomCheckbox("Health Bar##Clan", &g_Config::bDrawHealthBarClan);
                        DrawCustomCheckbox("Show Species##Clan", &g_Config::bDrawSpeciesClan);
                        DrawCustomCheckbox("Show Growth Stage##Clan", &g_Config::bDrawGrowthClan);

                        DrawCustomCheckbox("Show Distance##Clan", &g_Config::bDrawDistanceClan);
                        if (g_Config::bDrawDistanceClan) {
                            AlignWidgetRight(ImGui::GetFrameHeight());
                            DrawCustomColorPicker("DistCol##Clan", g_Config::DistanceColorClan);
                        }
                        EndCustomGroup();

                        ImGui::PopStyleVar();
                        ImGui::EndTabItem();
                    }

                    // 已删除 Flags 选项卡

                    ImGui::EndTabBar();
                    ImGui::PopStyleColor(5);
                    ImGui::PopStyleVar(2);
                }

            }
            ImGui::PopStyleColor(2);
            ImGui::PopStyleVar(2);
            ImGui::End();
        }

        g_DrawESP::DrawESP();
    }
}