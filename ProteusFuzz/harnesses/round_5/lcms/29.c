// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsGetTransformOutputFormat at cmsxform.c:1432:27 in lcms2.h
// cmsDoTransform at cmsxform.c:192:16 in lcms2.h
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
// _cmsGetTransformWorkerFlags at cmsxform.c:801:27 in lcms2_plugin.h
// _cmsGetTransformFlags at cmsxform.c:780:27 in lcms2_plugin.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
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
#include <lcms2_plugin.h>

// Helper to create a minimal valid ICC profile from fuzzing data
static cmsHPROFILE create_dummy_profile(const uint8_t *data, size_t size) {
    if (size < 128) return NULL; // Too small to be a valid ICC profile
    
    // Ensure the signature is valid (ICC profile header starts with 'acsp')
    // We will write a minimal valid header to the data buffer if it's not already valid
    // However, for fuzzing, we often just pass the raw data and let the parser handle it.
    // But cmsOpenProfileFromMem requires a valid structure. 
    // To ensure we get a valid handle, we might need to generate a real profile or use a dummy file.
    // Since we cannot easily generate a full valid ICC profile from random bytes without complex logic,
    // we will try to open it directly. If it fails, we return NULL and the fuzzer moves on.
    
    return cmsOpenProfileFromMem((const uint8_t*)data, size);
}

// Helper to create a dummy file if needed (though we mostly use memory here)
static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    // Initialize LCMS context
    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (!ContextID) return 0;

    // We need at least 3 profiles for cmsCreateProofingTransform (Input, Output, Proofing)
    // We will split the input data into chunks to try and create multiple profiles.
    // If the data is too small to create valid profiles, the functions will return NULL,
    // which is a valid state to test for NULL handling.

    cmsHPROFILE hInput = NULL;
    cmsHPROFILE hOutput = NULL;
    cmsHPROFILE hProofing = NULL;
    cmsHTRANSFORM hTransform = NULL;
    
    // Attempt to create profiles from different parts of the input data
    // We use the whole data for the first, a slice for the second, etc.
    // This increases the chance of hitting different code paths in the parser.
    
    // Try to create Input profile
    if (Size >= 128) {
        hInput = create_dummy_profile(Data, Size);
    }
    
    // Try to create Output profile (shifted offset)
    if (Size >= 256) {
        hOutput = create_dummy_profile(Data + 128, Size - 128);
    } else if (Size >= 128) {
        hOutput = create_dummy_profile(Data, Size); // Fallback to same data
    }

    // Try to create Proofing profile
    if (Size >= 384) {
        hProofing = create_dummy_profile(Data + 256, Size - 256);
    } else if (Size >= 128) {
        hProofing = create_dummy_profile(Data, Size);
    }

    // If we have valid profiles, try to create a transform
    if (hInput && hOutput && hProofing) {
        // Use some data bytes as parameters for the transform creation
        cmsUInt32Number InputFormat = (Size > 0) ? Data[0] : 0;
        cmsUInt32Number OutputFormat = (Size > 1) ? Data[1] : 0;
        cmsUInt32Number Intent = (Size > 2) ? (Data[2] % 4) : 0; // Valid intents 0-3
        cmsUInt32Number ProofingIntent = (Size > 3) ? (Data[3] % 4) : 0;
        cmsUInt32Number dwFlags = (Size > 4) ? Data[4] : 0;

        // Fuzz cmsCreateProofingTransform
        hTransform = cmsCreateProofingTransform(
            hInput, InputFormat,
            hOutput, OutputFormat,
            hProofing, Intent,
            ProofingIntent, dwFlags
        );
    }

    // If transform creation failed, try to create a simple transform for testing other APIs
    // if we have at least two profiles
    if (!hTransform && hInput && hOutput) {
        cmsUInt32Number InputFormat = (Size > 0) ? Data[0] : 0;
        cmsUInt32Number OutputFormat = (Size > 1) ? Data[1] : 0;
        cmsUInt32Number Intent = (Size > 2) ? (Data[2] % 4) : 0;
        
        hTransform = cmsCreateTransform(hInput, InputFormat, hOutput, OutputFormat, Intent, 0);
    }

    // Prepare buffers for transformation
    // Allocate a small buffer for input and output
    size_t bufSize = 1024;
    uint8_t *inBuf = malloc(bufSize);
    uint8_t *outBuf = malloc(bufSize);
    
    if (inBuf && outBuf) {
        // Fill buffers with fuzzing data
        size_t fillSize = (Size < bufSize) ? Size : bufSize;
        memcpy(inBuf, Data, fillSize);
        memset(outBuf, 0, bufSize);

        if (hTransform) {
            // 1. Test cmsGetTransformOutputFormat
            cmsUInt32Number outFmt = cmsGetTransformOutputFormat(hTransform);
            (void)outFmt; // Use the value to avoid warnings

            // 2. Test cmsDoTransform
            cmsUInt32Number pixelCount = (Size > 5) ? (Data[5] % 100) : 10;
            cmsDoTransform(hTransform, inBuf, outBuf, pixelCount);

            // 3. Test cmsDoTransformLineStride
            cmsUInt32Number PixelsPerLine = (Size > 6) ? (Data[6] % 50) : 10;
            cmsUInt32Number LineCount = (Size > 7) ? (Data[7] % 20) : 5;
            cmsUInt32Number BytesPerLineIn = (Size > 8) ? (Data[8] % 100) : 40;
            cmsUInt32Number BytesPerLineOut = (Size > 9) ? (Data[9] % 100) : 40;
            cmsUInt32Number BytesPerPlaneIn = (Size > 10) ? (Data[10] % 50) : 20;
            cmsUInt32Number BytesPerPlaneOut = (Size > 11) ? (Data[11] % 50) : 20;
            
            cmsDoTransformLineStride(hTransform, inBuf, outBuf, 
                PixelsPerLine, LineCount, 
                BytesPerLineIn, BytesPerLineOut, 
                BytesPerPlaneIn, BytesPerPlaneOut);

            // 4. Test internal flags APIs
            // These require the internal struct pointer. 
            // cmsHTRANSFORM is a void*, but internally it points to _cmstransform_struct.
            // We cast it directly. Note: This relies on the internal structure layout.
            // If the cast is invalid or the pointer is NULL, these assert.
            // The fuzzer should handle the assertion as a crash (which is good for finding bugs).
            struct _cmstransform_struct *internalTransform = (struct _cmstransform_struct *)hTransform;
            
            if (internalTransform) {
                cmsUInt32Number workerFlags = _cmsGetTransformWorkerFlags(internalTransform);
                cmsUInt32Number originalFlags = _cmsGetTransformFlags(internalTransform);
                (void)workerFlags;
                (void)originalFlags;
            }
        }
    }

    // Cleanup
    if (inBuf) free(inBuf);
    if (outBuf) free(outBuf);
    
    if (hTransform) cmsDeleteTransform(hTransform);
    if (hInput) cmsCloseProfile(hInput);
    if (hOutput) cmsCloseProfile(hOutput);
    if (hProofing) cmsCloseProfile(hProofing);
    
    cmsDeleteContext(ContextID);

    return 0;
}