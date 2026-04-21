#include "Forth.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

static void HaruForthDefaultPrint(HaruForthContext *Context, const char *Text) {
    (void) Context;

    fputs(Text, stdout);
    fputc('\n', stdout);
}

static int HaruForthWordDup(HaruForthContext *Context) {
    HaruForthValue Value;
    if (!HaruForthPeek(Context, 0, &Value))
        return 0;
        
    if (Value.Type != HARU_FORTH_TYPE_NUMBER)
        return 0;
    
    return HaruForthPushNumber(Context, Value.As.Number);
}

static int HaruForthWordDrop(HaruForthContext *Context) {
    HaruForthValue Dummy;

    return HaruForthPop(Context, &Dummy);
}

static int HaruForthWordSwap(HaruForthContext *Context) {
    HaruForthValue A, B;

    if (!HaruForthPop(Context, &A))
        return 0;

    if (!HaruForthPop(Context, &B))
        return 0;

    if (!HaruForthPushNumber(Context, A.As.Number))
        return 0;

    if (!HaruForthPushNumber(Context, B.As.Number))
        return 0;

    return 1;
}

static int HaruForthWordOver(HaruForthContext *Context) {
    HaruForthValue Value;

    if (!HaruForthPeek(Context, 1, &Value))
        return 0;

    return HaruForthPushNumber(Context, Value.As.Number);
}

static int HaruForthWordAdd(HaruForthContext *Context) {
    HaruForthValue A, B;

    if (!HaruForthPop(Context, &B))
        return 0;

    if (!HaruForthPop(Context, &A))
        return 0;

    if (A.Type != HARU_FORTH_TYPE_NUMBER || B.Type != HARU_FORTH_TYPE_NUMBER) {
        if (Context -> Print)
            Context -> Print(Context, "Type error: + expects numbers");

        return 0;
    }

    return HaruForthPushNumber(Context, A.As.Number + B.As.Number);
}

static int HaruForthWordSub(HaruForthContext *Context) {
    HaruForthValue A, B;

    if (!HaruForthPop(Context, &B))
        return 0;

    if (!HaruForthPop(Context, &A))
        return 0;

    return HaruForthPushNumber(Context, A.As.Number - B.As.Number);
}

static int HaruForthWordMul(HaruForthContext *Context) {
    HaruForthValue A, B;

    if (!HaruForthPop(Context, &B))
        return 0;
        
    if (!HaruForthPop(Context, &A))
        return 0;
        
    return HaruForthPushNumber(Context, A.As.Number * B.As.Number);
}

static int HaruForthWordDiv(HaruForthContext *Context) {
    HaruForthValue A, B;

    if (!HaruForthPop(Context, &B))
        return 0;

    if (!HaruForthPop(Context, &A))
        return 0;

    if (B.As.Number == 0.0) {
        if (Context -> Print)
            Context -> Print(Context, "Forth error: division by zero");

        return 0;
    }
    return HaruForthPushNumber(Context, A.As.Number / B.As.Number);
}

static int HaruForthWordDot(HaruForthContext *Context) {
    HaruForthValue Value;
    if (!HaruForthPop(Context, &Value))
        return 0;

    char Buffer[64];

    if (Value.Type == HARU_FORTH_TYPE_NUMBER)
        snprintf(Buffer, sizeof(Buffer), "%.17g", Value.As.Number);
    else
        snprintf(Buffer, sizeof(Buffer), "%s", Value.As.String);

    if (Context -> Print)
        Context -> Print(Context, Buffer);

    return 1;
}

