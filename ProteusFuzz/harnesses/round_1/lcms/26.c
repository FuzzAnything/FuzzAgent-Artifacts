// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsMallocZero at cmserr.c:272:17 in lcms2_plugin.h
// _cmsCalloc at cmserr.c:279:17 in lcms2_plugin.h
// _cmsDupMem at cmserr.c:302:17 in lcms2_plugin.h
// _cmsRealloc at cmserr.c:286:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsRealloc at cmserr.c:286:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2_plugin.h>

static cmsContext create_context(const uint8_t *Data, size_t Size) {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return NULL;
    }
    return ctx;
}

static void cleanup_context(cmsContext ctx) {
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext ctx = create_context(Data, Size);
    if (ctx == NULL) {
        return 0;
    }

    // Extract parameters from input data if available
    cmsUInt32Number size = 0;
    cmsUInt32Number num = 0;
    cmsUInt32Number elem_size = 0;

    if (Size >= 4) {
        size = *(const cmsUInt32Number*)(Data);
    }
    if (Size >= 8) {
        num = *(const cmsUInt32Number*)(Data + 4);
    }
    if (Size >= 12) {
        elem_size = *(const cmsUInt32Number*)(Data + 8);
    }

    // Limit sizes to prevent excessive allocation
    if (size > 1024 * 1024) size = 1024 * 1024;
    if (num > 10000) num = 10000;
    if (elem_size > 1024 * 1024) elem_size = 1024 * 1024;

    void *ptr1 = NULL;
    void *ptr2 = NULL;
    void *ptr3 = NULL;
    void *ptr4 = NULL;
    void *ptr5 = NULL;
    void *ptr6 = NULL;

    // Test _cmsMalloc
    ptr1 = _cmsMalloc(ctx, size);
    
    // Test _cmsMallocZero
    ptr2 = _cmsMallocZero(ctx, size);
    
    // Test _cmsCalloc
    ptr3 = _cmsCalloc(ctx, num, elem_size);
    
    // Test _cmsDupMem (only if ptr1 is valid)
    if (ptr1 != NULL && size > 0) {
        ptr4 = _cmsDupMem(ctx, ptr1, size);
    }

    // Test _cmsRealloc on ptr1
    if (ptr1 != NULL) {
        ptr5 = _cmsRealloc(ctx, ptr1, size + 100);
        // ptr1 is now invalid if realloc succeeded (it was moved or freed internally)
        // We must NOT free ptr1 here.
        if (ptr5 != NULL) {
            _cmsFree(ctx, ptr5);
            ptr5 = NULL;
        }
        // Do not free ptr1 here as it was passed to realloc
        ptr1 = NULL;
    }

    // Test _cmsRealloc on NULL (should behave like malloc)
    ptr6 = _cmsRealloc(ctx, NULL, size);
    if (ptr6 != NULL) {
        _cmsFree(ctx, ptr6);
        ptr6 = NULL;
    }

    // Cleanup allocated pointers
    if (ptr2 != NULL) {
        _cmsFree(ctx, ptr2);
    }
    if (ptr3 != NULL) {
        _cmsFree(ctx, ptr3);
    }
    if (ptr4 != NULL) {
        _cmsFree(ctx, ptr4);
    }

    cleanup_context(ctx);
    return 0;
}