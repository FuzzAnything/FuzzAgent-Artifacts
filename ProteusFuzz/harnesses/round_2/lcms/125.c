// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromMemTHR at cmsio0.c:1271:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsGetHeaderModel at cmsio0.c:1026:27 in lcms2.h
// cmsSetHeaderModel at cmsio0.c:1032:16 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsSaveProfileToIOhandler at cmsio0.c:1446:27 in lcms2.h
// cmsGetPostScriptColorResource at cmsps2.c:1525:27 in lcms2.h
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

int LLVMFuzzerTestOneInput_125(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create context with two NULL arguments as required by cmsCreateContext
    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) return 0;

    // Open profile from memory
    cmsHPROFILE hProfile = cmsOpenProfileFromMemTHR(ContextID, Data, Size);
    
    // If profile is invalid, we can still test some functions or just return
    // However, most target functions require a valid profile. 
    // We will proceed to test functions that might handle NULL or invalid states gracefully 
    // or just skip if the API strictly requires valid handles.
    // Based on the description, cmsGetHeaderModel has undefined behavior if NULL.
    // So we only proceed if hProfile is valid.
    if (hProfile == NULL) {
        cmsDeleteContext(ContextID);
        return 0;
    }

    // 1. Fuzz cmsGetHeaderModel
    cmsUInt32Number model = cmsGetHeaderModel(hProfile);
    (void)model;

    // 2. Fuzz cmsSetHeaderModel
    cmsUInt32Number setModel = 0;
    if (Size >= 4) {
        memcpy(&setModel, Data, 4);
    }
    cmsSetHeaderModel(hProfile, setModel);

    // 3. Fuzz cmsSetHeaderRenderingIntent
    cmsUInt32Number intent = 0;
    if (Size >= 1) {
        intent = Data[0] % 4; // Ensure intent is between 0 and 3
    }
    cmsSetHeaderRenderingIntent(hProfile, intent);

    // 4. Fuzz cmsSaveProfileToIOhandler
    // We cannot instantiate cmsIOHANDLER directly as it is an incomplete type.
    // We pass NULL to save to an internal buffer as per documentation.
    cmsUInt32Number savedBytes = cmsSaveProfileToIOhandler(hProfile, NULL);
    (void)savedBytes;

    // 5. Fuzz cmsGetPostScriptColorResource
    // We pass NULL for the IO handler as we cannot construct one.
    cmsUInt32Number resourceType = 0;
    if (Size >= 2) {
        resourceType = ((cmsUInt32Number)Data[1] << 8) | Data[0];
    }
    cmsUInt32Number flags = 0;
    if (Size >= 3) {
        flags = Data[2];
    }
    cmsUInt32Number status = cmsGetPostScriptColorResource(ContextID, (cmsPSResourceType)resourceType, hProfile, intent, flags, NULL);
    (void)status;

    // Cleanup
    cleanup_profile(hProfile);
    cmsDeleteContext(ContextID);

    return 0;
}