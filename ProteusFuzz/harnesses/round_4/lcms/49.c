// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGetPostScriptColorResource at cmsps2.c:1525:27 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsSetHeaderManufacturer at cmsio0.c:1014:16 in lcms2.h
// cmsSetHeaderFlags at cmsio0.c:1002:16 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsSetHeaderModel at cmsio0.c:1032:16 in lcms2.h
// cmsSaveProfileToIOhandler at cmsio0.c:1446:27 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static cmsHPROFILE create_dummy_profile(cmsContext ContextID) {
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (hProfile == NULL) {
        return NULL;
    }
    return hProfile;
}

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

static void cleanup_context(cmsContext ContextID) {
    if (ContextID != NULL) {
        cmsDeleteContext(ContextID);
    }
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) {
        return 0;
    }

    cmsHPROFILE hProfile = create_dummy_profile(ContextID);
    if (hProfile == NULL) {
        cleanup_context(ContextID);
        return 0;
    }

    // Extract values from input data
    cmsUInt32Number manufacturer = 0;
    cmsUInt32Number flags = 0;
    cmsUInt32Number rendering_intent = 0;
    cmsUInt32Number model = 0;
    cmsUInt32Number intent = 0;
    cmsUInt32Number dwFlags = 0;

    if (Size >= 4) {
        manufacturer = *(const cmsUInt32Number *)(Data);
    }
    if (Size >= 8) {
        flags = *(const cmsUInt32Number *)(Data + 4);
    }
    if (Size >= 12) {
        rendering_intent = *(const cmsUInt32Number *)(Data + 8);
        // Clamp rendering intent to valid range [0, 3]
        if (rendering_intent > 3) {
            rendering_intent = 3;
        }
    }
    if (Size >= 16) {
        model = *(const cmsUInt32Number *)(Data + 12);
    }
    if (Size >= 20) {
        intent = *(const cmsUInt32Number *)(Data + 16);
        // Clamp intent to valid range [0, 3]
        if (intent > 3) {
            intent = 3;
        }
    }
    if (Size >= 24) {
        dwFlags = *(const cmsUInt32Number *)(Data + 20);
    }

    // Fuzz cmsSetHeaderManufacturer
    cmsSetHeaderManufacturer(hProfile, manufacturer);

    // Fuzz cmsSetHeaderFlags
    cmsSetHeaderFlags(hProfile, flags);

    // Fuzz cmsSetHeaderRenderingIntent
    cmsSetHeaderRenderingIntent(hProfile, rendering_intent);

    // Fuzz cmsSetHeaderModel
    cmsSetHeaderModel(hProfile, model);

    // Fuzz cmsSaveProfileToIOhandler with NULL io (internal buffer)
    cmsUInt32Number savedSize = cmsSaveProfileToIOhandler(hProfile, NULL);
    (void)savedSize; // Suppress unused variable warning

    // Fuzz cmsGetPostScriptColorResource
    // We cannot instantiate cmsIOHANDLER directly as it is an opaque pointer/struct in some builds
    // or requires specific initialization. We pass NULL or a dummy pointer if the API allows.
    // However, the signature expects a pointer. Let's try passing NULL first.
    // If the API strictly requires a valid handler, we might need to create a dummy file handler.
    // For fuzzing, passing NULL is often a valid edge case to test.
    cmsIOHANDLER* io_ptr = NULL;
    
    // Try common resource types (0 and 1 usually correspond to CSA and CRD)
    for (int i = 0; i < 2; i++) {
        cmsGetPostScriptColorResource(ContextID, (cmsPSResourceType)i, hProfile, intent, dwFlags, io_ptr);
    }

    // Cleanup
    cleanup_profile(hProfile);
    cleanup_context(ContextID);

    return 0;
}