//
//  Haruhi Engine
// 
//  This file is of derivation from Kunamo Engine source code & has been adapted for integration of Haruhi Engine.
//  The following software is DIRECTLY seeded from Kunamo Engine source with minuscle adjustments to architecture, if any.
//
//  Portions or sectors of this file originate from Kunamo Engine source code and may include minimal modifications
// 
//  Original source code derived from Kunamo Engine:
//    Copyright © 2026 Kunamo Entertainment. All rights reserved.
//
//  Modifications & Integration:
//    Copyright © 2026 AveriC & Averi
//

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <windows.h>
    #include <intrin.h>
#endif

#include "../../Platform.h"

HaruArchitecture HaruPlatformRetrieveArchitecture() {
    #if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) || defined(_M_X64)
        return HARU_ARCH_X64;
    #elif defined(__aarch64__) || defined(_M_ARM64)
        return HARU_ARCH_ARM64;
    #elif defined(__i386__) || defined(_M_IX86)
        return HARU_ARCH_X86;
    #else
        return HARU_ARCH_UNKNOWN;
    #endif
}

HaruProcessorInformation HaruPlatformRetrieveCPUCoreAndThreadCount() {
    HaruProcessorInformation ProcessorInformation = {0};

    DWORD Length = 0;
    
    GetLogicalProcessorInformation(NULL, &Length);
    
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION Buffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION) malloc(Length);

    GetLogicalProcessorInformation(Buffer, &Length);

    int CPUCores = 0;
    int CPUThreads = 0;

    int Elements = Length / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);

    for (int i = 0; i < Elements; i++) {
        if (Buffer[i].Relationship == RelationProcessorCore) {
            CPUCores++;
            
            ULONG_PTR Mask = Buffer[i].ProcessorMask;

            while (Mask) {
                if (Mask & 1)
                    CPUThreads++;

                Mask >>= 1;
            }
        }
    }

    ProcessorInformation.CoreCount = CPUCores;
    ProcessorInformation.ThreadCount = CPUThreads;

    free(Buffer);

    return ProcessorInformation;
}

HaruSIMDInformation HaruPlatformRetrieveCPUSIMDSupportType() {
    HaruSIMDInformation CPUSIMDInformation = {0, "None"};
    
    int CPUInfo[4];

    __cpuidex(CPUInfo, 1, 0);

    int HasSSE = (CPUInfo[3] >> 25) & 1;
    int HasSSE2 = (CPUInfo[3] >> 26) & 1;

    int HasSSE3 = (CPUInfo[2] >> 0) & 1;
    int HasSSSE3 = (CPUInfo[2] >> 9) & 1;
    int HasSSE41 = (CPUInfo[2] >> 19) & 1;
    int HasSSE42 = (CPUInfo[2] >> 20) & 1;
    
    int HasAVX = (CPUInfo[2] >> 28) & 1;
    
    if (HasSSE || HasSSE2 || HasSSE3 || HasSSSE3 || HasSSE41 || HasSSE42 || HasAVX) {
        CPUSIMDInformation.Supported = 1;
    }

    if (HasAVX) {
        CPUSIMDInformation.Type = "AVX";
    } else if (HasSSE42) {
        CPUSIMDInformation.Type = "SSE4.2";
    } else if (HasSSE41) {
        CPUSIMDInformation.Type = "SSE4.1";
    } else if (HasSSSE3) {
        CPUSIMDInformation.Type = "SSSE3";
    } else if (HasSSE3) {
        CPUSIMDInformation.Type = "SSE3";
    } else if (HasSSE2) {
        CPUSIMDInformation.Type = "SSE2";
    } else if (HasSSE) {
        CPUSIMDInformation.Type = "SSE";
    }
    
    return CPUSIMDInformation;
}

