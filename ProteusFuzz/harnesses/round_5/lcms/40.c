// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGetColorSpace at cmsio0.c:1081:34 in lcms2.h
// cmsGetColorSpace at cmsio0.c:1081:34 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsGetTransformContextID at cmsxform.c:1415:22 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateProfilePlaceholder at cmsio0.c:526:23 in lcms2.h
// cmsGetProfileContextID at cmsio0.c:571:22 in lcms2.h
// cmsCreateXYZProfileTHR at cmsvirt.c:577:23 in lcms2.h
// cmsGetContextUserData at cmsplugin.c:1021:17 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_resources(cmsContext ctx, cmsHPROFILE profile1, cmsHPROFILE profile2, cmsHTRANSFORM transform) {
    if (profile1 != NULL) {
        cmsCloseProfile(profile1);
    }
    if (profile2 != NULL) {
        cmsCloseProfile(profile2);
    }
    if (transform != NULL) {
        cmsDeleteTransform(transform);
    }
    // Only delete context if it is not the global context.
    // We identify the global context by checking if it's the default one returned by cmsCreateContext(NULL, NULL) if we created it,
    // but since we can't easily compare pointers to the internal global context without a getter, 
    // we rely on the fact that cmsDeleteContext(NULL) or deleting the global context is bad.
    // However, cmsCreateContext(NULL, NULL) returns the global context.
    // We will track if we created a custom context.
    if (ctx != NULL) {
        // In LCMS2, cmsDeleteContext(NULL) is safe and does nothing, but deleting the global context is not intended.
        // To be safe, we only delete if we know we created a new one.
        // Since we don't have a direct way to check "is this the global context" without a specific API or global variable,
        // and the error log showed cmsGetGlobalContext is missing (likely an older version or specific build),
        // we will assume if we called cmsCreateContext with NULL args, we got the global context.
        // If we passed a custom allocator, we got a new one.
        // For this fuzzer, we only create a new context if Data[0] > 128, passing NULLs which returns global.
        // So actually, in the logic below, customCtx will be the global context in both branches if we pass NULL.
        // Let's adjust logic: if we want a new context, we need a custom allocator.
        // Since we don't have one, we will just use the global context (cmsCreateContext(NULL, NULL) returns global).
        // Therefore, we should NOT delete customCtx if it is the global context.
        // We will assume ctx is global if we passed NULL to cmsCreateContext.
        // To avoid deleting global, we simply don't delete ctx if we didn't create a custom allocator.
        // For simplicity in this fix, we will not delete the context in cleanup_resources if it might be global.
        // We will handle context deletion explicitly in main logic based on creation.
    }
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext customCtx = NULL;
    cmsHPROFILE profile1 = NULL;
    cmsHPROFILE profile2 = NULL;
    cmsHTRANSFORM transform = NULL;
    cmsContext ctxFromProfile = NULL;
    cmsContext ctxFromTransform = NULL;
    void *userData = NULL;
    int isCustomContext = 0;

    // Initialize custom context.
    // Note: cmsCreateContext(NULL, NULL) returns the global context.
    // To create a truly separate context, we would need a custom allocator.
    // Since we don't define one, we will just use the global context for safety,
    // or try to create one if the build supports it, but the error suggests missing functions.
    // We will stick to using the global context by default.
    // If Data[0] > 128, we attempt to create a context (which will likely be global anyway without allocator).
    // We will not delete the context in cleanup if it's the global one.
    
    customCtx = cmsCreateContext(NULL, NULL);
    // Since we passed NULL, customCtx is the global context. We must not delete it.
    isCustomContext = 0; 

    // 1. cmsCreateProfilePlaceholder
    profile1 = cmsCreateProfilePlaceholder(customCtx);
    if (profile1 == NULL) {
        // Memory failure
        return 0;
    }

    // 2. cmsGetProfileContextID
    ctxFromProfile = cmsGetProfileContextID(profile1);
    if (ctxFromProfile != NULL) {
        // Verification logic
        (void)ctxFromProfile;
    }

    // 3. cmsCreateXYZProfileTHR
    profile2 = cmsCreateXYZProfileTHR(customCtx);
    if (profile2 == NULL) {
        cleanup_resources(customCtx, profile1, NULL, NULL);
        return 0;
    }

    // 4. cmsGetContextUserData
    // We cannot set user data without cmsSetContextUserData which was flagged as undeclared in the error.
    // The error log says: "note: did you mean 'cmsGetContextUserData'?" and shows it's declared.
    // It implies cmsSetContextUserData might be missing in this specific header version or build.
    // We will skip setting data and just call get to test the function if it exists.
    // If cmsSetContextUserData is not available, we can't test the round-trip, but we can test the getter.
    // However, to avoid compilation errors, we only call the declared function.
    userData = cmsGetContextUserData(customCtx);
    // userData will likely be NULL since we didn't set it (and can't set it safely if function is missing).

    // 5. Create a simple transform to test cmsGetTransformContextID
    cmsUInt32Number srcType = cmsGetColorSpace(profile2);
    cmsUInt32Number dstType = cmsGetColorSpace(profile2);
    
    if (srcType != 0 && dstType != 0) {
        transform = cmsCreateTransform(profile2, srcType, profile2, dstType, 8, 0);
    }

    if (transform != NULL) {
        ctxFromTransform = cmsGetTransformContextID(transform);
        if (ctxFromTransform != NULL) {
            (void)ctxFromTransform;
        }
    }

    // Cleanup
    cleanup_resources(customCtx, profile1, profile2, transform);

    return 0;
}