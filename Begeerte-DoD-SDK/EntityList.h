#pragma once
#include <Windows.h>
#include <vector>
#include "Vector.h"
#include "CheatData.h"

namespace EntityList {
    // ������ҽṹ���� LocalPlayer.h һ��
    struct Player {
        byte validFlag;         // +0x0 (208 = valid)

        char padding_1[0x58 - 0x1];
        byte aliveFlag;         // +0x58

        char padding_6[0x759 - 0x58 - 0x1];
        byte Health;            // +0x759

        // ����������Ч����Һͷ����ʵ��
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

    // ˢ��ʵ���б�
    void Update();

    // ��ȡ��ǰʵ��������������������
    size_t GetMaxPlayers();

    // ��ȡָ�� ID ��ʵ���ַ��ID �� 1 ��ʼ��
    DWORD64 GetEntity(int id);

    // ��ȡָ�� ID �� Player �ṹ��ID �� 1 ��ʼ��
    Player* GetPlayer(int id);

    // ��ȡ����ʵ��ĵ�ַ�б�
    const std::vector<DWORD64>& GetAllEntities();
}