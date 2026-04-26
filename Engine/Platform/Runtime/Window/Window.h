//
//  Haruhi Engine
//
//  Original source code derived from Kunamo Engine:
//    Copyright © 2026 Kunamo Entertainment. All rights reserved.
//
//  Modifications & Integration:
//    Copyright © 2026 AveriC & Averi
//

#ifndef PWINDOW_H
#define PWINDOW_H

    #include "../../../Core/System.h"
    #include "Core/Input/Input.h"

    #define HARUHI_RESOLVE 1

    typedef enum {
        HARU_WINDOW_BACKEND_GLFW,
        HARU_WINDOW_BACKEND_SDL,
        
        HARU_WINDOW_BACKEND_NONE
    } HaruWindowBackend;

    typedef struct HaruApplication HaruApplication;

    typedef struct HaruWindow {
        HaruWindowBackend Backend;
        
        void *Handle;

        HaruApplication *Application;

        int X;
        int Y;

        int WIDTH;
        int HEIGHT;

        int FramebufferWidth;
        int FramebufferHeight;

        int ShouldClose;

        int Focused;
        int Minimized;
        int Fullscreen;

        float DPIScale;
    } HaruWindow;

    extern HaruWindowBackend gWindowBackend;

    void HaruSetWindowBackend(HaruWindowBackend BACKEND);

    HaruWindow *HaruCreateWindow(const char *Title, int WIDTH, int HEIGHT);
    void HaruDestroyWindow(HaruWindow *WINDOW);
    HaruResult HaruInitializeWindowing();

    void HaruWindowPollEvents(HaruWindow *WINDOW);
    void HaruPlatformPollEvents(HaruPlatform *Platform);
    int HaruWindowShouldClose(HaruWindow *WINDOW);

#endif
