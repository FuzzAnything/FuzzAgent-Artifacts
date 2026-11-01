// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
// cmsDoTransform at cmsxform.c:192:16 in lcms2.h
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
// cmsDoTransform at cmsxform.c:192:16 in lcms2.h
// cmsCreateProofingTransform at cmsxform.c:1393:25 in lcms2.h
// cmsDoTransform at cmsxform.c:192:16 in lcms2.h
// cmsCreateProfilePlaceholder at cmsio0.c:526:23 in lcms2.h
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
// cmsDoTransformStride at cmsxform.c:211:16 in lcms2.h
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
// cmsSetProfileVersion at cmsio0.c:1138:17 in lcms2.h
// cmsTransform2DeviceLink at cmsvirt.c:1194:23 in lcms2.h
// cmsSetPCS at cmsio0.c:1075:16 in lcms2.h
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
// cmsDoTransform at cmsxform.c:192:16 in lcms2.h
// cmsTransform2DeviceLink at cmsvirt.c:1194:23 in lcms2.h
// cmsSetDeviceClass at cmsio0.c:1099:16 in lcms2.h
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
// cmsSetColorSpace at cmsio0.c:1087:16 in lcms2.h
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
// cmsDoTransform at cmsxform.c:192:16 in lcms2.h
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
// cmsDoTransform at cmsxform.c:192:16 in lcms2.h
// cmsCreateProofingTransform at cmsxform.c:1393:25 in lcms2.h
// cmsDoTransform at cmsxform.c:192:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static cmsContext create_dummy_context(void) {
    return cmsCreateContext(NULL, NULL);
}

static cmsHPROFILE create_dummy_profile(cmsContext ctx, cmsUInt32Number colorSpace) {
    cmsHPROFILE hProfile = cmsCreateProfilePlaceholder(ctx);
    if (!hProfile) return NULL;
    cmsSetProfileVersion(hProfile, 0x02000000);
    cmsSetPCS(hProfile, cmsSigXYZData);
    cmsSetDeviceClass(hProfile, cmsSigDisplayClass);
    cmsSetColorSpace(hProfile, colorSpace);
    return hProfile;
}

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile) cmsCloseProfile(hProfile);
}

static void cleanup_transform(cmsHTRANSFORM hTransform) {
    if (hTransform) cmsDeleteTransform(hTransform);
}

static void cleanup_context(cmsContext ctx) {
    if (ctx) cmsDeleteContext(ctx);
}

