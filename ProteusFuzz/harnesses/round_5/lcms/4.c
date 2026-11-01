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

static cmsContext create_dummy_context(void) {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return NULL;
    }
    return ctx;
}

static cmsHPROFILE create_dummy_profile(void) {
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (hProfile == NULL) {
        return NULL;
    }
    return hProfile;
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    cmsContext ContextID = create_dummy_context();
    if (ContextID == NULL) {
        return 0;
    }

    cmsHPROFILE hProfile = create_dummy_profile();
    if (hProfile == NULL) {
        cmsDeleteContext(ContextID);
        return 0;
    }

    // Step 1: First call to cmsGetPostScriptCRD to query size
    cmsUInt32Number Intent = (Data[0] % 4);
    cmsUInt32Number dwFlags = (Data[1] % 16);
    
    cmsUInt32Number requiredSize = cmsGetPostScriptCRD(ContextID, hProfile, Intent, dwFlags, NULL, 0);
    
    if (requiredSize == 0) {
        // If size is 0, we might still proceed with other steps or just clean up
        // But to explore states, let's try to allocate a small buffer if 0
        requiredSize = 1;
    }

    // Step 2: Call _cmsMalloc
    void *ptr = _cmsMalloc(ContextID, requiredSize);
    
    if (ptr != NULL) {
        // Step 3: Second call to cmsGetPostScriptCRD with allocated buffer
        cmsUInt32Number written = cmsGetPostScriptCRD(ContextID, hProfile, Intent, dwFlags, ptr, requiredSize);
        (void)written; // Suppress unused variable warning
        
        // Step 4: Call _cmsFree
        _cmsFree(ContextID, ptr);
    }

    // Cleanup
    cmsCloseProfile(hProfile);
    cmsDeleteContext(ContextID);

    return 0;
}