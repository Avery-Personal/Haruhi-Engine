#include <stdlib.h>
#include <string.h>

#include "../../Core/Logging/Logging.h"
#include "../../Platform/Runtime/Window/Window.h"
#include "Renderer.h"

u32 HaruRendererFindMemoryType(HaruRenderer *Renderer, u32 TypeFilter, VkMemoryPropertyFlags Properties) {
    VkPhysicalDeviceMemoryProperties MemoryProperties;

    vkGetPhysicalDeviceMemoryProperties(Renderer -> PhysicalDevice, &MemoryProperties);

    for (u32 i = 0; i < MemoryProperties.memoryTypeCount; i++) {
        if ((TypeFilter & (1u << i)) && (MemoryProperties.memoryTypes[i].propertyFlags & Properties) == Properties) {
            return i;
        }
    }

    return U32_MAX;
}

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

void HaruRendererEndFrame(HaruRenderer *Renderer) {
    if (!Renderer)
        return;

    if (!Renderer -> FrameActive) {
        HARU_LOG_WARN(&gLogger, "EndFrame called without BeginFrame.\n");
        
        return;
    }

    Renderer -> FrameActive = HARU_FALSE;
}

HaruMesh HaruRendererCreateMesh(HaruRenderer *Renderer, const HaruVertex *Vertices, int VertexCount) {
    HaruMesh Mesh = {-1};

    if (!Renderer || !Vertices || VertexCount <= 0)
        return Mesh;

    if (Renderer -> MeshCount >= 256)
        return Mesh;

    int Index = Renderer -> MeshCount++;

    HaruMeshInternal *Internal = &Renderer -> Meshes[Index];

    Internal -> VertexCount = VertexCount;

    size_t Size = sizeof(HaruVertex) * VertexCount;

    Internal -> VertexBuffer = malloc(Size);

    memcpy(Internal -> VertexBuffer, Vertices, Size);

    Mesh.Handle = Index;

    return Mesh;
}

void HaruRendererDrawMesh(HaruRenderer *Renderer, HaruMesh Mesh, HaruPipeline Pipeline) {
    if (!Renderer || !Renderer -> FrameActive)
        return;

    if (Mesh.Handle < 0 || Mesh.Handle >= Renderer -> MeshCount)
        return;

    HaruMeshInternal *Internal = &Renderer -> Meshes[Mesh.Handle];

    HaruVertex *Vertices = (HaruVertex *) Internal -> VertexBuffer;

    glBegin(GL_TRIANGLES);

    for (int i = 0; i < Internal -> VertexCount; i++) {
        glColor4f(Vertices[i].Color.R, Vertices[i].Color.G, Vertices[i].Color.B, Vertices[i].Color.A);
        glVertex2f(Vertices[i].Position.X, Vertices[i].Position.Y);
    }

    glEnd();
}
