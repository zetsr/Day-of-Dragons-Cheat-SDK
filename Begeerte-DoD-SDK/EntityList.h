#pragma once
#include <Windows.h>
#include <vector>
#include "Vector.h"
#include "CheatData.h"

namespace EntityList {
    // 定义玩家结构，与 LocalPlayer.h 一致
    struct Player {
        byte validFlag;         // +0x0 (208 = valid)

        char padding_1[0x58 - 0x1];
        byte aliveFlag;         // +0x58

        char padding_6[0x759 - 0x58 - 0x1];
        byte Health;            // +0x759

        // 包括所有有效的玩家和非玩家实体
        bool IsValid() const {
            return validFlag == g_cheatdata->EntityValidFlag;
        }

        bool IsAlive() const {
            return aliveFlag == g_cheatdata->EntityAliveFlag;
        }

        bool IsDead() const {
            return aliveFlag == g_cheatdata->EntityDeadFlag;
        }
    };

    // 刷新实体列表
    void Update();

    // 获取当前实体数量（最大玩家数量）
    size_t GetMaxPlayers();

    // 获取指定 ID 的实体地址（ID 从 1 开始）
    DWORD64 GetEntity(int id);

    // 获取指定 ID 的 Player 结构（ID 从 1 开始）
    Player* GetPlayer(int id);

    // 获取所有实体的地址列表
    const std::vector<DWORD64>& GetAllEntities();
}