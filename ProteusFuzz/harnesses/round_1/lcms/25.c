// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsGetHeaderModel at cmsio0.c:1026:27 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsSetHeaderModel at cmsio0.c:1032:16 in lcms2.h
// cmsSetHeaderModel at cmsio0.c:1032:16 in lcms2.h
// cmsSetHeaderModel at cmsio0.c:1032:16 in lcms2.h
// cmsSetHeaderModel at cmsio0.c:1032:16 in lcms2.h
// cmsSetHeaderModel at cmsio0.c:1032:16 in lcms2.h
// cmsGetPostScriptColorResource at cmsps2.c:1525:27 in lcms2.h
// cmsGetPostScriptColorResource at cmsps2.c:1525:27 in lcms2.h
// cmsGetPostScriptColorResource at cmsps2.c:1525:27 in lcms2.h
// cmsSaveProfileToIOhandler at cmsio0.c:1446:27 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a context with NULL plugin and NULL user data
    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) {
        return 0;
    }

    // Attempt to open a profile from the input memory
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(Data, Size);

    if (hProfile != NULL) {
        // Test cmsGetHeaderModel
        cmsUInt32Number model = cmsGetHeaderModel(hProfile);
        (void)model; // Suppress unused variable warning

        // Test cmsSetHeaderRenderingIntent with various intents
        cmsSetHeaderRenderingIntent(hProfile, 0);
        cmsSetHeaderRenderingIntent(hProfile, 1);
        cmsSetHeaderRenderingIntent(hProfile, 2);
        cmsSetHeaderRenderingIntent(hProfile, 3);
        // Test with out-of-range intent
        cmsSetHeaderRenderingIntent(hProfile, 4);
        cmsSetHeaderRenderingIntent(hProfile, 255);

        // Test cmsSetHeaderModel with various models
        cmsSetHeaderModel(hProfile, 0);
        cmsSetHeaderModel(hProfile, 1);
        cmsSetHeaderModel(hProfile, 2);
        cmsSetHeaderModel(hProfile, 3);
        cmsSetHeaderModel(hProfile, 255);

        // Test cmsGetPostScriptColorResource
        // Since cmsIOHANDLER is an opaque struct, we cannot initialize it directly.
        // We pass NULL for the IO handler to test the function path that handles NULL.
        // Alternatively, we could try to create a file-based handler, but NULL is safer for fuzzing
        // to avoid complex setup and potential crashes in custom IO callbacks.
        
        // Try different resource types (assuming enum values 0 and 1 for testing)
        cmsUInt32Number status = cmsGetPostScriptColorResource(ContextID, 0, hProfile, 0, 0, NULL);
        (void)status;
        status = cmsGetPostScriptColorResource(ContextID, 1, hProfile, 0, 0, NULL);
        (void)status;
        status = cmsGetPostScriptColorResource(ContextID, 2, hProfile, 0, 0, NULL);
        (void)status;

        // Test cmsSaveProfileToIOhandler
        // First try with NULL io (saves to internal buffer)
        cmsUInt32Number bytesUsed = cmsSaveProfileToIOhandler(hProfile, NULL);
        (void)bytesUsed;

        // Cleanup the profile
        cleanup_profile(hProfile);
    }

    // Cleanup context
    cmsDeleteContext(ContextID);

    return 0;
}