// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
// cmsChangeBuffersFormat at cmsxform.c:1441:19 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
// cmsDoTransform at cmsxform.c:192:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsChangeBuffersFormat at cmsxform.c:1441:19 in lcms2.h
// cmsChangeBuffersFormat at cmsxform.c:1441:19 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
// cmsFormatterForColorspaceOfProfile at cmspack.c:4025:27 in lcms2.h
// cmsFormatterForColorspaceOfProfile at cmspack.c:4025:27 in lcms2.h
// cmsFormatterForColorspaceOfProfile at cmspack.c:4025:27 in lcms2.h
// cmsFormatterForColorspaceOfProfile at cmspack.c:4025:27 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsHPROFILE create_test_profile(const uint8_t* data, size_t size) {
    // Try to create profile from fuzzer data
    cmsHPROFILE profile = cmsOpenProfileFromMem(data, size);
    if (profile != NULL) {
        return profile;
    }
    
    // Fallback to sRGB profile if fuzzer data is invalid
    return cmsCreate_sRGBProfile();
}

static void fuzz_cmsDoTransformLineStride(cmsHTRANSFORM transform, const uint8_t* data, size_t size) {
    if (transform == NULL || size < 64) return;
    
    // Use small dimensions to avoid excessive memory usage
    cmsUInt32Number pixelsPerLine = 4;
    cmsUInt32Number lineCount = 2;
    
    // Calculate buffer sizes based on transform format
    cmsUInt32Number inputFormat = 0;
    cmsUInt32Number outputFormat = 0;
    // Use TYPE_RGB_8 as default format
    inputFormat = TYPE_RGB_8;
    outputFormat = TYPE_RGB_8;
    
    cmsUInt32Number bytesPerPixelIn = T_BYTES(inputFormat) * T_CHANNELS(inputFormat);
    cmsUInt32Number bytesPerPixelOut = T_BYTES(outputFormat) * T_CHANNELS(outputFormat);
    
    cmsUInt32Number bytesPerLineIn = bytesPerPixelIn * pixelsPerLine;
    cmsUInt32Number bytesPerLineOut = bytesPerPixelOut * pixelsPerLine;
    
    size_t inputBufferSize = bytesPerLineIn * lineCount;
    size_t outputBufferSize = bytesPerLineOut * lineCount;
    
    if (inputBufferSize == 0 || outputBufferSize == 0) return;
    if (size < inputBufferSize) return;
    
    void* inputBuffer = malloc(inputBufferSize);
    void* outputBuffer = malloc(outputBufferSize);
    
    if (inputBuffer && outputBuffer) {
        memcpy(inputBuffer, data, inputBufferSize);
        
        cmsDoTransformLineStride(transform,
                                inputBuffer,
                                outputBuffer,
                                pixelsPerLine,
                                lineCount,
                                bytesPerLineIn,
                                bytesPerLineOut,
                                0,  // Non-planar input
                                0); // Non-planar output
    }
    
    free(inputBuffer);
    free(outputBuffer);
}

static void fuzz_cmsFormatterForColorspaceOfProfile(cmsHPROFILE profile) {
    if (profile == NULL) return;
    
    // Test different combinations
    cmsFormatterForColorspaceOfProfile(profile, 1, FALSE);  // 8-bit integer
    cmsFormatterForColorspaceOfProfile(profile, 2, FALSE);  // 16-bit integer
    cmsFormatterForColorspaceOfProfile(profile, 4, TRUE);   // 32-bit float
    cmsFormatterForColorspaceOfProfile(profile, 2, TRUE);   // Invalid combination
}

static void fuzz_cmsCreateExtendedTransform(const uint8_t* data, size_t size) {
    if (size < 16) return;
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context == NULL) return;
    
    // Create test profiles
    cmsHPROFILE profiles[2];
    profiles[0] = create_test_profile(data, size > 100 ? 100 : size);
    profiles[1] = cmsCreate_sRGBProfile();
    
    if (profiles[0] && profiles[1]) {
        cmsBool bpc[2] = {FALSE, FALSE};
        cmsUInt32Number intents[2] = {INTENT_PERCEPTUAL, INTENT_RELATIVE_COLORIMETRIC};
        cmsFloat64Number adaptationStates[2] = {1.0, 1.0};
        
        // Test with different formats
        cmsUInt32Number inputFormats[] = {TYPE_RGB_8, TYPE_RGB_16, TYPE_RGB_FLT};
        cmsUInt32Number outputFormats[] = {TYPE_CMYK_8, TYPE_CMYK_16, TYPE_CMYK_FLT};
        
        for (int i = 0; i < 3; i++) {
            cmsHTRANSFORM transform = cmsCreateExtendedTransform(
                context,
                2,                    // nProfiles
                profiles,            // hProfiles
                bpc,                 // BPC
                intents,             // Intents
                adaptationStates,    // AdaptationStates
                NULL,                // hGamutProfile
                0,                   // nGamutPCSposition
                inputFormats[i],     // InputFormat
                outputFormats[i],    // OutputFormat
                cmsFLAGS_NOCACHE    // dwFlags
            );
            
            if (transform) {
                cmsDeleteTransform(transform);
            }
        }
    }
    
    if (profiles[0]) cmsCloseProfile(profiles[0]);
    if (profiles[1]) cmsCloseProfile(profiles[1]);
    cmsDeleteContext(context);
}

