// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsCreateProofingTransform at cmsxform.c:1393:25 in lcms2.h
// cmsGetTransformOutputFormat at cmsxform.c:1432:27 in lcms2.h
// cmsGetTransformOutputFormat at cmsxform.c:1432:27 in lcms2.h
// cmsDoTransform at cmsxform.c:192:16 in lcms2.h
// cmsDoTransform at cmsxform.c:192:16 in lcms2.h
// cmsDoTransformStride at cmsxform.c:211:16 in lcms2.h
// cmsDoTransformStride at cmsxform.c:211:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

static cmsHPROFILE create_dummy_profile(const uint8_t *Data, size_t Size) {
    if (Size < 128) return NULL;
    // Use cmsOpenProfileFromMem to load a profile from raw data.
    // This function is available in lcms2 and handles invalid data gracefully.
    cmsHPROFILE h = cmsOpenProfileFromMem((const void*)Data, (cmsUInt32Number)Size);
    return h;
}

static void cleanup_profile(cmsHPROFILE h) {
    if (h) cmsCloseProfile(h);
}

static void cleanup_transform(cmsHTRANSFORM h) {
    if (h) cmsDeleteTransform(h);
}

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    if (Size < 256) return 0;

    // Split data for two potential profiles (Input and Output/Proofing)
    size_t half = Size / 2;
    cmsHPROFILE hInput = create_dummy_profile(Data, half);
    cmsHPROFILE hOutput = create_dummy_profile(Data + half, Size - half);
    cmsHPROFILE hProofing = NULL;
    
    // If we have enough data, try to create a third profile for proofing
    if (Size > 512) {
        hProofing = create_dummy_profile(Data + half, Size - half);
    }

    // Prepare buffers for transformation
    size_t buffer_size = 1024;
    uint8_t *input_buf = (uint8_t *)malloc(buffer_size);
    uint8_t *output_buf = (uint8_t *)malloc(buffer_size);
    
    if (!input_buf || !output_buf) {
        cleanup_profile(hInput);
        cleanup_profile(hOutput);
        cleanup_profile(hProofing);
        free(input_buf);
        free(output_buf);
        return 0;
    }
    memset(input_buf, 0, buffer_size);
    memset(output_buf, 0, buffer_size);

    // Fuzzing parameters
    // Use standard formats to increase chance of valid transform creation, 
    // but also fuzz with random values to test error handling.
    cmsUInt32Number inputFormat = (Size % 100) + 1; 
    cmsUInt32Number outputFormat = ((Size >> 8) % 100) + 1;
    cmsUInt32Number intent = Size % 4; // 0-3
    cmsUInt32Number flags = (Size >> 16) % 10;
    cmsUInt32Number proofingIntent = (Size >> 24) % 4;

    cmsHTRANSFORM hTransform = NULL;
    cmsHTRANSFORM hProofTransform = NULL;

    // 1. Test cmsCreateTransform
    if (hInput && hOutput) {
        hTransform = cmsCreateTransform(hInput, inputFormat, hOutput, outputFormat, intent, flags);
    }

    // 2. Test cmsCreateProofingTransform
    if (hInput && hOutput && hProofing) {
        hProofTransform = cmsCreateProofingTransform(hInput, inputFormat, hOutput, outputFormat, hProofing, intent, proofingIntent, flags);
    }

    // 3. Test cmsGetTransformOutputFormat
    if (hTransform) {
        cmsGetTransformOutputFormat(hTransform);
    }
    if (hProofTransform) {
        cmsGetTransformOutputFormat(hProofTransform);
    }

    // 4. Test cmsDoTransform
    cmsUInt32Number pixelCount = (Size % 100) + 1;
    if (hTransform) {
        cmsDoTransform(hTransform, input_buf, output_buf, pixelCount);
    }
    if (hProofTransform) {
        cmsDoTransform(hProofTransform, input_buf, output_buf, pixelCount);
    }

    // 5. Test cmsDoTransformStride (Deprecated)
    cmsUInt32Number stride = (Size % 100) + 1;
    if (hTransform) {
        cmsDoTransformStride(hTransform, input_buf, output_buf, pixelCount, stride);
    }
    if (hProofTransform) {
        cmsDoTransformStride(hProofTransform, input_buf, output_buf, pixelCount, stride);
    }

    // 6. Test cmsDoTransformLineStride
    cmsUInt32Number pixelsPerLine = (Size % 50) + 1;
    cmsUInt32Number lineCount = (Size % 10) + 1;
    cmsUInt32Number bytesPerLineIn = (Size % 100) + 1;
    cmsUInt32Number bytesPerLineOut = (Size % 100) + 1;
    cmsUInt32Number bytesPerPlaneIn = (Size % 100) + 1;
    cmsUInt32Number bytesPerPlaneOut = (Size % 100) + 1;

    if (hTransform) {
        cmsDoTransformLineStride(hTransform, input_buf, output_buf, pixelsPerLine, lineCount, 
                                 bytesPerLineIn, bytesPerLineOut, bytesPerPlaneIn, bytesPerPlaneOut);
    }
    if (hProofTransform) {
        cmsDoTransformLineStride(hProofTransform, input_buf, output_buf, pixelsPerLine, lineCount, 
                                 bytesPerLineIn, bytesPerLineOut, bytesPerPlaneIn, bytesPerPlaneOut);
    }

    // Cleanup
    cleanup_transform(hTransform);
    cleanup_transform(hProofTransform);
    cleanup_profile(hInput);
    cleanup_profile(hOutput);
    cleanup_profile(hProofing);
    free(input_buf);
    free(output_buf);

    return 0;
}