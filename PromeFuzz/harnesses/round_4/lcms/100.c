// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateBCHSWabstractProfile at cmsvirt.c:946:32 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateBCHSWabstractProfileTHR at cmsvirt.c:861:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsSetAdaptationState at cmsxform.c:77:28 in lcms2.h
// cmsSetAdaptationState at cmsxform.c:77:28 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateProofingTransform at cmsxform.c:1393:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsHPROFILE create_dummy_profile_from_data(const uint8_t *data, size_t size) {
    if (size == 0) return NULL;
    return cmsOpenProfileFromMem(data, size);
}

static cmsHPROFILE create_srgb_profile(void) {
    return cmsCreate_sRGBProfile();
}

static cmsContext create_context(void) {
    return cmsCreateContext(NULL, NULL);
}

int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size) {
    if (Size < 32) return 0;

    // Extract parameters from fuzzed data
    size_t offset = 0;
    
    // Read 32-bit integers
    cmsUInt32Number params[8];
    for (int i = 0; i < 8 && offset + sizeof(cmsUInt32Number) <= Size; i++) {
        memcpy(&params[i], Data + offset, sizeof(cmsUInt32Number));
        offset += sizeof(cmsUInt32Number);
    }
    
    // Read floating point values
    cmsFloat64Number float_params[5];
    for (int i = 0; i < 5 && offset + sizeof(cmsFloat64Number) <= Size; i++) {
        memcpy(&float_params[i], Data + offset, sizeof(cmsFloat64Number));
        offset += sizeof(cmsFloat64Number);
    }
    
    // Create profiles for testing
    cmsHPROFILE input_profile = NULL;
    cmsHPROFILE output_profile = NULL;
    cmsHPROFILE proofing_profile = NULL;
    cmsHTRANSFORM transform = NULL;
    cmsHPROFILE abstract_profile = NULL;
    cmsContext context = NULL;
    
    // Use part of fuzzed data to create dummy profiles
    size_t profile_data_size = Size / 3;
    if (profile_data_size > 0) {
        input_profile = create_dummy_profile_from_data(Data, profile_data_size);
        output_profile = create_dummy_profile_from_data(Data + profile_data_size, profile_data_size);
        proofing_profile = create_dummy_profile_from_data(Data + 2 * profile_data_size, 
                                                          Size - 2 * profile_data_size);
    }
    
    // Fallback to sRGB profiles if dummy creation failed
    if (!input_profile) input_profile = create_srgb_profile();
    if (!output_profile) output_profile = create_srgb_profile();
    if (!proofing_profile) proofing_profile = create_srgb_profile();
    
    // Create context for THR functions
    context = create_context();
    
    // Test cmsCreateProofingTransform
    transform = cmsCreateProofingTransform(
        input_profile,
        params[0] % 10,  // InputFormat
        output_profile,
        params[1] % 10,  // OutputFormat
        proofing_profile,
        params[2] % 4,   // Intent
        params[3] % 4,   // ProofingIntent
        params[4]        // dwFlags
    );
    if (transform) cmsDeleteTransform(transform);
    
    // Test cmsCreateTransformTHR
    transform = cmsCreateTransformTHR(
        context,
        input_profile,
        params[0] % 10,  // InputFormat
        output_profile,
        params[1] % 10,  // OutputFormat
        params[2] % 4,   // Intent
        params[4]        // dwFlags
    );
    if (transform) cmsDeleteTransform(transform);
    
    // Test cmsCreateTransform
    transform = cmsCreateTransform(
        input_profile,
        params[0] % 10,  // InputFormat
        output_profile,
        params[1] % 10,  // OutputFormat
        params[2] % 4,   // Intent
        params[4]        // dwFlags
    );
    if (transform) cmsDeleteTransform(transform);
    
    // Test cmsCreateBCHSWabstractProfile
    abstract_profile = cmsCreateBCHSWabstractProfile(
        params[5] % 256,     // nLUTPoints
        float_params[0],     // Bright
        float_params[1],     // Contrast
        float_params[2],     // Hue
        float_params[3],     // Saturation
        params[6],           // TempSrc
        params[7]            // TempDest
    );
    if (abstract_profile) cmsCloseProfile(abstract_profile);
    
    // Test cmsCreateBCHSWabstractProfileTHR
    if (context) {
        abstract_profile = cmsCreateBCHSWabstractProfileTHR(
            context,
            params[5] % 256,     // nLUTPoints
            float_params[0],     // Bright
            float_params[1],     // Contrast
            float_params[2],     // Hue
            float_params[3],     // Saturation
            params[6],           // TempSrc
            params[7]            // TempDest
        );
        if (abstract_profile) cmsCloseProfile(abstract_profile);
    }
    
    // Test cmsSetAdaptationState
    cmsFloat64Number prev_state = cmsSetAdaptationState(float_params[4]);
    // Restore previous state
    cmsSetAdaptationState(prev_state);
    
    // Cleanup
    if (input_profile) cmsCloseProfile(input_profile);
    if (output_profile) cmsCloseProfile(output_profile);
    if (proofing_profile) cmsCloseProfile(proofing_profile);
    if (context) cmsDeleteContext(context);
    
    return 0;
}