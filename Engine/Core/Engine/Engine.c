#include "Engine.h"

HaruEngine HaruEngineInitialize() {
    HaruEngine Engine;

    Engine.Running = 1;
    Engine.Platform = HaruPlatformInitialize();

    return Engine;
}
