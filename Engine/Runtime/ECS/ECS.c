#include <stdlib.h>
#include <string.h>

#include "../../Core/Logging/Logging.h"

#include "ECS.h"

Instance *InstanceCreate(const char *Name) {
    Instance *Object = malloc(sizeof(Instance));
    if (!Object) {
        HARU_LOG_ERROR(&gLogger, "ECS object INSTANCE creation failed.\n");

        return NULL;
    }

    memset(Object, 0, sizeof(Instance));

    Object -> Name = Name;

    return Object;
}