static int HaruForthWordDotS(HaruForthContext *Context) {
    char Buffer[1024];
    int Offset = 0;

    Offset += snprintf(Buffer + Offset, sizeof(Buffer) - (size_t) Offset, "<%d> ", Context -> SP);

    for (int i = 0; i < Context -> SP && Offset < (int) sizeof(Buffer); i++) {
        if (Context -> Stack[i].Type == HARU_FORTH_TYPE_NUMBER) {
            Offset += snprintf(Buffer + Offset, sizeof(Buffer) - Offset, "%.17g ", Context -> Stack[i].As.Number);
        } else {
            Offset += snprintf(Buffer + Offset, sizeof(Buffer) - Offset, "%s ", Context -> Stack[i].As.String);
        }
    }

    if (Context -> Print) Context -> Print(Context, Buffer);

    return 1;
}

static int HaruForthWordDepth(HaruForthContext *Context) {
    return HaruForthPushNumber(Context, (double) Context -> SP);
}

static int HaruForthWordClear(HaruForthContext *Context) {
    Context -> SP = 0;

    return 1;
}

static int HaruForthWordWords(HaruForthContext *Context) {
    char Buffer[1024];
    int Offset = 0;

    Offset += snprintf(Buffer + Offset, sizeof(Buffer) - (size_t) Offset, "words:");

    for (int i = 0; i < Context -> WordCount && Offset < (int) sizeof(Buffer); i++) {
        Offset += snprintf(Buffer + Offset, sizeof(Buffer) - (size_t) Offset, " %s", Context -> Words[i].Name);
    }

    if (Context -> Print)
        Context -> Print(Context, Buffer);

    return 1;
}

static int HaruForthWordCr(HaruForthContext *Context) {
    if (Context -> Print)
        Context -> Print(Context, "");

    return 1;
}

static int HaruForthWordEmit(HaruForthContext *Context) {
    HaruForthValue Value;
    char Buffer[2];

    if (!HaruForthPop(Context, &Value))
        return 0;

    Buffer[0] = (char)((int) Value.As.Number);
    Buffer[1] = '\0';

    if (Context -> Print)
        Context -> Print(Context, Buffer);

    return 1;
}

void HaruForthInitialize(HaruForthContext *Context) {
    if (!Context)
        return;

    memset(Context, 0, sizeof(*Context));

    Context -> Running = 1;
    Context -> Print = HaruForthDefaultPrint;

    HaruForthRegisterStandardWords(Context);
}

void HaruForthSetUserData(HaruForthContext *Context, void *UserData) {
    if (!Context)
        return;

    Context -> UserData = UserData;
}

void HaruForthSetPrintFunction(HaruForthContext *Context, HaruForthPrintFunction PrintFunction) {
    if (!Context)
        return;

    Context -> Print = PrintFunction ? PrintFunction : HaruForthDefaultPrint;
}

int HaruForthRegisterWord(HaruForthContext *Context, const char *Name, HaruForthWordFunction Function) {
    if (!Context || !Name || !Function)
        return 0;

    if (Context -> WordCount >= HARU_FORTH_WORD_MAX)
        return 0;

    Context -> Words[Context -> WordCount++] = (HaruForthWord){ Name, Function };

    return 1;
}

int HaruForthPushNumber(HaruForthContext *Context, double Value) {
    if (!Context)
        return 0;

    if (Context -> SP >= HARU_FORTH_STACK_MAX) {
        if (Context -> Print)
            Context -> Print(Context, "Forth error: stack overflow");

        return 0;
    }

    Context -> Stack[Context -> SP++] = (HaruForthValue){
        .Type = HARU_FORTH_TYPE_NUMBER,
        .As.Number = Value
    };

    return 1;
}

int HaruForthPushString(HaruForthContext *Context, const char *Value) {
    if (!Context)
        return 0;

    if (Context -> SP >= HARU_FORTH_STACK_MAX) {
        if (Context -> Print)
            Context -> Print(Context, "Forth error: stack overflow");

        return 0;
    }

    Context -> Stack[Context -> SP++] = (HaruForthValue){
        .Type = HARU_FORTH_TYPE_STRING,
        .As.String = Value
    };

    return 1;
}