static void fuzz_cmsCreateMultiprofileTransform(const uint8_t* data, size_t size) {
    if (size < 16) return;
    
    // Create test profiles
    cmsHPROFILE profiles[3];
    profiles[0] = create_test_profile(data, size > 50 ? 50 : size);
    
    // Use different parts of fuzzer data for additional profiles
    size_t offset = size > 100 ? 100 : size/2;
    profiles[1] = create_test_profile(data + offset, size - offset > 50 ? 50 : size - offset);
    profiles[2] = cmsCreate_sRGBProfile();
    
    if (profiles[0] && profiles[1] && profiles[2]) {
        // Test with different intents and formats
        cmsUInt32Number intents[] = {INTENT_PERCEPTUAL, INTENT_RELATIVE_COLORIMETRIC, 
                                    INTENT_ABSOLUTE_COLORIMETRIC, INTENT_SATURATION};
        
        for (int i = 0; i < 4; i++) {
            cmsHTRANSFORM transform = cmsCreateMultiprofileTransform(
                profiles,            // hProfiles
                3,                   // nProfiles
                TYPE_RGB_8,         // InputFormat
                TYPE_CMYK_8,        // OutputFormat
                intents[i],         // Intent
                cmsFLAGS_NOCACHE   // dwFlags
            );
            
            if (transform) {
                // Test changing buffer format
                cmsChangeBuffersFormat(transform, TYPE_RGB_16, TYPE_CMYK_16);
                cmsDeleteTransform(transform);
            }
        }
    }
    
    if (profiles[0]) cmsCloseProfile(profiles[0]);
    if (profiles[1]) cmsCloseProfile(profiles[1]);
    if (profiles[2]) cmsCloseProfile(profiles[2]);
}

static void fuzz_cmsCreateMultiprofileTransformTHR(const uint8_t* data, size_t size) {
    if (size < 16) return;
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context == NULL) return;
    
    // Create test profiles
    cmsHPROFILE profiles[2];
    profiles[0] = create_test_profile(data, size > 100 ? 100 : size);
    profiles[1] = cmsCreate_sRGBProfile();
    
    if (profiles[0] && profiles[1]) {
        // Test with different flags
        cmsUInt32Number flags[] = {0, cmsFLAGS_NOCACHE, cmsFLAGS_NOOPTIMIZE, 
                                  cmsFLAGS_BLACKPOINTCOMPENSATION};
        
        for (int i = 0; i < 4; i++) {
            cmsHTRANSFORM transform = cmsCreateMultiprofileTransformTHR(
                context,            // ContextID
                profiles,           // hProfiles
                2,                  // nProfiles
                TYPE_RGB_8,        // InputFormat
                TYPE_CMYK_8,       // OutputFormat
                INTENT_PERCEPTUAL, // Intent
                flags[i]           // dwFlags
            );
            
            if (transform) {
                // Use the transform
                uint8_t input[12] = {0};  // 4 RGB pixels
                uint8_t output[16];       // 4 CMYK pixels
                cmsDoTransform(transform, input, output, 4);
                cmsDeleteTransform(transform);
            }
        }
    }
    
    if (profiles[0]) cmsCloseProfile(profiles[0]);
    if (profiles[1]) cmsCloseProfile(profiles[1]);
    cmsDeleteContext(context);
}

int LLVMFuzzerTestOneInput_105(const uint8_t* data, size_t size) {
    if (size == 0) return 0;
    
    // Create a test profile from fuzzer data
    cmsHPROFILE test_profile = create_test_profile(data, size);
    if (test_profile == NULL) return 0;
    
    // Test cmsFormatterForColorspaceOfProfile
    fuzz_cmsFormatterForColorspaceOfProfile(test_profile);
    
    // Create a simple transform for testing cmsDoTransformLineStride
    cmsHTRANSFORM simple_transform = cmsCreateTransform(
        test_profile,
        TYPE_RGB_8,
        cmsCreate_sRGBProfile(),
        TYPE_RGB_8,
        INTENT_PERCEPTUAL,
        0
    );
    
    if (simple_transform) {
        // Test cmsDoTransformLineStride
        fuzz_cmsDoTransformLineStride(simple_transform, data, size);
        
        // Test cmsChangeBuffersFormat
        cmsChangeBuffersFormat(simple_transform, TYPE_RGB_16, TYPE_RGB_16);
        cmsChangeBuffersFormat(simple_transform, TYPE_RGB_8, TYPE_CMYK_8);
        
        cmsDeleteTransform(simple_transform);
    }
    
    // Test other transform creation functions
    fuzz_cmsCreateExtendedTransform(data, size);
    fuzz_cmsCreateMultiprofileTransform(data, size);
    fuzz_cmsCreateMultiprofileTransformTHR(data, size);
    
    // Cleanup
    cmsCloseProfile(test_profile);
    
    return 0;
}