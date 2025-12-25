#pragma once

namespace g_Config {
    // 辅助函数：将 float[4] 转换为 ImU32
    inline ImU32 GetU32Color(float color[4]) {
        return ImGui::ColorConvertFloat4ToU32(*(ImVec4*)color);
    }

    // --- 原有变量 ---
    inline bool bDrawBox = true;
    inline float BoxColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    inline bool bDrawHealthBar = true;
    inline bool bDrawName = true;
    inline float NameColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    inline bool bDrawSpecies = true;
    inline bool bDrawGrowth = true;
    inline bool bDrawDistance = true;
    inline float DistanceColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

    // --- OOF 新增设置 ---
    inline bool bEnableOOF = true;              // 是否启用 OOF
    inline float OOFColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; // 箭头颜色
    inline float OOFRadius = 0.75f;             // 屏幕边缘半径百分比 (0.0 - 1.0)
    inline float OOFSize = 12.0f;               // 箭头大小
    inline float OOFBreathSpeed = 2.0f;         // 呼吸效果基础速度
    inline float OOFMinAlpha = 0.25f;            // 最小透明度
    inline float OOFMaxAlpha = 1.0f;            // 最大透明度
}