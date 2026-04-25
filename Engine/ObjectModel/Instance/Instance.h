#ifndef INSTANCE_H
#define INSTANCE_H

    #define RAENTRO_IMPLEMENTATION
    #include "../../Third-Party/Raentro.h"

    #include "../../Core/System.h"
    #include "../Components/Component.h"

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

    void InstanceAddChild(Instance *Parent, Instance *Child);
    void InstanceAddComponent(Instance *_INSTANCE_, Component *_COMPONENT_);

    Component *InstanceGetComponent(Instance *_INSTANCE_, ComponentType Type);

#endif
