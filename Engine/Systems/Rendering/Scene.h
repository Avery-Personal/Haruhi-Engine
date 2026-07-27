//
//  Haruhi Engine
//
//  Copyright © 2026 AveriC & Averi
//

#ifndef SHADER_H
#define SHADER_H

    #include "Renderer.h"

    typedef struct {
        HaruMesh Mesh;
        HaruPipeline Pipeline;

        float PositionX, PositionY, PositionZ;
        float Scale;

        HaruColor Color;
    } HaruEntity;

    typedef struct {
        HaruEntity Entities[MAX_ENTITIES];
        int EntityCount;
    } HaruScene;

    HaruScene HaruSceneCreate(void);

    HaruBoolean HaruSceneAddEntity(HaruScene *Scene, HaruEntity Entity);
    HaruBoolean HaruSceneSpawnEntity(HaruScene *Scene, HaruMesh Mesh, HaruPipeline Pipeline, float X, float Y, float Z, float Scale, HaruColor Color);
    
    void HaruRendererDrawScene(HaruRenderer *Renderer, const HaruScene *Scene);
    void HaruSceneClear(HaruScene *Scene);
    
#endif