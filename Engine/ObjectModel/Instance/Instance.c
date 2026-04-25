#include "Instance.h"

Instance *InstanceCreate(const char *Name) {
    Instance *_INSTANCE_ = (Instance *) RaAlloc(sizeof(Instance));
    if (!_INSTANCE_)
        return NULL;

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

void InstanceAddChild(Instance *Parent, Instance *Child) {
    if (!Parent || !Child)
        return;

    Child -> Parent = Parent;

    if (Parent -> ChildCount >= Parent -> ChildCapacity) {
        Parent -> ChildCapacity = Parent -> ChildCapacity ? Parent -> ChildCapacity * 2 : 4;

        Parent -> Children = RaRealloc(Parent -> Children, Parent -> ChildCapacity * sizeof(Instance *));
    }

    Parent -> Children[Parent -> ChildCount++] = Child;
}

void InstanceAddComponent(Instance *_INSTANCE_, Component *_COMPONENT_) {
    if (!_INSTANCE_ || !_COMPONENT_)
        return;

    if (_INSTANCE_ -> ComponentCount >= _INSTANCE_ -> ComponentCapacity) {
        _INSTANCE_ -> ComponentCapacity = _INSTANCE_ -> ComponentCapacity ? _INSTANCE_ -> ComponentCapacity * 2 : 4;

        _INSTANCE_ -> Components = RaRealloc(_INSTANCE_ -> Components, _INSTANCE_ -> ComponentCapacity * sizeof(Component*));
    }

    _INSTANCE_ -> Components[_INSTANCE_ -> ComponentCount++] = _COMPONENT_;
}

Component *InstanceGetComponent(Instance *_INSTANCE_, ComponentType Type) {
    for (size_t i = 0; i < _INSTANCE_ -> ComponentCount; i++) {
        Component *_COMPONENT_ = (Component *) _INSTANCE_ -> Components[i];

        if (_COMPONENT_ -> Type == Type)
            return _COMPONENT_;
    }

    return NULL;
}
