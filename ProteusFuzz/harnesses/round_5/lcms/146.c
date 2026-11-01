// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// cmsGetContextUserData at cmsplugin.c:1021:17 in lcms2.h
// cmsDupContext at cmsplugin.c:893:22 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// _cmsUnlockMutex at cmserr.c:655:16 in lcms2_plugin.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
// _cmsUnlockMutex at cmserr.c:655:16 in lcms2_plugin.h
// _cmsUnlockMutex at cmserr.c:655:16 in lcms2_plugin.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsGetProfileContextID at cmsio0.c:571:22 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_context(cmsContext ctx) {
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_146(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext ctx = NULL;
    cmsContext ctx_dup = NULL;
    cmsHPROFILE hProfile = NULL;
    void *mutex = NULL;
    void *user_data = NULL;

    // Create a default context
    ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    // Allocate user data to test cmsGetContextUserData
    // Note: cmsSetContextUserData is not available in the current header environment,
    // so we cannot set user data. cmsGetContextUserData will return NULL, which is valid.
    user_data = malloc(Size > 0 ? 1 : 1);
    if (user_data != NULL) {
        memcpy(user_data, Data, Size > 1 ? 1 : Size);
        // cmsSetContextUserData(ctx, user_data); // Commented out due to compilation error
    }

    // Test _cmsCreateMutex
    mutex = _cmsCreateMutex(ctx);

    // Test cmsGetContextUserData
    void *retrieved_data = cmsGetContextUserData(ctx);
    (void)retrieved_data; // Suppress unused warning

    // Test cmsDupContext
    ctx_dup = cmsDupContext(ctx, user_data);
    if (ctx_dup != NULL) {
        // Test _cmsCreateMutex on duplicated context
        void *mutex_dup = _cmsCreateMutex(ctx_dup);
        if (mutex_dup != NULL) {
            // Test _cmsUnlockMutex on duplicated context mutex
            _cmsUnlockMutex(ctx_dup, mutex_dup);
            // Test _cmsDestroyMutex on duplicated context mutex
            _cmsDestroyMutex(ctx_dup, mutex_dup);
        }
        // Test _cmsUnlockMutex on original mutex with duplicated context (edge case)
        if (mutex != NULL) {
            _cmsUnlockMutex(ctx_dup, mutex);
        }
        cleanup_context(ctx_dup);
    }

    // Test _cmsUnlockMutex on original mutex
    if (mutex != NULL) {
        _cmsUnlockMutex(ctx, mutex);
    }

    // Create a dummy profile for testing cmsGetProfileContextID
    const char *dummy_file = "./dummy_file";
    FILE *f = fopen(dummy_file, "wb");
    if (f != NULL) {
        // Write some dummy data to simulate an ICC profile header (minimal)
        unsigned char dummy_icc[128] = {0};
        // ICC header signature
        dummy_icc[0] = 'a'; dummy_icc[1] = 'c'; dummy_icc[2] = 's'; dummy_icc[3] = 'p';
        // Size (4 bytes) - set to 128
        dummy_icc[4] = 0; dummy_icc[5] = 0; dummy_icc[6] = 0; dummy_icc[7] = 128;
        // CMM type, version, etc. (simplified)
        fwrite(dummy_icc, 1, 128, f);
        fclose(f);

        hProfile = cmsOpenProfileFromFile(dummy_file, "r");
        if (hProfile != NULL) {
            cmsContext profile_ctx = cmsGetProfileContextID(hProfile);
            (void)profile_ctx; // Suppress unused warning
            cleanup_profile(hProfile);
        }
    }

    // Test _cmsDestroyMutex
    if (mutex != NULL) {
        _cmsDestroyMutex(ctx, mutex);
    }

    // Cleanup
    if (user_data != NULL) {
        free(user_data);
    }
    cleanup_context(ctx);

    return 0;
}