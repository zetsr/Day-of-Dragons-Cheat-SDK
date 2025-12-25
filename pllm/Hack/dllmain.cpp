#include "../Minimal-D3D12-Hook-ImGui-1.0.0/Main/mdx12_api.h"
#include "../CppSDK/SDK.hpp"
#include "ESP.h"
#include "Configs.h"

void DrawESP() {
    SDK::UWorld* World = SDK::UWorld::GetWorld();
    if (!World || !World->GameState) return;

    SDK::APlayerController* LocalPC = g_ESP::GetLocalPC();
    if (!LocalPC || !LocalPC->Pawn) return;

    // 获取当前屏幕分辨率，用于边界判定
    ImGuiIO& io = ImGui::GetIO();
    float screenW = io.DisplaySize.x;
    float screenH = io.DisplaySize.y;

    SDK::TArray<SDK::APlayerState*>& PlayerArray = World->GameState->PlayerArray;

    for (int i = 0; i < PlayerArray.Num(); i++) {
        SDK::APlayerState* PS = PlayerArray[i];
        if (!PS || !PS->PawnPrivate || PS->PawnPrivate == LocalPC->Pawn) continue;

        auto TargetActor = reinterpret_cast<SDK::AActor*>(PS->PawnPrivate);
        auto BaseChar = reinterpret_cast<SDK::AChar_Parent_All_C*>(TargetActor);
        if (!BaseChar || BaseChar->IsDead) continue;

        auto PlayerChar = reinterpret_cast<SDK::AChar_Parent_Player_C*>(TargetActor);
        if (!PlayerChar) continue;

        // --- 判断是否在屏幕内 ---
        SDK::FVector2D screenPos;
        bool bProjected = SDK::UGameplayStatics::ProjectWorldToScreen(LocalPC, TargetActor->K2_GetActorLocation(), &screenPos, false);
        bool bInScreenBounds = (screenPos.X > 0 && screenPos.X < screenW&& screenPos.Y > 0 && screenPos.Y < screenH);

        // --- 如果在屏幕内，绘制常规 ESP ---
        if (bProjected && bInScreenBounds) {
            // 创建 Flag 管理器
            g_ESP::FlagManager flagMgr;

            // 绘制盒子
            g_ESP::BoxRect rect = g_ESP::DrawBox(TargetActor,
                g_Config::BoxColor[0] * 255, g_Config::BoxColor[1] * 255,
                g_Config::BoxColor[2] * 255, g_Config::BoxColor[3] * 255, 0.5f);

            if (rect.valid) {
                // 绘制血条
                if (g_Config::bDrawHealthBar) {
                    g_ESP::DrawHealthBar(rect, (float)BaseChar->HealthPercent, 200.0f, 255.0f);

                    // 血量文本添加到左侧 flags
                    std::string hpStr = std::to_string((int)BaseChar->HealthPercent);
                    float hpPercent = BaseChar->HealthPercent / 200.0f;
                    ImVec4 hpColor;
                    if (hpPercent > 0.5f)
                        hpColor = ImVec4((1.0f - hpPercent) * 2.0f, 1.0f, 0.0f, 1.0f);
                    else
                        hpColor = ImVec4(1.0f, hpPercent * 2.0f, 0.0f, 1.0f);
                    flagMgr.AddFlag(rect, hpStr, ImGui::ColorConvertFloat4ToU32(hpColor), g_ESP::FlagPos::Left);
                }

                // 绘制名字
                if (g_Config::bDrawName) {
                    g_ESP::DrawName(TargetActor, rect,
                        g_Config::NameColor[0] * 255, g_Config::NameColor[1] * 255,
                        g_Config::NameColor[2] * 255, g_Config::NameColor[3] * 255);
                }

                // 添加物种到右侧 flags
                if (g_Config::bDrawSpecies) {
                    std::string speciesName;
                    ImU32 speciesColor;
                    switch (PlayerChar->CharacterSpecies) {
                    case SDK::Enum_PlayerCharacter::NewEnumerator0: speciesName = "FS"; speciesColor = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 0.4f, 0.4f, 1.0f)); break;
                    case SDK::Enum_PlayerCharacter::NewEnumerator1: speciesName = "IR"; speciesColor = ImGui::ColorConvertFloat4ToU32(ImVec4(0.4f, 1.0f, 1.0f, 1.0f)); break;
                    case SDK::Enum_PlayerCharacter::NewEnumerator2: speciesName = "SS"; speciesColor = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 1.0f, 0.4f, 1.0f)); break;
                    case SDK::Enum_PlayerCharacter::NewEnumerator3: speciesName = "ASD"; speciesColor = ImGui::ColorConvertFloat4ToU32(ImVec4(0.6f, 1.0f, 0.4f, 1.0f)); break;
                    case SDK::Enum_PlayerCharacter::NewEnumerator5: speciesName = "BS"; speciesColor = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 0.4f, 0.7f, 1.0f)); break;
                    case SDK::Enum_PlayerCharacter::NewEnumerator10: speciesName = "BW"; speciesColor = ImGui::ColorConvertFloat4ToU32(ImVec4(0.4f, 0.4f, 1.0f, 1.0f)); break;
                    case SDK::Enum_PlayerCharacter::NewEnumerator12: speciesName = "BIO"; speciesColor = ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 1.0f, 1.0f, 1.0f)); break;
                    default:
                        speciesName = "Spec_" + std::to_string((unsigned char)PlayerChar->CharacterSpecies);
                        speciesColor = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                        break;
                    }
                    flagMgr.AddFlag(rect, speciesName, speciesColor, g_ESP::FlagPos::Right);
                }

                // 添加成长阶段到右侧 flags
                if (g_Config::bDrawGrowth) {
                    std::string stageName = "Unknown";
                    ImU32 stageColor = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                    switch (BaseChar->GrowthStage) {
                    case SDK::Enum_GrowthStage::NewEnumerator1: stageName = "Hatchling"; stageColor = ImGui::ColorConvertFloat4ToU32(ImVec4(0.8f, 0.8f, 0.8f, 1.0f)); break;
                    case SDK::Enum_GrowthStage::NewEnumerator2: stageName = "Juvenile"; stageColor = ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 1.0f, 0.5f, 1.0f)); break;
                    case SDK::Enum_GrowthStage::NewEnumerator3: stageName = "Adult"; stageColor = ImGui::ColorConvertFloat4ToU32(ImVec4(0.1f, 0.6f, 1.0f, 1.0f)); break;
                    case SDK::Enum_GrowthStage::NewEnumerator4: stageName = "Elder"; stageColor = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 0.5f, 0.0f, 1.0f)); break;
                    case SDK::Enum_GrowthStage::NewEnumerator5: stageName = "Ancient"; stageColor = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 0.0f, 1.0f, 1.0f)); break;
                    }
                    flagMgr.AddFlag(rect, stageName, stageColor, g_ESP::FlagPos::Right);
                }

                // 添加距离到右侧 flags
                if (g_Config::bDrawDistance) {
                    float distance = LocalPC->Pawn->GetDistanceTo(TargetActor) / 100.0f;
                    flagMgr.AddFlag(rect, std::to_string((int)distance) + "m", g_Config::GetU32Color(g_Config::DistanceColor), g_ESP::FlagPos::Right);
                }
            }
        }
        // --- 如果不在屏幕内且启用了 OOF，绘制 OOF ---
        else if (g_Config::bEnableOOF) {
            std::vector<g_ESP::OOFFlag> oofFlags;

            // 收集名称
            if (g_Config::bDrawName && PlayerChar->PlayerName.IsValid()) {
                oofFlags.push_back({ PlayerChar->PlayerName.ToString(), g_Config::GetU32Color(g_Config::NameColor) });
            }

            // 收集血量文本
            if (g_Config::bDrawHealthBar) {
                std::string hpStr = std::to_string((int)BaseChar->HealthPercent);
                float hpPercent = BaseChar->HealthPercent / 200.0f;
                ImVec4 hpColor;
                if (hpPercent > 0.5f)
                    hpColor = ImVec4((1.0f - hpPercent) * 2.0f, 1.0f, 0.0f, 1.0f);
                else
                    hpColor = ImVec4(1.0f, hpPercent * 2.0f, 0.0f, 1.0f);
                oofFlags.push_back({ hpStr, ImGui::ColorConvertFloat4ToU32(hpColor) });
            }

            // 收集物种
            if (g_Config::bDrawSpecies) {
                std::string speciesName;
                ImU32 speciesColor;
                switch (PlayerChar->CharacterSpecies) {
                case SDK::Enum_PlayerCharacter::NewEnumerator0: speciesName = "FS"; speciesColor = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 0.4f, 0.4f, 1.0f)); break;
                case SDK::Enum_PlayerCharacter::NewEnumerator1: speciesName = "IR"; speciesColor = ImGui::ColorConvertFloat4ToU32(ImVec4(0.4f, 1.0f, 1.0f, 1.0f)); break;
                case SDK::Enum_PlayerCharacter::NewEnumerator2: speciesName = "SS"; speciesColor = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 1.0f, 0.4f, 1.0f)); break;
                case SDK::Enum_PlayerCharacter::NewEnumerator3: speciesName = "ASD"; speciesColor = ImGui::ColorConvertFloat4ToU32(ImVec4(0.6f, 1.0f, 0.4f, 1.0f)); break;
                case SDK::Enum_PlayerCharacter::NewEnumerator5: speciesName = "BS"; speciesColor = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 0.4f, 0.7f, 1.0f)); break;
                case SDK::Enum_PlayerCharacter::NewEnumerator10: speciesName = "BW"; speciesColor = ImGui::ColorConvertFloat4ToU32(ImVec4(0.4f, 0.4f, 1.0f, 1.0f)); break;
                case SDK::Enum_PlayerCharacter::NewEnumerator12: speciesName = "BIO"; speciesColor = ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 1.0f, 1.0f, 1.0f)); break;
                default:
                    speciesName = "Spec_" + std::to_string((unsigned char)PlayerChar->CharacterSpecies);
                    speciesColor = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                    break;
                }
                oofFlags.push_back({ speciesName, speciesColor });
            }

            // 收集成长阶段
            if (g_Config::bDrawGrowth) {
                std::string stageName = "Unknown";
                ImU32 stageColor = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                switch (BaseChar->GrowthStage) {
                case SDK::Enum_GrowthStage::NewEnumerator1: stageName = "Hatchling"; stageColor = ImGui::ColorConvertFloat4ToU32(ImVec4(0.8f, 0.8f, 0.8f, 1.0f)); break;
                case SDK::Enum_GrowthStage::NewEnumerator2: stageName = "Juvenile"; stageColor = ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 1.0f, 0.5f, 1.0f)); break;
                case SDK::Enum_GrowthStage::NewEnumerator3: stageName = "Adult"; stageColor = ImGui::ColorConvertFloat4ToU32(ImVec4(0.1f, 0.6f, 1.0f, 1.0f)); break;
                case SDK::Enum_GrowthStage::NewEnumerator4: stageName = "Elder"; stageColor = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 0.5f, 0.0f, 1.0f)); break;
                case SDK::Enum_GrowthStage::NewEnumerator5: stageName = "Ancient"; stageColor = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 0.0f, 1.0f, 1.0f)); break;
                }
                oofFlags.push_back({ stageName, stageColor });
            }

            // 收集距离
            if (g_Config::bDrawDistance) {
                float distance = LocalPC->Pawn->GetDistanceTo(TargetActor) / 100.0f;
                oofFlags.push_back({ std::to_string((int)distance) + "m", g_Config::GetU32Color(g_Config::DistanceColor) });
            }

            g_ESP::DrawOutOfFOV(TargetActor, LocalPC, oofFlags);
        }
    }
}

