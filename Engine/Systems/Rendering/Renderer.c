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

    if (Renderer -> Device) {
        vkDeviceWaitIdle(Renderer -> Device);

        HaruRendererDestroyMeshes(Renderer);
        HaruRendererDestroyPipelines(Renderer);
        HaruRendererDestroySwapchainObjects(Renderer);

        if (Renderer -> CommandBuffers) {
            free(Renderer -> CommandBuffers);

            Renderer -> CommandBuffers = NULL;
        }

        if (Renderer -> CommandPool) {
            vkDestroyCommandPool(Renderer -> Device, Renderer -> CommandPool, NULL);

            Renderer -> CommandPool = VK_NULL_HANDLE;
        }

        for (int i = 0; i < 2; i++) {
            if (Renderer -> ImageAvailableSemaphores[i]) {
                vkDestroySemaphore(Renderer -> Device, Renderer -> ImageAvailableSemaphores[i], NULL);
            }

            if (Renderer -> RenderFinishedSemaphores[i]) {
                vkDestroySemaphore(Renderer -> Device, Renderer -> RenderFinishedSemaphores[i], NULL);
            }

            if (Renderer -> InFlightFences[i]) {
                vkDestroyFence(Renderer -> Device, Renderer -> InFlightFences[i], NULL);
            }
        }

        vkDestroyDevice(Renderer -> Device, NULL);

        Renderer -> Device = VK_NULL_HANDLE;
    }

    if (Renderer -> Surface && Renderer -> Instance) {
        vkDestroySurfaceKHR(Renderer -> Instance, Renderer -> Surface, NULL);

        Renderer -> Surface = VK_NULL_HANDLE;
    }

    if (Renderer -> Instance) {
        vkDestroyInstance(Renderer -> Instance, NULL);

        Renderer -> Instance = VK_NULL_HANDLE;
    }

    free(Renderer);
}

