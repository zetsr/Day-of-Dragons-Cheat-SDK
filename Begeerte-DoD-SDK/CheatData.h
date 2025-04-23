#ifndef CHEAT_DATA_H
#define CHEAT_DATA_H

#include <windows.h>

struct CheatData {
    byte EntityValidFlag;
    byte EntityAliveFlag;
    byte EntityDeadFlag;
};

inline static CheatData cheatDataInstance = {
    .EntityValidFlag = 208,
    .EntityAliveFlag = 112,
    .EntityDeadFlag = 96
};

// 全局指针，指向静态实例
inline static CheatData* g_cheatdata = &cheatDataInstance;

#endif // CHEAT_DATA_H