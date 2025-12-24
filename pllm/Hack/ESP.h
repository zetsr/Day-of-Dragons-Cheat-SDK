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

    // 修复后的 DrawBox：利用 GetActorBounds 动态计算缩放
    BoxRect DrawBox(SDK::AActor* entity, float r, float g, float b, float a, float width_scale);

    // 绘制名称：基于 Box 坐标
    void DrawName(SDK::AActor* entity, BoxRect rect, float r, float g, float b, float a);

    // Flag 系统
    struct Flag {
        std::string text;
        ImU32 color;
    };

    // AddFlags 渲染器：自动维护垂直队列
    void RenderFlags(BoxRect rect, const std::vector<Flag>& flags);
}