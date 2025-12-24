#include "../Minimal-D3D12-Hook-ImGui-1.0.0/Main/mdx12_api.h"
#include "../CppSDK/SDK.hpp"
#include "ESP.h"

void DrawESP() {
    SDK::UWorld* World = SDK::UWorld::GetWorld();
    if (!World || !World->GameState) return;

    SDK::APlayerController* LocalPC = g_ESP::GetLocalPC();
    if (!LocalPC || !LocalPC->Pawn) return;

    SDK::TArray<SDK::APlayerState*>& PlayerArray = World->GameState->PlayerArray;

    for (int i = 0; i < PlayerArray.Num(); i++) {
        SDK::APlayerState* PS = PlayerArray[i];
        if (!PS || !PS->PawnPrivate) continue;

        SDK::APawn* Pawn = PS->PawnPrivate;
        if (Pawn == LocalPC->Pawn) continue; // 排除自己

        auto TargetActor = reinterpret_cast<SDK::AActor*>(Pawn);
        auto BaseChar = reinterpret_cast<SDK::AChar_Parent_All_C*>(TargetActor);

        // 1. 绘制方框并获取矩形坐标
        g_ESP::BoxRect rect = g_ESP::DrawBox(TargetActor, 255, 255, 255, 255, 0.5f);

        // 如果物体不在屏幕内，跳过后续绘制
        if (!rect.valid) continue;

        // 2. 绘制名称
        g_ESP::DrawName(TargetActor, rect, 255, 255, 255, 255);

        // 3. 维护 Flags 队列
        std::vector<g_ESP::Flag> flags;

        // 添加血量 Flag (代替 HealthBar)
        if (BaseChar) {
            byte health = BaseChar->HealthPercent;
            // 简单的颜色插值：血越少越红
            float hpFactor = health / 200.0f;
            ImU32 hpColor = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f - hpFactor, hpFactor, 0.0f, 1.0f));

            flags.push_back({ "HP: " + std::to_string(health), hpColor });
        }

        // 添加距离 Flag
        float distance = LocalPC->Pawn->GetDistanceTo(TargetActor) / 100.0f; // 转换成米
        flags.push_back({ std::to_string((int)distance) + "m", ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)) });

        // 4. 一次性渲染所有文字标签
        g_ESP::RenderFlags(rect, flags);
    }
}

// 定义自定义 ImGui 绘制函数
void MyImGuiDraw(IDXGISwapChain3* pSwapChain, UINT SyncInterval, UINT Flags)
{
    // 检查菜单是否打开（按 F1 切换）
    if (g_MDX12::g_MenuState::g_isOpen) {
        ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);

        if (ImGui::Begin("My Menu")) {
            ImGui::Text("Hello World!");

            static bool option = false;
            ImGui::Checkbox("My Option", &option);

            ImGui::End();
        }
    }

    DrawESP();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        // 初始化 Hook 系统
        g_MDX12::Initialize();

        // 设置自定义绘制回调
        g_MDX12::SetSetupImGuiCallback(MyImGuiDraw);
        break;

    case DLL_PROCESS_DETACH:
        // 清理资源
        g_MDX12::FinalCleanupAll();
        break;
    }
    return TRUE;
}