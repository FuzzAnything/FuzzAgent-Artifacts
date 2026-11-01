// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsCalloc at cmserr.c:279:17 in lcms2_plugin.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsMallocZero at cmserr.c:272:17 in lcms2_plugin.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8LoadFromMem at cmscgats.c:2556:22 in lcms2.h
// cmsIT8LoadFromMem at cmscgats.c:2556:22 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsGetTransformContextID at cmsxform.c:1415:22 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_resources(cmsContext ctx, cmsHANDLE it8, cmsHTRANSFORM transform) {
    if (it8 != NULL) {
        cmsIT8Free(it8);
    }
    if (transform != NULL) {
        cmsDeleteTransform(transform);
    }
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_157(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    cmsContext ctx = NULL;
    cmsHANDLE it8 = NULL;
    cmsHTRANSFORM transform = NULL;
    void *mem_ptr = NULL;
    void *mem_zero_ptr = NULL;
    void *calloc_ptr = NULL;
    
    // Always create a context to ensure valid pointer for _cmsMalloc/_cmsCalloc
    ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    // Use data to determine sizes, ensuring they are within safe bounds
    cmsUInt32Number num = (Data[1] % 100) + 1;
    cmsUInt32Number size = (Size > 2) ? (Data[2] % 1024) + 1 : 1;
    
    // Fuzz _cmsCalloc
    calloc_ptr = _cmsCalloc(ctx, num, size);
    if (calloc_ptr != NULL) {
        // Do minimal work to trigger logic
        memset(calloc_ptr, 0x41, size * num);
        // Note: We cannot free this directly as it uses the context's allocator.
        // It will be freed when the context is closed.
    }

    // Fuzz _cmsMalloc
    mem_ptr = _cmsMalloc(ctx, size);
    if (mem_ptr != NULL) {
        memset(mem_ptr, 0x42, size);
    }

    // Fuzz _cmsMallocZero
    mem_zero_ptr = _cmsMallocZero(ctx, size);
    if (mem_zero_ptr != NULL) {
        // Verify it is zeroed
        for (size_t i = 0; i < size; i++) {
            if (((uint8_t*)mem_zero_ptr)[i] != 0) {
                break;
            }
        }
    }

    // Fuzz cmsIT8Alloc
    it8 = cmsIT8Alloc(ctx);
    if (it8 != NULL) {
        // Try to load data into it if we have enough data
        if (Size > 4) {
            cmsIT8LoadFromMem(ctx, Data + 4, (Size > 4) ? (Size - 4) : 0);
        }
    }

    // Fuzz cmsIT8LoadFromMem directly (with potentially invalid data to test robustness)
    cmsHANDLE it8_direct = NULL;
    if (Size > 4) {
        it8_direct = cmsIT8LoadFromMem(ctx, Data + 4, Size - 4);
        if (it8_direct != NULL) {
            cmsIT8Free(it8_direct);
        }
    }

    // Fuzz cmsGetTransformContextID
    // Test with NULL transform
    cmsContext ctx_from_null = cmsGetTransformContextID(NULL);
    (void)ctx_from_null; // Should be NULL

    // Cleanup
    cleanup_resources(ctx, it8, transform);
    
    return 0;
}