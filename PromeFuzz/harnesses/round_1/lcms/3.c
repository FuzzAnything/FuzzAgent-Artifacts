// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsGetPostScriptCRD at cmsps2.c:1552:27 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsGetPostScriptCRD at cmsps2.c:1552:27 in lcms2.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    // Create a context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        return 0;
    }

    // Create a dummy profile from fuzzer data
    cmsHPROFILE profile = cmsOpenProfileFromMem(Data, Size);
    if (!profile) {
        // If profile creation fails, use a built-in profile for testing
        profile = cmsCreate_sRGBProfile();
        if (!profile) {
            cmsDeleteContext(context);
            return 0;
        }
    }

    // First call to cmsGetPostScriptCRD to get required buffer size
    cmsUInt32Number requiredSize = cmsGetPostScriptCRD(context, profile, 
                                                       0,  // INTENT_PERCEPTUAL
                                                       0, NULL, 0);

    // Allocate memory using _cmsMalloc
    void* buffer = _cmsMalloc(context, requiredSize);
    if (!buffer) {
        cmsCloseProfile(profile);
        cmsDeleteContext(context);
        return 0;
    }

    // Second call to cmsGetPostScriptCRD to write CRD into buffer
    cmsUInt32Number writtenSize = cmsGetPostScriptCRD(context, profile, 
                                                      0,  // INTENT_PERCEPTUAL
                                                      0, buffer, requiredSize);

    // Free the allocated memory
    _cmsFree(context, buffer);

    // Cleanup
    cmsCloseProfile(profile);
    cmsDeleteContext(context);

    return 0;
}