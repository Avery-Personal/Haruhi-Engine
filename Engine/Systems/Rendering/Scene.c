//
//  Haruhi Engine
//
//  Copyright © 2026 AveriC & Averi
//

#include <stdlib.h>
#include <string.h>

#include "Scene.h"

HaruScene HaruSceneCreate(void) {
    HaruScene Scene;

    memset(&Scene, 0, sizeof(HaruScene));

    return Scene;
}

HaruBoolean HaruSceneAddEntity(HaruScene *Scene, HaruEntity Entity) {
    if (!Scene)
        return HARU_FALSE;

    if (Scene -> EntityCount >= MAX_ENTITIES) {
        return HARU_FALSE;
    }

    Scene -> Entities[Scene -> EntityCount++] = Entity;

    return HARU_TRUE;
}

HaruBoolean HaruSceneSpawnEntity(HaruScene *Scene, HaruMesh Mesh, HaruPipeline Pipeline, float X, float Y, float Z, float Scale) {
    if (!Scene)
        return HARU_FALSE;

    HaruEntity Entity = {
        .Mesh = Mesh,
        .Pipeline = Pipeline,

        .PositionX = X,
        .PositionY = Y,
        .PositionZ = Z,

        .Scale = Scale
    };

    return HaruSceneAddEntity(Scene, Entity);
}

void HaruSceneClear(HaruScene *Scene) {
    if (!Scene)
        return;

    Scene -> EntityCount = 0;
    
    memset(Scene -> Entities, 0, sizeof(Scene -> Entities));
}

void HaruRendererDrawScene(HaruRenderer *Renderer, const HaruScene *Scene) {
    if (!Renderer || !Scene || !Renderer -> FrameActive)
        return;

    for (int i = 0; i < Scene -> EntityCount; i++) {
        const HaruEntity *Entity = &Scene -> Entities[i];

        HaruRendererDrawMesh(Renderer, Entity -> Mesh, Entity -> Pipeline);
    }
}