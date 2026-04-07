//
//  Haruhi Engine
//
//  Original source code derived from Kunamo Engine:
//    Copyright © 2026 Kunamo Entertainment. All rights reserved.
//
//  Modifications & Integration:
//    Copyright © 2026 AveriC & Averi
//

#include "Platform.h"

#ifdef _WIN32
    #include "OperatingSystem/Windows/PlatformWin.c"
#elif defined(__APPLE__)
    #include "OperatingSystem/MacOS/PlatformMacOS.c"
#elif defined(__linux__)
    #include "OperatingSystem/Linux/PlatformLinux.c"
#endif
