// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
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

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    // Create a context for the operations
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        return 0;
    }

    // Create a dummy profile from fuzz data for testing
    cmsHPROFILE hProfile = NULL;
    if (Size > 0) {
        // Use fuzz data to create a profile in memory
        hProfile = cmsOpenProfileFromMem(Data, Size);
    }
    
    // If profile creation failed, create a default sRGB profile
    if (!hProfile) {
        hProfile = cmsCreate_sRGBProfile();
        if (!hProfile) {
            cmsDeleteContext(ctx);
            return 0;
        }
    }

    // First call to cmsGetPostScriptCSA to get required buffer size
    cmsUInt32Number intent = 0;
    cmsUInt32Number flags = 0;
    
    // Use some fuzz data to determine intent and flags if available
    if (Size >= 8) {
        intent = *(cmsUInt32Number*)Data;
        flags = *(cmsUInt32Number*)(Data + 4);
    }
    
    // Get required buffer size
    cmsUInt32Number bufferSize = cmsGetPostScriptCSA(ctx, hProfile, intent, flags, NULL, 0);
    
    // Allocate memory using _cmsMalloc
    void* buffer = _cmsMalloc(ctx, bufferSize);
    
    // Second call to cmsGetPostScriptCSA to actually write the data
    if (buffer && bufferSize > 0) {
        cmsUInt32Number written = cmsGetPostScriptCSA(ctx, hProfile, intent, flags, buffer, bufferSize);
        (void)written; // Use variable to avoid unused warning
    }
    
    // Free the allocated memory
    _cmsFree(ctx, buffer);
    
    // Clean up
    cmsCloseProfile(hProfile);
    cmsDeleteContext(ctx);
    
    return 0;
}