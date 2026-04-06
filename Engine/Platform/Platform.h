//
//  Haruhi Engine
//
//  Original source code derived from Kunamo Engine:
//    Copyright © 2026 Kunamo Entertainment. All rights reserved.
//
//  Modifications & Integration:
//    Copyright © 2026 AveriC & Averi
//

#ifndef PLATFORM_H
#define PLATFORM_H

    #include <stdio.h>
    #include <stdlib.h>

    // TODO: Reconstruct Kunamo's windowing system to be a hybrid mix of GLFW & SDL3
    #include <GLFW/glfw3.h>

    #define MAX_KEYS 256

    // TODO: Will need to refine Kunamo's architecture to support detection workflow support
    typedef enum {
        HARU_PLATFORM_MARKING_UNDETECTED,
        HARU_PLATFORM_MARKING_DETECTED,
        HARU_PLATFORM_MARKING_ERROR,
        HARU_PLATFORM_MARKING_FAILED,
    } HaruPlatformMarker;

    typedef enum {
        HARU_OS_WINDOWS,
        HARU_OS_LINUX,
        HARU_OS_MAC
    } HaruOperatingSystem;

    typedef enum {
        HARU_ARCH_X86,
        HARU_ARCH_X64,
        HARU_ARCH_ARM64,

        HARU_ARCH_UNKNOWN
    } HaruArchitecture;

    typedef struct {
        int IsPressed;
        int WasPressed;
    } HaruKeyState;

    typedef struct {
        int IsDown;
        int WasDown;
    } HaruMouseButtonState;

    typedef struct {
        HaruKeyState Keys[MAX_KEYS];

        int MouseX;
        int MouseY;
        
        int MouseDeltaX;
        int MouseDeltaY;

        int HasFocus;

        HaruMouseButtonState MouseButtons[3];
    } HaruInputState;

    typedef struct {
        int Supported;

        const char *Type;
    } HaruSIMDInformation;

    typedef struct {
        uint32_t CoreCount;
        uint32_t ThreadCount;
    } HaruProcessorInformation;

    typedef struct {
        uint32_t CPUCoreCount;
        uint32_t CPUThreadCount;

        int SIMDSupport;
        const char *SIMDType;
    } HaruPlatformCPUInformation;

    typedef struct {
        uint64_t TotalRAMBytes;
        uint64_t AvailableRAMBytes;

        struct ExtraRAMInformation {
            const char *RAMVersion;

            uint32_t RAMSpeed;
        };
    } HaruPlatformRAMInformation;

    typedef struct {
        int Detected;
    } HaruPlatformGPUInformation;

    typedef struct {
        int IsSSD;
    } HaruPlatformStorageInformation;

    typedef struct {
        HaruPlatformCPUInformation HaruCPUInformation;
        HaruPlatformGPUInformation HaruGPUInformation;

        HaruPlatformRAMInformation HaruRAMInformation;
        HaruPlatformStorageInformation HaruDiskInformation;
    } HaruPlatformHardwareInformation;

    typedef struct {
        HaruOperatingSystem OperatingSystem;
        HaruArchitecture Architecture;

        HaruInputState Input;

        HaruPlatformHardwareInformation HaruHardwareInformation;
    } HaruPlatform;
    
    // Hardware information collection

    HaruArchitecture HaruPlatformRetrieveArchitecture();

    HaruProcessorInformation HaruPlatformRetrieveCPUCoreAndThreadCount();
    HaruSIMDInformation HaruPlatformRetrieveCPUSIMDSupportType();

    char *HaruPlatformRetrieveRAMGeneration();
    int HaruPlatformRetrieveRAMSpeed();

    HaruPlatform HaruPlatformInitialize();

    // Windowing context & creation

    void HaruInputBeginFrame(HaruPlatform *Platform);

    static void KeyCallback(GLFWwindow *Window, int Key, int Scancode, int Action, int Mods);
    static void MouseButtonCallback(GLFWwindow *Window, int Button, int Action, int Mods);
    static void CursorPositionCallback(GLFWwindow *Window, double XPosition, double YPosition);

    void HaruPlatformPollEvents(HaruPlatform *Platform);

#endif
