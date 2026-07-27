//
//  Haruhi Engine
//
//  Copyright © 2026 AveriC & Averi
//

#ifndef RENDERER_H
#define RENDERER_H

    #include "../../Core/System.h"
    #include "../../Third-Party/sokol_gfx.h"
    //#include "../../Third-Party/sokol_glue.h"

    #define MAX_ENTITIES 1024

    typedef struct HaruWindow HaruWindow;
    typedef struct HaruRenderer HaruRenderer;

    typedef enum {
        HARU_RENDERER_TOPOLOGY_TRIANGLES,
        HARU_RENDERER_TOPOLOGY_LINES
    } HaruTopology;

    typedef struct {
        float R;
        float G;
        float B;
        float A;
    } HaruColor;

    typedef struct {
        float X;
        float Y;
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
        int ID;
    } HaruMesh;

    typedef struct {
        int ID;
    } HaruPipeline;

    typedef struct {
        sg_buffer *VertexBuffer;
        int VertexCount;
    } HaruMeshInternal;

    typedef struct {
        sg_pipeline *Pipeline;
    } HaruPipelineInternal;

    typedef struct {
        HaruMesh Mesh;
        HaruPipeline Pipeline;
        float PositionX, PositionY, PositionZ;
        float Scale;
    } HaruEntity;

    typedef struct {
        HaruEntity Entities[MAX_ENTITIES];
        int EntityCount;
    } HaruScene;

    struct HaruRenderer {
        HaruWindow *Window;

        int Width;
        int Height;

        HaruBoolean FrameActive;

        sg_environment Environment;

        HaruMeshInternal Meshes[256];
        int MeshCount;

        HaruPipelineInternal Pipelines[64];
        int PipelineCount;
    };

    HaruRenderer *HaruRendererCreate(HaruWindow *Window);
    void HaruRendererDestroy(HaruRenderer *Renderer);

    void HaruRendererResize(HaruRenderer *Renderer, int Width, int Height);
    void HaruRendererBeginFrame(HaruRenderer *Renderer, HaruColor ClearColor);
    void HaruRendererEndFrame(HaruRenderer *Renderer);
    void HaruRendererWaitIdle(HaruRenderer *Renderer);

    HaruMesh HaruRendererCreateMesh(HaruRenderer *Renderer, const HaruVertex *Vertices, int VertexCount);
    void HaruRendererDestroyMesh(HaruRenderer *Renderer, HaruMesh Mesh);

    HaruPipeline HaruRendererCreatePipeline(HaruRenderer *Renderer, HaruPipelineDescription Description);
    void HaruRendererDestroyPipeline(HaruRenderer *Renderer, HaruPipeline Pipeline);

    void HaruRendererDrawMesh(HaruRenderer *Renderer, HaruMesh Mesh, HaruPipeline Pipeline);

#endif
