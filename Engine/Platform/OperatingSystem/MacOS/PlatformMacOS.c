//
//  Haruhi Engine
//
//  Copyright © 2026 AveriC & Averi
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <sys/sysctl.h>
#include <sys/types.h>
#include <unistd.h>

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
    HaruProcessorInformation ProcessorInformation = {0};

    int Cores = 0;
    size_t len = sizeof(Cores);

    sysctlbyname("hw.physicalcpu", &Cores, &len, NULL, 0);

    ProcessorInformation.CoreCount = Cores;

    int threads = 0;

    len = sizeof(threads);

    sysctlbyname("hw.logicalcpu", &threads, &len, NULL, 0);
    
    ProcessorInformation.ThreadCount = threads;

    return ProcessorInformation;
}

HaruSIMDInformation HaruPlatformRetrieveCPUSIMDSupportType() {
    HaruSIMDInformation CPUSIMDInformation = {0, "None"};

    int HasAVX = 0;
    int HasSSE = 0;

    size_t len = sizeof(int);

    #if defined(__x86_64__)
        sysctlbyname("hw.optional.avx1_0", &HasAVX, &len, NULL, 0);
        sysctlbyname("hw.optional.sse", &HasSSE, &len, NULL, 0);
    #endif

    if (HasAVX) {
        CPUSIMDInformation.Supported = 1;
        CPUSIMDInformation.Type = "AVX";
    } else if (HasSSE) {
        CPUSIMDInformation.Supported = 1;
        CPUSIMDInformation.Type = "SSE";
    }

    #if defined(__aarch64__)
        CPUSIMDInformation.Supported = 1;
        CPUSIMDInformation.Type = "NEON";
    #endif

    return CPUSIMDInformation;
}

// Sadly MacOS doesn't give public RAM access
char *HaruPlatformRetrieveRAMGeneration() {
    return "Unknown";
}

int HaruPlatformRetrieveRAMSpeed() {
    return 0;
}

HaruPlatform HaruPlatformInitialize() {
    HaruPlatform Platform = {0};

    HaruProcessorInformation CPUInfo = HaruPlatformRetrieveCPUCoreAndThreadCount();
    HaruSIMDInformation SIMDInfo = HaruPlatformRetrieveCPUSIMDSupportType();

    uint64_t TotalRAM = 0;
    size_t len = sizeof(TotalRAM);
    
    sysctlbyname("hw.memsize", &TotalRAM, &len, NULL, 0);

    Platform.OperatingSystem = HARU_OS_MAC;
    Platform.Architecture = HaruPlatformRetrieveArchitecture();

    Platform.HaruHardwareInformation.HaruCPUInformation.CPUCoreCount = CPUInfo.CoreCount;
    Platform.HaruHardwareInformation.HaruCPUInformation.CPUThreadCount = CPUInfo.ThreadCount;

    Platform.HaruHardwareInformation.HaruCPUInformation.SIMDSupport = SIMDInfo.Supported;
    Platform.HaruHardwareInformation.HaruCPUInformation.SIMDType = SIMDInfo.Type;

    Platform.HaruHardwareInformation.HaruRAMInformation.TotalRAMBytes = TotalRAM;
    Platform.HaruHardwareInformation.HaruRAMInformation.AvailableRAMBytes = 0;

    Platform.HaruHardwareInformation.HaruRAMInformation.ExtraRAMInformation.RAMVersion = HaruPlatformRetrieveRAMGeneration();
    Platform.HaruHardwareInformation.HaruRAMInformation.ExtraRAMInformation.RAMSpeed = (uint32_t)HaruPlatformRetrieveRAMSpeed();

    return Platform;
}
