// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDoTransform at cmsxform.c:192:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
// cmsDoTransform at cmsxform.c:192:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsDoTransformStride at cmsxform.c:211:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsDoTransform at cmsxform.c:192:16 in lcms2.h
// cmsDoTransform at cmsxform.c:192:16 in lcms2.h
// cmsDoTransform at cmsxform.c:192:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsDoTransform at cmsxform.c:192:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateProofingTransform at cmsxform.c:1393:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <lcms2.h>

static void cleanup_profile(cmsHPROFILE *profile) {
    if (profile && *profile) {
        cmsCloseProfile(*profile);
        *profile = NULL;
    }
}

static cmsHPROFILE create_dummy_profile(const uint8_t *data, size_t size) {
    if (size < 128) {
        return NULL;
    }
    // Write data to a temporary file to simulate a profile
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) {
        return NULL;
    }
    fwrite(data, 1, size, f);
    fclose(f);

    cmsHPROFILE hProfile = cmsOpenProfileFromFile("./dummy_file", "r");
    if (hProfile) {
        return hProfile;
    }

    // Fallback: create a small sRGB-like profile in memory if file open fails
    // Note: cmsCreateProfile is complex, so we rely on the file method primarily.
    // If the file method fails, we return NULL to let the fuzzer explore error paths.
    return NULL;
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size < 20) {
        return 0;
    }

    cmsHPROFILE hInput = NULL;
    cmsHPROFILE hOutput = NULL;
    cmsHPROFILE hProofing = NULL;
    cmsHTRANSFORM hTransform = NULL;
    cmsHTRANSFORM hTransformMulti = NULL;
    cmsHTRANSFORM hTransformProof = NULL;

    // Parse input data to determine which API to test and generate parameters
    uint8_t mode = Data[0] % 6;
    cmsUInt32Number inputFormat = (Data[1] % 4) + 1; // 1-4 (e.g., TYPE_RGB_8, TYPE_BGR_8, etc.)
    cmsUInt32Number outputFormat = (Data[2] % 4) + 1;
    cmsUInt32Number intent = Data[3] % 4; // 0-3
    cmsUInt32Number proofIntent = Data[4] % 4;
    cmsUInt32Number flags = Data[5] % 10;

    // Allocate buffers for transform operations
    size_t bufSize = (Size > 1024) ? 1024 : Size;
    uint8_t *inBuf = (uint8_t *)malloc(bufSize);
    uint8_t *outBuf = (uint8_t *)malloc(bufSize);
    if (!inBuf || !outBuf) {
        free(inBuf);
        free(outBuf);
        return 0;
    }
    memset(inBuf, 0, bufSize);
    memset(outBuf, 0, bufSize);

    // Create profiles from input data if enough data exists
    if (Size > 256) {
        hInput = create_dummy_profile(Data + 6, Size - 6);
        if (Size > 512) {
            hOutput = create_dummy_profile(Data + 6 + (Size - 6) / 2, (Size - 6) / 2);
        }
        if (Size > 768) {
            hProofing = create_dummy_profile(Data + 6 + (Size - 6) / 2 + (Size - 6) / 4, (Size - 6) / 4);
        }
    }

    // If profiles failed to load, try to create minimal valid profiles using lcms2 internal APIs if possible,
    // but for fuzzing, NULL profiles are valid inputs to test error handling.
    // However, cmsCreateTransform usually requires valid profiles to succeed.
    // We will proceed with whatever we have (NULL or valid).

    switch (mode) {
        case 0: // cmsCreateTransform
            hTransform = cmsCreateTransform(hInput, inputFormat, hOutput, outputFormat, intent, flags);
            if (hTransform) {
                cmsDoTransform(hTransform, inBuf, outBuf, bufSize / 4);
                cmsDeleteTransform(hTransform);
            }
            break;

        case 1: // cmsCreateProofingTransform
            hTransformProof = cmsCreateProofingTransform(hInput, inputFormat, hOutput, outputFormat, hProofing, intent, proofIntent, flags);
            if (hTransformProof) {
                cmsDoTransform(hTransformProof, inBuf, outBuf, bufSize / 4);
                cmsDeleteTransform(hTransformProof);
            }
            break;

        case 2: // cmsCreateMultiprofileTransform
            if (Size > 512) {
                cmsHPROFILE profiles[2];
                profiles[0] = hInput;
                profiles[1] = hOutput;
                cmsUInt32Number nProfiles = (hInput && hOutput) ? 2 : ((hInput || hOutput) ? 1 : 0);
                if (nProfiles > 0) {
                    hTransformMulti = cmsCreateMultiprofileTransform(profiles, nProfiles, inputFormat, outputFormat, intent, flags);
                    if (hTransformMulti) {
                        cmsDoTransform(hTransformMulti, inBuf, outBuf, bufSize / 4);
                        cmsDeleteTransform(hTransformMulti);
                    }
                }
            }
            break;

        case 3: // cmsDoTransformStride (Deprecated)
            // First create a transform
            hTransform = cmsCreateTransform(hInput, inputFormat, hOutput, outputFormat, intent, flags);
            if (hTransform) {
                cmsUInt32Number stride = (Data[6] % 16) + 1;
                cmsDoTransformStride(hTransform, inBuf, outBuf, bufSize / 4, stride);
                cmsDeleteTransform(hTransform);
            }
            break;

        case 4: // cmsDoTransformLineStride
            hTransform = cmsCreateTransform(hInput, inputFormat, hOutput, outputFormat, intent, flags);
            if (hTransform) {
                cmsUInt32Number pixelsPerLine = (Data[6] % 10) + 1;
                cmsUInt32Number lineCount = (Data[7] % 10) + 1;
                cmsUInt32Number bytesPerLineIn = (Data[8] % 20) + 1;
                cmsUInt32Number bytesPerLineOut = (Data[9] % 20) + 1;
                cmsUInt32Number bytesPerPlaneIn = (Data[10] % 20) + 1;
                cmsUInt32Number bytesPerPlaneOut = (Data[11] % 20) + 1;
                
                cmsDoTransformLineStride(hTransform, inBuf, outBuf, pixelsPerLine, lineCount, 
                                         bytesPerLineIn, bytesPerLineOut, bytesPerPlaneIn, bytesPerPlaneOut);
                cmsDeleteTransform(hTransform);
            }
            break;

        case 5: // Mixed operations
            hTransform = cmsCreateTransform(hInput, inputFormat, hOutput, outputFormat, intent, flags);
            if (hTransform) {
                // Call multiple times with different parameters
                cmsDoTransform(hTransform, inBuf, outBuf, bufSize / 4);
                cmsDoTransform(hTransform, inBuf, outBuf, 1);
                cmsDoTransform(hTransform, inBuf, outBuf, 0);
                cmsDeleteTransform(hTransform);
            }
            break;
    }

    // Cleanup
    cleanup_profile(&hInput);
    cleanup_profile(&hOutput);
    cleanup_profile(&hProofing);
    free(inBuf);
    free(outBuf);

    return 0;
}