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
    #include "OS/Windows/PlatformWin.c"
#elifdef __APPLE__
    #include "OS/MacOS/PlatformMacOS.c"
#else
    #include "OS/Linux/PlatformLinux.c"
#endif
