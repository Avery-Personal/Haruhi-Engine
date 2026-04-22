//
//  Haruhi Engine
//
//  Copyright © 2026 AveriC & Averi
//

#include "Context.h"

HaruContext HaruContextInitialize(void) {
    HaruContext Context = {0};

    Context.Name = "Haruhi Engine";
    Context.Version = HARU_ENGINE_VERSION;

    Context.argc = 0;
    Context.argv = NULL;

    Context.Logger = NULL;
    Context.Application = NULL;

    Context.Bootstrapped = HARU_FALSE;
    Context.Running = HARU_TRUE;
    Context.Headless = HARU_FALSE;
    Context.EditorMode = HARU_FALSE;

    #if defined(_WIN32)
        Context.OperatingSystem = HARU_OS_WINDOWS;
    #elif defined(__APPLE__)
        Context.OperatingSystem = HARU_OS_MAC;
    #elif defined(__linux__)
        Context.OperatingSystem = HARU_OS_LINUX;
    #else
        Context.OperatingSystem = HARU_OS_LINUX;
    #endif

    #if defined(_M_X64) || defined(__x86_64__)
        Context.Architecture = HARU_ARCH_X64;
    #elif defined(_M_IX86) || defined(__i386__)
        Context.Architecture = HARU_ARCH_X86;
    #elif defined(_M_ARM64) || defined(__aarch64__)
        Context.Architecture = HARU_ARCH_ARM64;
    #else
        Context.Architecture = HARU_ARCH_UNKNOWN;
    #endif

    return Context;
}

void HaruContextSetArguments(HaruContext *Context, int argc, char **argv) {
    if (!Context)
        return;

    Context -> argc = argc;
    Context -> argv = argv;
}

void HaruContextSetLogger(HaruContext *Context, HaruLogger *Logger) {
    if (!Context)
        return;

    Context -> Logger = Logger;
}

void HaruContextAttachApplication(HaruContext *Context, HaruApplication *Application) {
    if (!Context)
        return;

    Context -> Application = Application;

    if (Application) {
        Context -> OperatingSystem = Application -> Platform.OperatingSystem;
        Context -> Architecture = Application -> Platform.Architecture;
    }
}

void HaruContextSynchronizeFromApplication(HaruContext *Context) {
    if (!Context || !Context -> Application)
        return;

    Context -> OperatingSystem = Context -> Application -> Platform.OperatingSystem;
    Context -> Architecture = Context -> Application -> Platform.Architecture;
    Context -> Running = Context -> Application -> Running ? HARU_TRUE : HARU_FALSE;
    Context -> Version = Context -> Application -> Version;
    Context -> Name = Context -> Application -> Name;
}

HaruResult HaruContextBootstrap(HaruContext *Context) {
    if (!Context)
        return HARU_RESULT_FAILURE;

    Context -> Bootstrapped = HARU_TRUE;
    Context -> Running = HARU_TRUE;

    if (Context -> Application)
        HaruContextSynchronizeFromApplication(Context);

    return HARU_RESULT_SUCCESS;
}

void HaruContextShutdown(HaruContext *Context) {
    if (!Context)
        return;

    Context -> Running = HARU_FALSE;
    Context -> Bootstrapped = HARU_FALSE;
}
