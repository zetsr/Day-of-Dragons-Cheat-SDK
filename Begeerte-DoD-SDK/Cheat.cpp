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
        // ��ʱ����Ҫ��
    }

    static void CheatThread() {
        while (isRunning) {
            // �������е�ַ
            UpdateThread();

            auto local_player = LocalPlayer::GetPlayer(Hook::GetLocalPlayerAddress());

            // ���������ҿ�ָ��ֱ������
            if (!local_player) {
                printf("No player data.\n");
                continue;
            }

            // ������������Чֱ������
            if (!local_player->IsValid()) {
                printf("Player invalid.\n");
                continue;
            }

            // ��������������ֱ������
            //// �ƺ��Ѿ�����ָ������Ч�Լ������ // ������ʵ������ĳЩ��������� // ��������ʱ����ڱ�����Ҵ���ʱ��Ī���������Ϊ�����������
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

            // �ڱ��������Ч�Ҵ���ʱ�����ʵ���б�
            EntityList::Update();

            for (int i = 1; i <= EntityList::GetMaxPlayers(); i++) {

                auto entity = EntityList::GetEntity(i);

                // û��ʵ��
                if (entity == 0) {
                    continue;
                }

                auto player = EntityList::GetPlayer(i);

                // ��ָ��
                if (!player) {
                    continue;
                }

                // ��Чʵ��
                if (!player->IsValid()) {
                    continue;
                }

                // Ϊʲô���ǲ����� Health Ϊ 0 ��ʵ�壿����Ļ��ҵ��ڴ����
                // �����ʵ��û�� Health ��
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