//
//  Haruhi Engine
//
//  Copyright © 2026 AveriC & Averi
//

#ifndef HARU_CONTEXT_H
#define HARU_CONTEXT_H

    #include <stdint.h>

    #include "../System.h"
    #include "../Logging/Logging.h"
    #include "../Engine/Engine.h"
    #include "../Application/Application.h"
    #include "../../Platform/Platform.h"

    typedef struct HaruContext {
        const char *Name;

        HaruVersion Version;

        int argc;
        char **argv;

        HaruLogger *Logger;
        HaruApplication *Application;

        HaruOperatingSystem OperatingSystem;
        HaruArchitecture Architecture;

        HaruBoolean Bootstrapped;
        HaruBoolean Running;
        HaruBoolean Headless;
        HaruBoolean EditorMode;
    } HaruContext;

    HaruContext HaruContextInitialize(void);

    void HaruContextSetArguments(HaruContext *Context, int argc, char **argv);
    void HaruContextSetLogger(HaruContext *Context, HaruLogger *Logger);
    void HaruContextAttachApplication(HaruContext *Context, HaruApplication *Application);

    void HaruContextSynchronizeFromApplication(HaruContext *Context);
    HaruResult HaruContextBootstrap(HaruContext *Context);
    void HaruContextShutdown(HaruContext *Context);

#endif