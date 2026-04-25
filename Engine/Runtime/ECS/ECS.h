#ifndef ECS_H
#define ECS_H

    #include "../../Core/System.h"

    typedef u64 EntityID;

    typedef enum {
        COMPONENT_TRANSFORM,
        COMPONENT_RENDER,
        COMPONENT_PHYSICS,
        COMPONENT_AUDIO,
        COMPONENT_SCRIPT
    } ComponentType;

    typedef struct Instance Instance;
    typedef struct Component Component;

    typedef struct Instance {
        const char *Name;

        EntityID ID;

        Instance *Parent;
        Instance **Children;

        u32 ChildCount;
        u32 ChildCapacity;

        Component **Components;

        u32 ComponentCount;
        u32 ComponentCapacity;

        u32 Flags;
    } Instance;

    typedef struct Component {
        ComponentType Type;

        void *Data;
    } Component;

    Instance *InstanceCreate(const char *Name);
    void InstanceDestroy(Instance *Object);

    void InstanceAddChild(Instance *Parent, Instance *Child);
    void InstanceRemoveChild(Instance *Child);

    void InstanceAddComponent(Instance *Object, Component *_Component);
    Component *InstanceGetComponent(Instance *Object, ComponentType Type);

#endif
