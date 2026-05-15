// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDoTransform at cmsxform.c:192:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineOutputChannels at cmslut.c:1419:27 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateProofingTransform at cmsxform.c:1393:25 in lcms2.h
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

static void dummy_cleanup(cmsHPROFILE input, cmsHPROFILE output, cmsHPROFILE proofing, cmsHTRANSFORM transform) {
    if (transform) cmsDeleteTransform(transform);
    if (input) cmsCloseProfile(input);
    if (output) cmsCloseProfile(output);
    if (proofing) cmsCloseProfile(proofing);
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Create dummy profiles from fuzzer data
    cmsHPROFILE input_profile = NULL;
    cmsHPROFILE output_profile = NULL;
    cmsHPROFILE proofing_profile = NULL;
    
    // Use first third of data for input profile
    size_t chunk = Size / 3;
    if (chunk > 0) {
        input_profile = cmsOpenProfileFromMem(Data, chunk);
    }
    
    // Use second third for output profile
    if (chunk > 0 && Size >= chunk * 2) {
        output_profile = cmsOpenProfileFromMem(Data + chunk, chunk);
    }
    
    // Use remaining for proofing profile
    if (chunk > 0 && Size >= chunk * 2) {
        size_t remaining = Size - (chunk * 2);
        if (remaining > 0) {
            proofing_profile = cmsOpenProfileFromMem(Data + chunk * 2, remaining);
        }
    }
    
    // If any profile creation failed, create default profiles
    if (!input_profile) input_profile = cmsCreate_sRGBProfile();
    if (!output_profile) output_profile = cmsCreate_sRGBProfile();
    if (!proofing_profile) proofing_profile = cmsCreate_sRGBProfile();
    
    if (!input_profile || !output_profile || !proofing_profile) {
        dummy_cleanup(input_profile, output_profile, proofing_profile, NULL);
        return 0;
    }
    
    // Extract parameters from fuzzer data
    cmsUInt32Number input_format = 0;
    cmsUInt32Number output_format = 0;
    cmsUInt32Number intent = 0;
    cmsUInt32Number proofing_intent = 0;
    cmsUInt32Number flags = 0;
    
    if (Size >= 20) {
        // Use bytes 0-3 for input_format, 4-7 for output_format, etc.
        input_format = *(cmsUInt32Number*)(Data);
        output_format = *(cmsUInt32Number*)(Data + 4);
        intent = *(cmsUInt32Number*)(Data + 8) % 4;  // Intent values 0-3
        proofing_intent = *(cmsUInt32Number*)(Data + 12) % 4;
        flags = *(cmsUInt32Number*)(Data + 16);
    } else {
        // Default values
        input_format = TYPE_RGB_8;
        output_format = TYPE_RGB_8;
        intent = INTENT_PERCEPTUAL;
        proofing_intent = INTENT_PERCEPTUAL;
        flags = 0;
    }
    
    // Create proofing transform
    cmsHTRANSFORM transform = cmsCreateProofingTransform(
        input_profile,
        input_format,
        output_profile,
        output_format,
        proofing_profile,
        intent,
        proofing_intent,
        flags
    );
    
    if (!transform) {
        dummy_cleanup(input_profile, output_profile, proofing_profile, NULL);
        return 0;
    }
    
    // Test cmsGetTransformInputFormat and cmsGetTransformOutputFormat
    cmsUInt32Number retrieved_input_format = cmsGetTransformInputFormat(transform);
    cmsUInt32Number retrieved_output_format = cmsGetTransformOutputFormat(transform);
    
    // Test cmsDoTransform with random pixel data
    if (Size > 20) {
        size_t data_offset = 20;
        size_t remaining = Size - data_offset;
        
        // Calculate buffer sizes based on format
        cmsUInt32Number input_channels = (retrieved_input_format >> 16) & 0xFF;
        cmsUInt32Number input_bytes = (retrieved_input_format >> 24) & 0xFF;
        cmsUInt32Number output_channels = (retrieved_output_format >> 16) & 0xFF;
        cmsUInt32Number output_bytes = (retrieved_output_format >> 24) & 0xFF;
        
        if (input_channels > 0 && input_bytes > 0 && output_channels > 0 && output_bytes > 0) {
            // Calculate how many pixels we can process
            size_t pixel_size_in = input_channels * input_bytes;
            size_t pixel_size_out = output_channels * output_bytes;
            
            if (pixel_size_in > 0 && pixel_size_out > 0) {
                cmsUInt32Number max_pixels = (remaining / pixel_size_in);
                if (max_pixels > 100) max_pixels = 100;  // Limit to prevent excessive allocation
                
                if (max_pixels > 0) {
                    void* input_buffer = (void*)(Data + data_offset);
                    void* output_buffer = malloc(max_pixels * pixel_size_out);
                    
                    if (output_buffer) {
                        // Test regular transform
                        cmsDoTransform(transform, input_buffer, output_buffer, max_pixels);
                        
                        // Test stride transform if we have enough data
                        if (Size >= data_offset + (max_pixels * pixel_size_in) + 24) {
                            cmsUInt32Number pixels_per_line = max_pixels > 10 ? 10 : max_pixels;
                            cmsUInt32Number line_count = max_pixels / pixels_per_line;
                            if (line_count == 0) line_count = 1;
                            
                            cmsUInt32Number bytes_per_line_in = pixels_per_line * pixel_size_in;
                            cmsUInt32Number bytes_per_line_out = pixels_per_line * pixel_size_out;
                            cmsUInt32Number bytes_per_plane_in = 0;
                            cmsUInt32Number bytes_per_plane_out = 0;
                            
                            cmsDoTransformLineStride(
                                transform,
                                input_buffer,
                                output_buffer,
                                pixels_per_line,
                                line_count,
                                bytes_per_line_in,
                                bytes_per_line_out,
                                bytes_per_plane_in,
                                bytes_per_plane_out
                            );
                        }
                        
                        free(output_buffer);
                    }
                }
            }
        }
    }
    
    // Test cmsPipelineOutputChannels by getting pipeline from transform
    // Note: This requires accessing internal structures, so we'll create a simple pipeline instead
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx) {
        cmsPipeline* pipeline = cmsPipelineAlloc(ctx, 3, 3);
        if (pipeline) {
            cmsUInt32Number output_channels = cmsPipelineOutputChannels(pipeline);
            cmsPipelineFree(pipeline);
        }
        cmsDeleteContext(ctx);
    }
    
    dummy_cleanup(input_profile, output_profile, proofing_profile, transform);
    return 0;
}