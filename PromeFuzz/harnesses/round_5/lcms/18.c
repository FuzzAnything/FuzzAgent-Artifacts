// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// _cmsLockMutex at cmserr.c:646:19 in lcms2_plugin.h
// _cmsUnlockMutex at cmserr.c:655:16 in lcms2_plugin.h
// _cmsLockMutex at cmserr.c:646:19 in lcms2_plugin.h
// _cmsUnlockMutex at cmserr.c:655:16 in lcms2_plugin.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
// cmsOpenProfileFromMemTHR at cmsio0.c:1271:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMemTHR at cmsio0.c:1271:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMemTHR at cmsio0.c:1271:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// _cmsLockMutex at cmserr.c:646:19 in lcms2_plugin.h
// _cmsUnlockMutex at cmserr.c:655:16 in lcms2_plugin.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    // Create context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        return 0;
    }

    // Test mutex operations
    void* mutex = _cmsCreateMutex(context);
    if (mutex) {
        // Try locking and unlocking with valid mutex
        cmsBool locked = _cmsLockMutex(context, mutex);
        if (locked) {
            _cmsUnlockMutex(context, mutex);
        }
        
        // Try locking/unlocking multiple times to explore different states
        for (int i = 0; i < 3 && i < Size; i++) {
            if (_cmsLockMutex(context, mutex)) {
                _cmsUnlockMutex(context, mutex);
            }
        }
        
        // Destroy mutex
        _cmsDestroyMutex(context, mutex);
    }

    // Test profile opening from memory if we have enough data
    if (Size > 0) {
        cmsHPROFILE profile = cmsOpenProfileFromMemTHR(context, Data, (cmsUInt32Number)Size);
        if (profile) {
            cmsCloseProfile(profile);
        }
    }

    // Test with minimal data size
    if (Size == 1) {
        uint8_t single_byte = Data[0];
        cmsHPROFILE profile2 = cmsOpenProfileFromMemTHR(context, &single_byte, 1);
        if (profile2) {
            cmsCloseProfile(profile2);
        }
    }

    // Test with zero size
    if (Size > 0) {
        cmsHPROFILE profile3 = cmsOpenProfileFromMemTHR(context, Data, 0);
        if (profile3) {
            cmsCloseProfile(profile3);
        }
    }

    // Test mutex operations again after profile operations
    void* mutex2 = _cmsCreateMutex(context);
    if (mutex2) {
        // Try to lock/unlock with potential different state
        if (_cmsLockMutex(context, mutex2)) {
            _cmsUnlockMutex(context, mutex2);
        }
        _cmsDestroyMutex(context, mutex2);
    }

    // Clean up context
    cmsDeleteContext(context);

    return 0;
}

#ifdef __cplusplus
}
#endif