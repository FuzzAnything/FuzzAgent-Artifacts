// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateProfilePlaceholder at cmsio0.c:526:23 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsGetPostScriptCSA at cmsps2.c:1579:27 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// cmsGetPostScriptCSA at cmsps2.c:1579:27 in lcms2.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static cmsHPROFILE create_dummy_profile(cmsContext ContextID, const uint8_t *Data, size_t Size) {
    if (Size < 128) {
        return NULL;
    }
    // Create a minimal valid ICC profile in memory for testing
    cmsHPROFILE hProfile = cmsCreateProfilePlaceholder(ContextID);
    if (hProfile == NULL) {
        return NULL;
    }
    // Attempt to load profile data if it looks like an ICC profile
    // For fuzzing, we often just use the placeholder or try to load raw data
    if (Size > 128) {
        cmsHPROFILE hLoaded = cmsOpenProfileFromMem((const unsigned char*)Data, Size);
        if (hLoaded != NULL) {
            cmsCloseProfile(hProfile);
            return hLoaded;
        }
    }
    return hProfile;
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) {
        return 0;
    }

    cmsHPROFILE hProfile = create_dummy_profile(ContextID, Data, Size);
    if (hProfile == NULL) {
        cmsDeleteContext(ContextID);
        return 0;
    }

    cmsUInt32Number Intent = 0;
    cmsUInt32Number dwFlags = 0;
    
    // Extract intent and flags from input data if available
    if (Size > 0) {
        Intent = Data[0] & 0x03; // Limit to valid intents (0-3)
    }
    if (Size > 1) {
        dwFlags = Data[1];
    }

    // Step 1: Get required size for cmsGetPostScriptCSA
    cmsUInt32Number requiredSize = cmsGetPostScriptCSA(ContextID, hProfile, Intent, dwFlags, NULL, 0);
    
    // Step 2: Allocate memory using _cmsMalloc
    void *buffer = _cmsMalloc(ContextID, requiredSize);
    
    // Step 3: Invoke cmsGetPostScriptCSA again with allocated buffer
    if (buffer != NULL && requiredSize > 0) {
        cmsUInt32Number written = cmsGetPostScriptCSA(ContextID, hProfile, Intent, dwFlags, buffer, requiredSize);
        (void)written; // Use the return value to suppress warnings
    }

    // Step 4: Free memory using _cmsFree
    if (buffer != NULL) {
        _cmsFree(ContextID, buffer);
    }

    // Cleanup
    cmsCloseProfile(hProfile);
    cmsDeleteContext(ContextID);

    return 0;
}