#include <stdio.h>

#include "../../Engine/ObjectModel/Instance/Instance.h"

int main() {
    Instance *World = InstanceCreate("World");

    Instance *Character = InstanceCreate("Goblin");
    Instance *Coins = InstanceCreate("Coins");

    InstanceAddChild(World, Character);
    InstanceAddChild(Character, Coins);

    TransformComponent *Transform = (TransformComponent *) RaAlloc(sizeof(TransformComponent));

    Transform -> x = 10;
    Transform -> y = 2;
    Transform -> z = -5;

    Component *_Component_ = (Component *) RaAlloc(sizeof(Component));

    _Component_ -> Type = COMPONENT_TRANSFORM;
    _Component_ -> Data = Transform;

    InstanceAddComponent(Character, _Component_);

    if (Coins -> Parent != Character) {
        fprintf(stderr, "Character is NOT parent of coins.\n");

        return HARU_EXIT_FAILURE;
    } else if (!Character -> Parent != World) {
        fprintf(stderr, "World is NOT parent of character.\n");

        return HARU_EXIT_FAILURE;
    }

    return HARU_EXIT_SUCCESS;
}
