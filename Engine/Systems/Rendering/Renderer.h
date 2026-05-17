//
//  Haruhi Engine
//
//  Copyright © 2026 AveriC & Averi
//

#ifndef RENDERER_H
#define RENDERER_H

    #include "../Core/System.h"

    typedef struct HaruRenderer HaruRenderer;

    typedef struct {
        float R, G, B, A;
    } HaruColor;

    typedef struct {
        float X, Y;
    } HaruVec2;

    typedef struct {
        HaruVec2 Position;
        HaruColor Color;
    } HaruVertex;

    typedef enum {
        HARU_RENDERER_TOPOLOGY_TRIANGLES,
        HARU_RENDERER_TOPOLOGY_LINES
    } HaruTopology;

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
        void *VertexBuffer; // TO FIND Rendering API
        
        int VertexCount;
    } HaruMeshInternal;

    typedef struct {
        void *Pipeline; // TO FIND Rendering API
    } HaruPipelineInternal;

    typedef struct HaruRenderer {
        int Width;
        int Height;

        HaruBoolean FrameActive;

        void *PassAction; // TO FIND Rendering API

        void *DefaultPipeline; // TO FIND Rendering API
        void *Bindings; // TO FIND Rendering API

        HaruMeshInternal Meshes[256];
        int MeshCount;

        HaruPipelineInternal Pipelines[64];
        int PipelineCount;
    } HaruRenderer;

    HaruRenderer *HaruRendererCreate(int Width, int Height);
    void HaruRendererDestroy(HaruRenderer *Renderer);

    void HaruRendererBeginFrame(HaruRenderer *Renderer, HaruColor ClearColor);
    void HaruRendererEndFrame(HaruRenderer *Renderer);

    HaruMesh HaruRendererCreateMesh(HaruRenderer *Renderer, const HaruVertex *Vertices, int VertexCount);
    void HaruRendererDestroyMesh(HaruRenderer *Renderer, HaruMesh Mesh);

    void HaruRendererDrawMesh(HaruRenderer *Renderer, HaruMesh Mesh, HaruPipeline Pipeline);

    HaruPipeline HaruRendererCreatePipeline(HaruRenderer *Renderer, HaruPipelineDescription Description);

#endif
