//
//  Haruhi Engine
//
//  Copyright © 2026 AveriC & Averi
//

#ifndef HARU_INPUT_H
#define HARU_INPUT_H

    #include "../System.h"
    #include "../../Platform/Platform.h"

    typedef struct HaruWindow HaruWindow;

    void HaruInputInitialize(HaruPlatform *Platform);
    void HaruInputBeginFrame(HaruPlatform *Platform);

    void HaruInputBindWindow(HaruWindow *Window);

    int HaruInputKeyDown(const HaruPlatform *Platform, int Key);
    int HaruInputKeyPressed(const HaruPlatform *Platform, int Key);
    int HaruInputKeyReleased(const HaruPlatform *Platform, int Key);

    int HaruInputMouseDown(const HaruPlatform *Platform, int Button);
    int HaruInputMousePressed(const HaruPlatform *Platform, int Button);
    int HaruInputMouseReleased(const HaruPlatform *Platform, int Button);

    void HaruInputGetMousePosition(const HaruPlatform *Platform, int *X, int *Y);
    void HaruInputGetMouseDelta(const HaruPlatform *Platform, int *DX, int *DY);

#endif
