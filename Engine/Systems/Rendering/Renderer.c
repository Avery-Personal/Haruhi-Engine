//
//  Haruhi Engine
//
//  Copyright © 2026 AveriC & Averi
//

#define SOKOL_GFX_IMPL
#define SOKOL_GLCORE
//#define SOKOL_GLUE_IMPL

#include <stdlib.h>
#include <string.h>

#include "Renderer.h"
#include "../../Platform/Runtime/Window/Window.h"

static const char *DefaultVSSource =
    "#version 410\n"
    "layout(location=0) in vec2 Position;\n"
    "layout(location=1) in vec4 Color;\n"
    "out vec4 FragmentColor;\n"
    "void main() {\n"
    "  gl_Position = vec4(Position, 0.5, 1.0);\n"
    "  FragmentColor = Color;\n"
    "}\n";

static const char *DefaultFSSource =
    "#version 410\n"
    "in vec4 FragmentColor;\n"
    "out vec4 OutputColor;\n"
    "void main() {\n"
    "  OutputColor = FragmentColor;\n"
    "}\n";

HaruRenderer *HaruRendererCreate(HaruWindow *Window) {
    if (!Window)
        return NULL;

    HaruRenderer *Renderer = (HaruRenderer *) malloc(sizeof(HaruRenderer));

    if (!Renderer)
        return NULL;

    memset(Renderer, 0, sizeof(HaruRenderer));

    Renderer -> Window = Window;
    Renderer -> Width = Window -> WIDTH;
    Renderer -> Height = Window -> HEIGHT;

    sg_desc Description = {0};

    sg_setup(&Description);

    return Renderer;
}

void HaruRendererDestroy(HaruRenderer *Renderer) {
    if (!Renderer)
        return;

    for (int i = 0; i < Renderer -> MeshCount; i++) {
        HaruRendererDestroyMesh(Renderer, (HaruMesh){i});
    }

    for (int i = 0; i < Renderer -> PipelineCount; i++) {
        HaruRendererDestroyPipeline(Renderer, (HaruPipeline){i});
    }

    sg_shutdown();

    free(Renderer);
}

void HaruRendererResize(HaruRenderer *Renderer, int Width, int Height) {
    if (!Renderer)
        return;

    Renderer -> Width = Width;
    Renderer -> Height = Height;
}

void HaruRendererBeginFrame(HaruRenderer *Renderer, HaruColor ClearColor) {
    if (!Renderer)
        return;

    Renderer -> FrameActive = 1;

    sg_pass Pass = {0};

    Pass.action.colors[0].load_action = SG_LOADACTION_CLEAR;
    Pass.action.colors[0].clear_value = (sg_color) { ClearColor.R, ClearColor.G, ClearColor.B, ClearColor.A };
    
    Pass.swapchain.width = Renderer -> Width;
    Pass.swapchain.height = Renderer -> Height;
    Pass.swapchain.color_format = SG_PIXELFORMAT_RGBA8;
    Pass.swapchain.gl.framebuffer = 0;

    sg_begin_pass(&Pass);
}

void HaruRendererEndFrame(HaruRenderer *Renderer) {
    if (!Renderer)
        return;

    sg_end_pass();
    sg_commit();

    glfwSwapBuffers(Renderer -> Window -> Handle);

    Renderer -> FrameActive = 0;
}

void HaruRendererWaitIdle(HaruRenderer *Renderer) {
    (void) Renderer;
}

HaruMesh HaruRendererCreateMesh(HaruRenderer *Renderer, const HaruVertex *Vertices, int VertexCount) {
    if (!Renderer || !Vertices || VertexCount <= 0)
        return (HaruMesh) {-1};

    if (Renderer -> MeshCount >= 256)
        return (HaruMesh) {-1};

    sg_buffer_desc BufferDescription = {0};

    BufferDescription.data.ptr = Vertices;
    BufferDescription.data.size = (size_t) VertexCount * sizeof(HaruVertex);

    sg_buffer VertexBuffer = sg_make_buffer(&BufferDescription);

    int id = Renderer -> MeshCount++;

    HaruMeshInternal *InternalMesh = &Renderer -> Meshes[id];

    InternalMesh -> VertexBuffer = (sg_buffer *) malloc(sizeof(sg_buffer));
    *InternalMesh -> VertexBuffer = VertexBuffer;
    InternalMesh -> VertexCount = VertexCount;

    return (HaruMesh){ .ID = id };
}

