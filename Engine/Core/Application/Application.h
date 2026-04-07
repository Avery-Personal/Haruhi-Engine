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

#ifndef APPLICATION_H
#define APPLICATION_H

    #include <stdint.h>

    #include "../Engine/Engine.h"
    #include "../../Platform/Platform.h"
    #include "../../Platform/Runtime/Window/Window.h"
    #include "../System.h"

    #define MAX_WINDOWS 8

    typedef struct HaruApplication {
        const char *Name;

        HaruVersion Version;

        HaruEngine Engine;
        HaruPlatform Platform;

        HaruWindow *Windows[MAX_WINDOWS];
        HaruWindow *MainWindow;
        
        int WindowCount;

        int Running;
        float DeltaTime;
    } HaruApplication;

    typedef struct {
        const char *Name;

        HaruVersion Version;

        HaruBoolean EnableValidation;
        HaruBoolean EnableHighDPI;
        
        const char *InitialWindowTitle;

        uint32_t InitialWindowWidth;
        uint32_t InitialWindowHeight;
    } HaruApplicationCreateInfo;
    
    void HaruApplicationRun(HaruApplication *Application);

    HaruWindow *HaruApplicationCreateWindow(HaruApplication *Application, const char *Title, int WIDTH, int HEIGHT);
    //HaruResult *HaruApplicationCreateInfoWindow(HaruApplication *Application, HaruWindowCreateInfo *WindowCreateInfo);
    void HaruApplicationAddWindow(HaruApplication *Application, HaruWindow *WINDOW);

    HaruApplication HaruApplicationInitialize();
    //sHaruResult HaruApplicationCreate(HaruApplicationCreateInfo *ApplicationCreateInfo, HaruApplication **OutputApplication);
    void HaruApplicationShutdown(HaruApplication *Application);

#endif