void MyImGuiDraw(IDXGISwapChain3* pSwapChain, UINT SyncInterval, UINT Flags)
{
    if (g_MDX12::g_MenuState::g_isOpen) {
        ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);

        if (ImGui::Begin("Begeerte", nullptr)) {

            static ImGuiColorEditFlags color_flags = ImGuiColorEditFlags_AlphaBar |
                ImGuiColorEditFlags_AlphaPreview |
                ImGuiColorEditFlags_NoLabel;

            static float saved_color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

            // 内部辅助 Lambda 函数：绘制颜色选择器
            auto DrawColorPicker = [&](const char* label_id, float* col_ptr) {
                ImGui::ColorEdit4(label_id, col_ptr, color_flags | ImGuiColorEditFlags_NoInputs);

                if (ImGui::BeginPopupContextItem(label_id)) {
                    if (ImGui::MenuItem("Copy Color")) {
                        memcpy(saved_color, col_ptr, sizeof(float) * 4);
                    }
                    if (ImGui::MenuItem("Paste Color")) {
                        memcpy(col_ptr, saved_color, sizeof(float) * 4);
                    }
                    ImGui::EndPopup();
                }
            };

            if (ImGui::BeginTabBar("Main")) {
                // --- 视觉选项卡 (原有 ESP 内容) ---
                if (ImGui::BeginTabItem("Visuals")) {
                    ImGui::Checkbox("Box", &g_Config::bDrawBox);
                    if (g_Config::bDrawBox) {
                        ImGui::SameLine();
                        DrawColorPicker("BoxCol##1", g_Config::BoxColor);
                    }

                    ImGui::Checkbox("Name", &g_Config::bDrawName);
                    if (g_Config::bDrawName) {
                        ImGui::SameLine();
                        DrawColorPicker("NameCol##1", g_Config::NameColor);
                    }

                    ImGui::Checkbox("Health Bar", &g_Config::bDrawHealthBar);

                    ImGui::Checkbox("Enable Out of FOV", &g_Config::bEnableOOF);
                    if (g_Config::bEnableOOF) {
                        ImGui::Text("Arrow Color:");
                        ImGui::SameLine();
                        DrawColorPicker("OOFCol##1", g_Config::OOFColor);

                        ImGui::SliderFloat("Arrow Size", &g_Config::OOFSize, 5.0f, 30.0f, "%.1f");
                        ImGui::SliderFloat("Screen Radius", &g_Config::OOFRadius, 0.5f, 0.95f, "%.2f");
                        ImGui::SliderFloat("Breath Speed", &g_Config::OOFBreathSpeed, 0.5f, 5.0f, "%.1f");
                        ImGui::SliderFloat("Min Alpha", &g_Config::OOFMinAlpha, 0.1f, 0.9f, "%.2f");
                        ImGui::SliderFloat("Max Alpha", &g_Config::OOFMaxAlpha, 0.2f, 1.0f, "%.2f");
                    }

                    ImGui::EndTabItem();
                }

                // --- 标记选项卡 (原有 ESP Flags) ---
                if (ImGui::BeginTabItem("Flags")) {
                    ImGui::Checkbox("Show Species", &g_Config::bDrawSpecies);
                    ImGui::Checkbox("Show Growth Stage", &g_Config::bDrawGrowth);

                    ImGui::Checkbox("Show Distance", &g_Config::bDrawDistance);
                    if (g_Config::bDrawDistance) {
                        ImGui::SameLine();
                        DrawColorPicker("DistCol##1", g_Config::DistanceColor);
                    }

                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }

            ImGui::Separator();
            ImGui::End();
        }
    }

    // 在每一帧渲染时调用绘制函数
    DrawESP();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        g_MDX12::Initialize();
        g_MDX12::SetSetupImGuiCallback(MyImGuiDraw);
        break;

    case DLL_PROCESS_DETACH:
        g_MDX12::FinalCleanupAll();
        break;
    }
    return TRUE;
}