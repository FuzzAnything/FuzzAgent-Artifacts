// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
// cmsGetProfileContextID at cmsio0.c:571:22 in lcms2.h
// cmsGBDAlloc at cmssm.c:302:22 in lcms2.h
// cmsGetContextUserData at cmsplugin.c:1021:17 in lcms2.h
// cmsGetContextUserData at cmsplugin.c:1021:17 in lcms2.h
// cmsCreateProfilePlaceholder at cmsio0.c:526:23 in lcms2.h
// cmsGetProfileContextID at cmsio0.c:571:22 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static void cleanup_context(cmsContext ctx) {
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

static void cleanup_profile(cmsHPROFILE profile) {
    if (profile != NULL) {
        cmsCloseProfile(profile);
    }
}

static void cleanup_mutex(cmsContext ctx, void* mtx) {
    if (ctx != NULL && mtx != NULL) {
        _cmsDestroyMutex(ctx, mtx);
    }
}

static void cleanup_gbd(cmsContext ctx, cmsHANDLE gbd) {
    if (ctx != NULL && gbd != NULL) {
        // Use _cmsFree as cmsFree is not declared in the public API
        _cmsFree(ctx, gbd);
    }
}

int LLVMFuzzerTestOneInput_132(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    // Test 2: _cmsCreateMutex
    void* mutex = _cmsCreateMutex(ctx);
    if (mutex != NULL) {
        // Test 6: _cmsDestroyMutex
        _cmsDestroyMutex(ctx, mutex);
    }

    // Test 3: cmsGetProfileContextID with NULL
    cmsContext profile_ctx = cmsGetProfileContextID(NULL);
    (void)profile_ctx; // Expected to be NULL

    // Test 4: cmsGBDAlloc
    cmsHANDLE gbd = cmsGBDAlloc(ctx);
    if (gbd != NULL) {
        // Test 5: cmsGetContextUserData (before setting)
        void* user_data = cmsGetContextUserData(ctx);
        (void)user_data; // Expected to be NULL

        // Cleanup GBD
        cleanup_gbd(ctx, gbd);
    }

    // Test 5: cmsGetContextUserData again after GBD cleanup
    void* user_data_after = cmsGetContextUserData(ctx);
    (void)user_data_after;

    // Try to create a profile to test cmsGetProfileContextID with valid profile
    cmsHPROFILE profile = cmsCreateProfilePlaceholder(ctx);
    if (profile != NULL) {
        cmsContext profile_ctx_valid = cmsGetProfileContextID(profile);
        (void)profile_ctx_valid;
        cleanup_profile(profile);
    }

    // Test 1: cmsGetStageContextID
    // We need a valid cmsStage. Since creating one directly is complex and internal,
    // we skip direct invocation to avoid undefined behavior with NULL or invalid pointers.
    // The API requires a valid cmsStage pointer.

    // Cleanup context
    cleanup_context(ctx);

    return 0;
}