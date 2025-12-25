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

    // Flag 管理器
    class FlagManager {
    private:
        float leftY = 0.0f;
        float rightY = 0.0f;

    public:
        void Reset() {
            leftY = 0.0f;
            rightY = 0.0f;
        }

        void AddFlag(BoxRect rect, const std::string& text, ImU32 color, FlagPos pos);
    };

    // 原有函数
    BoxRect DrawBox(SDK::AActor* entity, float r, float g, float b, float a, float width_scale);
    void DrawHealthBar(BoxRect rect, float healthPercent, float maxHealth, float a);
    void DrawName(SDK::AActor* entity, BoxRect rect, float r, float g, float b, float a);

    // 新增：OOF 结构体，用于传递需要显示的文本
    struct OOFFlag {
        std::string text;
        ImU32 color;
    };

    // 新增：OOF 渲染函数
    void DrawOutOfFOV(SDK::AActor* entity, SDK::APlayerController* LocalPC, const std::vector<OOFFlag>& flags);
}