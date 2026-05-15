// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsGetTransformFlags at cmsxform.c:780:27 in lcms2_plugin.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsChangeBuffersFormat at cmsxform.c:1441:19 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
// _cmsGetTransformWorkerFlags at cmsxform.c:801:27 in lcms2_plugin.h
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

static cmsHTRANSFORM create_test_transform(cmsContext ctx) {
    cmsHPROFILE src = create_dummy_profile(ctx);
    cmsHPROFILE dst = create_dummy_profile(ctx);
    if (!src || !dst) {
        if (src) cmsCloseProfile(src);
        if (dst) cmsCloseProfile(dst);
        return NULL;
    }
    
    cmsHTRANSFORM transform = cmsCreateTransformTHR(ctx, src, TYPE_RGB_8, dst, TYPE_RGB_8, INTENT_PERCEPTUAL, 0);
    
    cmsCloseProfile(src);
    cmsCloseProfile(dst);
    return transform;
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Initialize context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Test 1: cmsChangeBuffersFormat
    cmsHTRANSFORM transform1 = create_test_transform(ctx);
    if (transform1) {
        // Use fuzz data to determine formats
        uint32_t input_format = *(uint32_t*)Data;
        uint32_t output_format = *(uint32_t*)(Data + 4 % Size);
        cmsChangeBuffersFormat(transform1, input_format, output_format);
        cmsDeleteTransform(transform1);
    }
    
    // Test 2: cmsCreateMultiprofileTransformTHR
    cmsHPROFILE profiles[3];
    for (int i = 0; i < 3; i++) {
        profiles[i] = create_dummy_profile(ctx);
    }
    
    if (profiles[0] && profiles[1] && profiles[2]) {
        uint32_t intent = *(uint32_t*)Data % 4;
        uint32_t flags = *(uint32_t*)(Data + 8 % Size);
        cmsHTRANSFORM transform2 = cmsCreateMultiprofileTransformTHR(
            ctx, profiles, 3, TYPE_RGB_8, TYPE_RGB_8, intent, flags);
        if (transform2) {
            cmsDeleteTransform(transform2);
        }
    }
    
    for (int i = 0; i < 3; i++) {
        if (profiles[i]) cmsCloseProfile(profiles[i]);
    }
    
    // Test 3: cmsCreateTransformTHR
    cmsHPROFILE src = create_dummy_profile(ctx);
    cmsHPROFILE dst = create_dummy_profile(ctx);
    if (src && dst) {
        uint32_t intent = *(uint32_t*)(Data + 12 % Size) % 4;
        uint32_t flags = *(uint32_t*)(Data + 16 % Size);
        cmsHTRANSFORM transform3 = cmsCreateTransformTHR(
            ctx, src, TYPE_RGB_8, dst, TYPE_RGB_8, intent, flags);
        if (transform3) {
            // Test 4: cmsDoTransformLineStride
            static uint8_t input_buf[64 * 64 * 3];
            static uint8_t output_buf[64 * 64 * 3];
            
            // Use fuzz data to determine stride parameters
            uint32_t pixels_per_line = 64;
            uint32_t line_count = 64;
            uint32_t bytes_per_line_in = *(uint32_t*)(Data + 20 % Size) % 256;
            uint32_t bytes_per_line_out = *(uint32_t*)(Data + 24 % Size) % 256;
            uint32_t bytes_per_plane_in = *(uint32_t*)(Data + 28 % Size) % 256;
            uint32_t bytes_per_plane_out = *(uint32_t*)(Data + 32 % Size) % 256;
            
            cmsDoTransformLineStride(transform3, input_buf, output_buf,
                                    pixels_per_line, line_count,
                                    bytes_per_line_in, bytes_per_line_out,
                                    bytes_per_plane_in, bytes_per_plane_out);
            
            // Test 5 & 6: Internal functions (cast transform to internal struct)
            struct _cmstransform_struct* internal_transform = (struct _cmstransform_struct*)transform3;
            if (internal_transform) {
                _cmsGetTransformWorkerFlags(internal_transform);
                _cmsGetTransformFlags(internal_transform);
            }
            
            cmsDeleteTransform(transform3);
        }
    }
    
    if (src) cmsCloseProfile(src);
    if (dst) cmsCloseProfile(dst);
    
    cmsDeleteContext(ctx);
    return 0;
}