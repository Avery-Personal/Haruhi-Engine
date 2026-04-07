//
//  Haruhi Engine
//
//  Copyright © 2026 AveriC & Averi
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <unistd.h>
#include <sys/sysinfo.h>

#include "../../Platform.h"

HaruArchitecture HaruPlatformRetrieveArchitecture() {
    #if defined(__x86_64__) || defined(_M_X64)
        return HARU_ARCH_X64;
    #elif defined(__aarch64__)
        return HARU_ARCH_ARM64;
    #elif defined(__i386__) || defined(_M_IX86)
        return HARU_ARCH_X86;
    #else
        return HARU_ARCH_UNKNOWN;
    #endif
}

HaruProcessorInformation HaruPlatformRetrieveCPUCoreAndThreadCount() {
    HaruProcessorInformation Information = {0};

    long Logical = sysconf(_SC_NPROCESSORS_ONLN);
    if (Logical > 0) {
        Information.ThreadCount = (int) Logical;
    }

    FILE *CPUPipe = fopen("/proc/cpuinfo", "r");
    if (CPUPipe) {
        char Line[256];

        int Cores = 0;

        while (fgets(Line, sizeof(Line), CPUPipe)) {
            if (strncmp(Line, "CPU Cores", 9) == 0) {
                int Value = 0;

                sscanf(Line, "CPU Cores : %d", &Value);

                Cores = Value;

                break;
            }
        }

        fclose(CPUPipe);

        Information.CoreCount = Cores > 0 ? Cores : (int) Logical;
    } else {
        Information.CoreCount = Information.ThreadCount;
    }

    return Information;
}

HaruSIMDInformation HaruPlatformRetrieveCPUSIMDSupportType() {
    HaruSIMDInformation Information = {0, "None"};

    FILE *CPUPipe = fopen("/proc/cpuinfo", "r");
    if (!CPUPipe) return Information;

    char Line[1024];
    int HasSSE = 0, HasAVX = 0;

    while (fgets(Line, sizeof(Line), CPUPipe)) {
        if (strncmp(Line, "flags", 5) == 0 || strncmp(Line, "Features", 8) == 0) {
            if (strstr(Line, "avx")) HasAVX = 1;
            if (strstr(Line, "sse")) HasSSE = 1;
        }
    }

    fclose(CPUPipe);

    if (HasAVX) {
        Information.Supported = 1;
        Information.Type = "AVX";
    } else if (HasSSE) {
        Information.Supported = 1;
        Information.Type = "SSE";
    }

    #if defined(__aarch64__)
        Information.Supported = 1;
        Information.Type = "NEON";
    #endif

    return Information;
}

char *HaruPlatformRetrieveRAMGeneration() {
    return "Unknown";
}

int HaruPlatformRetrieveRAMSpeed() {
    return 0;
}

HaruPlatform HaruPlatformInitialize() {
    HaruPlatform Platform = {0};

    struct sysinfo MemoryInformation;
    if (sysinfo(&MemoryInformation) != 0) {
        perror("sysinfo failed");
    }

    HaruProcessorInformation CPUInfo = HaruPlatformRetrieveCPUCoreAndThreadCount();
    HaruSIMDInformation SIMDInfo = HaruPlatformRetrieveCPUSIMDSupportType();

    uint64_t TotalRAM = (uint64_t)MemoryInformation.totalram * MemoryInformation.mem_unit;
    uint64_t FreeRAM  = (uint64_t)MemoryInformation.freeram * MemoryInformation.mem_unit;

    Platform.OperatingSystem = HARU_OS_LINUX;
    Platform.Architecture = HaruPlatformRetrieveArchitecture();

    Platform.HaruHardwareInformation.HaruCPUInformation.CPUCoreCount = CPUInfo.CoreCount;
    Platform.HaruHardwareInformation.HaruCPUInformation.CPUThreadCount = CPUInfo.ThreadCount;

    Platform.HaruHardwareInformation.HaruCPUInformation.SIMDSupport = SIMDInfo.Supported;
    Platform.HaruHardwareInformation.HaruCPUInformation.SIMDType = SIMDInfo.Type;

    Platform.HaruHardwareInformation.HaruRAMInformation.TotalRAMBytes = TotalRAM;
    Platform.HaruHardwareInformation.HaruRAMInformation.AvailableRAMBytes = FreeRAM;

    Platform.HaruHardwareInformation.HaruRAMInformation.ExtraRAMInformation.RAMVersion = HaruPlatformRetrieveRAMGeneration();
    Platform.HaruHardwareInformation.HaruRAMInformation.ExtraRAMInformation.RAMSpeed = (uint32_t)HaruPlatformRetrieveRAMSpeed();

    return Platform;
}
