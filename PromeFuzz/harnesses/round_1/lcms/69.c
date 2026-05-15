// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// _cmsGetTransformWorkerFlags at cmsxform.c:801:27 in lcms2_plugin.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateLab4ProfileTHR at cmsvirt.c:524:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// _cmsGetTransformFlags at cmsxform.c:780:27 in lcms2_plugin.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsChangeBuffersFormat at cmsxform.c:1441:19 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsHPROFILE create_dummy_profile(cmsContext ctx) {
    // Create a simple sRGB profile for testing
    return cmsCreate_sRGBProfileTHR(ctx);
}

static cmsHPROFILE create_dummy_lab_profile(cmsContext ctx) {
    // Create a Lab profile for testing
    return cmsCreateLab4ProfileTHR(ctx, NULL);
}

int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Use first byte to determine test path
    uint8_t test_case = Data[0] % 6;
    Data++; Size--;
    
    // Create dummy profiles for testing
    cmsHPROFILE profiles[2];
    profiles[0] = create_dummy_profile(ctx);
    profiles[1] = create_dummy_lab_profile(ctx);
    
    if (!profiles[0] || !profiles[1]) {
        if (profiles[0]) cmsCloseProfile(profiles[0]);
        if (profiles[1]) cmsCloseProfile(profiles[1]);
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Extract parameters from fuzzer input
    cmsUInt32Number params[6] = {0};
    size_t param_count = Size / sizeof(cmsUInt32Number);
    if (param_count > 6) param_count = 6;
    
    for (size_t i = 0; i < param_count; i++) {
        memcpy(&params[i], Data + i * sizeof(cmsUInt32Number), sizeof(cmsUInt32Number));
    }
    
    cmsUInt32Number input_format = params[0] % 10;
    cmsUInt32Number output_format = params[1] % 10;
    cmsUInt32Number intent = params[2] % 4;
    cmsUInt32Number flags = params[3];
    cmsUInt32Number n_profiles = 2;
    
    switch (test_case) {
        case 0: {
            // Test _cmsGetTransformFlags
            cmsHTRANSFORM transform = cmsCreateTransformTHR(ctx, profiles[0], 
                                                           TYPE_RGB_8, 
                                                           profiles[1], 
                                                           TYPE_Lab_8, 
                                                           intent, flags);
            if (transform) {
                struct _cmstransform_struct* internal = (struct _cmstransform_struct*)transform;
                cmsUInt32Number retrieved_flags = _cmsGetTransformFlags(internal);
                (void)retrieved_flags; // Use result to avoid optimization
                cmsDeleteTransform(transform);
            }
            break;
        }
        
        case 1: {
            // Test cmsCreateTransformTHR
            cmsHTRANSFORM transform = cmsCreateTransformTHR(ctx, profiles[0], 
                                                           input_format, 
                                                           profiles[1], 
                                                           output_format, 
                                                           intent, flags);
            if (transform) {
                cmsDeleteTransform(transform);
            }
            break;
        }
        
        case 2: {
            // Test cmsChangeBuffersFormat
            cmsHTRANSFORM transform = cmsCreateTransformTHR(ctx, profiles[0], 
                                                           TYPE_RGB_8, 
                                                           profiles[1], 
                                                           TYPE_Lab_8, 
                                                           intent, flags);
            if (transform) {
                cmsBool changed = cmsChangeBuffersFormat(transform, input_format, output_format);
                (void)changed; // Use result
                cmsDeleteTransform(transform);
            }
            break;
        }
        
        case 3: {
            // Test cmsCreateMultiprofileTransformTHR
            cmsHTRANSFORM transform = cmsCreateMultiprofileTransformTHR(ctx, profiles, 
                                                                       n_profiles, 
                                                                       input_format, 
                                                                       output_format, 
                                                                       intent, flags);
            if (transform) {
                cmsDeleteTransform(transform);
            }
            break;
        }
        
        case 4: {
            // Test cmsDoTransformLineStride
            cmsHTRANSFORM transform = cmsCreateTransformTHR(ctx, profiles[0], 
                                                           TYPE_RGB_8, 
                                                           profiles[1], 
                                                           TYPE_Lab_8, 
                                                           intent, flags);
            if (transform) {
                // Allocate buffers
                cmsUInt32Number pixels_per_line = params[4] % 100 + 1;
                cmsUInt32Number line_count = params[5] % 10 + 1;
                cmsUInt32Number bytes_per_line_in = pixels_per_line * 3; // RGB 8-bit
                cmsUInt32Number bytes_per_line_out = pixels_per_line * 3; // Lab 8-bit
                cmsUInt32Number bytes_per_plane_in = bytes_per_line_in;
                cmsUInt32Number bytes_per_plane_out = bytes_per_line_out;
                
                void* input_buffer = malloc(bytes_per_line_in * line_count);
                void* output_buffer = malloc(bytes_per_line_out * line_count);
                
                if (input_buffer && output_buffer) {
                    // Fill input buffer with fuzzer data
                    size_t copy_size = Size;
                    if (copy_size > bytes_per_line_in * line_count) {
                        copy_size = bytes_per_line_in * line_count;
                    }
                    memcpy(input_buffer, Data, copy_size);
                    
                    cmsDoTransformLineStride(transform, input_buffer, output_buffer,
                                            pixels_per_line, line_count,
                                            bytes_per_line_in, bytes_per_line_out,
                                            bytes_per_plane_in, bytes_per_plane_out);
                }
                
                free(input_buffer);
                free(output_buffer);
                cmsDeleteTransform(transform);
            }
            break;
        }
        
        case 5: {
            // Test _cmsGetTransformWorkerFlags
            cmsHTRANSFORM transform = cmsCreateTransformTHR(ctx, profiles[0], 
                                                           TYPE_RGB_8, 
                                                           profiles[1], 
                                                           TYPE_Lab_8, 
                                                           intent, flags);
            if (transform) {
                struct _cmstransform_struct* internal = (struct _cmstransform_struct*)transform;
                cmsUInt32Number worker_flags = _cmsGetTransformWorkerFlags(internal);
                (void)worker_flags; // Use result
                cmsDeleteTransform(transform);
            }
            break;
        }
    }
    
    // Cleanup
    cmsCloseProfile(profiles[0]);
    cmsCloseProfile(profiles[1]);
    cmsDeleteContext(ctx);
    
    return 0;
}