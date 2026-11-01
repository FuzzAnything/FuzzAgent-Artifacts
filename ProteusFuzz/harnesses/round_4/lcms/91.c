// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
// cmsGetTransformOutputFormat at cmsxform.c:1432:27 in lcms2.h
// cmsFormatterForColorspaceOfProfile at cmspack.c:4025:27 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateProfilePlaceholder at cmsio0.c:526:23 in lcms2.h
// cmsCreateProfilePlaceholder at cmsio0.c:526:23 in lcms2.h
// cmsCreateProfilePlaceholder at cmsio0.c:526:23 in lcms2.h
// cmsCreateProofingTransform at cmsxform.c:1393:25 in lcms2.h
// cmsGetTransformOutputFormat at cmsxform.c:1432:27 in lcms2.h
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
// cmsChangeBuffersFormat at cmsxform.c:1441:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static cmsHPROFILE load_profile_from_data(const uint8_t *data, size_t size) {
    if (size == 0) return NULL;
    // Attempt to load profile from memory buffer
    return cmsOpenProfileFromMem((const void*)data, size);
}

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
}

static void cleanup_transform(cmsHTRANSFORM hTransform) {
    if (hTransform) {
        cmsDeleteTransform(hTransform);
    }
}

int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        // Fallback to default global context if creation fails
        ctx = NULL; // In lcms2, NULL usually implies global context for most functions
    }

    // Split input data into potential profiles
    size_t part_size = Size / 3;
    if (part_size < 4) part_size = Size / 2;
    if (part_size < 4) part_size = Size;

    cmsHPROFILE hInput = load_profile_from_data(Data, Size);
    cmsHPROFILE hOutput = NULL;
    cmsHPROFILE hProofing = NULL;

    // Try to create additional profiles from different slices if available
    if (Size > part_size) {
        hOutput = load_profile_from_data(Data + part_size, Size - part_size);
    }
    if (Size > part_size * 2) {
        hProofing = load_profile_from_data(Data + part_size * 2, Size - part_size * 2);
    }

    // Fallback to dummy profiles if loading failed, to test API robustness
    // Note: cmsCreateProfilePlaceholder is often used for testing invalid paths
    if (!hInput) hInput = cmsCreateProfilePlaceholder(ctx);
    if (!hOutput) hOutput = cmsCreateProfilePlaceholder(ctx);
    if (!hProofing) hProofing = cmsCreateProfilePlaceholder(ctx);

    // Extract some fuzzing parameters from the input data
    cmsUInt32Number inputFormat = (Size > 0) ? Data[0] : 0;
    cmsUInt32Number outputFormat = (Size > 1) ? Data[1] : 0;
    cmsUInt32Number intent = (Size > 2) ? (Data[2] % 4) : 0;
    cmsUInt32Number proofingIntent = (Size > 3) ? (Data[3] % 4) : 0;
    cmsUInt32Number flags = (Size > 4) ? Data[4] : 0;
    cmsUInt32Number nBytes = (Size > 5) ? (Data[5] % 3) + 1 : 1;
    cmsBool isFloat = (Size > 6) ? (Data[6] % 2) : 0;

    cmsHTRANSFORM hTransform = NULL;
    cmsHTRANSFORM hExtendedTransform = NULL;

    // 1. Test cmsCreateProofingTransform
    hTransform = cmsCreateProofingTransform(hInput, inputFormat, hOutput, outputFormat,
                                            hProofing, intent, proofingIntent, flags);

    if (hTransform) {
        // 2. Test cmsGetTransformOutputFormat
        cmsUInt32Number outFmt = cmsGetTransformOutputFormat(hTransform);
        (void)outFmt; // Use value to avoid unused warning

        // 3. Test cmsDoTransformLineStride
        // Allocate small buffers for transformation
        size_t bufSize = 1024;
        uint8_t *inBuf = malloc(bufSize);
        uint8_t *outBuf = malloc(bufSize);
        if (inBuf && outBuf) {
            memset(inBuf, 0, bufSize);
            memset(outBuf, 0, bufSize);
            cmsDoTransformLineStride(hTransform, inBuf, outBuf, 1, 1,
                                     bufSize, bufSize, bufSize, bufSize);
            free(inBuf);
            free(outBuf);
        }

        // 4. Test cmsChangeBuffersFormat
        cmsBool changed = cmsChangeBuffersFormat(hTransform, inputFormat, outputFormat);
        (void)changed;

        cleanup_transform(hTransform);
    }

    // 5. Test cmsCreateExtendedTransform
    // Prepare arrays for extended transform
    cmsHPROFILE profiles[2];
    cmsBool bpc[2];
    cmsUInt32Number intents[2];
    cmsFloat64Number adaptation[2];

    profiles[0] = hInput;
    profiles[1] = hOutput;
    bpc[0] = (Size > 7) ? (Data[7] % 2) : 0;
    bpc[1] = (Size > 8) ? (Data[8] % 2) : 0;
    intents[0] = intent;
    intents[1] = proofingIntent;
    adaptation[0] = 0.0;
    adaptation[1] = 0.0;

    cmsUInt32Number nProfiles = 2;
    if (Size > 9) nProfiles = (Data[9] % 3); // 0, 1, or 2

    if (nProfiles == 0) {
        // Test with 0 profiles (should fail or handle gracefully)
        hExtendedTransform = cmsCreateExtendedTransform(ctx, 0, NULL, NULL, NULL, NULL, NULL, 0,
                                                        inputFormat, outputFormat, flags);
    } else {
        hExtendedTransform = cmsCreateExtendedTransform(ctx, nProfiles, profiles, bpc, intents,
                                                        adaptation, NULL, 0, inputFormat, outputFormat, flags);
    }

    if (hExtendedTransform) {
        cmsGetTransformOutputFormat(hExtendedTransform);
        cleanup_transform(hExtendedTransform);
    }

    // 6. Test cmsFormatterForColorspaceOfProfile
    cmsUInt32Number formatter = cmsFormatterForColorspaceOfProfile(hInput, nBytes, isFloat);
    (void)formatter;

    // Cleanup
    cleanup_profile(hInput);
    cleanup_profile(hOutput);
    cleanup_profile(hProofing);

    if (ctx) {
        cmsDeleteContext(ctx);
    }

    return 0;
}