#ifndef INSTANCE_H
#define INSTANCE_H

    #include "../../Core/System.h"

    typedef u64 InstanceID;

    typedef struct Instance Instance;
    typedef struct Component Component;

    typedef struct Instance {
        const char *Name;

        InstanceID ID;

        Instance *Parent;
        Instance **Children;

        u32 ChildCount;
        u32 ChildCapacity;

        Component **Components;

        u32 ComponentCount;
        u32 ComponentCapacity;

        u32 Flags;
    } Instance;

    Instance *InstanceCreate(const char *Name);

#endif
