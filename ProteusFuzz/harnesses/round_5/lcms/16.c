// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateBCHSWabstractProfile at cmsvirt.c:946:32 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreateBCHSWabstractProfile at cmsvirt.c:946:32 in lcms2.h
// cmsCreateBCHSWabstractProfileTHR at cmsvirt.c:861:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <lcms2.h>

static void cleanup_profiles(cmsHPROFILE *profiles, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (profiles[i] != NULL) {
            cmsCloseProfile(profiles[i]);
            profiles[i] = NULL;
        }
    }
}

static void cleanup_transforms(cmsHTRANSFORM *transforms, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (transforms[i] != NULL) {
            cmsDeleteTransform(transforms[i]);
            transforms[i] = NULL;
        }
    }
}

static cmsHPROFILE create_dummy_profile_from_data(const uint8_t *data, size_t size) {
    if (size < 128) return NULL;
    return cmsOpenProfileFromMem(data, size);
}

static cmsHPROFILE create_abstract_profile_from_data(const uint8_t *data, size_t size) {
    cmsUInt32Number nLUTPoints = 16;
    cmsFloat64Number Bright = 0.0;
    cmsFloat64Number Contrast = 0.0;
    cmsFloat64Number Hue = 0.0;
    cmsFloat64Number Saturation = 0.0;
    cmsUInt32Number TempSrc = 6500;
    cmsUInt32Number TempDest = 6500;

    if (size >= 32) {
        nLUTPoints = *(const cmsUInt32Number *)(data);
        Bright = *(const cmsFloat64Number *)(data + 4);
        Contrast = *(const cmsFloat64Number *)(data + 12);
        Hue = *(const cmsFloat64Number *)(data + 20);
        Saturation = *(const cmsFloat64Number *)(data + 28);
    }
    if (size >= 40) {
        TempSrc = *(const cmsUInt32Number *)(data + 32);
        TempDest = *(const cmsUInt32Number *)(data + 36);
    }

    if (nLUTPoints < 2 || nLUTPoints > 1024) nLUTPoints = 16;

    return cmsCreateBCHSWabstractProfile(nLUTPoints, Bright, Contrast, Hue, Saturation, TempSrc, TempDest);
}

static cmsHPROFILE create_abstract_profile_thr_from_data(cmsContext ctx, const uint8_t *data, size_t size) {
    cmsUInt32Number nLUTPoints = 16;
    cmsFloat64Number Bright = 0.0;
    cmsFloat64Number Contrast = 0.0;
    cmsFloat64Number Hue = 0.0;
    cmsFloat64Number Saturation = 0.0;
    cmsUInt32Number TempSrc = 6500;
    cmsUInt32Number TempDest = 6500;

    if (size >= 32) {
        nLUTPoints = *(const cmsUInt32Number *)(data);
        Bright = *(const cmsFloat64Number *)(data + 4);
        Contrast = *(const cmsFloat64Number *)(data + 12);
        Hue = *(const cmsFloat64Number *)(data + 20);
        Saturation = *(const cmsFloat64Number *)(data + 28);
    }
    if (size >= 40) {
        TempSrc = *(const cmsUInt32Number *)(data + 32);
        TempDest = *(const cmsUInt32Number *)(data + 36);
    }

    if (nLUTPoints < 2 || nLUTPoints > 1024) nLUTPoints = 16;

    return cmsCreateBCHSWabstractProfileTHR(ctx, nLUTPoints, Bright, Contrast, Hue, Saturation, TempSrc, TempDest);
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;

    cmsHPROFILE profiles[256] = {0};
    cmsHTRANSFORM transforms[10] = {0};
    size_t profile_count = 0;
    size_t transform_count = 0;

    // Create some dummy profiles from data or abstract profiles
    if (Size >= 128) {
        profiles[0] = create_dummy_profile_from_data(Data, Size);
        if (profiles[0]) profile_count++;
    }

    if (Size >= 40) {
        profiles[profile_count] = create_abstract_profile_from_data(Data, Size);
        if (profiles[profile_count]) profile_count++;
    }

    if (ctx != NULL && Size >= 40) {
        profiles[profile_count] = create_abstract_profile_thr_from_data(ctx, Data, Size);
        if (profiles[profile_count]) profile_count++;
    }

    // Ensure at least one profile exists for transform creation
    if (profile_count == 0) {
        profiles[0] = cmsCreateBCHSWabstractProfile(16, 0.0, 0.0, 0.0, 0.0, 6500, 6500);
        if (profiles[0]) profile_count = 1;
    }

    // Extract parameters from data if possible
    cmsUInt32Number inputFormat = 0;
    cmsUInt32Number outputFormat = 0;
    cmsUInt32Number intent = 0;
    cmsUInt32Number flags = 0;

    if (Size >= 16) {
        inputFormat = *(const cmsUInt32Number *)(Data + profile_count * 4);
        outputFormat = *(const cmsUInt32Number *)(Data + profile_count * 4 + 4);
        intent = *(const cmsUInt32Number *)(Data + profile_count * 4 + 8);
        flags = *(const cmsUInt32Number *)(Data + profile_count * 4 + 12);
    }

    // Limit intent to valid range
    if (intent > 3) intent = 0;

    // Test cmsCreateTransform
    if (profile_count >= 2) {
        transforms[transform_count++] = cmsCreateTransform(profiles[0], inputFormat, profiles[1], outputFormat, intent, flags);
    } else if (profile_count >= 1) {
        transforms[transform_count++] = cmsCreateTransform(profiles[0], inputFormat, profiles[0], outputFormat, intent, flags);
    }

    // Test cmsCreateMultiprofileTransform
    if (profile_count >= 1) {
        transforms[transform_count++] = cmsCreateMultiprofileTransform(profiles, profile_count, inputFormat, outputFormat, intent, flags);
    }

    // Test cmsCreateMultiprofileTransformTHR
    if (profile_count >= 1 && ctx != NULL) {
        transforms[transform_count++] = cmsCreateMultiprofileTransformTHR(ctx, profiles, profile_count, inputFormat, outputFormat, intent, flags);
    }

    // Test cmsCreateExtendedTransform
    if (profile_count >= 1 && ctx != NULL) {
        cmsBool *bpc = (cmsBool *)malloc(profile_count * sizeof(cmsBool));
        cmsUInt32Number *intents = (cmsUInt32Number *)malloc(profile_count * sizeof(cmsUInt32Number));
        cmsFloat64Number *adaptation = (cmsFloat64Number *)malloc(profile_count * sizeof(cmsFloat64Number));

        if (bpc && intents && adaptation) {
            for (size_t i = 0; i < profile_count; i++) {
                bpc[i] = (i % 2 == 0);
                intents[i] = (i % 4);
                adaptation[i] = 0.0;
            }

            transforms[transform_count++] = cmsCreateExtendedTransform(
                ctx,
                profile_count,
                profiles,
                bpc,
                intents,
                adaptation,
                NULL,
                0,
                inputFormat,
                outputFormat,
                flags
            );
        }

        free(bpc);
        free(intents);
        free(adaptation);
    }

    // Cleanup
    cleanup_transforms(transforms, transform_count);
    cleanup_profiles(profiles, profile_count);
    cmsDeleteContext(ctx);

    return 0;
}