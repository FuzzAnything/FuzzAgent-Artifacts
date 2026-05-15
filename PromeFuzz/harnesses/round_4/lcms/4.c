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

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    // Initialize context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        return 0;
    }

    // Create a dummy profile from fuzz data
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(Data, Size);
    if (!hProfile) {
        // If profile creation fails, use a default sRGB profile
        hProfile = cmsCreate_sRGBProfile();
        if (!hProfile) {
            cmsDeleteContext(context);
            return 0;
        }
    }

    // Parse Intent and dwFlags from fuzz data
    cmsUInt32Number Intent = 0;
    cmsUInt32Number dwFlags = 0;
    
    if (Size >= 1) {
        Intent = Data[0] % 4;  // Valid intents: 0-3
    }
    if (Size >= 5) {
        // Use bytes 1-4 for dwFlags
        dwFlags = *(cmsUInt32Number*)(Data + 1);
    }

    // First call to cmsGetPostScriptCSA to get required buffer size
    cmsUInt32Number bufferSize = cmsGetPostScriptCSA(context, hProfile, Intent, dwFlags, NULL, 0);

    // Allocate buffer using _cmsMalloc
    void* buffer = _cmsMalloc(context, bufferSize);
    
    // Second call to cmsGetPostScriptCSA to fill the buffer
    if (buffer) {
        cmsGetPostScriptCSA(context, hProfile, Intent, dwFlags, buffer, bufferSize);
    }

    // Free the allocated buffer
    _cmsFree(context, buffer);

    // Cleanup
    cmsCloseProfile(hProfile);
    cmsDeleteContext(context);

    return 0;
}