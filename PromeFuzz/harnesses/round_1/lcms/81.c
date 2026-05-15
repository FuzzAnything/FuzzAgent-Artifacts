// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPluginTHR at cmsplugin.c:546:19 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// _cmsLockMutex at cmserr.c:646:19 in lcms2_plugin.h
// cmsOpenProfileFromMemTHR at cmsio0.c:1271:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    // Create context with default settings
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        return 0;
    }

    // Try to register a plugin (using NULL as plugin to test error path)
    cmsPluginTHR(context, NULL);

    // Create a mutex
    void* mutex = _cmsCreateMutex(context);
    
    // Lock the mutex if created
    if (mutex) {
        _cmsLockMutex(context, mutex);
    }

    // Try to open profile from fuzzer input
    // Use the entire input as profile data
    cmsHPROFILE profile = cmsOpenProfileFromMemTHR(context, Data, (cmsUInt32Number)Size);
    
    // Clean up profile if successfully opened
    if (profile) {
        cmsCloseProfile(profile);
    }

    // Destroy mutex if created
    if (mutex) {
        _cmsDestroyMutex(context, mutex);
    }

    // Clean up context
    cmsDeleteContext(context);

    return 0;
}