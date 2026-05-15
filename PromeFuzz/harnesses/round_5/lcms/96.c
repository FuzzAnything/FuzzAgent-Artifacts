// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
// cmsDoTransform at cmsxform.c:192:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsGetTransformInputFormat at cmsxform.c:1424:27 in lcms2.h
// cmsDoTransform at cmsxform.c:192:16 in lcms2.h
// cmsDoTransformStride at cmsxform.c:211:16 in lcms2.h
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static cmsHPROFILE create_dummy_profile_from_data(const uint8_t *data, size_t size) {
    if (size < 128) return NULL;
    return cmsOpenProfileFromMem(data, size);
}

static cmsHPROFILE create_srgb_profile(void) {
    return cmsCreate_sRGBProfile();
}

int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Create dummy profiles
    cmsHPROFILE hProfile1 = create_dummy_profile_from_data(Data, Size);
    cmsHPROFILE hProfile2 = create_srgb_profile();
    
    if (!hProfile1 || !hProfile2) {
        if (hProfile1) cmsCloseProfile(hProfile1);
        if (hProfile2) cmsCloseProfile(hProfile2);
        return 0;
    }

    // Test cmsCreateTransform
    cmsUInt32Number inputFormat = TYPE_RGB_8;
    cmsUInt32Number outputFormat = TYPE_RGB_8;
    cmsUInt32Number intent = INTENT_PERCEPTUAL;
    cmsUInt32Number flags = 0;
    
    cmsHTRANSFORM transform = cmsCreateTransform(hProfile1, inputFormat,
                                                 hProfile2, outputFormat,
                                                 intent, flags);
    
    if (transform) {
        // Test cmsGetTransformInputFormat
        cmsUInt32Number retrievedFormat = cmsGetTransformInputFormat(transform);
        (void)retrievedFormat; // Use to avoid unused variable warning
        
        // Prepare buffers for transform operations
        size_t pixel_count = 1;
        if (Size >= 3) pixel_count = Size / 3;
        if (pixel_count > 100) pixel_count = 100;
        
        size_t buffer_size = pixel_count * 3; // 3 bytes per RGB pixel
        void *input_buffer = malloc(buffer_size);
        void *output_buffer = malloc(buffer_size);
        
        if (input_buffer && output_buffer) {
            // Fill input buffer with fuzzer data
            size_t copy_size = Size < buffer_size ? Size : buffer_size;
            memcpy(input_buffer, Data, copy_size);
            
            // Test cmsDoTransform
            cmsDoTransform(transform, input_buffer, output_buffer, pixel_count);
            
            // Test cmsDoTransformStride
            cmsUInt32Number stride = 3; // Standard RGB stride
            cmsDoTransformStride(transform, input_buffer, output_buffer, 
                                pixel_count, stride);
            
            // Test cmsDoTransformLineStride
            if (pixel_count >= 2) {
                cmsUInt32Number pixels_per_line = pixel_count / 2;
                cmsUInt32Number line_count = 2;
                cmsUInt32Number bytes_per_line_in = pixels_per_line * 3;
                cmsUInt32Number bytes_per_line_out = pixels_per_line * 3;
                cmsUInt32Number bytes_per_plane_in = 0;
                cmsUInt32Number bytes_per_plane_out = 0;
                
                cmsDoTransformLineStride(transform, input_buffer, output_buffer,
                                        pixels_per_line, line_count,
                                        bytes_per_line_in, bytes_per_line_out,
                                        bytes_per_plane_in, bytes_per_plane_out);
            }
        }
        
        free(input_buffer);
        free(output_buffer);
        
        // Test cmsCreateMultiprofileTransformTHR
        cmsHPROFILE profiles[2] = {hProfile1, hProfile2};
        cmsContext context = NULL;
        
        cmsHTRANSFORM multi_transform = cmsCreateMultiprofileTransformTHR(
            context, profiles, 2, inputFormat, outputFormat, intent, flags);
        
        if (multi_transform) {
            // Test transform with multi_transform
            if (input_buffer && output_buffer) {
                cmsDoTransform(multi_transform, input_buffer, output_buffer, 
                              pixel_count);
            }
            cmsDeleteTransform(multi_transform);
        }
        
        cmsDeleteTransform(transform);
    }
    
    // Cleanup
    cmsCloseProfile(hProfile1);
    cmsCloseProfile(hProfile2);
    
    // Test with file-based profile
    write_dummy_file(Data, Size);
    cmsHPROFILE file_profile = cmsOpenProfileFromFile("./dummy_file", "r");
    if (file_profile) {
        cmsHTRANSFORM file_transform = cmsCreateTransform(file_profile, 
                                                         TYPE_RGB_8,
                                                         hProfile2,
                                                         TYPE_RGB_8,
                                                         INTENT_PERCEPTUAL,
                                                         0);
        if (file_transform) {
            cmsDeleteTransform(file_transform);
        }
        cmsCloseProfile(file_profile);
    }
    
    return 0;
}