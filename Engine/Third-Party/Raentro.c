#include "Raentro.h"

static const char *RaEnumOSToString(RaOperatingSystemType Type) {
    switch (Type) {
        case RaType_WindowsOS: return "Windows";
        case RaType_MacOS: return "MacOS";
        case RaType_LinuxOS: return "Linux*";
        case RaType_FreeOS: return "FreeBSD";

        default: return "Unknown";
    }

    return "Unknown";
}

static const char *RaEnumAllocToString(RaAllocationType Type) {
    switch (Type) {
        case RaType_WindowsAllocation: return "Virtual Allocation";
        case RaType_UNIXAllocation: return "Mapping";
        case RaType_FreestandingAllocation: return "Static heap";

        default: return "Unknown";
    }

    return "Unknown";
}

int RaStringLength(const char *String) {
    int Length = 0;

    while (String[Length] != '\0') {
        Length++;
    }

    return Length;
}

const char *RaFormat_Size(size_t Bytes) {
    static char Buffer[32];

    const char *Units[] = { "B", "KB", "MB", "GB" };

    double Size = (double) Bytes;

    int Unit = 0;

    while (Size >= 1024.0 && Unit < 3) {
        Size /= 1024.0;
        Unit++;
    }

    snprintf(Buffer, sizeof(Buffer), "%.2f %s", Size, Units[Unit]);

    return Buffer;
}

static RaTagStat *RaFindOrCreateTag(const char *Tag) {
    RaTagStat *_Tag = g_TagStats;

    while (_Tag) {
        if (_Tag -> Tag == Tag)
            return _Tag;

        _Tag = _Tag -> Next;
    }

    _Tag = (RaTagStat *)RaSys_Alloc(sizeof(RaTagStat));
    
    _Tag -> Tag = Tag;
    _Tag -> Size = 0;
    _Tag -> Count = 0;
    _Tag -> Next = g_TagStats;

    g_TagStats = _Tag;

    return _Tag;
}

static size_t RaAlign(size_t _Size) {
    return (_Size + (RA_ALIGNMENT - 1)) & ~(RA_ALIGNMENT - 1);
}

static RaBlock *RaHeap_RequestFromOS(size_t _Size) {
    size_t Total = sizeof(RaBlock) + _Size + RA_FOOTER_SIZE;

    void *Memory = RaSys_Alloc(Total);
    if (!Memory)
        return 0;

    RaBlock *Block = (RaBlock *) Memory;
    
    Block -> _Size = _Size;
    Block -> Free = 0;
    Block -> Next = 0;
    Block -> Previous = g_Heap.Tail;

    if (g_Heap.Tail)
        g_Heap.Tail -> Next = Block;
    else
        g_Heap.Head = Block;

    g_Heap.Tail = Block;
    g_Heap.TotalSize += Total;

    return Block;
}

static void RaHeap_SplitBlock(RaBlock *Block, size_t _Size) {
    if (Block -> _Size <= _Size + sizeof(RaBlock) + RA_FOOTER_SIZE)
        return;

    RaBlock *NewBlock = (RaBlock *)((unsigned char *)(Block + 1) + _Size + RA_FOOTER_SIZE);

    NewBlock -> _Size = Block -> _Size - _Size - sizeof(RaBlock);
    NewBlock -> Free = 1;

    NewBlock -> Next = Block -> Next;
    NewBlock -> Previous = Block;

    if (NewBlock -> Next)
        NewBlock -> Next -> Previous = NewBlock;

    Block -> Next = NewBlock;
    Block -> _Size = _Size;

    if (g_Heap.Tail == Block)
        g_Heap.Tail = NewBlock;
}

static void RaHeap_Coalesce(RaBlock *Block) {
    if (Block -> Next && Block -> Next -> Free) {
        Block -> _Size += sizeof(RaBlock) + Block -> Next -> _Size + RA_FOOTER_SIZE;
        Block -> Next = Block -> Next -> Next;

        if (Block -> Next)
            Block -> Next -> Previous = Block;
        else
            g_Heap.Tail = Block;
    }

    if (Block -> Previous && Block -> Previous -> Free) {
        Block = Block -> Previous;

        RaHeap_Coalesce(Block);
    }
}

