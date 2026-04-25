/*

    Raentro | Single-file memory manager

    Version - 0.3.4
    License - MIT
    Copyright © 2026 Averi

    This is of a MODIFIED version of the Raentro memory manager and/or library.
    The usage of a restricted variable, has been removed to fit PORTABILITY of a cross-platform engine.
    Other of such changes might be noting, NOT to be explicitly.

*/

#ifndef RAENTRO_H
#define RAENTRO_H

    #include <stdio.h>
    #include <stdint.h>

    #define RA_ALIGNMENT 16
    
    #define RA_CANARY_VALUE 0xDEADC0DE

    #define RA_FREE_POISON 0xDD
    #define RA_ALLOC_POISON 0xAA

    #define RA_DEBUG 1
    #define RA_VALIDATE_LEVEL 4

    #define RA_HEAP_VALIDATE_NONE 0
    #define RA_HEAP_VALIDATE_BASIC 1
    #define RA_HEAP_VALIDATE_FULL 2
    #define RA_HEAP_VALIDATE_RUNDOWN 3
    #define RA_HEAP_VALIDATE_EXTENSIVE 4

    typedef enum {
        RaType_UnknownOS,

        RaType_WindowsOS,
        RaType_MacOS,
        RaType_LinuxOS,
        RaType_FreeOS
    } RaOperatingSystemType;

    typedef enum {
        RaType_UnknownAllocation = 0,

        RaType_WindowsAllocation,
        RaType_UNIXAllocation,
        RaType_FreestandingAllocation
    } RaAllocationType;

    typedef struct RaBlock {
        size_t _Size;
        uint8_t Free;

        struct RaBlock *Next;
        struct RaBlock *Previous;

        #if RA_DEBUG
            uint32_t Canary;
        #endif
    } RaBlock;

    typedef struct {
        RaBlock *Head;
        RaBlock *Tail;

        size_t TotalSize;
        size_t UsedSize;
    } RaHeap;

    typedef struct RaRecord {
        void *Pointer;

        size_t _Size;

        const char *File;
        int Line;

        const char *Tag;
        uint64_t ID;

        uint8_t Freed;

        struct RaRecord *Next;
    } RaRecord;

    typedef struct RaStats {
        size_t TotalAllocated;
        size_t TotalFreed;

        size_t CurrentUsage;
        size_t PeakUsage;

        size_t AllocationCount;
        size_t FreeCount;
    } RaStats;

    typedef struct RaTagStat {
        const char *Tag;

        size_t Size;
        size_t Count;

        struct RaTagStat *Next;
    } RaTagStat;

    typedef struct RaConfiguration {
        int Initialized;

        int Debugging;
        int ValidationLevel;

        union {
            RaOperatingSystemType OSType;
            RaAllocationType AllocType;
        };
    } RaConfiguration;

    static RaConfiguration g_SystemSettings;

    static RaHeap g_Heap;

    static RaStats g_Stats;
    static RaTagStat *g_TagStats;
    
    static RaRecord *g_RecordHead = NULL;
    static uint64_t g_NextAllocID = 1;
    static size_t g_LiveAllocs = 0;

    // Unused, to be? static void RaInitialize_SystemData();

    static const char *RaEnumOSToString(RaOperatingSystemType Type);
    static const char *RaEnumAllocToString(RaAllocationType Type);

    int RaStringLength(const char *String);
    const char *RaFormat_Size(size_t Bytes);

    static RaTagStat *RaFindOrCreateTag(const char *Tag);

    static size_t RaAlign(size_t _Size);

    static RaBlock *RaHeap_RequestFromOS(size_t _Size);
    static void RaHeap_SplitBlock(RaBlock *Block, size_t _Size);
    static void RaHeap_Coalesce(RaBlock *Block);

    void RaHeap_Validate(void);

    static void RaHeap_ValidateBasic(void);
    static void RaHeap_ValidateFull(void);
    static void RaHeap_ValidateRundown(void);
    static void RaHeap_ValidateExtensive(void);

    static void RaHeap_CheckFooter(RaBlock *Block);
    static void RaHeap_ValidateFreePattern(RaBlock *Block);
    static void RaHeap_ValidateUsedPattern(RaBlock *Block);
    static void RaHeap_ValidateAdjacency(RaBlock *Block);
    static void RaHeap_ValidateReverseWalk(void);
    static void RaHeap_RecalculateStats(void);

    void *RaSys_Alloc(size_t _Size);
    void RaSys_Free(void *Pointer, size_t _Size);

    void *RaAlloc(size_t _Size);
    void *RaRealloc(void *Pointer, size_t NewSize);
    void RaFree(void *Pointer);

    void *RaDebugging_Allocation(size_t _Size, const char *FileName, int Line);
    void RaDebugging_Free(void *Pointer, const char *FileName, int Line);

    void RaDebugging_DumpLeaks(void);
    void RaDebugging_ReportLeaks(void);
    
    void RaStats_PrintTags(void);
    void RaStats_SystemSettings(void);

    void RaMem_Set(void *Destination, int Value, size_t _Size);
    void RaMem_Copy(void *Destination, const void *Source, size_t _Size);
    void RaMem_Move(void *Destination, const void *Source, size_t _Size);
    int RaMem_Compare(const void *A, const void *B, size_t _Size);

    #if RA_DEBUG
        #define RA_VALIDATE_HEAP() RaHeap_Validate()

        #define RaAllocDebug(_Size) RaDebugging_Allocation(_Size, __FILE_NAME__, __LINE__)
        #define RaFreeDebug(Pointer) RaDebugging_Free(Pointer, __FILE_NAME__, __LINE__)

        #define RA_FOOTER_SIZE sizeof(uint32_t)
    #else
        #define RA_VALIDATE_HEAP() ((void) 0)
        #define RA_FOOTER_SIZE 0
    #endif

#endif
