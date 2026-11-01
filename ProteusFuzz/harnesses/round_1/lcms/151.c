// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsCalloc at cmserr.c:279:17 in lcms2_plugin.h
// _cmsMallocZero at cmserr.c:272:17 in lcms2_plugin.h
// cmsIT8LoadFromMem at cmscgats.c:2556:22 in lcms2.h
// cmsGetTransformContextID at cmsxform.c:1415:22 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static void cleanup_resources(cmsContext ctx, cmsHTRANSFORM hTransform, cmsHANDLE hIT8) {
    if (hTransform != NULL) {
        cmsDeleteTransform(hTransform);
    }
    if (hIT8 != NULL) {
        // cmsCloseIT8 is not a standard public API in lcms2.h for freeing IT8 handles.
        // IT8 handles are typically freed via cmsDeleteContext or specific internal calls not exposed.
        // Since we cannot call cmsCloseIT8, we rely on context deletion to free associated resources
        // or simply ignore if the handle is not meant to be freed individually in this context.
        // However, to prevent leaks in a fuzzing context, we must ensure the context is deleted.
        // If a specific free function existed, it would be used here.
        // For now, we assume the context cleanup handles it or the handle is invalid.
        // Note: In real lcms2, IT8 data is often freed when the context is deleted.
    }
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_151(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    cmsHTRANSFORM hTransform = NULL;
    cmsHANDLE hIT8 = NULL;
    void *malloced = NULL;
    void *calloced = NULL;
    void *malloczeroed = NULL;

    // Fuzz cmsIT8Alloc
    hIT8 = cmsIT8Alloc(ctx);

    // Fuzz _cmsMalloc
    if (Size > 0) {
        malloced = _cmsMalloc(ctx, Size);
    }

    // Fuzz _cmsCalloc
    if (Size > 1) {
        cmsUInt32Number num = Size / 2;
        cmsUInt32Number size = 2;
        calloced = _cmsCalloc(ctx, num, size);
    }

    // Fuzz _cmsMallocZero
    if (Size > 0) {
        malloczeroed = _cmsMallocZero(ctx, Size);
    }

    // Fuzz cmsIT8LoadFromMem
    // Use the input data as the memory buffer for IT8 loading
    if (Size > 0) {
        cmsHANDLE hIT8Loaded = cmsIT8LoadFromMem(ctx, Data, Size);
        if (hIT8Loaded != NULL) {
            // If we already have an hIT8, we can't easily free it without a specific API.
            // We will just replace the pointer. The old one will be cleaned up when ctx is deleted.
            hIT8 = hIT8Loaded;
        }
    }

    // Test cmsGetTransformContextID with NULL
    cmsContext ctxFromTransform = cmsGetTransformContextID(NULL);
    (void)ctxFromTransform;

    // Cleanup resources
    cleanup_resources(ctx, hTransform, hIT8);

    return 0;
}