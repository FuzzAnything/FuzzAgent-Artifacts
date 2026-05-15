// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsFormatterForColorspaceOfProfile at cmspack.c:4025:27 in lcms2.h
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsChangeBuffersFormat at cmsxform.c:1441:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsHPROFILE create_test_profile(const uint8_t* data, size_t size) {
    if (size > 0) {
        return cmsOpenProfileFromMem(data, size);
    }
    return cmsCreate_sRGBProfile();
}

static cmsHTRANSFORM create_test_transform(const uint8_t* data, size_t size) {
    cmsHPROFILE profiles[2];
    profiles[0] = cmsCreate_sRGBProfile();
    profiles[1] = create_test_profile(data, size);
    
    cmsHTRANSFORM transform = cmsCreateMultiprofileTransform(profiles, 2, 
        TYPE_RGB_8, TYPE_RGB_8, INTENT_PERCEPTUAL, 0);
    
    if (profiles[0]) cmsCloseProfile(profiles[0]);
    if (profiles[1] && profiles[1] != profiles[0]) cmsCloseProfile(profiles[1]);
    
    return transform;
}

int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Create test profiles
    cmsHPROFILE profile1 = create_test_profile(Data, Size);
    cmsHPROFILE profile2 = create_test_profile(Data + Size/2, Size/2);
    
    // Test cmsFormatterForColorspaceOfProfile
    if (profile1) {
        cmsUInt32Number formatter = cmsFormatterForColorspaceOfProfile(
            profile1, 
            (Data[0] % 2) + 1,  // 1 or 2 bytes
            Data[1] % 2         // boolean
        );
        (void)formatter;
    }
    
    // Test cmsCreateExtendedTransform
    if (profile1 && profile2) {
        cmsHPROFILE profiles[2] = {profile1, profile2};
        cmsBool bpc[2] = {Data[2] % 2, Data[3] % 2};
        cmsUInt32Number intents[2] = {Data[4] % 4, Data[5] % 4};
        cmsFloat64Number adapt[2] = {Data[6] / 255.0, Data[7] / 255.0};
        
        cmsHTRANSFORM ext_transform = cmsCreateExtendedTransform(
            NULL, 2, profiles, bpc, intents, adapt,
            NULL, 0, TYPE_RGB_8, TYPE_RGB_8, 0
        );
        if (ext_transform) cmsDeleteTransform(ext_transform);
    }
    
    // Test cmsCreateMultiprofileTransform
    if (profile1 && profile2) {
        cmsHPROFILE profiles[2] = {profile1, profile2};
        cmsHTRANSFORM mp_transform = cmsCreateMultiprofileTransform(
            profiles, 2, TYPE_RGB_8, TYPE_RGB_8, 
            Data[8] % 4, Data[9]
        );
        if (mp_transform) cmsDeleteTransform(mp_transform);
    }
    
    // Test cmsCreateMultiprofileTransformTHR
    if (profile1 && profile2) {
        cmsHPROFILE profiles[2] = {profile1, profile2};
        cmsHTRANSFORM mpthr_transform = cmsCreateMultiprofileTransformTHR(
            NULL, profiles, 2, TYPE_RGB_8, TYPE_RGB_8,
            Data[10] % 4, Data[11]
        );
        if (mpthr_transform) cmsDeleteTransform(mpthr_transform);
    }
    
    // Create a transform for testing transform-related functions
    cmsHTRANSFORM transform = create_test_transform(Data, Size);
    
    if (transform) {
        // Test cmsChangeBuffersFormat
        cmsBool changed = cmsChangeBuffersFormat(
            transform,
            (Data[12] % 2) ? TYPE_RGB_8 : TYPE_RGB_16,
            (Data[13] % 2) ? TYPE_RGB_8 : TYPE_RGB_16
        );
        (void)changed;
        
        // Test cmsDoTransformLineStride
        if (Size >= 100) {
            cmsUInt32Number pixels_per_line = (Data[14] << 8) | Data[15];
            cmsUInt32Number line_count = (Data[16] << 8) | Data[17];
            cmsUInt32Number bytes_per_line_in = (Data[18] << 8) | Data[19];
            cmsUInt32Number bytes_per_line_out = (Data[20] << 8) | Data[21];
            cmsUInt32Number bytes_per_plane_in = (Data[22] << 8) | Data[23];
            cmsUInt32Number bytes_per_plane_out = (Data[24] << 8) | Data[25];
            
            // Limit to reasonable values to avoid excessive memory allocation
            pixels_per_line = pixels_per_line % 1024 + 1;
            line_count = line_count % 100 + 1;
            bytes_per_line_in = bytes_per_line_in % 4096 + 4;
            bytes_per_line_out = bytes_per_line_out % 4096 + 4;
            bytes_per_plane_in = bytes_per_plane_in % 4096;
            bytes_per_plane_out = bytes_per_plane_out % 4096;
            
            // Allocate buffers
            void* input_buffer = malloc(bytes_per_line_in * line_count);
            void* output_buffer = malloc(bytes_per_line_out * line_count);
            
            if (input_buffer && output_buffer) {
                // Fill input buffer with fuzzer data
                size_t copy_size = bytes_per_line_in * line_count;
                if (copy_size > Size - 26) copy_size = Size - 26;
                if (copy_size > 0) {
                    memcpy(input_buffer, Data + 26, copy_size);
                }
                
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
                
                free(input_buffer);
                free(output_buffer);
            }
        }
        
        cmsDeleteTransform(transform);
    }
    
    // Cleanup
    if (profile1) cmsCloseProfile(profile1);
    if (profile2 && profile2 != profile1) cmsCloseProfile(profile2);
    
    return 0;
}