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

void HaruRendererDestroySwapchainObjects(HaruRenderer *Renderer) {
    if (!Renderer || !Renderer -> Device)
        return;

    if (Renderer -> Framebuffers) {
        for (u32 i = 0; i < Renderer -> SwapchainImageCount; i++) {
            if (Renderer -> Framebuffers[i]) {
                vkDestroyFramebuffer(Renderer -> Device, Renderer -> Framebuffers[i], NULL);
            }
        }

        free(Renderer -> Framebuffers);

        Renderer -> Framebuffers = NULL;
    }

    if (Renderer -> SwapchainImageViews) {
        for (u32 i = 0; i < Renderer -> SwapchainImageCount; i++) {
            if (Renderer -> SwapchainImageViews[i]) {
                vkDestroyImageView(Renderer -> Device, Renderer -> SwapchainImageViews[i], NULL);
            }
        }

        free(Renderer -> SwapchainImageViews);

        Renderer -> SwapchainImageViews = NULL;
    }

    if (Renderer -> Swapchain) {
        vkDestroySwapchainKHR(Renderer -> Device, Renderer -> Swapchain, NULL);

        Renderer -> Swapchain = VK_NULL_HANDLE;
    }

    Renderer -> SwapchainImageCount = 0;
}

void HaruRendererDestroyPipelines(HaruRenderer *Renderer) {
    if (!Renderer || !Renderer -> Device)
        return;

    for (int i = 0; i < Renderer -> PipelineCount; i++) {
        if (Renderer -> Pipelines[i].Pipeline) {
            vkDestroyPipeline(Renderer -> Device, Renderer -> Pipelines[i].Pipeline, NULL);
        }

        if (Renderer -> Pipelines[i].Layout) {
            vkDestroyPipelineLayout(Renderer -> Device, Renderer -> Pipelines[i].Layout, NULL);
        }
    }

    Renderer -> PipelineCount = 0;
}

void HaruRendererDestroyMeshes(HaruRenderer *Renderer) {
    if (!Renderer || !Renderer -> Device)
        return;

    for (int i = 0; i < Renderer -> MeshCount; i++) {
        if (Renderer -> Meshes[i].VertexBuffer) {
            vkDestroyBuffer(Renderer -> Device, Renderer -> Meshes[i].VertexBuffer, NULL);
        }

        if (Renderer -> Meshes[i].VertexMemory) {
            vkFreeMemory(Renderer -> Device, Renderer -> Meshes[i].VertexMemory, NULL);
        }
    }

    Renderer -> MeshCount = 0;
}

HaruRenderer *HaruRendererCreate(HaruWindow *Window) {
    HaruRenderer *Renderer = malloc(sizeof(HaruRenderer));
    if (!Renderer) {
        HARU_LOG_ERROR(&gLogger, "Failed to allocate renderer.\n");

        return NULL;
    }

    memset(Renderer, 0, sizeof(HaruRenderer));

    Renderer -> Window = Window;

    if (Window) {
        Renderer -> Width = Window -> WIDTH;
        Renderer -> Height = Window -> HEIGHT;
    }

    Renderer -> FrameActive = HARU_FALSE;

    Renderer -> CurrentFrame = 0;
    Renderer -> CurrentImageIndex = 0;

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
