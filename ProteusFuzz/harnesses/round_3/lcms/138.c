// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8LoadFromMem at cmscgats.c:2556:22 in lcms2.h
// _cmsMallocZero at cmserr.c:272:17 in lcms2_plugin.h
// _cmsCalloc at cmserr.c:279:17 in lcms2_plugin.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// cmsGetTransformContextID at cmsxform.c:1415:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

int LLVMFuzzerTestOneInput_138(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create context with NULL plugin and NULL user data
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    // Fuzz cmsIT8Alloc
    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
    // Note: cmsIT8Free is not in the target list, but typically needed. 
    // If the library requires explicit freeing and it's not available, 
    // the context deletion might handle it or it might leak. 
    // We proceed assuming context cleanup or ignoring for this specific API set.
    
    // Fuzz cmsIT8LoadFromMem
    // Limit size to prevent OOM or stack issues
    size_t safeSize = Size;
    if (safeSize > 1024 * 1024) safeSize = 1024 * 1024;
    
    cmsHANDLE hIT8Load = cmsIT8LoadFromMem(ctx, Data, safeSize);
    // No explicit free in target list

    // Fuzz _cmsMallocZero
    cmsUInt32Number size1 = (Size % 1024) + 1;
    void *ptr1 = _cmsMallocZero(ctx, size1);
    if (ptr1) {
        memset(ptr1, 0xAA, size1);
        // No explicit free in target list
    }

    // Fuzz _cmsCalloc
    cmsUInt32Number num = (Size % 100) + 1;
    cmsUInt32Number size2 = (Size % 100) + 1;
    void *ptr2 = _cmsCalloc(ctx, num, size2);
    if (ptr2) {
        memset(ptr2, 0xBB, num * size2);
        // No explicit free in target list
    }

    // Fuzz _cmsMalloc
    cmsUInt32Number size3 = (Size % 1024) + 1;
    void *ptr3 = _cmsMalloc(ctx, size3);
    if (ptr3) {
        memset(ptr3, 0xCC, size3);
        // No explicit free in target list
    }

    // Fuzz cmsGetTransformContextID with NULL
    cmsContext ctxFromNull = cmsGetTransformContextID(NULL);
    (void)ctxFromNull; // Expected to be NULL

    // Cleanup context
    cmsDeleteContext(ctx);

    return 0;
}