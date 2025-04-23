#pragma once
#include <Windows.h>
#include <vector>

// 在这里维护所有指针
namespace Offset {
    namespace LocalPlayerBase {
        static constexpr DWORD64 MODULE_OFFSET = 0x085AF500;
        static const std::vector<DWORD64> OFFSETS{ 0x30, 0x2E0, 0x320, 0x150, 0x0 };
    }
    // 定义实体列表 (EntityList) 的指针扫描参数
    namespace EntityList {
        static constexpr DWORD64 MODULE_OFFSET = 0x085CA2B0;                  // 实体列表的基址偏移
        static const std::vector<DWORD64> FIXED_PREFIX_OFFSETS{ 0x8, 0x8 };   // 前置固定偏移
        static constexpr DWORD64 ENTITY_OFFSET_START = 0x0;                 // 实体偏移范围起点
        static constexpr DWORD64 ENTITY_OFFSET_END = 0x4000;                  // 实体偏移范围终点
        static constexpr DWORD64 ENTITY_OFFSET_STEP = 0x10;                   // 实体偏移步长
        static const std::vector<DWORD64> FIXED_SUFFIX_OFFSETS{ 0x60, 0x0 };  // 后置固定偏移
        // 不应该在这里检查 // static constexpr BYTE TARGET_VALUE = 208;                      // 目标字节值，用于筛选实体
        // 不要硬编码 // static constexpr size_t MAX_ENTITY_COUNT = 200;                      // 最大实体数量 (id1 到 id200)
    }
}