// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsGetTransformFlags at cmsxform.c:780:27 in lcms2_plugin.h
// _cmsGetTransformWorkerFlags at cmsxform.c:801:27 in lcms2_plugin.h
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsChangeBuffersFormat at cmsxform.c:1441:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "lcms2.h"
#include "lcms2_plugin.h"

static cmsHPROFILE create_dummy_profile(cmsContext ctx) {
    // Create a simple sRGB profile for testing
    cmsToneCurve* curves[3];
    cmsCIExyY whitePoint = {0.3127, 0.3290, 1.0};
    cmsCIExyYTRIPLE primaries = {
        {0.6400, 0.3300, 1.0},
        {0.3000, 0.6000, 1.0},
        {0.1500, 0.0600, 1.0}
    };
    
    // Gamma 2.2 curve
    cmsFloat64Number gamma = 2.2;
    curves[0] = curves[1] = curves[2] = cmsBuildGamma(ctx, gamma);
    
    cmsHPROFILE profile = cmsCreateRGBProfileTHR(ctx, &whitePoint, &primaries, curves);
    
    cmsFreeToneCurve(curves[0]);
    return profile;
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

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;
    
    // Create context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Create dummy profiles
    cmsHPROFILE input_profile = create_dummy_profile(ctx);
    cmsHPROFILE output_profile = create_dummy_profile(ctx);
    
    if (!input_profile || !output_profile) {
        cleanup_profile(input_profile);
        cleanup_profile(output_profile);
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Extract parameters from fuzzer input
    size_t offset = 0;
    
    // For cmsCreateTransformTHR
    cmsUInt32Number input_format = *(cmsUInt32Number*)(Data + offset);
    offset += 4;
    if (offset >= Size) goto cleanup;
    
    cmsUInt32Number output_format = *(cmsUInt32Number*)(Data + offset);
    offset += 4;
    if (offset >= Size) goto cleanup;
    
    cmsUInt32Number intent = *(cmsUInt32Number*)(Data + offset) % 4; // 0-3 valid intents
    offset += 4;
    if (offset >= Size) goto cleanup;
    
    cmsUInt32Number flags = *(cmsUInt32Number*)(Data + offset);
    offset += 4;
    if (offset >= Size) goto cleanup;
    
    // Create transform
    cmsHTRANSFORM transform = cmsCreateTransformTHR(ctx, input_profile, 
                                                   input_format, 
                                                   output_profile, 
                                                   output_format, 
                                                   intent, 
                                                   flags);
    
    if (transform) {
        // Test cmsChangeBuffersFormat
        cmsUInt32Number new_input_format = *(cmsUInt32Number*)(Data + (offset % Size));
        offset = (offset + 4) % Size;
        cmsUInt32Number new_output_format = *(cmsUInt32Number*)(Data + (offset % Size));
        
        cmsChangeBuffersFormat(transform, new_input_format, new_output_format);
        
        // Test _cmsGetTransformFlags
        struct _cmstransform_struct* transform_struct = (struct _cmstransform_struct*)transform;
        cmsUInt32Number transform_flags = _cmsGetTransformFlags(transform_struct);
        (void)transform_flags; // Use result to avoid unused variable warning
        
        // Test _cmsGetTransformWorkerFlags
        cmsUInt32Number worker_flags = _cmsGetTransformWorkerFlags(transform_struct);
        (void)worker_flags;
        
        // Test cmsDoTransformLineStride with small buffer
        unsigned char input_buffer[64];
        unsigned char output_buffer[64];
        
        // Fill input buffer with fuzzer data
        size_t copy_size = Size > sizeof(input_buffer) ? sizeof(input_buffer) : Size;
        memcpy(input_buffer, Data, copy_size);
        
        cmsDoTransformLineStride(transform, 
                                input_buffer, 
                                output_buffer, 
                                1,           // PixelsPerLine
                                1,           // LineCount
                                64,          // BytesPerLineIn
                                64,          // BytesPerLineOut
                                64,          // BytesPerPlaneIn
                                64);         // BytesPerPlaneOut
        
        // Test multiprofile transform
        cmsHPROFILE profiles[2] = {input_profile, output_profile};
        cmsUInt32Number n_profiles = 2;
        
        cmsUInt32Number mp_input_format = *(cmsUInt32Number*)(Data + ((offset + 8) % Size));
        cmsUInt32Number mp_output_format = *(cmsUInt32Number*)(Data + ((offset + 12) % Size));
        cmsUInt32Number mp_intent = *(cmsUInt32Number*)(Data + ((offset + 16) % Size)) % 4;
        cmsUInt32Number mp_flags = *(cmsUInt32Number*)(Data + ((offset + 20) % Size));
        
        cmsHTRANSFORM mp_transform = cmsCreateMultiprofileTransformTHR(ctx, 
                                                                      profiles, 
                                                                      n_profiles, 
                                                                      mp_input_format, 
                                                                      mp_output_format, 
                                                                      mp_intent, 
                                                                      mp_flags);
        
        if (mp_transform) {
            // Test with the multiprofile transform
            cmsDoTransformLineStride(mp_transform, 
                                    input_buffer, 
                                    output_buffer, 
                                    1, 1, 64, 64, 64, 64);
            
            cleanup_transform(mp_transform);
        }
        
        cleanup_transform(transform);
    }
    
cleanup:
    cleanup_profile(input_profile);
    cleanup_profile(output_profile);
    cmsDeleteContext(ctx);
    
    return 0;
}