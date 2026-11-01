// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDoTransformStride at cmsxform.c:211:16 in lcms2.h
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
// cmsGetTransformInputFormat at cmsxform.c:1424:27 in lcms2.h
// cmsGetTransformOutputFormat at cmsxform.c:1432:27 in lcms2.h
// cmsDoTransform at cmsxform.c:192:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <lcms2.h>

static cmsHPROFILE create_dummy_profile(const uint8_t *data, size_t size) {
    if (size < 128) return NULL; // Minimal size for a valid ICC header
    // Write data to a temporary file to load as a profile
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return NULL;
    fwrite(data, 1, size, f);
    fclose(f);

    cmsHPROFILE hProfile = cmsOpenProfileFromFile("./dummy_file", "r");
    return hProfile;
}

static void cleanup_profiles(cmsHPROFILE *profiles, int count) {
    for (int i = 0; i < count; i++) {
        if (profiles[i]) {
            cmsCloseProfile(profiles[i]);
        }
    }
}

int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Extract parameters from fuzz input
    cmsUInt32Number nProfiles = (Data[0] % 255) + 1;
    cmsUInt32Number inputFormat = Data[1];
    cmsUInt32Number outputFormat = Data[2];
    cmsUInt32Number intent = Data[3] % 4; // 0 to 3
    cmsUInt32Number flags = 0;

    // Limit profile count to avoid excessive allocation
    if (nProfiles > 10) nProfiles = 10;

    cmsHPROFILE profiles[10] = {0};
    cmsHTRANSFORM hTransform = NULL;

    // Create dummy profiles from fuzz data if possible
    size_t profileSize = Size / (nProfiles + 1);
    if (profileSize < 128) profileSize = 128;

    for (int i = 0; i < nProfiles; i++) {
        size_t offset = i * profileSize;
        if (offset + profileSize > Size) {
            // Fallback: reuse first chunk or skip
            profiles[i] = create_dummy_profile(Data, Size);
        } else {
            profiles[i] = create_dummy_profile(Data + offset, profileSize);
        }
        if (!profiles[i]) {
            // If profile creation fails, try to create a built-in sRGB profile
            profiles[i] = cmsCreate_sRGBProfile();
        }
    }

    // Create transform
    if (nProfiles > 0 && profiles[0] != NULL) {
        hTransform = cmsCreateMultiprofileTransform(
            profiles, nProfiles,
            inputFormat, outputFormat,
            intent, flags
        );
    }

    // Test cmsGetTransformInputFormat and cmsGetTransformOutputFormat
    if (hTransform) {
        cmsUInt32Number inFmt = cmsGetTransformInputFormat(hTransform);
        cmsUInt32Number outFmt = cmsGetTransformOutputFormat(hTransform);
        (void)inFmt; // Suppress unused warning
        (void)outFmt;
    }

    // Prepare buffers for transformation
    size_t bufSize = 1024;
    uint8_t *inputBuf = malloc(bufSize);
    uint8_t *outputBuf = malloc(bufSize);
    if (!inputBuf || !outputBuf) {
        goto cleanup;
    }
    memset(inputBuf, 0, bufSize);
    memset(outputBuf, 0, bufSize);

    // Test cmsDoTransform
    if (hTransform) {
        cmsUInt32Number pixelCount = (Size % 1000) + 1;
        cmsDoTransform(hTransform, inputBuf, outputBuf, pixelCount);
    }

    // Test cmsDoTransformStride
    if (hTransform) {
        cmsUInt32Number stride = (Size % 256) + 1;
        cmsDoTransformStride(hTransform, inputBuf, outputBuf, (Size % 100) + 1, stride);
    }

    // Test cmsDoTransformLineStride
    if (hTransform) {
        cmsUInt32Number pixelsPerLine = (Size % 100) + 1;
        cmsUInt32Number lineCount = (Size % 10) + 1;
        cmsUInt32Number bytesPerLineIn = (Size % 256) + 1;
        cmsUInt32Number bytesPerLineOut = (Size % 256) + 1;
        cmsUInt32Number bytesPerPlaneIn = (Size % 256) + 1;
        cmsUInt32Number bytesPerPlaneOut = (Size % 256) + 1;
        cmsDoTransformLineStride(
            hTransform, inputBuf, outputBuf,
            pixelsPerLine, lineCount,
            bytesPerLineIn, bytesPerLineOut,
            bytesPerPlaneIn, bytesPerPlaneOut
        );
    }

cleanup:
    if (hTransform) {
        cmsDeleteTransform(hTransform);
    }
    cleanup_profiles(profiles, nProfiles);
    free(inputBuf);
    free(outputBuf);
    return 0;
}