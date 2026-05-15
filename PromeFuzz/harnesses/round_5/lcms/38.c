// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDoTransform at cmsxform.c:192:16 in lcms2.h
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateProofingTransform at cmsxform.c:1393:25 in lcms2.h
// cmsDoTransform at cmsxform.c:192:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsSetLogErrorHandler at cmserr.c:503:16 in lcms2.h
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

static void dummy_error_handler(cmsContext context, cmsUInt32Number error_code, const char* error_text) {
    // Silently ignore errors during fuzzing
    (void)context;
    (void)error_code;
    (void)error_text;
}

static cmsHPROFILE create_dummy_profile(const uint8_t* data, size_t size) {
    if (size < 128) {
        // Create a simple sRGB profile if we don't have enough data
        return cmsCreate_sRGBProfile();
    }
    
    // Try to create profile from memory
    cmsHPROFILE profile = cmsOpenProfileFromMem(data, (cmsUInt32Number)size);
    if (profile == NULL) {
        // Fall back to sRGB profile
        profile = cmsCreate_sRGBProfile();
    }
    return profile;
}

int LLVMFuzzerTestOneInput_38(const uint8_t* Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }
    
    // Set error handler to ignore errors
    cmsSetLogErrorHandler(dummy_error_handler);
    
    // Create dummy profiles for testing
    cmsHPROFILE input_profile = create_dummy_profile(Data, Size);
    cmsHPROFILE output_profile = create_dummy_profile(Data + Size/2, Size/2);
    cmsHPROFILE proofing_profile = create_dummy_profile(Data + Size/3, Size/3);
    
    if (input_profile == NULL || output_profile == NULL || proofing_profile == NULL) {
        if (input_profile) cmsCloseProfile(input_profile);
        if (output_profile) cmsCloseProfile(output_profile);
        if (proofing_profile) cmsCloseProfile(proofing_profile);
        return 0;
    }
    
    // Extract parameters from fuzzer input
    cmsUInt32Number input_format = 0;
    cmsUInt32Number output_format = 0;
    cmsUInt32Number intent = 0;
    cmsUInt32Number flags = 0;
    cmsUInt32Number proofing_intent = 0;
    
    if (Size >= 20) {
        // Use first 20 bytes for parameters
        memcpy(&input_format, Data, sizeof(input_format));
        memcpy(&output_format, Data + 4, sizeof(output_format));
        memcpy(&intent, Data + 8, sizeof(intent));
        memcpy(&flags, Data + 12, sizeof(flags));
        memcpy(&proofing_intent, Data + 16, sizeof(proofing_intent));
        
        // Limit values to reasonable ranges
        input_format = input_format % 100;
        output_format = output_format % 100;
        intent = intent % 4;  // 0-3 for standard intents
        proofing_intent = proofing_intent % 4;
    } else {
        // Default values
        input_format = TYPE_RGB_8;
        output_format = TYPE_RGB_8;
        intent = INTENT_PERCEPTUAL;
        flags = 0;
        proofing_intent = INTENT_RELATIVE_COLORIMETRIC;
    }
    
    // Test cmsCreateTransform
    cmsHTRANSFORM transform = cmsCreateTransform(input_profile, input_format,
                                                 output_profile, output_format,
                                                 intent, flags);
    
    if (transform != NULL) {
        // Test cmsGetTransformInputFormat and cmsGetTransformOutputFormat
        cmsUInt32Number retrieved_input_format = cmsGetTransformInputFormat(transform);
        cmsUInt32Number retrieved_output_format = cmsGetTransformOutputFormat(transform);
        
        (void)retrieved_input_format;
        (void)retrieved_output_format;
        
        // Test cmsDoTransform with some pixel data
        if (Size >= 30) {
            size_t pixel_data_size = Size - 20;
            if (pixel_data_size > 100) pixel_data_size = 100;
            
            void* input_buffer = malloc(pixel_data_size);
            void* output_buffer = malloc(pixel_data_size);
            
            if (input_buffer && output_buffer) {
                // Copy remaining data as pixel data
                memcpy(input_buffer, Data + 20, pixel_data_size);
                
                // Calculate number of pixels (assuming 3 bytes per pixel for RGB8)
                cmsUInt32Number num_pixels = (cmsUInt32Number)(pixel_data_size / 3);
                if (num_pixels > 0) {
                    cmsDoTransform(transform, input_buffer, output_buffer, num_pixels);
                    
                    // Test cmsDoTransformLineStride
                    if (num_pixels >= 4) {
                        cmsUInt32Number pixels_per_line = 2;
                        cmsUInt32Number line_count = 2;
                        cmsUInt32Number bytes_per_line_in = 6;  // 2 pixels * 3 bytes
                        cmsUInt32Number bytes_per_line_out = 6;
                        cmsUInt32Number bytes_per_plane_in = 0;
                        cmsUInt32Number bytes_per_plane_out = 0;
                        
                        cmsDoTransformLineStride(transform, input_buffer, output_buffer,
                                                pixels_per_line, line_count,
                                                bytes_per_line_in, bytes_per_line_out,
                                                bytes_per_plane_in, bytes_per_plane_out);
                    }
                }
            }
            
            free(input_buffer);
            free(output_buffer);
        }
        
        cmsDeleteTransform(transform);
    }
    
    // Test cmsCreateProofingTransform
    cmsHTRANSFORM proofing_transform = cmsCreateProofingTransform(input_profile, input_format,
                                                                  output_profile, output_format,
                                                                  proofing_profile,
                                                                  intent, proofing_intent,
                                                                  flags);
    
    if (proofing_transform != NULL) {
        // Test with some pixel data
        if (Size >= 40) {
            size_t pixel_data_size = Size - 30;
            if (pixel_data_size > 50) pixel_data_size = 50;
            
            void* input_buffer = malloc(pixel_data_size);
            void* output_buffer = malloc(pixel_data_size);
            
            if (input_buffer && output_buffer) {
                memcpy(input_buffer, Data + 30, pixel_data_size);
                
                cmsUInt32Number num_pixels = (cmsUInt32Number)(pixel_data_size / 3);
                if (num_pixels > 0) {
                    cmsDoTransform(proofing_transform, input_buffer, output_buffer, num_pixels);
                }
            }
            
            free(input_buffer);
            free(output_buffer);
        }
        
        cmsDeleteTransform(proofing_transform);
    }
    
    // Clean up
    cmsCloseProfile(input_profile);
    cmsCloseProfile(output_profile);
    cmsCloseProfile(proofing_profile);
    
    return 0;
}