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

#include "Engine.h"

HaruEngine HaruEngineInitialize() {
    HaruEngine Engine;

    Engine.Running = 1;
    Engine.Platform = HaruPlatformInitialize();

    return Engine;
}
