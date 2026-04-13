#include "Timer.h"

#include <stdio.h>
#include <time.h>

#if defined(_WIN32)
    #include <windows.h>
#else
    #include <sys/time.h>
#endif

HaruTime HaruTimeNow(void) {
    #if defined(_WIN32)
        static LARGE_INTEGER Frequency;
        static int Initialized = 0;

        if (!Initialized) {
            QueryPerformanceFrequency(&Frequency);

            Initialized = 1;
        }

        LARGE_INTEGER Counter;

        QueryPerformanceCounter(&Counter);

        return (HaruTime) Counter.QuadPart / (HaruTime) Frequency.QuadPart;
    #else
        struct timespec TimeSpecifications;

        clock_gettime(CLOCK_MONOTONIC, &TimeSpecifications);

        return (HaruTime) TimeSpecifications.tv_sec + (HaruTime) TimeSpecifications.tv_nsec / 1e9;
    #endif
}

void HaruTimeInitialize(HaruTimeContext *Context) {
    Context -> DeltaTime = 0.0;
    Context -> TotalTime = 0.0;
    Context -> LastFrame = HaruTimeNow();
}

void HaruTimeUpdate(HaruTimeContext *Context) {
    HaruTime Now = HaruTimeNow();

    Context -> DeltaTime = Now - Context -> LastFrame;
    Context -> TotalTime += Context -> DeltaTime;
    Context -> LastFrame = Now;
}

void HaruTimerStart(HaruTimer *Timer) {
    Timer -> Start = HaruTimeNow();
    Timer -> Running = 1;
}

void HaruTimerStop(HaruTimer *Timer) {
    if (Timer -> Running) {
        Timer -> Elapsed += HaruTimeNow() - Timer -> Start;
        Timer -> Running = 0;
    }
}

void HaruTimerReset(HaruTimer *Timer) {
    Timer -> Start = 0.0;
    Timer -> Elapsed = 0.0;
    Timer -> Running = 0;
}

HaruTime HaruTimerElapsed(HaruTimer *Timer) {
    if (Timer -> Running) {
        return Timer -> Elapsed + (HaruTimeNow() - Timer -> Start);
    }

    return Timer -> Elapsed;
}

const char *HaruTimeFormat(HaruTime Time) {
    static char Buffer[64];

    int Minutes = (int)(Time / 60.0);
    int Seconds = (int) Time % 60;
    int Milliseconds  = (int)((Time - (int) Time)  *1000.0);

    snprintf(Buffer, sizeof(Buffer), "%02d:%02d.%03d", Minutes, Seconds, Milliseconds);

    return Buffer;
}
