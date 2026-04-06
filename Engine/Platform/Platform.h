#ifndef PLATFORM_H
#define PLATFORM_H

    typedef enum {
        HARU_PLATFORM_MARKING_UNDETECTED,
        HARU_PLATFORM_MARKING_DETECTED,
        HARU_PLATFORM_MARKING_ERROR,
        HARU_PLATFORM_MARKING_FAILED, // Cut short; not of error
    } HaruPlatformMarker;

    typedef enum {
        HARU_OS_WINDOWS,
        HARU_OS_MACOS,
        HARU_OS_LINUX,
        HARU_OS_BSD,
        HARU_OS_OTHER,
    } HaruOperatingSystem;

    typedef enum {

    } HaruArchitecture;

#endif
