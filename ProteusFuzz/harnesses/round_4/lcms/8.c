// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsAllocNamedColorList at cmsnamed.c:749:30 in lcms2.h
// cmsSignalError at cmserr.c:510:16 in lcms2_plugin.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsRealloc at cmserr.c:286:17 in lcms2_plugin.h
// cmsOpenProfileFromMemTHR at cmsio0.c:1271:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

// Fuzz driver for lcms library functions

static void cleanup_resources(cmsContext ctx, void* ptr, cmsHPROFILE profile, cmsNAMEDCOLORLIST* ncl) {
    if (ncl != NULL) {
        cmsFreeNamedColorList(ncl);
    }
    if (profile != NULL) {
        cmsCloseProfile(profile);
    }
    if (ptr != NULL) {
        _cmsFree(ctx, ptr);
    }
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a new context for fuzzing
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        // If we can't create a context, we cannot safely proceed with context-specific APIs
        return 0;
    }

    // Extract parameters from input data
    cmsUInt32Number size1 = 0;
    cmsUInt32Number size2 = 0;
    cmsUInt32Number n = 0;
    cmsUInt32Number colorantCount = 0;
    cmsUInt32Number errorCode = 0;
    
    if (Size >= 4) {
        size1 = *(const cmsUInt32Number*)(Data);
    }
    if (Size >= 8) {
        size2 = *(const cmsUInt32Number*)(Data + 4);
    }
    if (Size >= 12) {
        n = *(const cmsUInt32Number*)(Data + 8);
    }
    if (Size >= 16) {
        colorantCount = *(const cmsUInt32Number*)(Data + 12);
    }
    if (Size >= 20) {
        errorCode = *(const cmsUInt32Number*)(Data + 16);
    }

    // Limit values to reasonable ranges to avoid crashes
    if (size1 > 1000000) size1 = 1000000;
    if (size2 > 1000000) size2 = 1000000;
    if (n > 1000) n = 1000;
    if (colorantCount > 16) colorantCount = 16; // cmsMAXCHANNELS is typically 16

    void* ptr = NULL;
    cmsHPROFILE profile = NULL;
    cmsNAMEDCOLORLIST* ncl = NULL;

    // Test _cmsMalloc
    ptr = _cmsMalloc(ctx, size1);

    // Test _cmsRealloc
    void* reallocPtr = _cmsRealloc(ctx, ptr, size2);
    if (reallocPtr != NULL) {
        // If realloc succeeded, the old ptr is invalid, so don't free it separately
        // unless the API docs say otherwise. Usually realloc frees the old block.
        // We just update ptr.
        ptr = reallocPtr;
    }

    // Test cmsOpenProfileFromMemTHR with input data as profile
    if (Size > 128) {
        profile = cmsOpenProfileFromMemTHR(ctx, Data, Size);
    }

    // Test cmsAllocNamedColorList
    const char* prefix = "C";
    const char* suffix = "L";
    
    if (Size > 256) {
        // Ensure we don't read past buffer
        if (Size >= 256 + 32) {
            prefix = (const char*)(Data + 256);
        }
    }
    if (Size > 288) {
        if (Size >= 288 + 32) {
            suffix = (const char*)(Data + 288);
        }
    }
    
    ncl = cmsAllocNamedColorList(ctx, n, colorantCount, prefix, suffix);

    // Test cmsSignalError
    if (Size > 320) {
        const char* formatStr = (const char*)(Data + 320);
        // Ensure format string is null-terminated or safe
        char safeFormat[256];
        size_t copyLen = (Size - 320) < 255 ? (Size - 320) : 255;
        memcpy(safeFormat, formatStr, copyLen);
        safeFormat[copyLen] = '\0';
        
        cmsSignalError(ctx, errorCode, safeFormat, "arg1", "arg2");
    }

    // Cleanup
    cleanup_resources(ctx, ptr, profile, ncl);

    return 0;
}