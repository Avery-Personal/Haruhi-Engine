#define RAENTRO_IMPLEMENTATION
#include "../../Third-Party/Raentro.h"
#include "../../Core/Logging/Logging.h"

#include "Instance.h"

Instance *InstanceCreate(const char *Name) {
    Instance *_INSTANCE_ = (Instance *) RaAlloc(sizeof(Instance));
    if (!_INSTANCE_) {
        HARU_LOG_ERROR(&gLogger, "Instance creation failed.\n");

        return NULL;
    }

    _INSTANCE_ -> Name = Name;

    _INSTANCE_ -> ID = (u64) _INSTANCE_;

    _INSTANCE_ -> Parent = NULL;

    _INSTANCE_ -> Children = NULL;
    _INSTANCE_ -> ChildCount = 0;
    _INSTANCE_ -> ChildCapacity = 0;

    _INSTANCE_ -> Components = NULL;
    _INSTANCE_ -> ComponentCount = 0;
    _INSTANCE_ -> ComponentCapacity = 0;

    return _INSTANCE_;
}