void HaruRendererDestroyMesh(HaruRenderer *Renderer, HaruMesh Mesh) {
    if (!Renderer || Mesh.ID < 0 || Mesh.ID >= Renderer -> MeshCount)
        return;
    
    HaruMeshInternal *InternalMesh = &Renderer -> Meshes[Mesh.ID];
    if (InternalMesh -> VertexBuffer) {
        sg_destroy_buffer(*InternalMesh -> VertexBuffer);

        free(InternalMesh -> VertexBuffer);

        InternalMesh -> VertexBuffer = NULL;
    }

    InternalMesh -> VertexCount = 0;
}

HaruPipeline HaruRendererCreatePipeline(HaruRenderer *Renderer, HaruPipelineDescription Description) {
    if (!Renderer || Renderer -> PipelineCount >= 64)
        return (HaruPipeline){-1};

    sg_shader_desc ShaderDescription = {0};

    ShaderDescription.vertex_func.source = DefaultVSSource;
    ShaderDescription.fragment_func.source = DefaultFSSource;
    
    sg_shader Shader = sg_make_shader(&ShaderDescription);
    sg_pipeline_desc PipelineDescription = {0};

    PipelineDescription.shader = Shader;
    
    PipelineDescription.layout.attrs[0].format = SG_VERTEXFORMAT_FLOAT2;
    PipelineDescription.layout.attrs[1].format = SG_VERTEXFORMAT_FLOAT4;
    
    PipelineDescription.primitive_type = (Description.Topology == HARU_RENDERER_TOPOLOGY_LINES)  ? SG_PRIMITIVETYPE_LINES  : SG_PRIMITIVETYPE_TRIANGLES;
    PipelineDescription.depth.compare = Description.EnableDepth ? SG_COMPAREFUNC_LESS_EQUAL : SG_COMPAREFUNC_ALWAYS;
    PipelineDescription.depth.write_enabled = Description.EnableDepth;

    sg_pipeline Pipeline = sg_make_pipeline(&PipelineDescription);

    int id = Renderer -> PipelineCount++;

    HaruPipelineInternal *InternalPipeline = &Renderer -> Pipelines[id];
    
    InternalPipeline -> Pipeline = (sg_pipeline *) malloc(sizeof(sg_pipeline));
    *InternalPipeline -> Pipeline = Pipeline;

    return (HaruPipeline){ .ID = id };
}

void HaruRendererDestroyPipeline(HaruRenderer *Renderer, HaruPipeline Pipeline) {
    if (!Renderer || Pipeline.ID < 0 || Pipeline.ID >= Renderer -> PipelineCount)
        return;
    
    HaruPipelineInternal *InternalPipeline = &Renderer -> Pipelines[Pipeline.ID];
    if (InternalPipeline -> Pipeline) {
        sg_destroy_pipeline(*InternalPipeline -> Pipeline);

        free(InternalPipeline -> Pipeline);

        InternalPipeline -> Pipeline = NULL;
    }
}

void HaruRendererDrawMesh(HaruRenderer *Renderer, HaruMesh Mesh, HaruPipeline Pipeline) {
    if (!Renderer || !Renderer -> FrameActive)
        return;

    if (Mesh.ID < 0 || Mesh.ID >= Renderer -> MeshCount)
        return;

    if (Pipeline.ID < 0 || Pipeline.ID >= Renderer -> PipelineCount)
        return;
        
    HaruMeshInternal *InternalMesh = &Renderer -> Meshes[Mesh.ID];
    HaruPipelineInternal *InternalPipeline = &Renderer -> Pipelines[Pipeline.ID];

    if (!InternalMesh -> VertexBuffer || !InternalPipeline -> Pipeline)
        return;

    sg_apply_pipeline(*InternalPipeline -> Pipeline);

    sg_bindings Binding = {0};
    
    Binding.vertex_buffers[0] = *InternalMesh -> VertexBuffer;

    sg_apply_bindings(&Binding);

    sg_draw(0, InternalMesh -> VertexCount, 1);
}
