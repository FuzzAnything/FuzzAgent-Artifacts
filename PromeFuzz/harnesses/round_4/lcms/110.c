// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
// cmsChangeBuffersFormat at cmsxform.c:1441:19 in lcms2.h
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsFormatterForColorspaceOfProfile at cmspack.c:4025:27 in lcms2.h
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROFILES 4
#define BUFFER_SIZE 1024
#define MAX_PIXELS 16

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

static cmsHPROFILE create_test_profile(const uint8_t* data, size_t size) {
    if (size < 128) {
        return cmsCreate_sRGBProfile();
    }
    return cmsOpenProfileFromMem(data, size);
}

int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) return 0;
    
    cmsHPROFILE profiles[MAX_PROFILES] = {0};
    cmsHTRANSFORM transform = NULL;
    
    // Create test profiles
    size_t profile_count = (Size % MAX_PROFILES) + 1;
    if (profile_count > MAX_PROFILES) profile_count = MAX_PROFILES;
    
    for (size_t i = 0; i < profile_count; i++) {
        size_t offset = (i * Size) / profile_count;
        size_t chunk_size = Size / profile_count;
        if (i == profile_count - 1) {
            chunk_size = Size - offset;
        }
        profiles[i] = create_test_profile(Data + offset, chunk_size);
    }
    
    // Test cmsFormatterForColorspaceOfProfile
    if (profiles[0]) {
        for (int nBytes = 1; nBytes <= 2; nBytes++) {
            for (int isFloat = 0; isFloat <= 1; isFloat++) {
                cmsFormatterForColorspaceOfProfile(profiles[0], nBytes, isFloat);
            }
        }
    }
    
    // Test cmsCreateMultiprofileTransform
    if (profile_count >= 1) {
        cmsUInt32Number intent = Data[0] % 4;
        cmsUInt32Number flags = Data[1] % 8;
        cmsUInt32Number input_format = TYPE_RGB_8;
        cmsUInt32Number output_format = TYPE_RGB_8;
        
        transform = cmsCreateMultiprofileTransform(profiles, profile_count,
                                                  input_format, output_format,
                                                  intent, flags);
    }
    
    // Test cmsCreateMultiprofileTransformTHR
    if (profile_count >= 1 && !transform) {
        cmsUInt32Number intent = Data[0] % 4;
        cmsUInt32Number flags = Data[1] % 8;
        cmsUInt32Number input_format = TYPE_RGB_8;
        cmsUInt32Number output_format = TYPE_RGB_8;
        
        transform = cmsCreateMultiprofileTransformTHR(context, profiles, profile_count,
                                                     input_format, output_format,
                                                     intent, flags);
    }
    
    // Test cmsCreateExtendedTransform
    if (profile_count >= 2 && !transform) {
        cmsBool bpc[MAX_PROFILES];
        cmsUInt32Number intents[MAX_PROFILES];
        cmsFloat64Number adaptation[MAX_PROFILES];
        
        for (size_t i = 0; i < profile_count; i++) {
            bpc[i] = (Data[i] & 1) ? TRUE : FALSE;
            intents[i] = Data[i] % 4;
            adaptation[i] = (Data[i] % 100) / 100.0;
        }
        
        cmsUInt32Number input_format = TYPE_RGB_8;
        cmsUInt32Number output_format = TYPE_RGB_8;
        cmsUInt32Number flags = Data[0] % 8;
        
        transform = cmsCreateExtendedTransform(context, profile_count, profiles,
                                              bpc, intents, adaptation,
                                              NULL, 0,
                                              input_format, output_format,
                                              flags);
    }
    
    // Test cmsChangeBuffersFormat
    if (transform) {
        cmsUInt32Number input_format = TYPE_RGB_8;
        cmsUInt32Number output_format = TYPE_RGB_16;
        
        if (Data[0] % 2) {
            input_format = TYPE_RGB_16;
            output_format = TYPE_RGB_8;
        }
        
        cmsChangeBuffersFormat(transform, input_format, output_format);
    }
    
    // Test cmsDoTransformLineStride
    if (transform) {
        uint8_t input_buffer[BUFFER_SIZE];
        uint8_t output_buffer[BUFFER_SIZE];
        
        size_t copy_size = Size < BUFFER_SIZE ? Size : BUFFER_SIZE;
        memcpy(input_buffer, Data, copy_size);
        
        cmsUInt32Number pixels_per_line = (Data[0] % MAX_PIXELS) + 1;
        cmsUInt32Number line_count = (Data[1] % MAX_PIXELS) + 1;
        cmsUInt32Number bytes_per_line_in = (Data[2] % 64) + 1;
        cmsUInt32Number bytes_per_line_out = (Data[3] % 64) + 1;
        cmsUInt32Number bytes_per_plane_in = (Data[4] % 64) + 1;
        cmsUInt32Number bytes_per_plane_out = (Data[5] % 64) + 1;
        
        cmsDoTransformLineStride(transform, input_buffer, output_buffer,
                                pixels_per_line, line_count,
                                bytes_per_line_in, bytes_per_line_out,
                                bytes_per_plane_in, bytes_per_plane_out);
    }
    
    // Cleanup
    cleanup_transform(transform);
    
    for (size_t i = 0; i < profile_count; i++) {
        cleanup_profile(profiles[i]);
    }
    
    cmsDeleteContext(context);
    
    return 0;
}