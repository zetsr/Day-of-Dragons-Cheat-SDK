#pragma once

namespace g_Config {
    // 辅助函数：将 float[4] 转换为 ImU32
    inline ImU32 GetU32Color(float color[4]) {
        return ImGui::ColorConvertFloat4ToU32(*(ImVec4*)color);
    }

    // 方框设置
    bool bDrawBox = true;
    float BoxColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

    // 血条设置
    bool bDrawHealthBar = true;

    // 名称设置
    bool bDrawName = true;
    float NameColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

    // 文字标签设置
    bool bDrawSpecies = true;
    bool bDrawGrowth = true;
    bool bDrawDistance = true;
    float DistanceColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
}