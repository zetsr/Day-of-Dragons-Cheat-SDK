#include "../Minimal-D3D12-Hook-ImGui-1.0.0/Main/mdx12_api.h"
#include "../CppSDK/SDK.hpp"
#include "ESP.h"
#include "Configs.h"

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
        if (Pawn == LocalPC->Pawn) continue;

        auto TargetActor = reinterpret_cast<SDK::AActor*>(Pawn);
        if (!TargetActor) continue;

        auto BaseChar = reinterpret_cast<SDK::AChar_Parent_All_C*>(TargetActor);
        if (!BaseChar || BaseChar->IsDead) continue;

        auto PlayerChar = reinterpret_cast<SDK::AChar_Parent_Player_C*>(TargetActor);
        if (!PlayerChar) continue;

        g_ESP::ResetFlagOffsets();

        g_ESP::BoxRect rect;
        if (g_Config::bDrawBox) {
            rect = g_ESP::DrawBox(TargetActor,
                g_Config::BoxColor[0] * 255.0f,
                g_Config::BoxColor[1] * 255.0f,
                g_Config::BoxColor[2] * 255.0f,
                g_Config::BoxColor[3] * 255.0f, 0.5f);
        }
        else {
            // 如果不画框，传透明色(alpha=0)来静默获取 rect 坐标
            rect = g_ESP::DrawBox(TargetActor, 0, 0, 0, 0, 0.5f);
        }

        if (!rect.valid) continue;

        if (g_Config::bDrawHealthBar) {
            g_ESP::DrawHealthBar(rect, (float)BaseChar->HealthPercent, 200.0f, 255.0f);
        }

        if (g_Config::bDrawName) {
            g_ESP::DrawName(TargetActor, rect,
                g_Config::NameColor[0] * 255.0f,
                g_Config::NameColor[1] * 255.0f,
                g_Config::NameColor[2] * 255.0f,
                g_Config::NameColor[3] * 255.0f);
        }

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
            g_ESP::RenderFlag(rect, speciesName, speciesColor, g_ESP::FlagPos::Right);
        }

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
            g_ESP::RenderFlag(rect, stageName, stageColor, g_ESP::FlagPos::Right);
        }

        if (g_Config::bDrawDistance) {
            float distance = LocalPC->Pawn->GetDistanceTo(TargetActor) / 100.0f;
            std::string distStr = std::to_string((int)distance) + "m";
            g_ESP::RenderFlag(rect, distStr, g_Config::GetU32Color(g_Config::DistanceColor), g_ESP::FlagPos::Right);
        }
    }
}

void MyImGuiDraw(IDXGISwapChain3* pSwapChain, UINT SyncInterval, UINT Flags)
{
    if (g_MDX12::g_MenuState::g_isOpen) {
        ImGui::SetNextWindowSize(ImVec2(450, 400), ImGuiCond_FirstUseEver);

        if (ImGui::Begin("Begeerte", nullptr)) {

            static ImGuiColorEditFlags color_flags = ImGuiColorEditFlags_AlphaBar |
                ImGuiColorEditFlags_AlphaPreview |
                ImGuiColorEditFlags_NoLabel;

            static float saved_color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

            auto DrawColorPicker = [&](const char* label_id, float* col_ptr) {
                ImGui::ColorEdit4(label_id, col_ptr, color_flags | ImGuiColorEditFlags_NoInputs);

                if (ImGui::BeginPopupContextItem(label_id)) {
                    if (ImGui::MenuItem("Copy")) {
                        memcpy(saved_color, col_ptr, sizeof(float) * 4);
                    }
                    if (ImGui::MenuItem("Paste")) {
                        memcpy(col_ptr, saved_color, sizeof(float) * 4);
                    }
                    ImGui::EndPopup();
                }
            };

            if (ImGui::BeginTabBar("Tabs")) {
                if (ImGui::BeginTabItem("Visuals")) {

                    ImGui::Checkbox("Box", &g_Config::bDrawBox);
                    if (g_Config::bDrawBox) {
                        ImGui::SameLine();
                        DrawColorPicker("BoxCol##1", g_Config::BoxColor);
                    }

                    ImGui::Checkbox("Health Bar", &g_Config::bDrawHealthBar);

                    ImGui::Checkbox("Player Name", &g_Config::bDrawName);
                    if (g_Config::bDrawName) {
                        ImGui::SameLine();
                        DrawColorPicker("NameCol##1", g_Config::NameColor);
                    }

                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Flags")) {

                    ImGui::Checkbox("Species", &g_Config::bDrawSpecies);
                    ImGui::Checkbox("Growth Stage", &g_Config::bDrawGrowth);

                    ImGui::Checkbox("Distance", &g_Config::bDrawDistance);
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