void RaHeap_Validate(void) {
    #if RA_HEAP_VALIDATE_LEVEL >= RA_HEAP_VALIDATE_BASIC
        RaHeap_ValidateBasic();
    #endif

    #if RA_HEAP_VALIDATE_LEVEL >= RA_HEAP_VALIDATE_FULL
        RaHeap_ValidateFull();
    #endif

    #if RA_HEAP_VALIDATE_LEVEL >= RA_HEAP_VALIDATE_RUNDOWN
        RaHeap_ValidateRundown();
    #endif

    #if RA_HEAP_VALIDATE_LEVEL >= RA_HEAP_VALIDATE_EXTENSIVE
        RaHeap_ValidateExtensive();
    #endif
}

static void RaHeap_ValidateBasic(void) {
    RaBlock *Block = g_Heap.Head;
    RaBlock *Previous  = 0;

    size_t Count = 0;

    while (Block) {
        if (Block -> _Size == 0) {
            printf("[HEAP ERROR] Block with size 0 at %p\n", (void *) Block);

            return;
        }

        if (Block -> Previous != Previous) {
            printf("[HEAP ERROR] Previous pointer mismatch at %p\n", (void *) Block);

            return;
        }

        if (Block -> Next && Block -> Next -> Previous != Block) {
            printf("[HEAP ERROR] Next -> Previous mismatch at %p\n", (void *) Block);

            return;
        }

        if (Block -> Next) {
            unsigned char *Expected = (unsigned char *)(Block + 1) + Block -> _Size + RA_FOOTER_SIZE;

            if ((void *) Expected != (void *) Block -> Next) {
                printf("[HEAP ERROR] Non-contiguous Block at %p\n", (void *) Block);

                return;
            }
        }

        Previous = Block;
        Block = Block -> Next;

        if (++Count > 100000) {
            printf("[HEAP ERROR] Possible heap cycle detected\n");

            return;
        }
    }

    if (g_Heap.Head && g_Heap.Head -> Previous != 0) {
        printf("[HEAP ERROR] Head Previous is not NULL\n");

        return;
    }

    if (g_Heap.Tail && g_Heap.Tail -> Next != 0) {
        printf("[HEAP ERROR] Tail Next is not NULL\n");

        return;
    }

    return;
}

static void RaHeap_ValidateFull(void) {
    size_t ComputedUsed = 0;
    uintptr_t LastEnd = 0;

    RaBlock *Block = g_Heap.Head;

    while (Block) {
        #if RA_DEBUG
            if (Block -> Canary != RA_CANARY_VALUE)
                printf("Header canary corrupted\n");
        #endif

        uintptr_t BlockStart = (uintptr_t)(Block + 1);
        uintptr_t BlockEnd = BlockStart + Block -> _Size;

        if (BlockStart < LastEnd)
            printf("Overlapping blocks detected\n");

        LastEnd = BlockEnd;

        if (!Block -> Free)
            ComputedUsed += Block -> _Size;

        Block = Block -> Next;
    }

    if (ComputedUsed != g_Heap.UsedSize)
        printf("Heap used size mismatch\n");
}

static void RaHeap_ValidateRundown(void) {
    RaBlock *Block = g_Heap.Head;

    while (Block) {
        unsigned char *Data = (unsigned char *)(Block + 1);

        if (Block -> Free) {
            for (size_t i = 0; i < Block -> _Size; i++) {
                if (Data[i] != RA_FREE_POISON)
                    printf("Write-after-free detected\n");
            }
        }

        Block = Block -> Next;
    }
}

static void RaHeap_ValidateExtensive(void) {
    RaBlock *Block = g_Heap.Head;

    while (Block) {
        RaHeap_CheckFooter(Block);
        RaHeap_ValidateFreePattern(Block);
        RaHeap_ValidateUsedPattern(Block);
        RaHeap_ValidateAdjacency(Block);

        Block = Block -> Next;
    }

    RaHeap_ValidateReverseWalk();
    RaHeap_RecalculateStats();
}

static void RaHeap_CheckFooter(RaBlock *Block) {
    uint32_t *Footer = (uint32_t *)((unsigned char *)(Block + 1) + Block -> _Size);

    if (*Footer != RA_CANARY_VALUE)
        printf("Footer canary corrupted\n");
}

