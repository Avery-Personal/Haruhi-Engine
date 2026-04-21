//
//  Haruhi Engine
//
//  Original source Code derived from Kunamo Engine:
//    Copyright © 2026 Kunamo Entertainment. All rights reserved.
//
//  Modifications & Integration:
//    Copyright © 2026 AveriC & Averi
//

#ifndef CORE_TYPES_H
#define CORE_TYPES_H

    #include <setjmp.h>

    typedef signed char i8;
    typedef signed short i16;
    typedef signed int i32;
    typedef signed long long i64;

    typedef unsigned char u8;
    typedef unsigned short u16;
    typedef unsigned int u32;
    typedef unsigned long long u64;

    typedef enum {
        HARU_FALSE = 0,
        HARU_TRUE = 1
    } HaruBoolean;

    typedef enum {
        HARU_EXIT_FAILURE = -1,
        HARU_EXIT_SUCCESS = 0
    } HaruExitCode;

    typedef enum {
        HARU_RESULT_SUCCESS = 1,
        HARU_RESULT_FAILURE = 0,
        HARU_RESULT_ERROR = -1,
    } HaruResult;

    typedef struct {
        jmp_buf Environment;
        int Code;
    } HaruException;

    #define TRY(Context) if (((Context).Code = setjmp((Context).Environment)) == 0)
    #define CATCH(Context) else
    #define THROW(Context, Error) do { (Context).Code = (Error); longjmp((Context).Environment, (Error)); } while(0)

#endif
