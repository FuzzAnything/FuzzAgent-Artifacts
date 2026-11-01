// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// _cmsUnlockMutex at cmserr.c:655:16 in lcms2_plugin.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
// cmsGetProfileContextID at cmsio0.c:571:22 in lcms2.h
// cmsGetContextUserData at cmsplugin.c:1021:17 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// _cmsUnlockMutex at cmserr.c:655:16 in lcms2_plugin.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
// cmsGetContextUserData at cmsplugin.c:1021:17 in lcms2.h
// cmsGetProfileContextID at cmsio0.c:571:22 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

// Helper to create a context
static cmsContext create_test_context(const uint8_t *Data, size_t Size) {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return NULL;

    // We cannot use cmsSetContextUserData as it is not in the public API (lcms2.h)
    // and the compiler error confirms it's missing. 
    // We rely on the context creation itself.
    return ctx;
}

// Helper to cleanup context
static void cleanup_context(cmsContext ctx) {
    if (ctx) {
        cmsDeleteContext(ctx);
    }
}

// Helper to create a dummy profile
static cmsHPROFILE create_dummy_profile(const uint8_t *Data, size_t Size) {
    if (Size < 128) return NULL;
    
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return NULL;
    fwrite(Data, 1, Size, f);
    fclose(f);

    cmsHPROFILE hProfile = cmsOpenProfileFromFile("./dummy_file", "r");
    return hProfile;
}

int LLVMFuzzerTestOneInput_140(const uint8_t *Data, size_t Size) {
    cmsContext ctx = NULL;
    cmsHPROFILE hProfile = NULL;
    void *mtx = NULL;
    cmsContext profileCtx = NULL;
    cmsContext stageCtx = NULL;
    void *userData = NULL;

    // 1. Prepare environment
    ctx = create_test_context(Data, Size);
    
    // Handle case where context creation fails
    if (!ctx) {
        // Test functions with NULL context if they are safe
        mtx = _cmsCreateMutex(NULL);
        if (mtx) {
            _cmsUnlockMutex(NULL, mtx);
            _cmsDestroyMutex(NULL, mtx);
        }
        profileCtx = cmsGetProfileContextID(NULL);
        userData = cmsGetContextUserData(NULL);
        return 0;
    }

    // 2. Test _cmsCreateMutex
    mtx = _cmsCreateMutex(ctx);
    
    // 3. Test _cmsUnlockMutex and _cmsDestroyMutex
    if (mtx) {
        _cmsUnlockMutex(ctx, mtx);
        _cmsDestroyMutex(ctx, mtx);
    }

    // 4. Test cmsGetContextUserData
    userData = cmsGetContextUserData(ctx);

    // 5. Test Profile related functions
    if (Size > 128) {
        hProfile = create_dummy_profile(Data, Size);
        if (hProfile) {
            profileCtx = cmsGetProfileContextID(hProfile);
            cmsCloseProfile(hProfile);
            hProfile = NULL;
        }
    }

    // 6. Test Stage related functions
    // We cannot manually allocate cmsStage because it is an incomplete type in lcms2.h.
    // We cannot easily create a valid cmsStage without internal API access.
    // However, we can try to create a transformation and see if we can get stages,
    // but that's complex and might not be robust for a simple fuzzer.
    // Instead, we skip manual stage creation to avoid compilation errors.
    // If we had a valid cmsStage pointer, we would call:
    // stageCtx = cmsGetStageContextID(stage);
    // Since we can't create one safely, we skip this specific API call in the driver
    // or we could try to crash intentionally if we had a pointer, but we don't.
    // To satisfy the requirement of exploring states, we just ensure the context is valid.
    
    // 7. Cleanup
    cleanup_context(ctx);

    return 0;
}