static void RaHeap_ValidateFreePattern(RaBlock *Block) {
    unsigned char *Data = (unsigned char *)(Block + 1);

    for (size_t i = 0; i < Block -> _Size; i++) {
        if (Data[i] != RA_FREE_POISON) {
            printf("Partial write-after-free detected\n");
        }
    }
}

static void RaHeap_ValidateUsedPattern(RaBlock *Block) {
    unsigned char *Data = (unsigned char *)(Block + 1);

    for (size_t i = 0; i < Block -> _Size; i++) {
        if (Data[i] == RA_FREE_POISON) {
            printf("Used block contains free poison\n");
        }
    }
}

static void RaHeap_ValidateAdjacency(RaBlock *Block) {
    if (Block -> Next) {
        uintptr_t Expected = (uintptr_t)(Block + 1) + Block -> _Size + RA_FOOTER_SIZE + sizeof(RaBlock);

        if ((uintptr_t) Block -> Next != Expected) {
            printf("Non-adjacent heap blocks detected\n");
        }
    }
}

static void RaHeap_ValidateReverseWalk(void) {
    RaBlock *Block = g_Heap.Tail;
    RaBlock *Next  = NULL;

    while (Block) {
        if (Block -> Next != Next)
            printf("Reverse walk linkage mismatch\n");

        Next = Block;
        Block = Block -> Previous;
    }

    if (Next != g_Heap.Head)
        printf("Reverse walk did not end at head\n");
}

static void RaHeap_RecalculateStats(void) {
    size_t Used = 0;
    size_t Blocks = 0;

    RaBlock *Block = g_Heap.Head;

    while (Block) {
        Blocks++;

        if (!Block -> Free)
            Used += Block -> _Size;

        Block = Block -> Next;
    }

    if (Used != g_Heap.UsedSize)
        printf("Heap usage desync\n");
}

#ifdef RA_NO_OS_ALLOC

    static unsigned char StaticHeap[1024  *1024];
    static size_t StaticOffset = 0;

    void *RaSys_Alloc(size_t _Size) {
        if (StaticOffset + _Size > sizeof(StaticHeap))
            return 0;

        void *Pointer = StaticHeap + StaticOffset;

        StaticOffset += _Size;

        if (g_SystemSettings.AllocType != RaType_FreestandingAllocation) {
            g_SystemSettings.AllocType = RaType_FreestandingAllocation;
        }

        return Pointer;
    }

    void RaSys_Free(void *Pointer, size_t _Size) {
        (void) Pointer;
        (void) _Size;
    }

