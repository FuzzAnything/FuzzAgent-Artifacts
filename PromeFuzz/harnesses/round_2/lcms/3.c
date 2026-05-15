// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
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

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    // Create a context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        return 0;
    }

    // Create a dummy profile from fuzzer data
    cmsHPROFILE profile = cmsOpenProfileFromMem(Data, Size);
    
    // If profile creation failed, create a default sRGB profile for testing
    if (!profile) {
        profile = cmsCreate_sRGBProfileTHR(context);
        if (!profile) {
            cmsDeleteContext(context);
            return 0;
        }
    }

    // First call to cmsGetPostScriptCSA to get required buffer size
    cmsUInt32Number intent = 0;
    cmsUInt32Number flags = 0;
    cmsUInt32Number bufferSize = cmsGetPostScriptCSA(context, profile, intent, flags, NULL, 0);

    // Allocate memory using _cmsMalloc
    void* buffer = _cmsMalloc(context, bufferSize);

    // Second call to cmsGetPostScriptCSA to fill the buffer
    if (buffer && bufferSize > 0) {
        cmsUInt32Number result = cmsGetPostScriptCSA(context, profile, intent, flags, buffer, bufferSize);
        (void)result; // Use result to avoid unused variable warning
    }

    // Free the allocated memory
    if (buffer) {
        _cmsFree(context, buffer);
    }

    // Clean up
    if (profile) {
        cmsCloseProfile(profile);
    }
    cmsDeleteContext(context);

    return 0;
}