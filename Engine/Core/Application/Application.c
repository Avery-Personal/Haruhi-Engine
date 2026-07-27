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

#include <stdio.h>
#include <stdlib.h>

#include "Application.h"
#include "../../Platform/Runtime/Window/Window.h"
#include "../Debug/Forth/HaruEngineForth.h"
#include "../Logging/Logging.h"

static HaruScene gMainScene;
static HaruMesh gTestMesh;
static HaruPipeline gTestPipeline;
static HaruBoolean gSceneInitialized = HARU_FALSE;

static float RandomFloat(float Minimum, float Maximum) {
    return Minimum + ((float) rand() / (float) RAND_MAX) * (Maximum - Minimum);
}

void InitializeSceneTest(HaruApplication *Application) {
    if (gSceneInitialized || !Application -> Renderer)
        return;
        
    gMainScene = HaruSceneCreate();

    HaruVertex Vertices[] = {
        {{-0.1f, -0.1f}, {1.0f, 0.0f, 0.0f, 1.0f}},
        {{0.1f, -0.1f}, {0.0f, 1.0f, 0.0f, 1.0f}},
        {{0.0f,  0.1f}, {0.0f, 0.0f, 1.0f, 1.0f}}
    };

    gTestMesh = HaruRendererCreateMesh(Application -> Renderer, Vertices, 3);

    HaruPipelineDescription PipelineDescription = {
        .Topology = HARU_RENDERER_TOPOLOGY_TRIANGLES,
        .EnableDepth = HARU_FALSE
    };

    gTestPipeline = HaruRendererCreatePipeline(Application -> Renderer, PipelineDescription);

    HaruColor White = {1.0f, 1.0f, 1.0f, 1.0f};

    HaruSceneSpawnEntity(&gMainScene, gTestMesh, gTestPipeline, 0.0f, 0.0f, 0.0f, 1.0f, White);

    gSceneInitialized = HARU_TRUE;
}

void HaruApplicationRun(HaruApplication *Application) {
    HaruRenderer *Renderer = Application -> Renderer;

    if (Renderer && !gSceneInitialized) {
        InitializeSceneTest(Application);
    }

    while (Application -> Running && Application -> Engine.Running) {
        HaruInputBeginFrame(&Application -> Platform);
        HaruPlatformPollEvents(&Application -> Platform);

        for (int i=0; i < Application -> WindowCount; i++) {
            HaruWindowPollEvents(Application -> Windows[i]);
        }

        if (HaruWindowShouldClose(Application -> MainWindow)) {
            Application -> Running = 0;
            Application -> Engine.Running = 0;
        }
        
        HaruTimeUpdate(&Application -> Time);

        if (HaruInputKeyPressed(&Application -> Platform, GLFW_KEY_J)) {
            float RandomX = RandomFloat(-0.8f, 0.8f);
            float RandomY = RandomFloat(-0.8f, 0.8f);
            float RandomScale = RandomFloat(0.05f, 0.4f);

            HaruColor RandomColor = {
                .R = RandomFloat(0.2f, 1.0f),
                .G = RandomFloat(0.2f, 1.0f),
                .B = RandomFloat(0.2f, 1.0f),
                .A = 1.0f
            };

            if (HaruSceneSpawnEntity(&gMainScene, gTestMesh, gTestPipeline, RandomX, RandomY, 0.0f, RandomScale, RandomColor)) {
                HARU_LOG_INFO(&gLogger, "Spawned entity at (%.2f, %.2f)\n   Scale: %.2f", RandomX, RandomY, RandomScale);
            }
        }

        if (Renderer) {
            HaruRendererResize(Renderer, Application -> MainWindow -> FramebufferWidth, Application -> MainWindow -> FramebufferHeight);

            //InitializeSceneTest(Application);

            HaruRendererBeginFrame(Renderer, (HaruColor){0.1f, 0.1f, 0.15f, 1.0f});
            HaruRendererDrawScene(Renderer, &gMainScene);
            HaruRendererEndFrame(Renderer);
        }
    }
}

HaruWindow *HaruApplicationCreateWindow(HaruApplication *Application, const char *Title, int WIDTH, int HEIGHT) {
    if (Application -> WindowCount >= MAX_WINDOWS)
        return NULL;

    HaruWindow *Window = HaruCreateWindow(Title, WIDTH, HEIGHT);
    if (!Window)
        return NULL;

    Window -> Application = Application;

    Application -> Windows[Application -> WindowCount++] = Window;

    if (!Application -> MainWindow) {
        Application -> MainWindow = Window;

        Application -> Renderer = HaruRendererCreate(Window);
    }

    return Window;
}

//HaruResult *HaruApplicationCreateInfoWindow(HaruApplication *Application, HaruWindowCreateInfo *WindowCreateInfo) {
//    if (WindowCreateInfo -> Title == NULL || WindowCreateInfo -> WIDTH <= 100 || WindowCreateInfo -> HEIGHT <= 100)
//        return HARU_RESULT_FAILURE;
//    
//    HaruWindow *WINDOW = HaruApplicationCreateWindow(Application, WindowCreateInfo -> Title, WindowCreateInfo -> WIDTH, WindowCreateInfo -> HEIGHT);
//
//    HARU_RESULT_SUCCESS;
//}

void HaruApplicationAddWindow(HaruApplication *Application, HaruWindow *WINDOW) {
    Application -> Windows[Application -> WindowCount++] = WINDOW;
}

HaruApplication HaruApplicationInitialize() {
    HaruApplication Application = {0};

    Application.Name = "Haruhi Engine App";
    Application.Version = HARU_ENGINE_VERSION;

    Application.Engine = HaruEngineInitialize();
    Application.Platform = Application.Engine.Platform;
    
    Application.WindowCount = 0;
    Application.Running = HARU_TRUE;

    HaruTimeInitialize(&Application.Time);
    HaruInputInitialize(&Application.Platform);

    HaruForthInitialize(&Application.DebugConsole);
    HaruForthSetUserData(&Application.DebugConsole, &Application);
    HaruForthRegisterEngineWords(&Application.DebugConsole);
    HaruForthRegisterStandardWords(&Application.DebugConsole);

    return Application;
}

HaruResult HaruApplicationCreate(HaruApplicationCreateInfo *ApplicationCreateInfo, HaruApplication **OutputApplication) {
    HARU_LOG_WARN(&gLogger, "[SOFTWARE]CreateInfo based creation is DEPRECATED\n");
    
    HaruApplication *Application = malloc(sizeof(HaruApplication));

    Application -> Name = ApplicationCreateInfo -> Name;
    Application -> Version = ApplicationCreateInfo -> Version;

    Application -> Engine = HaruEngineInitialize();
    Application -> Running = HARU_TRUE;

    if (ApplicationCreateInfo -> InitialWindowTitle) {
        //HaruWindowCreateInfo WindowInfo = {0};

        //WindowInfo.Title = ApplicationCreateInfo -> InitialWindowTitle;

        //WindowInfo.WIDTH = ApplicationCreateInfo -> InitialWindowWidth;
        //WindowInfo.HEIGHT = ApplicationCreateInfo -> InitialWindowHeight;

        //HaruApplicationCreateInfoWindow(Application, &WindowInfo);
    }

    *OutputApplication = Application;

    return HARU_RESULT_SUCCESS;
}

void HaruApplicationShutdown(HaruApplication *Application) {
    Application -> Running = 0;
    
    if (Application -> Renderer) {
        HaruRendererDestroy(Application -> Renderer);
        
        Application -> Renderer = NULL;
    }
}