#elifndef RA_NO_OS_ALLOC

    #ifdef _WIN32

        #include <windows.h>

        void *RaSys_Alloc(size_t _Size) {
            if (_Size == 0)
                return 0;

            void *Memory = VirtualAlloc(0, _Size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

            if (g_SystemSettings.AllocType != RaType_WindowsAllocation) {
                g_SystemSettings.AllocType = RaType_WindowsAllocation;
            }

            return Memory;
        }

        void RaSys_Free(void *Pointer, size_t _Size) {
            (void) _Size;

            if (!Pointer)
                return;

            VirtualFree(Pointer, 0, MEM_RELEASE);
        }

    #elifndef _WIN32
        
        #include <sys/mman.h>
        #include <unistd.h>

        void *RaSys_Alloc(size_t _Size) {
            if (_Size == 0)
                return 0;

            void *Memory = mmap(0, _Size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

            if (g_SystemSettings.AllocType != RaType_UNIXAllocation) {
                g_SystemSettings.AllocType = RaType_UNIXAllocation;
            }
            
            return Memory;
        }

        void RaSys_Free(void *Pointer, size_t _Size) {
            if (!Pointer || _Size == 0)
                return;

            munmap(Pointer,  _Size);
        }

    #else

        static unsigned char StaticHeap[1024  *1024];
        static size_t StaticOffset = 0;

        void *RaSys_Alloc(size_t _Size) {
            if (StaticOffset + _Size > sizeof(StaticHeap))
                return 0;

            void *Pointer = StaticHeap + StaticOffset;

            StaticOffset += _Size;

            if (g_SystemSettings.AllocType != RaType_FreestandingAllocation) {
                g_SystemSettings.AllocType = RaType_FreestandingAllocation;
            }

            return Pointer;
        }

        void RaSys_Free(void *Pointer, size_t _Size) {
            (void) Pointer;
            (void) _Size;
        }

    #endif

#endif

void *RaAlloc(size_t _Size) {
    if (_Size == 0)
        return 0;

    RA_VALIDATE_HEAP();

    _Size = RaAlign(_Size);

    RaBlock *Block = g_Heap.Head;

    while (Block) {
        if (Block -> Free && Block -> _Size >= _Size) {
            RaHeap_SplitBlock(Block, _Size);

            Block -> Free = 0;

            #if RA_DEBUG
                Block -> Canary = RA_CANARY_VALUE;
                
                uint32_t *Footer = (uint32_t *)((unsigned char *)(Block + 1) + Block -> _Size);

                *Footer = RA_CANARY_VALUE;
            #endif
            
            RaMem_Set(Block + 1, RA_ALLOC_POISON, Block -> _Size);

            g_Heap.UsedSize += _Size;

            RA_VALIDATE_HEAP();

            return (void *)(Block + 1);
        }

        Block = Block -> Next;
    }

    Block = RaHeap_RequestFromOS(_Size);
    if (!Block)
        return 0;

    #if RA_DEBUG
        Block -> Canary = RA_CANARY_VALUE;
        
        uint32_t *Footer = (uint32_t *)((unsigned char *)(Block + 1) + Block -> _Size);

        *Footer = RA_CANARY_VALUE;
    #endif

    RaMem_Set(Block + 1, RA_ALLOC_POISON, Block -> _Size);

    g_Heap.UsedSize += _Size;

    RA_VALIDATE_HEAP();

    return (void *)(Block + 1);
}

void *RaRealloc(void *Pointer, size_t NewSize) {
    if (!Pointer)
        return RaAlloc(NewSize);

    if (NewSize == 0) {
        RaFree(Pointer);

        return 0;
    }

    RA_VALIDATE_HEAP();

    RaBlock *Block = ((RaBlock *) Pointer) - 1;

    NewSize = RaAlign(NewSize);

    if (Block -> _Size >= NewSize) {
        RaHeap_SplitBlock(Block, NewSize);

        RA_VALIDATE_HEAP();

        return Pointer;
    }

    RaBlock *Next = Block -> Next;

    if (Next && Next -> Free && Block -> _Size + sizeof(RaBlock) + Next -> _Size >= NewSize) {
        Block -> _Size += sizeof(RaBlock) + Next -> _Size;
        Block -> Next = Next -> Next;

        if (Block -> Next)
            Block -> Next -> Previous = Block;
        else
            g_Heap.Tail = Block;

        RaHeap_SplitBlock(Block, NewSize);

        RA_VALIDATE_HEAP();

        return Pointer;
    }

    void *NewPointer = RaAlloc(NewSize);
    if (!NewPointer)
        return 0;

    RaMem_Copy(NewPointer, Pointer, Block -> _Size);
    RaFree(Pointer);

    RA_VALIDATE_HEAP();

    return NewPointer;
}

void RaFree(void *Pointer) {
    if (!Pointer)
        return;

    RA_VALIDATE_HEAP();

    RaBlock *Block = ((RaBlock *) Pointer) - 1;

    if (Block -> Free) {
        printf("Double free detected\n");

        return;
    }

    #if RA_DEBUG

        if (Block -> Canary != RA_CANARY_VALUE) {
            printf("[HEAP ERROR] Header canary corrupted at %p\n", Pointer);

            return;
        }

        uint32_t *Footer = (uint32_t *)((unsigned char *)(Block + 1) + Block -> _Size);

        if (*Footer != RA_CANARY_VALUE) {
            printf("[HEAP ERROR] Footer canary corrupted at %p\n", Pointer);

            return;
        }
        
    #endif

    Block -> Free = 1;

    g_Heap.UsedSize -= Block -> _Size;

    RaMem_Set(Block + 1, RA_FREE_POISON, Block -> _Size);
    RaHeap_Coalesce(Block);

    RA_VALIDATE_HEAP();
}

void *RaDebugging_Allocation(size_t _Size, const char *FileName, int Line) {
    void *Pointer = RaAlloc(_Size);
    if (!Pointer)
        return 0;

    RaRecord *Record = (RaRecord *)RaSys_Alloc(sizeof(RaRecord));
    if (!Record)
        return Pointer;

    Record -> Pointer = Pointer;
    Record -> _Size = _Size;
    Record -> File = FileName;
    Record -> Line = Line;
    Record -> Tag = NULL;
    Record -> ID = g_NextAllocID++;
    Record -> Freed = 0;

    Record -> Next = g_RecordHead;

    RaTagStat *TagStat = RaFindOrCreateTag(Record -> Tag);

    TagStat -> Size += _Size;
    TagStat -> Count += 1;

    g_RecordHead = Record;
    g_LiveAllocs++;

    g_Stats.TotalAllocated += _Size;
    g_Stats.CurrentUsage += _Size;
    g_Stats.AllocationCount++;

    if (g_Stats.CurrentUsage > g_Stats.PeakUsage)
        g_Stats.PeakUsage = g_Stats.CurrentUsage;

    #if RA_DEBUG
        printf("[ALLOC #%llu] %zu bytes at %p (%s:%d)\n", Record -> ID, _Size, Pointer, FileName, Line);
    #endif

    return Pointer;
}

void RaDebugging_Free(void *Pointer, const char *FileName, int Line) {
    if (!Pointer)
        return;

    RaRecord *Previous = NULL;
    RaRecord *Record = g_RecordHead;

    while (Record) {
        if (Record -> Pointer == Pointer)
            break;

        Previous = Record;
        Record = Record -> Next;
    }

    if (!Record) {
        printf("[FREE ERROR] Untracked pointer %p (%s:%d)\n", Pointer, FileName, Line);

        return;
    }

    if (Record -> Freed) {
        printf("[FREE ERROR] Double free of %p (alloc #%llu at %s:%d)\n", Pointer, Record -> ID, Record -> File, Record -> Line);

        return;
    }

    RaTagStat *TagStat = RaFindOrCreateTag(Record -> Tag);

    TagStat -> Size -= Record -> _Size;
    TagStat -> Count -= 1;

    Record -> Freed = 1;

    g_LiveAllocs--;

    g_Stats.TotalFreed += Record -> _Size;
    g_Stats.CurrentUsage -= Record -> _Size;
    g_Stats.FreeCount++;

    #if RA_DEBUG
        printf("[FREE #%llu] %p (%s:%d)\n", Record -> ID, Pointer, FileName, Line);
    #endif

    RaFree(Pointer);
}

void RaDebugging_DumpLeaks(void) {
    RaRecord *Record = g_RecordHead;

    size_t LeakCount = 0;
    size_t LeakBytes = 0;

    while (Record) {
        if (!Record -> Freed) {
            printf("[LEAK #%llu] %zu bytes at %p (%s:%d)\n", Record -> ID, Record -> _Size, Record -> Pointer, Record -> File, Record -> Line);

            LeakCount++;
            LeakBytes += Record -> _Size;
        }

        Record = Record -> Next;
    }

    printf("Leak summary: %zu leaks (%s)\n", LeakCount, RaFormat_Size(LeakBytes));
}

void RaDebugging_ReportLeaks(void) {
    RaRecord *Record = g_RecordHead;

    size_t LeakCount = 0;
    size_t LeakBytes = 0;

    while (Record) {
        if (!Record -> Freed) {
            printf("\n[RM] Leak detected:\n");
            printf("     Pointer = %p Size = %s\n", Record -> Pointer, RaFormat_Size(Record -> _Size));
            printf("     Allocated at %s:%d\n", Record -> File, Record -> Line);
            printf("     Tag = %s\n", Record -> Tag != NULL ? Record -> Tag : "N/A");

            LeakCount++;
            LeakBytes += Record -> _Size;

            #ifdef RA_FREE_LEAKS
                RaFree(Record -> Pointer);
            #endif
        }

        Record = Record->Next;
    }

    if (LeakCount > 0) {
        printf("\n[RM] Total leaks: %zu (%s)\n", LeakCount, RaFormat_Size(LeakBytes));

        #ifdef RA_FREE_LEAKS
            printf("[RM] Auto-freed leaked memory\n");
        #endif
    }
}

void RaStats_PrintTags(void) {
    RaTagStat *_Tag = g_TagStats;

    printf("[RM] Memory by tag:\n");

    while (_Tag) {
        if (_Tag -> Size > 0) {
            printf("     %s: %s (%zu allocs)\n", _Tag -> Tag, RaFormat_Size(_Tag -> Size), _Tag -> Count);
        }
        
        _Tag = _Tag -> Next;
    }
}

void RaStats_SystemSettings(void) {
    if (g_SystemSettings.Initialized != 1) {
        g_SystemSettings.Initialized = 1;

        g_SystemSettings.Debugging = (int) RA_DEBUG;
        g_SystemSettings.ValidationLevel = (int) RA_VALIDATE_LEVEL;

        #ifdef _WIN32
            g_SystemSettings.OSType = RaType_WindowsOS;
            g_SystemSettings.AllocType = RaType_WindowsAllocation;
        #elif defined(__APPLE__)
            g_SystemSettings.OSType = RaType_MacOS;
            g_SystemSettings.AllocType = RaType_UNIXAllocation;
        #elif defined(__linux__)
            g_SystemSettings.OSType = RaType_LinuxOS;
            g_SystemSettings.AllocType = RaType_UNIXAllocation;
        #elif defined(__FreeBSD__)
            g_SystemSettings.OSType = RaType_FreeOS;
            g_SystemSettings.AllocType = RaType_UNIXAllocation;
        #else
            g_SystemSettings.OSType = RaType_UnknownOS;
            g_SystemSettings.AllocType = RaType_UnknownAllocation;
        #endif

        #ifdef RA_NO_OS_ALLOC
            g_SystemSettings.AllocType = RaType_FreestandingAllocation;
        #endif
    }

    printf("[RM] System configuration:\n");
    printf("     OS: %s\n", RaEnumOSToString(g_SystemSettings.OSType));
    printf("     Debugging: %s | Validation Level: %i\n", g_SystemSettings.Debugging >= 1 ? "ON" : "OFF", g_SystemSettings.ValidationLevel);
    printf("     Allocation Type: %s\n\n", RaEnumAllocToString(g_SystemSettings.AllocType));
}

void RaMem_Set(void *Destination, int Value, size_t _Size) {
    unsigned char *_Destination = (unsigned char *) Destination;
    unsigned char _Value = (unsigned char) Value;

    while (((size_t) _Destination & (sizeof(size_t) - 1)) && _Size) {
        *_Destination++ =  _Value;
        _Size--;
    }

    size_t Word = 0;

    for (size_t i = 0; i < sizeof(size_t); i++)
        Word = (Word << 8) | _Value;

    size_t *DestinationSize = (size_t *) _Destination;

    while (_Size >= sizeof(size_t)) {
        *DestinationSize++ = Word;
        _Size -= sizeof(size_t);
    }

    _Destination = (unsigned char *) DestinationSize;

    while (_Size--) {
        *_Destination++ = _Value;
    }
}
 
void RaMem_Copy(void *Destination, const void *Source, size_t _Size) {
    unsigned char *_Destination = (unsigned char *) Destination;
    const unsigned char *_Source = (const unsigned char *) Source;

    if (_Destination == _Source || _Size == 0)
        return;

    for (size_t i = 0; i < _Size; i++) {
        _Destination[i] = _Source[i];
    }
}

void RaMem_Move(void *Destination, const void *Source, size_t _Size) {
    unsigned char *_Destination = (unsigned char *) Destination;
    const unsigned char *_Source = (const unsigned char *) Source;

    if (_Destination == _Source || _Size == 0)
        return;

    if (_Destination < _Source) {
        while (_Size--) {
            *_Destination++ = *_Source++;
        }
    } else {
        _Destination += _Size;
        _Source += _Size;

        while (_Size--) {
            *--_Destination = *--_Source;
        }
    }
}

int RaMem_Compare(const void *A, const void *B, size_t _Size) {
    const unsigned char *PointerA = A;
    const unsigned char *PointerB = B;

    for (size_t i = 0; i < _Size; i++) {
        if (PointerA[i] != PointerB[i]) {
            return (int) PointerA[i] - (int) PointerB[i];
        }
    }

    return 0;
}
