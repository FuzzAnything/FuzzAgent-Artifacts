// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// _cmsGetTransformWorkerFlags at cmsxform.c:801:27 in lcms2_plugin.h
// _cmsGetTransformFlags at cmsxform.c:780:27 in lcms2_plugin.h
// cmsChangeBuffersFormat at cmsxform.c:1441:19 in lcms2.h
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
#include "lcms2_plugin.h"

static cmsHPROFILE create_dummy_profile(cmsContext context) {
    // Create a simple sRGB profile for testing
    return cmsCreate_sRGBProfileTHR(context);
}

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

int LLVMFuzzerTestOneInput_111(const uint8_t *Data, size_t Size) {
    // Initialize context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        return 0;
    }

    // Create dummy profiles for testing
    cmsHPROFILE input_profile = create_dummy_profile(context);
    cmsHPROFILE output_profile = create_dummy_profile(context);
    
    if (!input_profile || !output_profile) {
        cleanup_profile(input_profile);
        cleanup_profile(output_profile);
        cleanup_context(context);
        return 0;
    }

    // Use fuzz data to determine parameters
    // We'll use the first bytes to determine various parameters
    if (Size < 32) {
        cleanup_profile(input_profile);
        cleanup_profile(output_profile);
        cleanup_context(context);
        return 0;
    }

    // Extract parameters from fuzz data
    cmsUInt32Number input_format = *(cmsUInt32Number*)Data % 0xFFFF;
    cmsUInt32Number output_format = *(cmsUInt32Number*)(Data + 4) % 0xFFFF;
    cmsUInt32Number intent = *(cmsUInt32Number*)(Data + 8) % 4;
    cmsUInt32Number flags = *(cmsUInt32Number*)(Data + 12);
    
    // Test cmsCreateTransformTHR
    cmsHTRANSFORM transform = cmsCreateTransformTHR(context, 
                                                   input_profile, 
                                                   input_format,
                                                   output_profile, 
                                                   output_format,
                                                   intent, 
                                                   flags);
    
    if (transform) {
        // Test _cmsGetTransformWorkerFlags
        struct _cmstransform_struct* transform_struct = (struct _cmstransform_struct*)transform;
        cmsUInt32Number worker_flags = _cmsGetTransformWorkerFlags(transform_struct);
        (void)worker_flags; // Use result to avoid unused variable warning
        
        // Test _cmsGetTransformFlags
        cmsUInt32Number transform_flags = _cmsGetTransformFlags(transform_struct);
        (void)transform_flags;
        
        // Test cmsChangeBuffersFormat
        cmsUInt32Number new_input_format = *(cmsUInt32Number*)(Data + 16) % 0xFFFF;
        cmsUInt32Number new_output_format = *(cmsUInt32Number*)(Data + 20) % 0xFFFF;
        cmsBool change_result = cmsChangeBuffersFormat(transform, 
                                                      new_input_format, 
                                                      new_output_format);
        (void)change_result;
        
        // Test cmsDoTransformLineStride if we have enough data
        if (Size >= 64) {
            size_t buffer_size = 1024; // Reasonable buffer size
            void* input_buffer = malloc(buffer_size);
            void* output_buffer = malloc(buffer_size);
            
            if (input_buffer && output_buffer) {
                // Fill input buffer with fuzz data
                size_t copy_size = Size - 32;
                if (copy_size > buffer_size) copy_size = buffer_size;
                memcpy(input_buffer, Data + 32, copy_size);
                
                // Extract stride parameters
                cmsUInt32Number pixels_per_line = *(cmsUInt32Number*)(Data + 24) % 100;
                cmsUInt32Number line_count = *(cmsUInt32Number*)(Data + 28) % 10;
                cmsUInt32Number bytes_per_line_in = pixels_per_line * 4;
                cmsUInt32Number bytes_per_line_out = pixels_per_line * 4;
                cmsUInt32Number bytes_per_plane_in = 0;
                cmsUInt32Number bytes_per_plane_out = 0;
                
                // Call the function
                cmsDoTransformLineStride(transform,
                                        input_buffer,
                                        output_buffer,
                                        pixels_per_line,
                                        line_count,
                                        bytes_per_line_in,
                                        bytes_per_line_out,
                                        bytes_per_plane_in,
                                        bytes_per_plane_out);
            }
            
            free(input_buffer);
            free(output_buffer);
        }
        
        // Clean up transform
        cleanup_transform(transform);
    }
    
    // Test cmsCreateMultiprofileTransformTHR
    cmsHPROFILE profiles[2] = {input_profile, output_profile};
    cmsUInt32Number n_profiles = 2;
    
    cmsHTRANSFORM multi_transform = cmsCreateMultiprofileTransformTHR(context,
                                                                     profiles,
                                                                     n_profiles,
                                                                     input_format,
                                                                     output_format,
                                                                     intent,
                                                                     flags);
    
    if (multi_transform) {
        cleanup_transform(multi_transform);
    }
    
    // Clean up
    cleanup_profile(input_profile);
    cleanup_profile(output_profile);
    cleanup_context(context);
    
    return 0;
}