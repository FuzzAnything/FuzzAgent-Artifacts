// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
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
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsHPROFILE create_dummy_profile(cmsContext ctx) {
    // Create a simple sRGB-like profile for testing
    cmsToneCurve* gamma = cmsBuildGamma(ctx, 2.2);
    if (!gamma) return NULL;
    
    cmsCIExyY whitePoint = {0.3127, 0.3290, 1.0};
    cmsCIExyYTRIPLE primaries = {
        {0.64, 0.33, 1.0},
        {0.30, 0.60, 1.0},
        {0.15, 0.06, 1.0}
    };
    
    cmsHPROFILE profile = cmsCreateRGBProfileTHR(ctx, &whitePoint, &primaries, gamma);
    cmsFreeToneCurve(gamma);
    return profile;
}

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;
    
    // Initialize context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Create dummy profiles
    cmsHPROFILE input_profile = create_dummy_profile(ctx);
    cmsHPROFILE output_profile = create_dummy_profile(ctx);
    
    if (!input_profile || !output_profile) {
        if (input_profile) cmsCloseProfile(input_profile);
        if (output_profile) cmsCloseProfile(output_profile);
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Extract parameters from fuzzer input
    cmsUInt32Number input_format = *(cmsUInt32Number*)Data;
    cmsUInt32Number output_format = *(cmsUInt32Number*)(Data + 4);
    cmsUInt32Number intent = *(cmsUInt32Number*)(Data + 8) % 4; // 0-3 valid intents
    cmsUInt32Number flags = *(cmsUInt32Number*)(Data + 12);
    
    // Create transforms using different methods
    cmsHTRANSFORM transform1 = cmsCreateTransformTHR(ctx, input_profile, input_format, 
                                                     output_profile, output_format, intent, flags);
    
    cmsHTRANSFORM transform2 = cmsCreateTransform(input_profile, input_format, 
                                                  output_profile, output_format, intent, flags);
    
    // Test transform functions if transforms were created
    if (transform1) {
        // Test cmsGetTransformInputFormat
        cmsUInt32Number retrieved_format = cmsGetTransformInputFormat(transform1);
        (void)retrieved_format; // Use result to avoid unused variable warning
        
        // Prepare buffers for transform operations
        size_t buffer_size = Size > 256 ? 256 : Size;
        size_t pixel_count = buffer_size / 4; // Rough estimate
        
        if (pixel_count > 0) {
            const void* input_buffer = Data;
            void* output_buffer1 = malloc(buffer_size);
            void* output_buffer2 = malloc(buffer_size);
            void* output_buffer3 = malloc(buffer_size);
            
            if (output_buffer1 && output_buffer2 && output_buffer3) {
                // Test cmsDoTransform
                cmsDoTransform(transform1, input_buffer, output_buffer1, pixel_count);
                
                // Test cmsDoTransformStride
                cmsUInt32Number stride = 16; // Reasonable stride
                cmsDoTransformStride(transform1, input_buffer, output_buffer2, pixel_count, stride);
                
                // Test cmsDoTransformLineStride
                cmsUInt32Number pixels_per_line = 8;
                cmsUInt32Number line_count = pixel_count / pixels_per_line;
                if (line_count > 0) {
                    cmsUInt32Number bytes_per_line_in = pixels_per_line * 4;
                    cmsUInt32Number bytes_per_line_out = pixels_per_line * 4;
                    cmsDoTransformLineStride(transform1, input_buffer, output_buffer3,
                                            pixels_per_line, line_count,
                                            bytes_per_line_in, bytes_per_line_out,
                                            0, 0);
                }
            }
            
            free(output_buffer1);
            free(output_buffer2);
            free(output_buffer3);
        }
        
        cmsDeleteTransform(transform1);
    }
    
    if (transform2) {
        cmsDeleteTransform(transform2);
    }
    
    // Cleanup
    cmsCloseProfile(input_profile);
    cmsCloseProfile(output_profile);
    cmsDeleteContext(ctx);
    
    // Test with file-based operations
    write_dummy_file(Data, Size);
    
    return 0;
}