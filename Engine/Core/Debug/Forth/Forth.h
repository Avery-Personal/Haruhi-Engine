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

    typedef enum {
        HARU_FORTH_TYPE_NUMBER,
        HARU_FORTH_TYPE_STRING
    } HaruForthValueType;

    typedef struct {
        HaruForthValueType Type;

        union {
            double Number;
            const char *String;
        } As;
    } HaruForthValue;

    typedef struct {
        const char *Name;
        HaruForthWordFunction Function;
    } HaruForthWord;

    struct HaruForthContext {
        HaruForthValue Stack[HARU_FORTH_STACK_MAX];
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

    int HaruForthPushNumber(HaruForthContext *Context, double Value);
    int HaruForthPushString(HaruForthContext *Context, const char *Value);
    int HaruForthPop(HaruForthContext *Context, HaruForthValue *Output);
    int HaruForthPeek(HaruForthContext *Context, int IndexFromTop, HaruForthValue *Output);

    void HaruForthRegisterStandardWords(HaruForthContext *Context);

#endif
