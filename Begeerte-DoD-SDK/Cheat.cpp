#include "Cheat.h"
#include "Hook.h"
#include "Console.h"
#include "Memory.h"
#include <thread>
#include <stdio.h>
#include "SDK.h"
#include "Vector.h"
#include "EntityList.h"

namespace Cheat {
    static bool isRunning = false;
    static HANDLE cheatThread = nullptr;

    static void UpdateThread() {
        // 暂时不需要它
    }

    static void CheatThread() {
        while (isRunning) {
            // 更新所有地址
            UpdateThread();

            auto local_player = LocalPlayer::GetPlayer(Hook::GetLocalPlayerAddress());

            // 如果本地玩家空指针直接跳过
            if (!local_player) {
                printf("No player data.\n");
                continue;
            }

            // 如果本地玩家无效直接跳过
            if (!local_player->IsValid()) {
                printf("Player invalid.\n");
                continue;
            }

            // 如果本地玩家死亡直接跳过
            //// 似乎已经被空指针与有效性检查跳过 // 并不，实际上在某些情况下有用 // 并不，有时候会在本地玩家存活的时候莫名其妙的认为本地玩家死了
            if (!local_player->IsAlive()) {
                printf("Player dead.\n");
                continue;
            }

            printf("Character: %s (%d) | Gender: %s | GrowthStage: %s (%d) | SkinIndex: %d | Health: %.1f | Armor: %.1f | ViewAngles: (%.1f, %.1f) | CamDistance: %.1f\n",
                local_player->GetCharacter(),
                local_player->Character,
                local_player->GetGender(),
                local_player->GetGrowthStage(),
                local_player->GrowthStage,
                local_player->SkinIndex,
                local_player->Health,
                local_player->Armor,
                local_player->ViewAngleX,
                local_player->ViewAngleY,
                local_player->CameraDistance

            );

            // 在本地玩家有效且存活的时候更新实体列表
            EntityList::Update();

            for (int i = 1; i <= EntityList::GetMaxPlayers(); i++) {

                auto entity = EntityList::GetEntity(i);

                // 没有实体
                if (entity == 0) {
                    continue;
                }

                auto player = EntityList::GetPlayer(i);

                // 空指针
                if (!player) {
                    continue;
                }

                // 无效实体
                if (!player->IsValid()) {
                    continue;
                }

                // 为什么我们不跳过 Health 为 0 的实体？他妈的混乱的内存管理
                // 非玩家实体没有 Health 吗
                printf("Entity (%d) | HP: %d\n",
                    i,
                    player->Health
                );
            }
        }
    }

    void Start() {
        if (!isRunning) {
            isRunning = true;
            cheatThread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)CheatThread, nullptr, 0, nullptr);
            printf("Cheat thread started.\n");
        }
    }

    void Stop() {
        isRunning = false;
        if (cheatThread) {
            WaitForSingleObject(cheatThread, INFINITE);
            CloseHandle(cheatThread);
            cheatThread = nullptr;
        }
        printf("Cheat thread stopped.\n");
    }
}