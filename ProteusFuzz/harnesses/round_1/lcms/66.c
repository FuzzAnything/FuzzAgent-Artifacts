// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsMallocZero at cmserr.c:272:17 in lcms2_plugin.h
// _cmsCalloc at cmserr.c:279:17 in lcms2_plugin.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsMD5computeID at cmsmd5.c:257:19 in lcms2.h
// cmsMD5alloc at cmsmd5.c:154:21 in lcms2_plugin.h
// cmsMD5add at cmsmd5.c:172:16 in lcms2_plugin.h
// cmsMD5finish at cmsmd5.c:215:16 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

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

int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext ContextID = NULL;
    cmsHPROFILE hProfile = NULL;
    cmsHANDLE md5Handle = NULL;
    cmsProfileID ProfileID;
    void *ptr = NULL;
    cmsBool result;

    // Initialize context with NULL plugin and userdata
    ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) {
        return 0;
    }

    // Test _cmsMallocZero
    ptr = _cmsMallocZero(ContextID, Size);
    if (ptr != NULL) {
        // Memory allocated via plugin, rely on context cleanup.
    }

    // Test _cmsCalloc
    cmsUInt32Number num = (Size > 0) ? 1 : 0;
    cmsUInt32Number size = (Size > 0) ? Size : 1;
    ptr = _cmsCalloc(ContextID, num, size);
    if (ptr != NULL) {
        // Rely on context cleanup
    }

    // Create a profile from input data if it looks like a valid ICC profile
    hProfile = cmsOpenProfileFromMem((const cmsUInt8Number*)Data, Size);
    if (hProfile != NULL) {
        // Test cmsMD5computeID
        result = cmsMD5computeID(hProfile);
        (void)result;
        cleanup_profile(hProfile);
        hProfile = NULL;
    }

    // Test MD5 functions
    md5Handle = cmsMD5alloc(ContextID);
    if (md5Handle != NULL) {
        // Test cmsMD5add with input data
        cmsMD5add(md5Handle, Data, Size);
        
        // Test cmsMD5finish
        cmsMD5finish(&ProfileID, md5Handle);
        
        // cmsMD5free is not a public API in lcms2. 
        // The handle is managed by the context. 
        // We rely on cmsDeleteContext to free the memory associated with the context.
        // We do not call free() or cmsMD5free() explicitly.
        md5Handle = NULL;
    }

    // Cleanup context, which should free all associated resources including the MD5 handle
    cleanup_context(ContextID);

    return 0;
}