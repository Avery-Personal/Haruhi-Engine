//
//  Haruhi Engine
//
//  Copyright © 2026 AveriC & Averi
//

#ifndef HARU_TIMER_H
#define HARU_TIMER_H

    #include <stdint.h>

    typedef double HaruTime;

    typedef struct {
        HaruTime Start;
        HaruTime Elapsed;

        int Running;
    } HaruTimer;

    typedef struct {
        HaruTime DeltaTime;
        HaruTime TotalTime;
        HaruTime LastFrame;
    } HaruTimeContext;

    void HaruTimeInitialize(HaruTimeContext *Context);
    void HaruTimeUpdate(HaruTimeContext *Context);

    void HaruTimerStart(HaruTimer *Timer);
    void HaruTimerStop(HaruTimer *Timer);
    void HaruTimerReset(HaruTimer *Timer);
    HaruTime HaruTimerElapsed(HaruTimer *Timer);

    HaruTime HaruTimeNow(void);

    const char *HaruTimeFormat(HaruTime Time);

    #define HARU_TIMER_BEGIN(Timer) HaruTimerStart(Timer)
    #define HARU_TIMER_END(Timer) HaruTimerStop(Timer)

#endif
