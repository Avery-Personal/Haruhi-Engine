#include <stdio.h>

#include "Context/Boot/Entrypoint/HaruEntry.h"

HaruEntry HaruMain() {
    printf("Hello, from Haruhi");

    return HARU_EXIT_FAILURE;
}
