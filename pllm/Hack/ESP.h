#pragma once
#include <vector>
#include <string>

namespace g_ESP {
    SDK::APlayerController* GetLocalPC();

    struct BoxRect {
        ImVec2 topLeft;
        ImVec2 bottomRight;
        bool valid = false;
    };

    enum class FlagPos {
        Left,
        Right
    };

    BoxRect DrawBox(SDK::AActor* entity, float r, float g, float b, float a, float width_scale);
    void DrawHealthBar(BoxRect rect, float healthPercent, float maxHealth, float a);
    void DrawName(SDK::AActor* entity, BoxRect rect, float r, float g, float b, float a);
    void RenderFlag(BoxRect rect, const std::string& text, ImU32 color, FlagPos pos);
    void ResetFlagOffsets();
}