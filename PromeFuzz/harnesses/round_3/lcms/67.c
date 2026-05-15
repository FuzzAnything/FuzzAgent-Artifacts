// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// _cmsLockMutex at cmserr.c:646:19 in lcms2_plugin.h
// _cmsUnlockMutex at cmserr.c:655:16 in lcms2_plugin.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
// cmsOpenProfileFromMemTHR at cmsio0.c:1271:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size) {
    // Create context with NULL plugin and user data
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        return 0;
    }

    // Test mutex operations
    void* mtx = _cmsCreateMutex(ctx);
    if (mtx) {
        // Try locking the mutex
        cmsBool locked = _cmsLockMutex(ctx, mtx);
        
        // Try unlocking the mutex
        _cmsUnlockMutex(ctx, mtx);
        
        // Destroy the mutex
        _cmsDestroyMutex(ctx, mtx);
    }

    // Test opening profile from memory
    if (Size > 0) {
        cmsHPROFILE profile = cmsOpenProfileFromMemTHR(ctx, Data, (cmsUInt32Number)Size);
        if (profile) {
            cmsCloseProfile(profile);
        }
    }

    // Clean up context
    cmsDeleteContext(ctx);
    
    return 0;
}