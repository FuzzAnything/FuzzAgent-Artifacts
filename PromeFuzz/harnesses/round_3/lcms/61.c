// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
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

static cmsUInt32Number read_uint32(const uint8_t **data, size_t *size, cmsUInt32Number default_val) {
    if (*size >= 4) {
        cmsUInt32Number val = 0;
        val |= (cmsUInt32Number)(*data)[0] << 24;
        val |= (cmsUInt32Number)(*data)[1] << 16;
        val |= (cmsUInt32Number)(*data)[2] << 8;
        val |= (cmsUInt32Number)(*data)[3];
        *data += 4;
        *size -= 4;
        return val;
    }
    return default_val;
}

static void create_dummy_profile_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    fwrite(data, 1, size, f);
    fclose(f);
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    const uint8_t *ptr = Data;
    size_t remaining = Size;
    
    cmsUInt32Number input_format = read_uint32(&ptr, &remaining, TYPE_RGB_8);
    cmsUInt32Number output_format = read_uint32(&ptr, &remaining, TYPE_RGB_8);
    cmsUInt32Number intent = read_uint32(&ptr, &remaining, INTENT_PERCEPTUAL);
    cmsUInt32Number flags = read_uint32(&ptr, &remaining, 0);
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) return 0;
    
    static const char *dummy_file = "./dummy_file";
    create_dummy_profile_file(dummy_file, ptr, remaining);
    
    cmsHPROFILE hIn = cmsOpenProfileFromFile(dummy_file, "r");
    cmsHPROFILE hOut = cmsOpenProfileFromFile(dummy_file, "r");
    
    if (!hIn || !hOut) {
        if (hIn) cmsCloseProfile(hIn);
        if (hOut) cmsCloseProfile(hOut);
        cmsDeleteContext(context);
        remove(dummy_file);
        return 0;
    }
    
    cmsHPROFILE profiles[2] = {hIn, hOut};
    cmsHTRANSFORM transform = cmsCreateTransform(hIn, input_format, hOut, output_format, intent, flags);
    cmsHTRANSFORM multi_transform = cmsCreateMultiprofileTransformTHR(context, profiles, 2, input_format, output_format, intent, flags);
    
    if (transform) {
        cmsUInt32Number retrieved_format = cmsGetTransformInputFormat(transform);
        (void)retrieved_format;
        
        if (remaining > 0) {
            size_t buffer_size = remaining > 4096 ? 4096 : remaining;
            void *input_buffer = malloc(buffer_size);
            void *output_buffer = malloc(buffer_size);
            
            if (input_buffer && output_buffer) {
                memcpy(input_buffer, ptr, buffer_size);
                
                cmsUInt32Number pixel_count = buffer_size / 4;
                if (pixel_count > 0) {
                    cmsDoTransform(transform, input_buffer, output_buffer, pixel_count);
                    
                    cmsUInt32Number stride = read_uint32(&ptr, &remaining, 4);
                    cmsDoTransformStride(transform, input_buffer, output_buffer, pixel_count, stride);
                    
                    cmsUInt32Number pixels_per_line = read_uint32(&ptr, &remaining, 1);
                    cmsUInt32Number line_count = read_uint32(&ptr, &remaining, 1);
                    cmsUInt32Number bytes_per_line_in = read_uint32(&ptr, &remaining, 4);
                    cmsUInt32Number bytes_per_line_out = read_uint32(&ptr, &remaining, 4);
                    cmsUInt32Number bytes_per_plane_in = read_uint32(&ptr, &remaining, 4);
                    cmsUInt32Number bytes_per_plane_out = read_uint32(&ptr, &remaining, 4);
                    
                    cmsDoTransformLineStride(transform, input_buffer, output_buffer,
                                           pixels_per_line, line_count,
                                           bytes_per_line_in, bytes_per_line_out,
                                           bytes_per_plane_in, bytes_per_plane_out);
                }
            }
            
            free(input_buffer);
            free(output_buffer);
        }
        
        cmsDeleteTransform(transform);
    }
    
    if (multi_transform) {
        cmsDeleteTransform(multi_transform);
    }
    
    cmsCloseProfile(hIn);
    cmsCloseProfile(hOut);
    cmsDeleteContext(context);
    remove(dummy_file);
    
    return 0;
}