char *HaruPlatformRetrieveRAMGeneration() {
    char Buffer[128];
    static char DDRType[16] = "Unknown";

    HKEY Key;

    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &Key) != ERROR_SUCCESS)
        return DDRType;
        
    RegCloseKey(Key);

    int TypeCode = 0;

    FILE *GenerationPipe = _popen("powershell -NoProfile -Command \"(Get-CimInstance Win32_PhysicalMemory).SMBIOSMemoryType | Select-Object -First 1\"", "r");
    if (!GenerationPipe) {
        fprintf(stderr, "Failed to run command\n");

        return DDRType;
    }

    while (fgets(Buffer, sizeof(Buffer), GenerationPipe)) {
        char *Pointer = Buffer;

        while (*Pointer && !isdigit(*Pointer))
            Pointer++;
        
        if (isdigit(*Pointer)) {
            TypeCode = atoi(Pointer);

            break; 
        }
    }
    
    _pclose(GenerationPipe);

    switch (TypeCode) {
        case 20: strcpy(DDRType, "DDR"); break;
        case 21: strcpy(DDRType, "DDR2"); break;
        case 24: strcpy(DDRType, "DDR3"); break;
        case 26: strcpy(DDRType, "DDR4"); break;
        case 34: strcpy(DDRType, "DDR5"); break;

        default: sprintf(DDRType, "Code %d", TypeCode); break;
    }

    return DDRType;
}

int HaruPlatformRetrieveRAMSpeed() {
    char Buffer[128];

    int RAMSpeed = 0;

    FILE *SpeedPipe = _popen("powershell -NoProfile -Command \"(Get-CimInstance Win32_PhysicalMemory).Speed | Select-Object -First 1\"", "r");
    if (!SpeedPipe) {
        fprintf(stderr, "Failed to run command\n");

        return RAMSpeed;
    }

    if (SpeedPipe) {
        while (fgets(Buffer, sizeof(Buffer), SpeedPipe)) {
            char *Pointer = Buffer;

            while (*Pointer && !isdigit(*Pointer))
                Pointer++;

            if (isdigit(*Pointer)) {
                RAMSpeed = atoi(Pointer);

                break; 
            }
        }

        _pclose(SpeedPipe);
    }

    return RAMSpeed;
}

HaruPlatform HaruPlatformInitialize() {
    SYSTEM_INFO SystemInformation = {0};
    MEMORYSTATUSEX MemoryInformation = {0};

    HaruPlatform Platform = {0};

    HaruProcessorInformation CPUProcessingInformation = HaruPlatformRetrieveCPUCoreAndThreadCount();
    HaruSIMDInformation CPUSIMDInformation = HaruPlatformRetrieveCPUSIMDSupportType();

    MemoryInformation.dwLength = sizeof(MemoryInformation);

    if (!GlobalMemoryStatusEx(&MemoryInformation)) {
        fprintf(stderr, "GlobalMemoryStatusEx failed: %lu\n", GetLastError());
    }

    GetSystemInfo(&SystemInformation);
    GlobalMemoryStatusEx(&MemoryInformation);

    Platform.OperatingSystem = HARU_OS_WINDOWS;
    Platform.Architecture = HaruPlatformRetrieveArchitecture();

    Platform.HaruHardwareInformation.HaruCPUInformation.CPUCoreCount = CPUProcessingInformation.CoreCount;
    Platform.HaruHardwareInformation.HaruCPUInformation.CPUThreadCount = CPUProcessingInformation.ThreadCount;

    Platform.HaruHardwareInformation.HaruCPUInformation.SIMDSupport = CPUSIMDInformation.Supported;
    Platform.HaruHardwareInformation.HaruCPUInformation.SIMDType = CPUSIMDInformation.Type;

    Platform.HaruHardwareInformation.HaruRAMInformation.TotalRAMBytes = MemoryInformation.ullTotalPhys;
    Platform.HaruHardwareInformation.HaruRAMInformation.AvailableRAMBytes = MemoryInformation.ullAvailPhys;

    Platform.HaruHardwareInformation.HaruRAMInformation.ExtraRAMInformation.RAMVersion = HaruPlatformRetrieveRAMGeneration();
    Platform.HaruHardwareInformation.HaruRAMInformation.ExtraRAMInformation.RAMSpeed = (uint32_t) HaruPlatformRetrieveRAMSpeed();

    return Platform;
}