void HaruRendererBeginFrame(HaruRenderer *Renderer, HaruColor ClearColor) {
    if (!Renderer || Renderer -> FrameActive || !Renderer -> Device || !Renderer -> Swapchain)
        return;

    vkWaitForFences(Renderer -> Device, 1, &Renderer -> InFlightFences[Renderer -> CurrentFrame], VK_TRUE, UINT64_MAX);

    VkResult AcquireResult = vkAcquireNextImageKHR(Renderer -> Device, Renderer -> Swapchain, UINT64_MAX, Renderer -> ImageAvailableSemaphores[Renderer -> CurrentFrame], VK_NULL_HANDLE, &Renderer -> CurrentImageIndex);


    if (AcquireResult == VK_ERROR_OUT_OF_DATE_KHR) {
        HARU_LOG_WARN(&gLogger, "Swapchain out of date; recreate swapchain before continuing.\n");

        return;
    }

    if (AcquireResult != VK_SUCCESS && AcquireResult != VK_SUBOPTIMAL_KHR) {
        HARU_LOG_ERROR(&gLogger, "vkAcquireNextImageKHR failed.\n");

        return;
    }

    vkResetFences(Renderer -> Device, 1, &Renderer -> InFlightFences[Renderer -> CurrentFrame]);

    VkCommandBuffer CommandBuffer = Renderer -> CommandBuffers[Renderer -> CurrentImageIndex];

    vkResetCommandBuffer(CommandBuffer, 0);

    VkCommandBufferBeginInfo BeginInfo = {0};

    BeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    BeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    if (vkBeginCommandBuffer(CommandBuffer, &BeginInfo) != VK_SUCCESS) {
        HARU_LOG_ERROR(&gLogger, "vkBeginCommandBuffer failed.\n");

        return;
    }

    VkClearValue ClearValues[1];

    ClearValues[0].color.float32[0] = ClearColor.R;
    ClearValues[0].color.float32[1] = ClearColor.G;
    ClearValues[0].color.float32[2] = ClearColor.B;
    ClearValues[0].color.float32[3] = ClearColor.A;

    VkRenderPassBeginInfo RenderPassInfo = {0};

    RenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    RenderPassInfo.renderPass = Renderer -> RenderPass;
    RenderPassInfo.framebuffer = Renderer -> Framebuffers[Renderer -> CurrentImageIndex];
    RenderPassInfo.renderArea.offset.x = 0;
    RenderPassInfo.renderArea.offset.y = 0;
    RenderPassInfo.renderArea.extent = Renderer -> SwapchainExtent;
    RenderPassInfo.clearValueCount = 1;
    RenderPassInfo.pClearValues = ClearValues;

    vkCmdBeginRenderPass(CommandBuffer, &RenderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport Viewport = {0};

    Viewport.x = 0.0f;
    Viewport.y = 0.0f;
    Viewport.width = (float)Renderer -> SwapchainExtent.width;
    Viewport.height = (float)Renderer -> SwapchainExtent.height;
    Viewport.minDepth = 0.0f;
    Viewport.maxDepth = 1.0f;

    VkRect2D Scissor = {0};

    Scissor.offset.x = 0;
    Scissor.offset.y = 0;
    Scissor.extent = Renderer -> SwapchainExtent;

    vkCmdSetViewport(CommandBuffer, 0, 1, &Viewport);
    vkCmdSetScissor(CommandBuffer, 0, 1, &Scissor);

    Renderer -> FrameActive = HARU_TRUE;
}

void HaruRendererEndFrame(HaruRenderer *Renderer) {
    if (!Renderer || !Renderer -> FrameActive)
        return;

    VkCommandBuffer CommandBuffer = Renderer -> CommandBuffers[Renderer -> CurrentImageIndex];

    vkCmdEndRenderPass(CommandBuffer);

    if (vkEndCommandBuffer(CommandBuffer) != VK_SUCCESS) {
        HARU_LOG_ERROR(&gLogger, "vkEndCommandBuffer failed.\n");

        Renderer -> FrameActive = HARU_FALSE;

        return;
    }

    VkPipelineStageFlags WaitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

    VkSubmitInfo SubmitInfo = {0};

    SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    SubmitInfo.waitSemaphoreCount = 1;
    SubmitInfo.pWaitSemaphores = &Renderer -> ImageAvailableSemaphores[Renderer -> CurrentFrame];
    SubmitInfo.pWaitDstStageMask = WaitStages;
    SubmitInfo.commandBufferCount = 1;
    SubmitInfo.pCommandBuffers = &CommandBuffer;
    SubmitInfo.signalSemaphoreCount = 1;
    SubmitInfo.pSignalSemaphores = &Renderer -> RenderFinishedSemaphores[Renderer -> CurrentFrame];

    if (vkQueueSubmit(Renderer -> GraphicsQueue, 1, &SubmitInfo, Renderer -> InFlightFences[Renderer -> CurrentFrame]) != VK_SUCCESS) {
        HARU_LOG_ERROR(&gLogger, "vkQueueSubmit failed.\n");
        
        Renderer -> FrameActive = HARU_FALSE;
        
        return;
    }

    VkPresentInfoKHR PresentInfo = {0};

    PresentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    PresentInfo.waitSemaphoreCount = 1;
    PresentInfo.pWaitSemaphores = &Renderer -> RenderFinishedSemaphores[Renderer -> CurrentFrame];
    PresentInfo.swapchainCount = 1;
    PresentInfo.pSwapchains = &Renderer -> Swapchain;
    PresentInfo.pImageIndices = &Renderer -> CurrentImageIndex;

    VkResult PresentResult = vkQueuePresentKHR(Renderer -> PresentQueue, &PresentInfo);
    if (PresentResult == VK_ERROR_OUT_OF_DATE_KHR || PresentResult == VK_SUBOPTIMAL_KHR) {
        HARU_LOG_WARN(&gLogger, "Present reported out-of-date/suboptimal; recreate swapchain.\n");
    } else if (PresentResult != VK_SUCCESS) {
        HARU_LOG_ERROR(&gLogger, "vkQueuePresentKHR failed.\n");
    }

    Renderer -> CurrentFrame = (Renderer -> CurrentFrame + 1) % 2;
    Renderer -> FrameActive = HARU_FALSE;
}

HaruMesh HaruRendererCreateMesh(HaruRenderer *Renderer, const HaruVertex *Vertices, int VertexCount) {
    HaruMesh Mesh = {-1};

    if (!Renderer || !Vertices || VertexCount <= 0)
        return Mesh;

    if (Renderer -> MeshCount >= 256 || !Renderer -> Device)
        return Mesh;

    int Index = Renderer -> MeshCount++;

    HaruMeshInternal *Internal = &Renderer -> Meshes[Index];

    memset(Internal, 0, sizeof(*Internal));

    VkDeviceSize BufferSize = sizeof(HaruVertex) * (VkDeviceSize) VertexCount;
    VkBufferCreateInfo BufferInfo = {0};

    BufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    BufferInfo.size = BufferSize;
    BufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    BufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(Renderer -> Device, &BufferInfo, NULL, &Internal -> VertexBuffer) != VK_SUCCESS) {
        HARU_LOG_ERROR(&gLogger, "vkCreateBuffer failed for mesh.\n");

        Renderer -> MeshCount--;

        return Mesh;
    }

    VkMemoryRequirements MemoryRequirements;

    vkGetBufferMemoryRequirements(Renderer -> Device, Internal -> VertexBuffer, &MemoryRequirements);

    VkMemoryAllocateInfo AllocInfo = {0};

    AllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    AllocInfo.allocationSize = MemoryRequirements.size;
    AllocInfo.memoryTypeIndex = HaruRendererFindMemoryType(Renderer, MemoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if (AllocInfo.memoryTypeIndex == UINT32_MAX || vkAllocateMemory(Renderer -> Device, &AllocInfo, NULL, &Internal -> VertexMemory) != VK_SUCCESS) {
        HARU_LOG_ERROR(&gLogger, "vkAllocateMemory failed for mesh.\n");

        vkDestroyBuffer(Renderer -> Device, Internal -> VertexBuffer, NULL);

        Renderer -> MeshCount--;

        return Mesh;
    }

    vkBindBufferMemory(Renderer -> Device, Internal -> VertexBuffer, Internal -> VertexMemory, 0);

    void *Mapped = NULL;
    if (vkMapMemory(Renderer -> Device, Internal -> VertexMemory, 0, BufferSize, 0, &Mapped) == VK_SUCCESS) {
        memcpy(Mapped, Vertices, (size_t) BufferSize);
        
        vkUnmapMemory(Renderer -> Device, Internal -> VertexMemory);
    } else {
        HARU_LOG_ERROR(&gLogger, "vkMapMemory failed for mesh.\n");
        
        vkFreeMemory(Renderer -> Device, Internal -> VertexMemory, NULL);
        vkDestroyBuffer(Renderer -> Device, Internal -> VertexBuffer, NULL);
        
        Renderer -> MeshCount--;
        
        return Mesh;
    }

    Internal -> VertexCount = VertexCount;

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
