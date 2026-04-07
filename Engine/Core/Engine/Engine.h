//
//  Haruhi Engine
// 
//  This file is of derivation from Kunamo Engine source code & has been adapted for integration of Haruhi Engine.
//  The following software is DIRECTLY seeded from Kunamo Engine source with minuscle adjustments to architecture, if any.
//
//  Portions or sectors of this file originate from Kunamo Engine source code and may include minimal modifications
// 
//  Original source code derived from Kunamo Engine:
//    Copyright © 2026 Kunamo Entertainment. All rights reserved.
//
//  Modifications & Integration:
//    Copyright © 2026 AveriC & Averi
//

#ifndef ENGINE_H
#define ENGINE_H

    #include "../../Platform/Platform.h"

    typedef struct {
        HaruPlatform Platform;

        int Running;
    } HaruEngine;
    
    HaruEngine HaruEngineInitialize();

#endif