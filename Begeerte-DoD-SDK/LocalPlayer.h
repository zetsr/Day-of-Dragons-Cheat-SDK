#pragma once
#include <Windows.h>
#include "SDK.h"

namespace LocalPlayer {
    struct Player {
        // 起始点
        byte validFlag;        // +0x0 (216 = valid)

        char padding_0[0x6C8 - 0x1];
        byte SkinIndex;        // +0x6C8
        byte Gender;           // +0x6C9

        char padding_1[0x6D4 - 0x6CA];
        float Health;          // +0x6D4
        float padding_2;       // +0x6D8
        float Armor;           // +0x6DC

        byte Stamina;          // +0x6E0
        byte Bile;             // +0x6E1
        byte Hunger;           // +0x6E2
        byte Thirsty;          // +0x6E3
        byte Disease;          // +0x6E4

        byte padding_4;        // +0x6E5 // 0 = Normal, else = 动画停止更新
        byte GrowthStage;      // +0x6E6
        char padding_5[0x6F8 - 0x6E7];
        byte VitalityHealth;            // +0x6F8 VitalityHealth_20_AD4FDC3944007D89AA165EA075C7A6A4
        byte VitalityArmor;             // +0x6F9 VitalityArmor_21_6058776047A93E704725F2B1017143B5
        byte VitalityBile;              // +0x6FA VitalityBile_22_1441154144FB80F99B0EA4ACCF940906
        byte VitalityStamina;           // +0x6FB VitalityStamina_23_4F7A985E444EDC983C8689B3141CAD36
        byte VitalityHunger;            // +0x6FC VitalityHunger_24_EC22292447DC9308AF7CAC8610072A5F
        byte VitalityThirst;            // +0x6FD VitalityThirst_25_9C3850144E3CD527F38898A71F752B8E
        byte VitalityTorpor;            // +0x6FE VitalityTorpor_26_8C2EE49042020674C4A1D4A05033BEE5
        byte DamageBite;                // +0x6FF DamageBite_30_1DF6387A4C50C4FFC3A80EA0664039FB
        byte DamageProjectile;          // +0x700 DamageProjectile_31_4AC9E8454D323D359F6CF089EBB5BEA6
        byte DamageSwipe;               // +0x701 DamageSwipe_32_7D472C2D4AC0E83F7B3050B9A5C7C389
        byte MitigationBlunt;           // +0x702 MitigationBlunt_41_DFC896ED4753A11A442EA489C8412BA5
        byte MitigationPierce;          // +0x703 MitigationPierce_42_AA53F66F45D0660054C2F18F6D587327
        byte MitigationFire;            // +0x704 MitigationFire_43_CC713117436D25BCC7F49B9EFBE6029B
        byte MitigationFrost;           // +0x705 MitigationFrost_44_6F1B808C4FED477721AA2EA984E5B3EA
        byte MitigationAcid;            // +0x706 MitigationAcid_45_E41BF8144F4656DDBFACB8915D9BF109
        byte MitigationVenom;           // +0x707 MitigationVenom_46_1F3565A44951C3A615F994822E39C4F2
        byte MitigationPlasma;          // +0x708 MitigationPlasma_47_1D8376A54DB8117876136FA72072B6A0
        byte MitigationElectricity;     // +0x709 MitigationElectricity_50_F0848404440B35626E72B398C528F968
        byte OverallQuality;            // +0x70A OverallQuality_56_56BD12AD4526647ACEF337ADFD3E2847

        char padding_6[0x738 - 0x70B];
        byte Character;         // +0x738

        char padding_7[0x778 - 0x739];
        byte Alive;             // +0x778

        char padding_8[0x924 - 0x779];
        float CameraDistance;   // +0x924

        char padding_9[0xADC - 0x928];
        float ViewAngleX;       // +0xADC
        float padding_10;       // +0xAE0
        float ViewAngleY;       // +0xAE4

        /*
        char padding_13[0xEAC - 0x9E4];
        float fsPositionX;      // +0xEAC
        float fsPositionY;      // +0xEB0
        float fsPositionZ;      // +0xEB4
        */

        bool IsValid() const {
            return validFlag == 144;
        }

        bool IsAlive() const {
            return Alive == 1;
        }

        const char* GetGender() const {
            auto Gender_Name = "Unknown";
            auto Gender_Index = Gender;

            if (Gender_Index == 0) {
                Gender_Name = "Female";
            }

            if (Gender_Index == 1) {
                Gender_Name = "Male";
            }

            return Gender_Name;
        }

        const char* GetCharacter() {
            auto Character_Name = "Unknown";
            auto Character_Index = Character;

            switch (Character_Index) {
            case SDK::Enum_PlayerCharacter::FlameStalker:
                Character_Name = "FlameStalker";
                break;
            case SDK::Enum_PlayerCharacter::InfernoRavager:
                Character_Name = "InfernoRavager";
                break;
            case SDK::Enum_PlayerCharacter::ShadowScale:
                Character_Name = "ShadowScale";
                break;
            case SDK::Enum_PlayerCharacter::AcidSpitter:
                Character_Name = "AcidSpitter";
                break;
            case SDK::Enum_PlayerCharacter::BlitzStriker:
                Character_Name = "BlitzStriker";
                break;
            case SDK::Enum_PlayerCharacter::Biolumin:
                Character_Name = "Biolumin";
                break;
            default:
                Character_Name = "Unknown";
                break;
            }

            return Character_Name;
        }

        const char* GetGrowthStage() {
            auto GrowthStage_Name = "Unknown";
            auto GrowthStage_Index = GrowthStage;

            switch (GrowthStage_Index) {
            case SDK::Enum_GrowthStage::Hatchling:
                GrowthStage_Name = "Hatchling";
                break;
            case SDK::Enum_GrowthStage::Juvenile:
                GrowthStage_Name = "Juvenile";
                break;
            case SDK::Enum_GrowthStage::Adult:
                GrowthStage_Name = "Adult";
                break;
            case SDK::Enum_GrowthStage::Elder:
                GrowthStage_Name = "Elder";
                break;
            case SDK::Enum_GrowthStage::Ancient:
                GrowthStage_Name = "Ancient";
                break;
            default:
                GrowthStage_Name = "Unknown";
                break;
            }

            return GrowthStage_Name;
        }
    };

    Player* GetPlayer(DWORD64 baseAddress);
}