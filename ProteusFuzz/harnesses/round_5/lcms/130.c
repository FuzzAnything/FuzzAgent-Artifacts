// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsUnregisterPluginsTHR at cmsplugin.c:780:16 in lcms2.h
// cmsUnregisterPluginsTHR at cmsplugin.c:780:16 in lcms2.h
// cmsUnregisterPluginsTHR at cmsplugin.c:780:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreate_OkLabProfile at cmsvirt.c:690:23 in lcms2.h
// cmsCreate_OkLabProfile at cmsvirt.c:690:23 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsCreateNULLProfileTHR at cmsvirt.c:960:23 in lcms2.h
// cmsCreateNULLProfileTHR at cmsvirt.c:960:23 in lcms2.h
// cmsCreateXYZProfileTHR at cmsvirt.c:577:23 in lcms2.h
// cmsCreateXYZProfileTHR at cmsvirt.c:577:23 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsGetProfileContextID at cmsio0.c:571:22 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsGetProfileContextID at cmsio0.c:571:22 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_profiles(cmsHPROFILE *profiles, int count) {
    for (int i = 0; i < count; i++) {
        if (profiles[i] != NULL) {
            cmsCloseProfile(profiles[i]);
            profiles[i] = NULL;
        }
    }
}

static void cleanup_contexts(cmsContext *contexts, int count) {
    for (int i = 0; i < count; i++) {
        if (contexts[i] != NULL) {
            cmsDeleteContext(contexts[i]);
            contexts[i] = NULL;
        }
    }
}

int LLVMFuzzerTestOneInput_130(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Limit allocation sizes to prevent excessive memory usage
    if (Size > 1000) Size = 1000;

    cmsContext ctx = NULL;
    cmsHPROFILE profiles[5] = {NULL};
    int profile_count = 0;
    int ret = 0;

    // Safe access to input data
    unsigned char op = 0;
    unsigned char secondary_op = 0;

    if (Size > 0) {
        op = Data[0] % 6;
    }
    if (Size > 1) {
        secondary_op = Data[1];
    }

    // Create a context if needed
    if (op != 0 || (Size > 1 && secondary_op % 2 == 0)) {
        ctx = cmsCreateContext(NULL, NULL);
        if (ctx == NULL) return 0;
    }

    switch (op) {
        case 0:
            // Test cmsCreate_OkLabProfile
            if (ctx != NULL) {
                profiles[profile_count++] = cmsCreate_OkLabProfile(ctx);
            } else {
                profiles[profile_count++] = cmsCreate_OkLabProfile(NULL);
            }
            break;
        case 1:
            // Test cmsCreate_sRGBProfileTHR
            if (ctx != NULL) {
                profiles[profile_count++] = cmsCreate_sRGBProfileTHR(ctx);
            } else {
                profiles[profile_count++] = cmsCreate_sRGBProfileTHR(NULL);
            }
            break;
        case 2:
            // Test cmsCreateNULLProfileTHR
            if (ctx != NULL) {
                profiles[profile_count++] = cmsCreateNULLProfileTHR(ctx);
            } else {
                profiles[profile_count++] = cmsCreateNULLProfileTHR(NULL);
            }
            break;
        case 3:
            // Test cmsCreateXYZProfileTHR
            if (ctx != NULL) {
                profiles[profile_count++] = cmsCreateXYZProfileTHR(ctx);
            } else {
                profiles[profile_count++] = cmsCreateXYZProfileTHR(NULL);
            }
            break;
        case 4:
            // Test cmsGetProfileContextID with a created profile
            if (ctx != NULL) {
                cmsHPROFILE temp = cmsCreate_sRGBProfileTHR(ctx);
                if (temp != NULL) {
                    cmsContext retrieved_ctx = cmsGetProfileContextID(temp);
                    (void)retrieved_ctx; // Use the value to avoid unused warning
                    profiles[profile_count++] = temp;
                }
            } else {
                cmsHPROFILE temp = cmsCreate_sRGBProfileTHR(NULL);
                if (temp != NULL) {
                    cmsContext retrieved_ctx = cmsGetProfileContextID(temp);
                    (void)retrieved_ctx;
                    profiles[profile_count++] = temp;
                }
            }
            break;
        case 5:
            // Test cmsUnregisterPluginsTHR
            if (ctx != NULL) {
                cmsUnregisterPluginsTHR(ctx);
            } else {
                cmsUnregisterPluginsTHR(NULL);
            }
            break;
    }

    // Additional random calls to explore state
    if (Size > 1 && secondary_op % 3 == 0 && ctx != NULL) {
        cmsUnregisterPluginsTHR(ctx);
    }

    // Cleanup
    cleanup_profiles(profiles, profile_count);
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }

    return ret;
}