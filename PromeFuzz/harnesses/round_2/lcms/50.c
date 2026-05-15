// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
// cmsDoTransformStride at cmsxform.c:211:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsGetTransformInputFormat at cmsxform.c:1424:27 in lcms2.h
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
// cmsDoTransform at cmsxform.c:192:16 in lcms2.h
// cmsDoTransform at cmsxform.c:192:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsHPROFILE create_test_profile_from_data(const uint8_t* data, size_t size) {
    if (size < 128) {
        return cmsCreate_sRGBProfile();
    }
    
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) return cmsCreate_sRGBProfile();
    
    fwrite(data, 1, size, fp);
    fclose(fp);
    
    cmsHPROFILE profile = cmsOpenProfileFromFile("./dummy_file", "r");
    if (!profile) {
        profile = cmsCreate_sRGBProfile();
    }
    
    remove("./dummy_file");
    return profile;
}

static cmsContext create_test_context(void) {
    return cmsCreateContext(NULL, NULL);
}

int LLVMFuzzerTestOneInput_50(const uint8_t* Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsContext context = create_test_context();
    if (!context) return 0;
    
    cmsHPROFILE input_profile = create_test_profile_from_data(Data, Size);
    cmsHPROFILE output_profile = create_test_profile_from_data(Data + Size/2, Size/2);
    
    if (!input_profile || !output_profile) {
        if (input_profile) cmsCloseProfile(input_profile);
        if (output_profile) cmsCloseProfile(output_profile);
        cmsDeleteContext(context);
        return 0;
    }
    
    uint32_t flags = 0;
    if (Size > 8) {
        flags = *((uint32_t*)Data);
    }
    
    uint32_t intent = (Size > 12) ? (*((uint32_t*)(Data + 4)) % 4) : 0;
    
    cmsUInt32Number input_format = TYPE_RGB_8;
    cmsUInt32Number output_format = TYPE_RGB_8;
    
    if (Size > 16) {
        input_format = *((uint32_t*)(Data + 8));
        output_format = *((uint32_t*)(Data + 12));
    }
    
    cmsHTRANSFORM transform = cmsCreateTransform(input_profile, input_format,
                                                 output_profile, output_format,
                                                 intent, flags);
    
    if (transform) {
        cmsUInt32Number retrieved_format = cmsGetTransformInputFormat(transform);
        (void)retrieved_format;
        
        cmsHPROFILE profiles[] = {input_profile, output_profile};
        cmsHTRANSFORM multi_transform = cmsCreateMultiprofileTransformTHR(
            context, profiles, 2, input_format, output_format, intent, flags);
        
        if (multi_transform) {
            uint8_t input_buffer[256];
            uint8_t output_buffer[256];
            
            size_t buffer_size = (Size < 256) ? Size : 256;
            memcpy(input_buffer, Data, buffer_size);
            
            cmsDoTransform(transform, input_buffer, output_buffer, 1);
            cmsDoTransform(multi_transform, input_buffer, output_buffer, 1);
            
            if (Size > 32) {
                uint32_t pixels_per_line = (*((uint32_t*)(Data + 16)) % 16) + 1;
                uint32_t line_count = (*((uint32_t*)(Data + 20)) % 16) + 1;
                uint32_t bytes_per_line_in = (*((uint32_t*)(Data + 24)) % 64) + 1;
                uint32_t bytes_per_line_out = (*((uint32_t*)(Data + 28)) % 64) + 1;
                uint32_t bytes_per_plane_in = 0;
                uint32_t bytes_per_plane_out = 0;
                
                if (Size > 40) {
                    bytes_per_plane_in = *((uint32_t*)(Data + 32));
                    bytes_per_plane_out = *((uint32_t*)(Data + 36));
                }
                
                cmsDoTransformLineStride(transform, input_buffer, output_buffer,
                                         pixels_per_line, line_count,
                                         bytes_per_line_in, bytes_per_line_out,
                                         bytes_per_plane_in, bytes_per_plane_out);
                
                uint32_t stride = (Size > 44) ? (*((uint32_t*)(Data + 40)) % 64) + 1 : 4;
                cmsDoTransformStride(transform, input_buffer, output_buffer, 1, stride);
            }
            
            cmsDeleteTransform(multi_transform);
        }
        
        cmsDeleteTransform(transform);
    }
    
    cmsCloseProfile(input_profile);
    cmsCloseProfile(output_profile);
    
    cmsDeleteContext(context);
    
    return 0;
}