#include "../Minimal-D3D12-Hook-ImGui-1.0.0/Main/mdx12_api.h"
#include "../CppSDK/SDK.hpp"
#include "ESP.h"

namespace g_ESP {

    SDK::APlayerController* GetLocalPC() {
        SDK::UWorld* World = SDK::UWorld::GetWorld();
        if (!World || !World->OwningGameInstance || World->OwningGameInstance->LocalPlayers.Num() == 0) return nullptr;
        return World->OwningGameInstance->LocalPlayers[0]->PlayerController;
    }

    inline ImU32 ToImColor(float r, float g, float b, float a) {
        return ImGui::ColorConvertFloat4ToU32(ImVec4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f));
    }

    // --- 修复“近小远大”逻辑 ---
    BoxRect DrawBox(SDK::AActor* entity, float r, float g, float b, float a, float width_scale) {
        BoxRect rect;
        if (!entity || entity->bHidden) return rect;

        auto PC = GetLocalPC();
        if (!PC) return rect;

        // 利用你提供的接口获取 3D 包围盒
        SDK::FVector origin, extent;
        entity->GetActorBounds(true, &origin, &extent, false);

        // 计算 3D 空间的顶部点和底部点
        SDK::FVector worldTop = { origin.X, origin.Y, origin.Z + extent.Z };
        SDK::FVector worldBottom = { origin.X, origin.Y, origin.Z - extent.Z };

        SDK::FVector2D screenTop, screenBottom;

        // 将 3D 点投影到 2D 屏幕
        if (SDK::UGameplayStatics::ProjectWorldToScreen(PC, worldTop, &screenTop, false) &&
            SDK::UGameplayStatics::ProjectWorldToScreen(PC, worldBottom, &screenBottom, false)) {

            // 屏幕上的高度 = 底部 Y - 顶部 Y (UE坐标系中 Y 向下递增)
            float height = abs(screenBottom.Y - screenTop.Y);
            float width = height * width_scale;

            rect.topLeft = ImVec2(screenTop.X - width / 2.0f, screenTop.Y);
            rect.bottomRight = ImVec2(screenTop.X + width / 2.0f, screenBottom.Y);
            rect.valid = true;

            ImDrawList* drawList = ImGui::GetBackgroundDrawList();

            // 绘制黑色外边框（阴影效果）
            drawList->AddRect(ImVec2(rect.topLeft.x - 1, rect.topLeft.y - 1),
                ImVec2(rect.bottomRight.x + 1, rect.bottomRight.y + 1),
                ToImColor(0, 0, 0, a), 0.0f, 0, 1.5f);

            // 绘制主方框
            drawList->AddRect(rect.topLeft, rect.bottomRight, ToImColor(r, g, b, a), 0.0f, 0, 1.0f);
        }
        return rect;
    }

    // --- 绘制名称 ---
    void DrawName(SDK::AActor* entity, BoxRect rect, float r, float g, float b, float a) {
        if (!rect.valid) return;

        auto PlayerChar = reinterpret_cast<SDK::AChar_Parent_Player_C*>(entity);
        if (!PlayerChar) return;

        SDK::FString fName = PlayerChar->PlayerName;
        if (!fName.IsValid()) return;

        std::string nameStr = fName.ToString();
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        ImVec2 textSize = ImGui::CalcTextSize(nameStr.c_str());

        // 将名字放在 Box 顶部正中央
        ImVec2 textPos = ImVec2(rect.topLeft.x + (rect.bottomRight.x - rect.topLeft.x) / 2.0f - textSize.x / 2.0f,
            rect.topLeft.y - textSize.y - 5.0f);

        drawList->AddText(ImVec2(textPos.x + 1, textPos.y + 1), ToImColor(0, 0, 0, a), nameStr.c_str());
        drawList->AddText(textPos, ToImColor(r, g, b, a), nameStr.c_str());
    }

    // --- AddFlags 自动维护队列系统 ---
    void RenderFlags(BoxRect rect, const std::vector<Flag>& flags) {
        if (!rect.valid || flags.empty()) return;

        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        float currentOffsetY = 0.0f;

        for (const auto& flag : flags) {
            // 起点：Box 的右上角 (rect.bottomRight.x, rect.topLeft.y)
            ImVec2 pos = ImVec2(rect.bottomRight.x + 5.0f, rect.topLeft.y + currentOffsetY);

            // 绘制文字阴影
            drawList->AddText(ImVec2(pos.x + 1, pos.y + 1), ToImColor(0, 0, 0, 255), flag.text.c_str());
            // 绘制文字内容
            drawList->AddText(pos, flag.color, flag.text.c_str());

            // 自动累加高度：字体大小 + 2像素行间距
            currentOffsetY += ImGui::GetFontSize() + 2.0f;
        }
    }
}