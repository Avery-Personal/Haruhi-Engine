#ifndef REGISTRY_H
#define REGISTRY_H

    #include "../../Core/System.h"

    typedef struct Instance Instace;

    typedef struct {
        Instance **Instances;

        u32 Count;
        u32 Cap;
    } InstanceRegistry;

    static InstanceRegistry gRegistry;

#endif
