// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateProofingTransform at cmsxform.c:1393:25 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsGetHeaderManufacturer at cmsio0.c:1008:27 in lcms2.h
// cmsGetHeaderRenderingIntent at cmsio0.c:984:27 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "lcms2.h"

static void cleanup_profiles(cmsHPROFILE *profiles, int count) {
    for (int i = 0; i < count; i++) {
        if (profiles[i] != NULL) {
            cmsCloseProfile(profiles[i]);
            profiles[i] = NULL;
        }
    }
}

static void cleanup_transforms(cmsHTRANSFORM *transforms, int count) {
    for (int i = 0; i < count; i++) {
        if (transforms[i] != NULL) {
            cmsDeleteTransform(transforms[i]);
            transforms[i] = NULL;
        }
    }
}

int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;

    // Create dummy profiles from input data if possible, or use built-in
    cmsHPROFILE profiles[5] = {NULL};
    int profile_count = 0;

    // Try to create up to 5 profiles from chunks of input data
    size_t chunk_size = Size / 5;
    if (chunk_size < 1) chunk_size = 1;

    for (int i = 0; i < 5 && i < Size / chunk_size; i++) {
        const uint8_t *chunk = Data + (i * chunk_size);
        size_t len = (i == 4 || i == Size / chunk_size - 1) ? Size - (i * chunk_size) : chunk_size;
        
        // Attempt to create a profile from the chunk
        // cmsOpenProfileFromMem takes (MemPtr, dwSize), no context argument
        cmsHPROFILE h = cmsOpenProfileFromMem(chunk, (cmsUInt32Number)len);
        if (h != NULL) {
            profiles[i] = h;
            profile_count++;
        }
    }

    // If no profiles could be created, try to create a simple sRGB profile
    if (profile_count == 0) {
        profiles[0] = cmsCreate_sRGBProfile();
        if (profiles[0] != NULL) profile_count = 1;
    }

    // Ensure we have at least one profile for testing
    if (profile_count == 0) {
        cmsDeleteContext(ctx);
        return 0;
    }

    cmsHTRANSFORM transforms[5] = {NULL};
    int transform_count = 0;

    // Test cmsGetHeaderManufacturer
    if (profiles[0] != NULL) {
        cmsUInt32Number man = cmsGetHeaderManufacturer(profiles[0]);
        (void)man; // Use variable to avoid unused warning
    }

    // Test cmsGetHeaderRenderingIntent
    if (profiles[0] != NULL) {
        cmsUInt32Number intent = cmsGetHeaderRenderingIntent(profiles[0]);
        (void)intent;
    }

    // Test cmsSetHeaderRenderingIntent
    if (profiles[0] != NULL) {
        cmsUInt32Number new_intent = (Data[0] % 4); // 0-3
        cmsSetHeaderRenderingIntent(profiles[0], new_intent);
    }

    // Test cmsCreateTransformTHR
    if (profile_count >= 1) {
        cmsHTRANSFORM t = cmsCreateTransformTHR(ctx, profiles[0], (cmsUInt32Number)(Data[0] % 10), NULL, 0, (cmsUInt32Number)(Data[1] % 4), 0);
        if (t != NULL) {
            transforms[transform_count++] = t;
        }
    }

    if (profile_count >= 2) {
        cmsHTRANSFORM t = cmsCreateTransformTHR(ctx, profiles[0], (cmsUInt32Number)(Data[0] % 10), profiles[1], (cmsUInt32Number)(Data[1] % 10), (cmsUInt32Number)(Data[2] % 4), 0);
        if (t != NULL) {
            transforms[transform_count++] = t;
        }
    }

    // Test cmsCreateMultiprofileTransformTHR
    if (profile_count >= 1) {
        cmsHPROFILE *p_arr = malloc(profile_count * sizeof(cmsHPROFILE));
        if (p_arr != NULL) {
            for (int i = 0; i < profile_count; i++) {
                p_arr[i] = profiles[i];
            }
            cmsHTRANSFORM t = cmsCreateMultiprofileTransformTHR(ctx, p_arr, (cmsUInt32Number)profile_count, (cmsUInt32Number)(Data[0] % 10), (cmsUInt32Number)(Data[1] % 10), (cmsUInt32Number)(Data[2] % 4), 0);
            if (t != NULL) {
                transforms[transform_count++] = t;
            }
            free(p_arr);
        }
    }

    // Test cmsCreateProofingTransform
    if (profile_count >= 3) {
        cmsHTRANSFORM t = cmsCreateProofingTransform(profiles[0], (cmsUInt32Number)(Data[0] % 10), profiles[1], (cmsUInt32Number)(Data[1] % 10), profiles[2], (cmsUInt32Number)(Data[2] % 4), (cmsUInt32Number)(Data[3] % 4), 0);
        if (t != NULL) {
            transforms[transform_count++] = t;
        }
    }

    // Cleanup transforms
    cleanup_transforms(transforms, transform_count);

    // Cleanup profiles
    cleanup_profiles(profiles, profile_count);

    // Cleanup context
    cmsDeleteContext(ctx);

    return 0;
}