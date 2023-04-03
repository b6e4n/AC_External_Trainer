// AC_External_Trainer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "stdafx.h"

#include <iostream>
#include <vector>
#include <Windows.h>
#include "proc.h"
#include "globalOffsets.h"
#include "mem.h"

int main() {

    HANDLE hProcess = 0;

    uintptr_t moduleBase = 0, localPlayerPtr = 0, healthAddr = 0, armorAddr = 0;
    bool bHealth = false, bAmmo = false, bRecoil = false, bArmor = false;

    const int newHealthValue = 1337;
    const int newArmorValue = 1337;

    DWORD procId = GetProcId(L"ac_client.exe");

    if (procId) {
        hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);
        moduleBase = GetModuleBaseAddress(procId, L"ac_client.exe");

        localPlayerPtr = moduleBase + OFFSET_PLAYER_CLASS;
        healthAddr = FindDMAAddy(hProcess, localPlayerPtr, { OFFSET_HEALTH });
        armorAddr = FindDMAAddy(hProcess, localPlayerPtr, { OFFSET_ARMOR });

    }
    else {
        std::cout << "Process not found, press enter to exit" << std::endl;
        getchar();
        return 0;
    }

    DWORD dwExit = 0;

    while (GetExitCodeProcess(hProcess, &dwExit) && dwExit == STILL_ACTIVE) {

        //P
        if (GetAsyncKeyState(0x50) & 1) {
            bHealth = !bHealth;
        }

        //O
        if (GetAsyncKeyState(0x4F) & 1) {
            bAmmo = !bAmmo;

            if (bAmmo) {
                //ff 06 = inc [esi]
                mem::PatchEx((BYTE*)(moduleBase + OFFSET_WRITE_AMMO), (BYTE*)"\xFF\x06", 2, hProcess);
            }
            else {
                //ff 0e = dec [esi]
                mem::PatchEx((BYTE*)(moduleBase + OFFSET_WRITE_AMMO), (BYTE*)"\xFF\x0E", 2, hProcess);
            }
        }

        //I
        if (GetAsyncKeyState(0x49) & 1) {
            bArmor = !bArmor;
        }

        //U
        if (GetAsyncKeyState(0x55) & 1) {
            bRecoil = !bRecoil;

            if (bRecoil) {
            mem::NopEx((BYTE*)(moduleBase + OFFSET_WRITE_RECOIL), 10, hProcess);
            }
            else {
                mem::PatchEx((BYTE*)(moduleBase + OFFSET_WRITE_RECOIL), (BYTE*)"\x50\x8d\x4c\x24\x1c\x51\x8b\xce\xff\xd2", 10, hProcess);
            }

        }

        if (GetAsyncKeyState(VK_INSERT) & 1) {
            return 0;
        }

        if (bHealth) {
            mem::PatchEx((BYTE*)healthAddr, (BYTE*)&newHealthValue, sizeof(newHealthValue), hProcess);
        }

        if (bArmor) {
            mem::PatchEx((BYTE*)armorAddr, (BYTE*)&newArmorValue, sizeof(newArmorValue), hProcess);
        }
        Sleep(10);
    }

    std::cout << "Process not found, press enter to exit" << std::endl;
    getchar();
    return 0;
}

