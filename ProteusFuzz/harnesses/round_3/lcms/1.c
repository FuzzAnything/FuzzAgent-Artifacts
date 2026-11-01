// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsGetPostScriptCRD at cmsps2.c:1552:27 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// cmsGetPostScriptCRD at cmsps2.c:1552:27 in lcms2.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) return 0;

    // Create a dummy profile to avoid NULL pointer dereference in cmsGetPostScriptCRD
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (hProfile == NULL) {
        cmsDeleteContext(ContextID);
        return 0;
    }

    cmsUInt32Number Intent = 0;
    cmsUInt32Number dwFlags = 0;
    void *mallocPtr = NULL;
    cmsUInt32Number requiredSize = 0;

    // First call to cmsGetPostScriptCRD to query size
    requiredSize = cmsGetPostScriptCRD(ContextID, hProfile, Intent, dwFlags, NULL, 0);

    // Only proceed if we got a valid size
    if (requiredSize > 0) {
        // Call _cmsMalloc
        mallocPtr = _cmsMalloc(ContextID, requiredSize);

        // Second call to cmsGetPostScriptCRD to fill buffer
        if (mallocPtr != NULL) {
            cmsGetPostScriptCRD(ContextID, hProfile, Intent, dwFlags, mallocPtr, requiredSize);
        }

        // Cleanup with _cmsFree
        if (mallocPtr != NULL) {
            _cmsFree(ContextID, mallocPtr);
        }
    }

    // Cleanup profile and context
    cmsCloseProfile(hProfile);
    cmsDeleteContext(ContextID);

    // Optional: Use input data to create a dummy file if needed for other tests
    if (Size > 0) {
        write_dummy_file(Data, Size);
    }

    return 0;
}