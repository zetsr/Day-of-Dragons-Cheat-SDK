#include "Hook.h"
#include "Console.h"
#include "PointerScanner.h"
#include "Offset.h"
#include "LocalPlayer.h"
#include "Memory.h"
#include <thread>
#include <stdio.h>

namespace Hook {
    static bool isRunning = false;
    static HANDLE hookThread = nullptr;

    static DWORD64 LocalPlayerAddress = 0;
    static DWORD64 LocalCameraPositionAddress = 0;
    static DWORD64 ClientCurrentTimeAddress = 0;
    static DWORD64 ClientCurrentTickAddress = 0;

    static void UpdateThread() {
        DWORD64 moduleBase = Memory::GetModuleBase("Dragons-Win64-Shipping.exe");
        std::vector<std::pair<DWORD64, bool>> debugSteps;

        while (isRunning) {
            DWORD64 local_player_base = moduleBase + Offset::LocalPlayerBase::MODULE_OFFSET;
            auto [local_player_address, local_player_success] = PointerScanner::Scan(local_player_base, Offset::LocalPlayerBase::OFFSETS, debugSteps);
            LocalPlayerAddress = local_player_success ? local_player_address : 0;
        }
    }

    void Initialize() {
        Console::Initialize();
        isRunning = true;
        hookThread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)UpdateThread, nullptr, 0, nullptr);
        printf("Hook initialized.\n");
    }

    void Cleanup() {
        isRunning = false;
        if (hookThread) {
            WaitForSingleObject(hookThread, INFINITE);
            CloseHandle(hookThread);
            hookThread = nullptr;
        }
        Console::Cleanup();
    }

    DWORD64 GetLocalPlayerAddress() {
        return LocalPlayerAddress;
    }

    DWORD64 GetLocalCameraPositionAddress() {
        return LocalCameraPositionAddress;
    }

    DWORD64 GetClientCurrentTimeAddress() {
        return ClientCurrentTimeAddress;
    }

    DWORD64 GetClientCurrentTickAddress() {
        return ClientCurrentTickAddress;
    }
}