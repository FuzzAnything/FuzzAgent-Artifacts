// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsGetPostScriptCRD at cmsps2.c:1552:27 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// cmsGetPostScriptCRD at cmsps2.c:1552:27 in lcms2.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "lcms2.h"
#include "lcms2_plugin.h"

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    // Create a dummy ICC profile from fuzzer data
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(Data, Size);
    if (!hProfile) {
        // If profile creation fails, we can't proceed with the test
        return 0;
    }

    cmsContext context = NULL; // Use default context

    // First call to cmsGetPostScriptCRD to get required buffer size
    cmsUInt32Number requiredSize = cmsGetPostScriptCRD(context, hProfile, 0, 0, NULL, 0);

    // Call _cmsMalloc to allocate memory
    void* buffer = _cmsMalloc(context, requiredSize);

    // Second call to cmsGetPostScriptCRD to write CRD into buffer
    if (buffer) {
        cmsUInt32Number writtenSize = cmsGetPostScriptCRD(context, hProfile, 0, 0, buffer, requiredSize);
        (void)writtenSize; // Avoid unused variable warning
    }

    // Free the allocated memory
    _cmsFree(context, buffer);

    // Clean up profile
    cmsCloseProfile(hProfile);

    return 0;
}