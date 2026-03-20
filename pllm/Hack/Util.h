#pragma once

namespace g_Util {
    // 辅助函数：将 float[4] 转换为 ImU32
    inline ImU32 GetU32Color(float color[4]) {
        return ImGui::ColorConvertFloat4ToU32(*(ImVec4*)color);
    }

    // 辅助函数：将 r, g, b, a 转换为 ImU32
    inline ImU32 ToImColor(float r, float g, float b, float a) {
        return ImGui::ColorConvertFloat4ToU32(ImVec4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f));
    }

    inline SDK::FString GetEntityName(SDK::AActor* entity) {
        // 使用 FString 初始化默认显示内容
        SDK::FString fName = L"未知生物";

        // 1. 尝试作为玩家处理
        if (entity && entity->IsA(SDK::AChar_Parent_Player_C::StaticClass())) {
            SDK::AChar_Parent_Player_C* PlayerChar = (SDK::AChar_Parent_Player_C*)entity;
            if (PlayerChar && PlayerChar->PlayerName.IsValid()) {
                fName = PlayerChar->PlayerName;
            }
        }

        // 2. 否则作为 AI 处理
        else {
            SDK::AChar_Parent_All_C* BaseChar = (SDK::AChar_Parent_All_C*)entity;
            if (BaseChar) {
                switch (BaseChar->BiologicalSpecies) {
                case SDK::Enum_Species::NewEnumerator11: fName = L"小龙虾"; break;
                case SDK::Enum_Species::NewEnumerator12: fName = L"螃蟹"; break;
                case SDK::Enum_Species::NewEnumerator17: fName = L"鳄鱼"; break;
                default: fName = L"AI生物"; break;
                }
            }
        }

        return fName;
    }

    inline SDK::FString GetClanName(SDK::AActor* entity) {
        // 使用 FString 初始化默认显示内容
        SDK::FString fName = L"";

        if (entity && entity->IsA(SDK::AChar_Parent_All_C::StaticClass())) {
            SDK::AChar_Parent_All_C* BaseChar = (SDK::AChar_Parent_All_C*)entity;
            if (entity && BaseChar) {
                SDK::APlayerState* TargetPS = BaseChar->PlayerState;
                if (entity && BaseChar && TargetPS) {
                    SDK::ADragonsPS_C* targetDragonPS = (SDK::ADragonsPS_C*)TargetPS;
                    if (entity && BaseChar && TargetPS && targetDragonPS) {
                        fName = targetDragonPS->ClanName;
                    }
                }
            }
        }

        return fName;
    }
}