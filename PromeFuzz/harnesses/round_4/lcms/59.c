// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateProofingTransform at cmsxform.c:1393:25 in lcms2.h
// cmsGetTransformInputFormat at cmsxform.c:1424:27 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsGetTransformOutputFormat at cmsxform.c:1432:27 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsGetTransformInputFormat at cmsxform.c:1424:27 in lcms2.h
// cmsGetTransformOutputFormat at cmsxform.c:1432:27 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_profiles(cmsHPROFILE* input, cmsHPROFILE* output, cmsHPROFILE* proofing) {
    // Create dummy sRGB profiles for testing
    *input = cmsCreate_sRGBProfile();
    *output = cmsCreate_sRGBProfile();
    *proofing = cmsCreate_sRGBProfile();
}

static void cleanup_profiles(cmsHPROFILE input, cmsHPROFILE output, cmsHPROFILE proofing) {
    if (input) cmsCloseProfile(input);
    if (output) cmsCloseProfile(output);
    if (proofing) cmsCloseProfile(proofing);
}

int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    if (Size < 20) return 0;  // Need at least 5 * 4 bytes for the parameters

    cmsHPROFILE input_profile = NULL;
    cmsHPROFILE output_profile = NULL;
    cmsHPROFILE proofing_profile = NULL;
    
    // Create dummy profiles for testing
    dummy_profiles(&input_profile, &output_profile, &proofing_profile);
    if (!input_profile || !output_profile || !proofing_profile) {
        cleanup_profiles(input_profile, output_profile, proofing_profile);
        return 0;
    }

    // Extract parameters from fuzzer input with bounds checking
    cmsUInt32Number input_format = 0;
    cmsUInt32Number output_format = 0;
    cmsUInt32Number intent = 0;
    cmsUInt32Number proofing_intent = 0;
    cmsUInt32Number flags = 0;
    
    // Read parameters safely
    memcpy(&input_format, Data, sizeof(cmsUInt32Number));
    Data += sizeof(cmsUInt32Number);
    Size -= sizeof(cmsUInt32Number);
    
    if (Size >= sizeof(cmsUInt32Number)) {
        memcpy(&output_format, Data, sizeof(cmsUInt32Number));
        Data += sizeof(cmsUInt32Number);
        Size -= sizeof(cmsUInt32Number);
    }
    
    if (Size >= sizeof(cmsUInt32Number)) {
        memcpy(&intent, Data, sizeof(cmsUInt32Number));
        Data += sizeof(cmsUInt32Number);
        Size -= sizeof(cmsUInt32Number);
    }
    
    if (Size >= sizeof(cmsUInt32Number)) {
        memcpy(&proofing_intent, Data, sizeof(cmsUInt32Number));
        Data += sizeof(cmsUInt32Number);
        Size -= sizeof(cmsUInt32Number);
    }
    
    if (Size >= sizeof(cmsUInt32Number)) {
        memcpy(&flags, Data, sizeof(cmsUInt32Number));
        Data += sizeof(cmsUInt32Number);
        Size -= sizeof(cmsUInt32Number);
    }

    // Test cmsCreateTransform
    cmsHTRANSFORM transform1 = cmsCreateTransform(input_profile, input_format,
                                                   output_profile, output_format,
                                                   intent, flags);
    if (transform1) {
        // Test cmsGetTransformInputFormat
        cmsUInt32Number retrieved_input_format = cmsGetTransformInputFormat(transform1);
        (void)retrieved_input_format;
        
        // Test cmsGetTransformOutputFormat
        cmsUInt32Number retrieved_output_format = cmsGetTransformOutputFormat(transform1);
        (void)retrieved_output_format;
        
        // Test cmsDoTransformLineStride if we have enough data
        if (Size >= 16) {
            cmsUInt32Number pixels_per_line = 1;
            cmsUInt32Number line_count = 1;
            cmsUInt32Number bytes_per_line_in = 16;
            cmsUInt32Number bytes_per_line_out = 16;
            cmsUInt32Number bytes_per_plane_in = 0;
            cmsUInt32Number bytes_per_plane_out = 0;
            
            void* input_buffer = malloc(bytes_per_line_in);
            void* output_buffer = malloc(bytes_per_line_out);
            
            if (input_buffer && output_buffer) {
                memcpy(input_buffer, Data, bytes_per_line_in);
                cmsDoTransformLineStride(transform1, input_buffer, output_buffer,
                                         pixels_per_line, line_count,
                                         bytes_per_line_in, bytes_per_line_out,
                                         bytes_per_plane_in, bytes_per_plane_out);
            }
            
            free(input_buffer);
            free(output_buffer);
        }
        
        cmsDeleteTransform(transform1);
    }

    // Test cmsCreateProofingTransform
    cmsHTRANSFORM transform2 = cmsCreateProofingTransform(input_profile, input_format,
                                                          output_profile, output_format,
                                                          proofing_profile, intent,
                                                          proofing_intent, flags);
    if (transform2) {
        cmsUInt32Number fmt = cmsGetTransformInputFormat(transform2);
        (void)fmt;
        cmsDeleteTransform(transform2);
    }

    // Test cmsCreateMultiprofileTransform
    cmsHPROFILE profiles[3] = {input_profile, output_profile, proofing_profile};
    cmsUInt32Number n_profiles = 3;
    
    if (Size >= sizeof(cmsUInt32Number)) {
        cmsUInt32Number temp = 0;
        memcpy(&temp, Data, sizeof(cmsUInt32Number));
        n_profiles = temp % 3;
        if (n_profiles < 1) n_profiles = 1;
    }
    
    cmsHTRANSFORM transform3 = cmsCreateMultiprofileTransform(profiles, n_profiles,
                                                              input_format, output_format,
                                                              intent, flags);
    if (transform3) {
        cmsUInt32Number fmt = cmsGetTransformOutputFormat(transform3);
        (void)fmt;
        cmsDeleteTransform(transform3);
    }

    cleanup_profiles(input_profile, output_profile, proofing_profile);
    return 0;
}