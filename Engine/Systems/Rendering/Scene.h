//
//  Haruhi Engine
//
//  Copyright © 2026 AveriC & Averi
//

#ifndef SHADER_H
#define SHADER_H

    #include "Renderer.h"

    HaruScene HaruSceneCreate(void);

    HaruBoolean HaruSceneAddEntity(HaruScene *Scene, HaruEntity Entity);
    HaruBoolean HaruSceneSpawnEntity(HaruScene *Scene, HaruMesh Mesh, HaruPipeline Pipeline, float X, float Y, float Z, float Scale);
    
    void HaruRendererDrawScene(HaruRenderer *Renderer, const HaruScene *Scene);
    void HaruSceneClear(HaruScene *Scene);
    
#endif