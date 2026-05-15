// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsChangeBuffersFormat at cmsxform.c:1441:19 in lcms2.h
// _cmsGetTransformFlags at cmsxform.c:780:27 in lcms2_plugin.h
// _cmsGetTransformWorkerFlags at cmsxform.c:801:27 in lcms2_plugin.h
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
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

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static cmsHPROFILE create_dummy_profile(cmsContext ctx, const uint8_t* data, size_t size) {
    write_dummy_file(data, size);
    return cmsOpenProfileFromFileTHR(ctx, "./dummy_file", "r");
}

static cmsUInt32Number extract_uint32(const uint8_t** data, size_t* size) {
    if (*size < 4) return 0;
    cmsUInt32Number val = *(cmsUInt32Number*)(*data);
    *data += 4;
    *size -= 4;
    return val;
}

int LLVMFuzzerTestOneInput_119(const uint8_t* Data, size_t Size) {
    if (Size < 64) return 0;
    
    const uint8_t* ptr = Data;
    size_t remaining = Size;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    cmsUInt32Number input_format = extract_uint32(&ptr, &remaining);
    cmsUInt32Number output_format = extract_uint32(&ptr, &remaining);
    cmsUInt32Number intent = extract_uint32(&ptr, &remaining);
    cmsUInt32Number flags = extract_uint32(&ptr, &remaining);
    cmsUInt32Number pixels_per_line = extract_uint32(&ptr, &remaining);
    cmsUInt32Number line_count = extract_uint32(&ptr, &remaining);
    cmsUInt32Number bytes_per_line_in = extract_uint32(&ptr, &remaining);
    cmsUInt32Number bytes_per_line_out = extract_uint32(&ptr, &remaining);
    cmsUInt32Number bytes_per_plane_in = extract_uint32(&ptr, &remaining);
    cmsUInt32Number bytes_per_plane_out = extract_uint32(&ptr, &remaining);
    cmsUInt32Number n_profiles = extract_uint32(&ptr, &remaining);
    
    if (n_profiles > 4) n_profiles = 4;
    
    cmsHPROFILE dummy_profile = create_dummy_profile(ctx, ptr, remaining > 1024 ? 1024 : remaining);
    if (!dummy_profile) {
        dummy_profile = cmsCreate_sRGBProfileTHR(ctx);
    }
    
    cmsHPROFILE profiles[4];
    for (cmsUInt32Number i = 0; i < n_profiles; i++) {
        profiles[i] = dummy_profile;
    }
    
    cmsHTRANSFORM transform1 = cmsCreateTransformTHR(ctx, dummy_profile, input_format, 
                                                     dummy_profile, output_format, intent, flags);
    
    if (transform1) {
        cmsBool changed = cmsChangeBuffersFormat(transform1, input_format, output_format);
        (void)changed;
        
        struct _cmstransform_struct* internal_transform = (struct _cmstransform_struct*)transform1;
        cmsUInt32Number transform_flags = _cmsGetTransformFlags(internal_transform);
        (void)transform_flags;
        
        cmsUInt32Number worker_flags = _cmsGetTransformWorkerFlags(internal_transform);
        (void)worker_flags;
        
        if (pixels_per_line > 0 && line_count > 0 && remaining > 0) {
            size_t buffer_size = pixels_per_line * line_count * 4;
            if (buffer_size > 0 && buffer_size < 1024 * 1024) {
                void* input_buffer = malloc(buffer_size);
                void* output_buffer = malloc(buffer_size);
                
                if (input_buffer && output_buffer) {
                    size_t copy_size = remaining > buffer_size ? buffer_size : remaining;
                    memcpy(input_buffer, ptr, copy_size);
                    
                    cmsDoTransformLineStride(transform1, input_buffer, output_buffer,
                                            pixels_per_line, line_count,
                                            bytes_per_line_in, bytes_per_line_out,
                                            bytes_per_plane_in, bytes_per_plane_out);
                }
                
                free(input_buffer);
                free(output_buffer);
            }
        }
        
        cmsDeleteTransform(transform1);
    }
    
    cmsHTRANSFORM transform2 = cmsCreateMultiprofileTransformTHR(ctx, profiles, n_profiles,
                                                                 input_format, output_format,
                                                                 intent, flags);
    if (transform2) {
        cmsDeleteTransform(transform2);
    }
    
    if (dummy_profile) {
        cmsCloseProfile(dummy_profile);
    }
    
    cmsDeleteContext(ctx);
    
    return 0;
}