int HaruForthPop(HaruForthContext *Context, HaruForthValue *Output) {
    if (!Context || !Output)
        return 0;

    if (Context -> SP <= 0) {
        if (Context -> Print)
            Context -> Print(Context, "Forth error: stack underflow");

        return 0;
    }

    *Output = Context -> Stack[--Context -> SP];

    return 1;
}

int HaruForthPeek(HaruForthContext *Context, int IndexFromTop, HaruForthValue *Output) {
    if (!Context || !Output)
        return 0;

    if (IndexFromTop < 0)
        return 0;

    if (Context -> SP - 1 - IndexFromTop < 0)
        return 0;

    *Output = Context -> Stack[Context -> SP - 1 - IndexFromTop];

    return 1;
}

static HaruForthWordFunction HaruForthFindWord(HaruForthContext *Context, const char *Name) {
    for (int i = 0; i < Context -> WordCount; i++) {
        if (strcmp(Context -> Words[i].Name, Name) == 0) {
            return Context -> Words[i].Function;
        }
    }

    return NULL;
}

int HaruForthExecuteLine(HaruForthContext *Context, const char *Line) {
    if (!Context || !Line)
        return 0;

    char Buffer[HARU_FORTH_LINE_MAX];

    strncpy(Buffer, Line, sizeof(Buffer) - 1);

    Buffer[sizeof(Buffer) - 1] = '\0';

    char *Token = strtok(Buffer, " \t\r\n");
    while (Token) {
        if (Token[0] == '\\') {
            break;
        } else if (Token[0] == '"') {
            char *Start = Line + (Token - Buffer) + 1;

            char *End = strchr(Start, '"');
            if (!End)
                return 0;

            *End = '\0';

            HaruForthPushString(Context, Start);

            Token = strtok(NULL, " \t\r\n");

            continue;
        }

        char *End = NULL;
        double Value = strtod(Token, &End);

        if (End && End != Token && *End == '\0') {
            if (!HaruForthPushNumber(Context, Value))
                return 0;

            Token = strtok(NULL, " \t\r\n");

            continue;
        }

        HaruForthWordFunction Word = HaruForthFindWord(Context, Token);
        if (!Word) {
            char Error[128];

            snprintf(Error, sizeof(Error), "Forth error: unknown word '%s'", Token);

            if (Context -> Print)
                Context -> Print(Context, Error);

            return 0;
        }

        if (!Word(Context)) {
            char Error[128];

            snprintf(Error, sizeof(Error), "Forth error: word failed '%s'", Token);

            if (Context -> Print)
                Context -> Print(Context, Error);

            return 0;
        }

        Token = strtok(NULL, " \t\r\n");
    }

    return 1;
}

void HaruForthRegisterStandardWords(HaruForthContext *Context) {
    HaruForthRegisterWord(Context, "dup", HaruForthWordDup);
    HaruForthRegisterWord(Context, "drop", HaruForthWordDrop);
    HaruForthRegisterWord(Context, "swap", HaruForthWordSwap);
    HaruForthRegisterWord(Context, "over", HaruForthWordOver);

    HaruForthRegisterWord(Context, "+", HaruForthWordAdd);
    HaruForthRegisterWord(Context, "-", HaruForthWordSub);
    HaruForthRegisterWord(Context, "*", HaruForthWordMul);
    HaruForthRegisterWord(Context, "/", HaruForthWordDiv);

    HaruForthRegisterWord(Context, ".", HaruForthWordDot);
    HaruForthRegisterWord(Context, ".s", HaruForthWordDotS);
    HaruForthRegisterWord(Context, "depth", HaruForthWordDepth);
    HaruForthRegisterWord(Context, "clear", HaruForthWordClear);
    HaruForthRegisterWord(Context, "words", HaruForthWordWords);
    HaruForthRegisterWord(Context, "cr", HaruForthWordCr);
    HaruForthRegisterWord(Context, "emit", HaruForthWordEmit);
}
