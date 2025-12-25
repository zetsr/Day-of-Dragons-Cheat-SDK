#pragma once
#include <vector>
#include <string>

namespace g_ESP {
    // 获取本地玩家控制器
    SDK::APlayerController* GetLocalPC();

    // 结构体：保存计算出的 2D 矩形信息
    struct BoxRect {
        ImVec2 topLeft;
        ImVec2 bottomRight;
        bool valid = false;
    };

    // 渲染位置枚举
    enum class FlagPos {
        Left,
        Right
    };

    // 绘制方框
    BoxRect DrawBox(SDK::AActor* entity, float r, float g, float b, float a, float width_scale);

    // 绘制血条（内部包含 Health Text）
    void DrawHealthBar(BoxRect rect, float healthPercent, float maxHealth, float a);

    // 绘制名称
    void DrawName(SDK::AActor* entity, BoxRect rect, float r, float g, float b, float a);

    // 核心改进：单行渲染 Flag，内部自动处理队列高度
    void RenderFlag(BoxRect rect, const std::string& text, ImU32 color, FlagPos pos);

    // 重置偏移量，每个玩家循环开始时调用
    void ResetFlagOffsets();
}