// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsCalloc at cmserr.c:279:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsMallocZero at cmserr.c:272:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8LoadFromMem at cmscgats.c:2556:22 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsGetTransformContextID at cmsxform.c:1415:22 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_resources(cmsContext ctx, cmsHANDLE hIT8, cmsHTRANSFORM hTrans) {
    if (hIT8 != NULL) {
        cmsIT8Free(hIT8);
    }
    if (hTrans != NULL) {
        cmsDeleteTransform(hTrans);
    }
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_158(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    cmsContext ctx = NULL;
    cmsHANDLE hIT8 = NULL;
    cmsHTRANSFORM hTrans = NULL;
    void *ptr = NULL;
    cmsUInt32Number num, size, len;

    // Extract parameters from fuzz input
    num = (Size > 0) ? Data[0] : 0;
    size = (Size > 1) ? Data[1] : 0;
    len = (Size > 2) ? Data[2] : 0;
    
    // Prevent excessive allocations
    if (num > 1000 || size > 10000 || len > 100000) {
        return 0;
    }

    // Create a context
    ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    // Test _cmsCalloc
    ptr = _cmsCalloc(ctx, num, size);
    if (ptr != NULL) {
        _cmsFree(ctx, ptr);
    }

    // Test _cmsMalloc
    ptr = _cmsMalloc(ctx, size);
    if (ptr != NULL) {
        _cmsFree(ctx, ptr);
    }

    // Test _cmsMallocZero
    ptr = _cmsMallocZero(ctx, size);
    if (ptr != NULL) {
        _cmsFree(ctx, ptr);
    }

    // Test cmsIT8Alloc
    hIT8 = cmsIT8Alloc(ctx);
    if (hIT8 != NULL) {
        // Test cmsIT8LoadFromMem with fuzz data
        cmsIT8LoadFromMem(ctx, Data, Size);
        cmsIT8Free(hIT8);
        hIT8 = NULL;
    }

    // Test cmsGetTransformContextID with NULL
    cmsContext ctxFromTrans = cmsGetTransformContextID(NULL);
    (void)ctxFromTrans; // Expected to be NULL

    // Cleanup
    cleanup_resources(ctx, hIT8, hTrans);

    return 0;
}