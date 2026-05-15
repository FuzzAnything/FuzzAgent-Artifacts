// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDoTransform at cmsxform.c:192:16 in lcms2.h
// cmsDoTransformStride at cmsxform.c:211:16 in lcms2.h
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
// cmsDoTransform at cmsxform.c:192:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsGetTransformInputFormat at cmsxform.c:1424:27 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lcms2.h"

#define MAX_BUFFER_SIZE 4096
#define MAX_PROFILES 8

static void cleanup_transform(cmsHTRANSFORM transform) {
    if (transform) {
        cmsDeleteTransform(transform);
    }
}

static void cleanup_profile(cmsHPROFILE profile) {
    if (profile) {
        cmsCloseProfile(profile);
    }
}

static void cleanup_context(cmsContext context) {
    if (context) {
        cmsDeleteContext(context);
    }
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Create dummy profiles for testing
    cmsHPROFILE input_profile = cmsCreate_sRGBProfile();
    cmsHPROFILE output_profile = cmsCreate_sRGBProfile();
    
    if (!input_profile || !output_profile) {
        cleanup_profile(input_profile);
        cleanup_profile(output_profile);
        return 0;
    }

    // Extract parameters from fuzzing input
    size_t offset = 0;
    
    // Use first byte for format selection
    cmsUInt32Number input_format = (Data[offset] & 0x0F) ? TYPE_RGB_8 : TYPE_RGBA_8;
    cmsUInt32Number output_format = (Data[offset] & 0xF0) ? TYPE_RGB_8 : TYPE_RGBA_8;
    offset = (offset + 1) % Size;
    
    // Use next bytes for intent and flags
    cmsUInt32Number intent = Data[offset] & 0x03;
    offset = (offset + 1) % Size;
    cmsUInt32Number flags = Data[offset] & 0xFF;
    offset = (offset + 1) % Size;
    
    // Calculate buffer sizes based on formats
    size_t bytes_per_pixel_in = (input_format == TYPE_RGB_8) ? 3 : 4;
    size_t bytes_per_pixel_out = (output_format == TYPE_RGB_8) ? 3 : 4;
    
    // Limit the number of pixels to avoid excessive memory usage
    size_t max_pixels = MAX_BUFFER_SIZE / (bytes_per_pixel_in > bytes_per_pixel_out ? 
                                           bytes_per_pixel_in : bytes_per_pixel_out);
    
    // Extract number of pixels from fuzzing input
    size_t num_pixels = Data[offset] % max_pixels;
    if (num_pixels < 1) num_pixels = 1;
    offset = (offset + 1) % Size;
    
    // Create transform using cmsCreateTransform
    cmsHTRANSFORM transform1 = cmsCreateTransform(input_profile, input_format,
                                                  output_profile, output_format,
                                                  intent, flags);
    
    if (transform1) {
        // Test cmsGetTransformInputFormat
        cmsUInt32Number retrieved_format = cmsGetTransformInputFormat(transform1);
        (void)retrieved_format; // Use result to avoid unused variable warning
        
        // Prepare input and output buffers
        size_t input_buffer_size = num_pixels * bytes_per_pixel_in;
        size_t output_buffer_size = num_pixels * bytes_per_pixel_out;
        
        uint8_t *input_buffer = (uint8_t *)malloc(input_buffer_size);
        uint8_t *output_buffer = (uint8_t *)malloc(output_buffer_size);
        
        if (input_buffer && output_buffer) {
            // Fill input buffer with fuzzing data
            size_t copy_size = (Size - offset) < input_buffer_size ? 
                               (Size - offset) : input_buffer_size;
            if (copy_size > 0) {
                memcpy(input_buffer, Data + offset, copy_size);
            }
            
            // Test cmsDoTransform
            cmsDoTransform(transform1, input_buffer, output_buffer, num_pixels);
            
            // Test cmsDoTransformStride (deprecated but still testable)
            // Use stride value from fuzzing input
            cmsUInt32Number stride = Data[offset] % 32;
            if (stride < bytes_per_pixel_in) stride = bytes_per_pixel_in;
            offset = (offset + 1) % Size;
            
            cmsDoTransformStride(transform1, input_buffer, output_buffer, 
                                 num_pixels, stride);
            
            // Test cmsDoTransformLineStride
            // Extract line parameters from fuzzing input
            cmsUInt32Number pixels_per_line = (Data[offset] % 64) + 1;
            offset = (offset + 1) % Size;
            cmsUInt32Number line_count = (Data[offset] % 8) + 1;
            offset = (offset + 1) % Size;
            cmsUInt32Number bytes_per_line_in = pixels_per_line * bytes_per_pixel_in;
            cmsUInt32Number bytes_per_line_out = pixels_per_line * bytes_per_pixel_out;
            cmsUInt32Number bytes_per_plane_in = 0;
            cmsUInt32Number bytes_per_plane_out = 0;
            
            // Ensure we have enough buffer for line stride
            if (pixels_per_line * line_count <= num_pixels) {
                cmsDoTransformLineStride(transform1, input_buffer, output_buffer,
                                         pixels_per_line, line_count,
                                         bytes_per_line_in, bytes_per_line_out,
                                         bytes_per_plane_in, bytes_per_plane_out);
            }
        }
        
        free(input_buffer);
        free(output_buffer);
        
        cleanup_transform(transform1);
    }
    
    // Test cmsCreateMultiprofileTransformTHR
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context) {
        cmsHPROFILE profiles[MAX_PROFILES];
        for (int i = 0; i < MAX_PROFILES; i++) {
            profiles[i] = cmsCreate_sRGBProfile();
        }
        
        cmsUInt32Number num_profiles = (Data[offset] % (MAX_PROFILES - 1)) + 2;
        offset = (offset + 1) % Size;
        
        cmsHTRANSFORM transform2 = cmsCreateMultiprofileTransformTHR(
            context, profiles, num_profiles, input_format, output_format, intent, flags);
        
        if (transform2) {
            // Test transform with small buffers
            size_t small_input_size = 16 * bytes_per_pixel_in;
            size_t small_output_size = 16 * bytes_per_pixel_out;
            uint8_t small_input[16 * 4]; // Max 4 bytes per pixel
            uint8_t small_output[16 * 4];
            
            if (Size - offset >= small_input_size) {
                memcpy(small_input, Data + offset, small_input_size);
                cmsDoTransform(transform2, small_input, small_output, 16);
            }
            
            cleanup_transform(transform2);
        }
        
        for (int i = 0; i < MAX_PROFILES; i++) {
            cleanup_profile(profiles[i]);
        }
        
        cleanup_context(context);
    }
    
    cleanup_profile(input_profile);
    cleanup_profile(output_profile);
    
    return 0;
}