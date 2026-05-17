//
//  Haruhi Engine
//
//  Copyright © 2026 AveriC & Averi
//

#ifndef RENDERER_H
#define RENDERER_H

    #include <vulkan/vulkan.h>

    #include "../../Core/System.h"

    typedef enum {
        HARU_RENDERER_TOPOLOGY_TRIANGLES,
        HARU_RENDERER_TOPOLOGY_LINES
    } HaruTopology;

    typedef struct HaruWindow HaruWindow;
    typedef struct HaruRenderer HaruRenderer;

    typedef struct {
        float R, G, B, A;
    } HaruColor;

    typedef struct {
        float X, Y;
    } HaruVector2;

    typedef struct {
        HaruVector2 Position;
        HaruColor Color;
    } HaruVertex;

    typedef struct {
        HaruTopology Topology;
        HaruBoolean EnableDepth;
    } HaruPipelineDescription;

    typedef struct {
        int Handle;
    } HaruMesh;

    typedef struct {
        int Handle;
    } HaruPipeline;

    typedef struct {
        VkBuffer VertexBuffer; // TODO: Rendering wrapper API
        VkDeviceMemory VertexMemory;
        
        int VertexCount;
    } HaruMeshInternal;

    typedef struct {
        VkPipeline Pipeline;
        VkPipelineLayout Layout;
    } HaruPipelineInternal;

    typedef struct HaruRenderer {
        HaruWindow *Window;

        int Width;
        int Height;

        HaruBoolean FrameActive;

        VkInstance Instance;
        VkSurfaceKHR Surface;

        VkPhysicalDevice PhysicalDevice;
        VkDevice Device;

        VkQueue GraphicsQueue;
        VkQueue PresentQueue;
        
        u32 GraphicsQueueFamily;
        u32 PresentQueueFamily;

        VkSwapchainKHR Swapchain;
        VkFormat SwapchainImageFormat;
        VkExtent2D SwapchainExtent;

        VkImage *SwapchainImages;
        VkImageView *SwapchainImageViews;
        u32 SwapchainImageCount;

        VkRenderPass RenderPass;
        VkFramebuffer *Framebuffers;

        VkCommandPool CommandPool;
        VkCommandBuffer *CommandBuffers;

        VkSemaphore ImageAvailableSemaphores[2];
        VkSemaphore RenderFinishedSemaphores[2];

        VkFence InFlightFences[2];

        u32 CurrentFrame;
        u32 CurrentImageIndex;

        HaruMeshInternal Meshes[256];
        int MeshCount;

        HaruPipelineInternal Pipelines[64];
        int PipelineCount;
    } HaruRenderer;

    HaruRenderer *HaruRendererCreate(HaruWindow *Window);
    void HaruRendererDestroy(HaruRenderer *Renderer);

    void HaruRendererBeginFrame(HaruRenderer *Renderer, HaruColor ClearColor);
    void HaruRendererEndFrame(HaruRenderer *Renderer);

    HaruMesh HaruRendererCreateMesh(HaruRenderer *Renderer, const HaruVertex *Vertices, int VertexCount);
    void HaruRendererDestroyMesh(HaruRenderer *Renderer, HaruMesh Mesh);

    void HaruRendererDrawMesh(HaruRenderer *Renderer, HaruMesh Mesh, HaruPipeline Pipeline);

    HaruPipeline HaruRendererCreatePipeline(HaruRenderer *Renderer, HaruPipelineDescription Description);

#endif
