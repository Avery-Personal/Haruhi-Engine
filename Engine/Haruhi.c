#include <stdio.h>

#include "Context/Boot/Entrypoint/HaruEntry.h"
#include "Core/Engine/Engine.h"
#include "Core/Application/Application.h"
#include "Platform/Runtime/Window/Window.h"

HaruEntry HaruMain() {
    HaruApplication Application = HaruApplicationInitialize();

    Application.Name = "Haruhi Engine";

    if (HaruInitializeWindowing() != HARU_RESULT_SUCCESS) {
        fprintf(stderr, "Haruhi windowing initialization failed.\n");
    }

    HaruWindow *WINDOW = HaruApplicationCreateWindow(&Application, "Haruhi Engine", 800, 600);
    if (!WINDOW) {
        fprintf(stderr, "Couldn't create window.");

        return HARU_EXIT_FAILURE;
    }

    HaruApplicationAddWindow(&Application, WINDOW);

    HaruApplicationRun(&Application);
    HaruApplicationShutdown(&Application);

    return HARU_EXIT_SUCCESS;
}
