//
//  Haruhi Engine
//
//  Original source code derived from Kunamo Engine:
//    Copyright © 2026 Kunamo Entertainment. All rights reserved.
//
//  Modifications & Integration:
//    Copyright © 2026 AveriC & Averi
//

#ifndef CORE_VERSIONING_H
#define CORE_VERSIONING_H

    #include "Types.h"

    typedef u32 HaruVersion;

    #define HARU_MAKE_VERSION(Major, Minor, Patch) (((u32)(Major) << 22) | ((u32)(Minor) << 12) | ((u32)(Patch)))

    #define HARU_VERSION_MAJOR(Version) ((u32)(Version) >> 22)
    #define HARU_VERSION_MINOR(Version) (((u32)(Version) >> 12) & 0x3FF)
    #define HARU_VERSION_PATCH(Version) ((u32)(Version) & 0xFFF)

    #define HARU_ENGINE_VERSION HARU_MAKE_VERSION(0, 1, 0)

#endif
