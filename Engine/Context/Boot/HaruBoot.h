#ifndef HARUHI_BOOT_H
#define HARUHI_BOOT_H

    typedef struct HaruContext HaruContext;    

    #define HARUHI_STANDARDIZED_ENTRY_POINT int
    #define HARUHI_CONTEXTUAL_ENTRY_POINT HaruContext

    typedef enum {
        HARU_EXIT_SUCCESS = 0,
        HARU_EXIT_FAILURE = -1
    } HaruhiExitCode;

#endif
