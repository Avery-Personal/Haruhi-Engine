#ifndef HARU_FORTH_H
#define HARU_FORTH_H

    #include <stddef.h>

    #include "../../Types.h"

    #define HARU_FORTH_STACK_MAX 256
    #define HARU_FORTH_WORD_MAX 64
    #define HARU_FORTH_NAME_MAX 32
    #define HARU_FORTH_LINE_MAX 1024

    typedef struct HaruForthContext HaruForthContext;

    typedef int (*HaruForthWordFunction)(HaruForthContext *Context);
    typedef void (*HaruForthPrintFunction)(HaruForthContext *Context, const char *Text);

    typedef struct {
        const char *Name;
        HaruForthWordFunction Function;
    } HaruForthWord;

    struct HaruForthContext {
        double Stack[HARU_FORTH_STACK_MAX];
        int SP;

        HaruForthWord Words[HARU_FORTH_WORD_MAX];
        int WordCount;

        void *UserData;
        HaruForthPrintFunction Print;
        int Running;
    };

    void HaruForthInitialize(HaruForthContext *Context);
    void HaruForthSetUserData(HaruForthContext *Context, void *UserData);
    void HaruForthSetPrintFunction(HaruForthContext *Context, HaruForthPrintFunction PrintFunction);

    int HaruForthRegisterWord(HaruForthContext *Context, const char *Name, HaruForthWordFunction Function);
    int HaruForthExecuteLine(HaruForthContext *Context, const char *Line);

    int HaruForthPush(HaruForthContext *Context, double Value);
    int HaruForthPop(HaruForthContext *Context, double *Output);
    int HaruForthPeek(HaruForthContext *Context, int IndexFromTop, double *Output);

    void HaruForthRegisterStandardWords(HaruForthContext *Context);

#endif