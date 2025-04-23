#pragma once
#include <Windows.h>
#include <vector>

// ������ά������ָ��
namespace Offset {
    namespace LocalPlayerBase {
        static constexpr DWORD64 MODULE_OFFSET = 0x085AF500;
        static const std::vector<DWORD64> OFFSETS{ 0x30, 0x2E0, 0x320, 0x150, 0x0 };
    }
    // ����ʵ���б� (EntityList) ��ָ��ɨ�����
    namespace EntityList {
        static constexpr DWORD64 MODULE_OFFSET = 0x085CA2B0;                  // ʵ���б�Ļ�ַƫ��
        static const std::vector<DWORD64> FIXED_PREFIX_OFFSETS{ 0x8, 0x8 };   // ǰ�ù̶�ƫ��
        static constexpr DWORD64 ENTITY_OFFSET_START = 0x0;                 // ʵ��ƫ�Ʒ�Χ���
        static constexpr DWORD64 ENTITY_OFFSET_END = 0x4000;                  // ʵ��ƫ�Ʒ�Χ�յ�
        static constexpr DWORD64 ENTITY_OFFSET_STEP = 0x10;                   // ʵ��ƫ�Ʋ���
        static const std::vector<DWORD64> FIXED_SUFFIX_OFFSETS{ 0x60, 0x0 };  // ���ù̶�ƫ��
        // ��Ӧ���������� // static constexpr BYTE TARGET_VALUE = 208;                      // Ŀ���ֽ�ֵ������ɸѡʵ��
        // ��ҪӲ���� // static constexpr size_t MAX_ENTITY_COUNT = 200;                      // ���ʵ������ (id1 �� id200)
    }
}