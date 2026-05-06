//
//  Haruhi Engine
//
//  Copyright © 2026 AveriC & Averi
//

#include <stdio.h>

#include "Context/Boot/Entrypoint/HaruEntry.h"
#include "Core/Context/Context.h"
#include "Core/Engine/Engine.h"
#include "Core/Application/Application.h"
#include "Platform/Runtime/Window/Window.h"
#include "Core/Logging/Logging.h"

HaruEntry HaruMain() {
    HaruLoggerInitialize(&gLogger);
    HaruLoggerSetLevel(&gLogger, HARU_LOGGING_INFO);

    gLogger.EnableFile = 0;

    HaruContext Context = HaruContextInitialize();

    HaruContextSetLogger(&Context, &gLogger);

    HaruApplication Application = HaruApplicationInitialize();

    Application.Name = "Haruhi Engine";
    Application.Version = HARU_ENGINE_VERSION;

    HaruContextAttachApplication(&Context, &Application);
    HaruContextBootstrap(&Context);

    if (HaruInitializeWindowing() != HARU_RESULT_SUCCESS) {
        HARU_LOG_FATAL(&gLogger, "Haruhi windowing initialization failed.\n");

        return HARU_EXIT_FAILURE;
    }

    HaruWindow *WINDOW = HaruApplicationCreateWindow(&Application, "Haruhi Engine", 800, 600);
    if (!WINDOW) {
        HARU_LOG_ERROR(&gLogger, "Couldn't create window.");

        return HARU_EXIT_FAILURE;
    }

    HaruApplicationRun(&Application);
    HaruApplicationShutdown(&Application);

    HaruContextShutdown(&Context);

    return HARU_EXIT_SUCCESS;
}
