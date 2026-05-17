#include <stdlib.h>
#include <string.h>

#include "../../Core/Logging/Logging.h"
#include "Renderer.h"

#if defined(__APPLE__)
    #include <OpenGL/gl3.h>
#else
    #include <GL/gl.h>
#endif

HaruRenderer *HaruRendererCreate(int Width, int Height) {
    HaruRenderer *Renderer = malloc(sizeof(HaruRenderer));
    if (!Renderer) {
        HARU_LOG_ERROR(&gLogger, "Failed to allocate renderer.\n");

        return NULL;
    }

    memset(Renderer, 0, sizeof(HaruRenderer));

    Renderer -> Width = Width;
    Renderer -> Height = Height;
    Renderer -> FrameActive = HARU_FALSE;

    Renderer -> MeshCount = 0;
    Renderer -> PipelineCount = 0;

    return Renderer;
}

void HaruRendererDestroy(HaruRenderer *Renderer) {
    if (!Renderer)
        return;

    free(Renderer);
}

void HaruRendererBeginFrame(HaruRenderer *Renderer, HaruColor ClearColor) {
    if (!Renderer)
        return;

    if (Renderer -> FrameActive) {
        HARU_LOG_WARN(&gLogger, "BeginFrame called while frame already active.\n");
        
        return;
    }

    Renderer -> FrameActive = HARU_TRUE;

    glViewport(0, 0, Renderer -> Width, Renderer -> Height);
    glClearColor(ClearColor.R, ClearColor.G, ClearColor.B, ClearColor.A);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
