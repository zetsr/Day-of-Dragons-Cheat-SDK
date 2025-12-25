#include "../Minimal-D3D12-Hook-ImGui-1.0.0/Main/mdx12_api.h"
#include "../CppSDK/SDK.hpp"
#include "ESP.h"

namespace g_ESP {
    static float g_CurrentLeftY = 0.0f;
    static float g_CurrentRightY = 0.0f;

    void ResetFlagOffsets() {
        g_CurrentLeftY = 0.0f;
        g_CurrentRightY = 0.0f;
    }

    SDK::APlayerController* GetLocalPC() {
        SDK::UWorld* World = SDK::UWorld::GetWorld();
        if (!World || !World->OwningGameInstance || World->OwningGameInstance->LocalPlayers.Num() == 0) return nullptr;
        return World->OwningGameInstance->LocalPlayers[0]->PlayerController;
    }

    inline ImU32 ToImColor(float r, float g, float b, float a) {
        return ImGui::ColorConvertFloat4ToU32(ImVec4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f));
    }

    BoxRect DrawBox(SDK::AActor* entity, float r, float g, float b, float a, float width_scale) {
        BoxRect rect;
        if (!entity || entity->bHidden) return rect;

        auto PC = GetLocalPC();
        if (!PC) return rect;

        SDK::FVector origin, extent;
        entity->GetActorBounds(true, &origin, &extent, false);

        SDK::FVector worldTop = { origin.X, origin.Y, origin.Z + extent.Z };
        SDK::FVector worldBottom = { origin.X, origin.Y, origin.Z - extent.Z };

        SDK::FVector2D screenTop, screenBottom;

        if (SDK::UGameplayStatics::ProjectWorldToScreen(PC, worldTop, &screenTop, false) &&
            SDK::UGameplayStatics::ProjectWorldToScreen(PC, worldBottom, &screenBottom, false)) {

            float height = abs(screenBottom.Y - screenTop.Y);
            float width = height * width_scale;

            rect.topLeft = ImVec2(screenTop.X - width / 2.0f, screenTop.Y);
            rect.bottomRight = ImVec2(screenTop.X + width / 2.0f, screenBottom.Y);
            rect.valid = true;

            if (a > 0.1f) {
                ImDrawList* drawList = ImGui::GetBackgroundDrawList();
                drawList->AddRect(ImVec2(rect.topLeft.x - 1, rect.topLeft.y - 1),
                    ImVec2(rect.bottomRight.x + 1, rect.bottomRight.y + 1),
                    ToImColor(0, 0, 0, a), 0.0f, 0, 1.5f);
                drawList->AddRect(rect.topLeft, rect.bottomRight, ToImColor(r, g, b, a), 0.0f, 0, 1.0f);
            }
        }
        return rect;
    }

    void RenderFlag(BoxRect rect, const std::string& text, ImU32 color, FlagPos pos) {
        if (!rect.valid || text.empty()) return;

        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
        ImVec2 drawPos;

        if (pos == FlagPos::Right) {
            drawPos = ImVec2(rect.bottomRight.x + 5.0f, rect.topLeft.y + g_CurrentRightY);
            g_CurrentRightY += textSize.y + 1.0f;
        }
        else {
            drawPos = ImVec2(rect.topLeft.x - 8.0f - textSize.x, rect.topLeft.y + g_CurrentLeftY);
            g_CurrentLeftY += textSize.y + 1.0f;
        }

        drawList->AddText(ImVec2(drawPos.x + 1, drawPos.y + 1), ToImColor(0, 0, 0, 255), text.c_str());
        drawList->AddText(drawPos, color, text.c_str());
    }

    void DrawHealthBar(BoxRect rect, float healthPercent, float maxHealth, float a) {
        if (!rect.valid) return;

        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        float boxHeight = rect.bottomRight.y - rect.topLeft.y;
        float barWidth = 2.5f;
        float barMargin = 4.0f;

        ImVec2 barBgTop = ImVec2(rect.topLeft.x - barMargin - barWidth, rect.topLeft.y);
        ImVec2 barBgBottom = ImVec2(rect.topLeft.x - barMargin, rect.bottomRight.y);

        drawList->AddRectFilled(ImVec2(barBgTop.x - 1, barBgTop.y - 1), ImVec2(barBgBottom.x + 1, barBgBottom.y + 1), ToImColor(0, 0, 0, a * 0.7f));

        float percentage = healthPercent / maxHealth;
        percentage = (percentage > 1.0f) ? 1.0f : (percentage < 0.0f ? 0.0f : percentage);

        ImVec4 col;
        if (percentage > 0.5f)
            col = ImVec4((1.0f - percentage) * 2.0f, 1.0f, 0.0f, a / 255.0f);
        else
            col = ImVec4(1.0f, percentage * 2.0f, 0.0f, a / 255.0f);
        ImU32 hpColor = ImGui::ColorConvertFloat4ToU32(col);

        if (healthPercent > 0) {
            float dynamicHeight = boxHeight * percentage;
            drawList->AddRectFilled(ImVec2(barBgTop.x, barBgBottom.y - dynamicHeight), barBgBottom, hpColor);
        }

        std::string hpText = std::to_string((int)healthPercent);
        RenderFlag(rect, hpText, hpColor, FlagPos::Left);
    }

    void DrawName(SDK::AActor* entity, BoxRect rect, float r, float g, float b, float a) {
        if (!rect.valid) return;
        auto PlayerChar = reinterpret_cast<SDK::AChar_Parent_Player_C*>(entity);
        if (!PlayerChar || !PlayerChar->PlayerName.IsValid()) return;

        std::string nameStr = PlayerChar->PlayerName.ToString();
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        ImVec2 textSize = ImGui::CalcTextSize(nameStr.c_str());
        ImVec2 textPos = ImVec2(rect.topLeft.x + (rect.bottomRight.x - rect.topLeft.x) / 2.0f - textSize.x / 2.0f, rect.topLeft.y - textSize.y - 5.0f);

        drawList->AddText(ImVec2(textPos.x + 1, textPos.y + 1), ToImColor(0, 0, 0, a), nameStr.c_str());
        drawList->AddText(textPos, ToImColor(r, g, b, a), nameStr.c_str());
    }
}