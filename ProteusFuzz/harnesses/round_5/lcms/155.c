// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsMallocZero at cmserr.c:272:17 in lcms2_plugin.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8LoadFromMem at cmscgats.c:2556:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsCalloc at cmserr.c:279:17 in lcms2_plugin.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsMallocZero at cmserr.c:272:17 in lcms2_plugin.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8LoadFromMem at cmscgats.c:2556:22 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsGetTransformContextID at cmsxform.c:1415:22 in lcms2.h
// _cmsCalloc at cmserr.c:279:17 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_context(cmsContext ctx) {
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

static void cleanup_transform(cmsHTRANSFORM hTransform) {
    if (hTransform != NULL) {
        cmsDeleteTransform(hTransform);
    }
}

static void cleanup_it8(cmsHANDLE hIT8) {
    if (hIT8 != NULL) {
        cmsIT8Free(hIT8);
    }
}

static void cleanup_memory(void *ptr) {
    if (ptr != NULL) {
        free(ptr);
    }
}

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_155(const uint8_t *Data, size_t Size) {
    if (Size < 6) {
        return 0;
    }

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    cmsUInt32Number num = (Data[0] << 16) | (Data[1] << 8) | Data[2];
    cmsUInt32Number size = (Data[3] << 16) | (Data[4] << 8) | Data[5];
    
    // Prevent excessively large allocations
    if (num > 1000000 || size > 1000000) {
        num = 1000000;
        size = 1000000;
    }

    // Test _cmsCalloc
    void *ptr1 = _cmsCalloc(ctx, num, size);
    cleanup_memory(ptr1);

    // Test _cmsMalloc
    void *ptr2 = _cmsMalloc(ctx, size);
    cleanup_memory(ptr2);

    // Test _cmsMallocZero
    void *ptr3 = _cmsMallocZero(ctx, size);
    cleanup_memory(ptr3);

    // Test cmsIT8Alloc and cmsIT8LoadFromMem
    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
    if (hIT8 != NULL) {
        if (Size >= 10) {
            cmsHANDLE hIT8Loaded = cmsIT8LoadFromMem(ctx, &Data[0], Size);
            cleanup_it8(hIT8Loaded);
        }
        cleanup_it8(hIT8);
    }

    // Test cmsGetTransformContextID
    cmsHTRANSFORM hTransform = NULL;
    cmsContext ctxFromTransform = NULL;

    if (Size >= 20) {
        cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
        if (hProfile != NULL) {
            // Use 0 for flags instead of NULL to fix type mismatch
            hTransform = cmsCreateTransform(hProfile, TYPE_RGB_8, hProfile, TYPE_RGB_8, 0, 0);
            if (hTransform != NULL) {
                ctxFromTransform = cmsGetTransformContextID(hTransform);
                cleanup_transform(hTransform);
            }
            cleanup_profile(hProfile);
        }
    }

    if (ctxFromTransform != NULL) {
        void *ptr4 = _cmsCalloc(ctxFromTransform, num, size);
        cleanup_memory(ptr4);

        void *ptr5 = _cmsMalloc(ctxFromTransform, size);
        cleanup_memory(ptr5);

        void *ptr6 = _cmsMallocZero(ctxFromTransform, size);
        cleanup_memory(ptr6);

        cmsHANDLE hIT8FromTransform = cmsIT8Alloc(ctxFromTransform);
        if (hIT8FromTransform != NULL) {
            if (Size >= 10) {
                cmsHANDLE hIT8LoadedFromTransform = cmsIT8LoadFromMem(ctxFromTransform, &Data[0], Size);
                cleanup_it8(hIT8LoadedFromTransform);
            }
            cleanup_it8(hIT8FromTransform);
        }
    }

    cleanup_context(ctx);

    return 0;
}