int LLVMFuzzerTestOneInput_167(const uint8_t *Data, size_t Size) {
    if (Size < 12) return 0;

    cmsContext ctx = create_dummy_context();
    if (!ctx) return 0;

    cmsHPROFILE profiles[256];
    cmsBool bpc[256];
    cmsUInt32Number intents[256];
    cmsFloat64Number adaptation[256];
    cmsHTRANSFORM transform = NULL;
    cmsHPROFILE deviceLink = NULL;
    cmsHPROFILE proofingProfile = NULL;
    cmsHPROFILE inputProfile = NULL;
    cmsHPROFILE outputProfile = NULL;
    cmsHPROFILE proofProfile = NULL;

    // Allocate buffers for transformation
    size_t max_pixels = 1024;
    size_t pixel_size = 12; // Max typical size (e.g., 4 channels * 32-bit float)
    uint8_t *inputBuf = malloc(max_pixels * pixel_size);
    uint8_t *outputBuf = malloc(max_pixels * pixel_size);
    if (!inputBuf || !outputBuf) {
        cleanup_context(ctx);
        free(inputBuf);
        free(outputBuf);
        return 0;
    }
    memset(inputBuf, 0, max_pixels * pixel_size);
    memset(outputBuf, 0, max_pixels * pixel_size);

    // Parse fuzz data to determine actions and parameters
    uint8_t action = Data[0] % 10;
    uint8_t nProfiles = (Data[1] % 255) + 1;
    cmsUInt32Number inputFormat = Data[2] % 10; // Simplified mapping
    cmsUInt32Number outputFormat = Data[3] % 10;
    cmsUInt32Number intent = Data[4] % 5;
    cmsUInt32Number flags = Data[5] % 100;
    cmsFloat64Number version = (Data[6] / 255.0) * 4.0 + 2.0;

    // Ensure we don't exceed the available input size when creating profiles
    // We need at least 10 bytes for the initial setup + nProfiles bytes for the loop
    if (Size < 10 + nProfiles) {
        nProfiles = (uint8_t)(Size - 10);
        if (nProfiles == 0) nProfiles = 1;
    }

    // Create dummy profiles for testing
    cmsUInt32Number colorSpaces[] = {cmsSigRgbData, cmsSigCmykData, cmsSigGrayData, cmsSigLabData};
    for (int i = 0; i < nProfiles && i < 256; i++) {
        profiles[i] = create_dummy_profile(ctx, colorSpaces[i % 4]);
        if (!profiles[i]) {
            // Cleanup created profiles so far
            for (int j = 0; j < i; j++) cleanup_profile(profiles[j]);
            nProfiles = i;
            break;
        }
        // Safe access to Data array
        if (7 + i < Size) bpc[i] = (Data[7 + i] % 2);
        else bpc[i] = 0;

        if (8 + i < Size) intents[i] = Data[8 + i] % 5;
        else intents[i] = 0;

        if (9 + i < Size) adaptation[i] = (Data[9 + i] / 255.0);
        else adaptation[i] = 0.0;
    }

    switch (action) {
        case 0: {
            // cmsCreateMultiprofileTransform
            if (nProfiles > 0) {
                transform = cmsCreateMultiprofileTransform(profiles, nProfiles, inputFormat, outputFormat, intent, flags);
                if (transform) {
                    cmsDoTransform(transform, inputBuf, outputBuf, (Data[10] % max_pixels) + 1);
                    cleanup_transform(transform);
                }
            }
            break;
        }
        case 1: {
            // cmsCreateExtendedTransform
            if (nProfiles > 0) {
                cmsHPROFILE gamutProfile = create_dummy_profile(ctx, cmsSigRgbData);
                transform = cmsCreateExtendedTransform(ctx, nProfiles, profiles, bpc, intents, adaptation, gamutProfile, 0, inputFormat, outputFormat, flags);
                if (transform) {
                    cmsDoTransform(transform, inputBuf, outputBuf, (Data[10] % max_pixels) + 1);
                    cleanup_transform(transform);
                }
                cleanup_profile(gamutProfile);
            }
            break;
        }
        case 2: {
            // cmsCreateProofingTransform
            inputProfile = create_dummy_profile(ctx, cmsSigRgbData);
            outputProfile = create_dummy_profile(ctx, cmsSigCmykData);
            proofProfile = create_dummy_profile(ctx, cmsSigGrayData);
            if (inputProfile && outputProfile && proofProfile) {
                transform = cmsCreateProofingTransform(inputProfile, inputFormat, outputProfile, outputFormat, proofProfile, intent, intent, flags);
                if (transform) {
                    cmsDoTransform(transform, inputBuf, outputBuf, (Data[10] % max_pixels) + 1);
                    cleanup_transform(transform);
                }
            }
            cleanup_profile(inputProfile);
            cleanup_profile(outputProfile);
            cleanup_profile(proofProfile);
            break;
        }
        case 3: {
            // cmsDoTransformStride
            if (nProfiles > 0) {
                transform = cmsCreateMultiprofileTransform(profiles, nProfiles, inputFormat, outputFormat, intent, flags);
                if (transform) {
                    cmsUInt32Number stride = (Data[11] % 100) + 1;
                    cmsDoTransformStride(transform, inputBuf, outputBuf, (Data[10] % max_pixels) + 1, stride);
                    cleanup_transform(transform);
                }
            }
            break;
        }
        case 4: {
            // cmsTransform2DeviceLink
            if (nProfiles > 0) {
                transform = cmsCreateMultiprofileTransform(profiles, nProfiles, inputFormat, outputFormat, intent, flags);
                if (transform) {
                    deviceLink = cmsTransform2DeviceLink(transform, version, flags);
                    if (deviceLink) {
                        cleanup_profile(deviceLink);
                    }
                    cleanup_transform(transform);
                }
            }
            break;
        }
        case 5: {
            // Mixed: Create transform, then convert to DeviceLink
            if (nProfiles > 0) {
                transform = cmsCreateExtendedTransform(ctx, nProfiles, profiles, bpc, intents, adaptation, NULL, 0, inputFormat, outputFormat, flags);
                if (transform) {
                    cmsDoTransform(transform, inputBuf, outputBuf, (Data[10] % max_pixels) + 1);
                    deviceLink = cmsTransform2DeviceLink(transform, version, flags);
                    if (deviceLink) cleanup_profile(deviceLink);
                    cleanup_transform(transform);
                }
            }
            break;
        }
        case 6: {
            // Edge case: Invalid profile count (0)
            transform = cmsCreateMultiprofileTransform(profiles, 0, inputFormat, outputFormat, intent, flags);
            if (transform) cleanup_transform(transform);
            break;
        }
        case 7: {
            // Edge case: High profile count (255)
            if (nProfiles == 255) {
                transform = cmsCreateMultiprofileTransform(profiles, 255, inputFormat, outputFormat, intent, flags);
                if (transform) {
                    cmsDoTransform(transform, inputBuf, outputBuf, 1);
                    cleanup_transform(transform);
                }
            }
            break;
        }
        case 8: {
            // Null transform flag
            if (nProfiles > 0) {
                transform = cmsCreateExtendedTransform(ctx, nProfiles, profiles, bpc, intents, adaptation, NULL, 0, inputFormat, outputFormat, cmsFLAGS_NULLTRANSFORM);
                if (transform) {
                    cmsDoTransform(transform, inputBuf, outputBuf, (Data[10] % max_pixels) + 1);
                    cleanup_transform(transform);
                }
            }
            break;
        }
        case 9: {
            // Proofing with null flags
            inputProfile = create_dummy_profile(ctx, cmsSigRgbData);
            outputProfile = create_dummy_profile(ctx, cmsSigCmykData);
            proofProfile = create_dummy_profile(ctx, cmsSigGrayData);
            if (inputProfile && outputProfile && proofProfile) {
                transform = cmsCreateProofingTransform(inputProfile, inputFormat, outputProfile, outputFormat, proofProfile, intent, intent, 0);
                if (transform) {
                    cmsDoTransform(transform, inputBuf, outputBuf, (Data[10] % max_pixels) + 1);
                    cleanup_transform(transform);
                }
            }
            cleanup_profile(inputProfile);
            cleanup_profile(outputProfile);
            cleanup_profile(proofProfile);
            break;
        }
    }

    // Cleanup created profiles
    for (int i = 0; i < nProfiles; i++) {
        cleanup_profile(profiles[i]);
    }

    free(inputBuf);
    free(outputBuf);
    cleanup_context(ctx);

    return 0;
}