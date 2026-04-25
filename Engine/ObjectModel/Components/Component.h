#ifndef COMPONENT_H
#define COMPONENT_H

    typedef enum {
        COMPONENT_TRANSFORM,
        COMPONENT_CUSTOM
    } ComponentType;

    typedef struct Component {
        ComponentType Type;

        void *Data;
    } Component;

    typedef struct {
        float x, y, z;
        float rx, ry, rz;
        float sx, sy, sz;
    } TransformComponent;

#endif
