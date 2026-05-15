// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsGetTransformInputFormat at cmsxform.c:1424:27 in lcms2.h
// cmsGetTransformOutputFormat at cmsxform.c:1432:27 in lcms2.h
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateProofingTransform at cmsxform.c:1393:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static cmsHPROFILE create_dummy_profile(const uint8_t* data, size_t size) {
    const char* filename = "./dummy_file";
    FILE* fp = fopen(filename, "wb");
    if (!fp) return NULL;
    
    size_t written = fwrite(data, 1, size, fp);
    fclose(fp);
    
    if (written != size) {
        unlink(filename);
        return NULL;
    }
    
    cmsHPROFILE profile = cmsOpenProfileFromFile(filename, "r");
    unlink(filename);
    return profile;
}

static cmsUInt32Number extract_format(const uint8_t** data, size_t* size) {
    if (*size < sizeof(cmsUInt32Number)) return 0;
    cmsUInt32Number format = *(cmsUInt32Number*)(*data);
    *data += sizeof(cmsUInt32Number);
    *size -= sizeof(cmsUInt32Number);
    return format;
}

int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsUInt32Number) * 5) return 0;
    
    const uint8_t* cursor = Data;
    size_t remaining = Size;
    
    cmsUInt32Number input_format = extract_format(&cursor, &remaining);
    cmsUInt32Number output_format = extract_format(&cursor, &remaining);
    cmsUInt32Number intent = extract_format(&cursor, &remaining);
    cmsUInt32Number proofing_intent = extract_format(&cursor, &remaining);
    cmsUInt32Number flags = extract_format(&cursor, &remaining);
    
    cmsHPROFILE dummy_profile = create_dummy_profile(cursor, remaining);
    if (!dummy_profile) return 0;
    
    cmsHPROFILE profiles[3] = {dummy_profile, dummy_profile, dummy_profile};
    
    cmsHTRANSFORM transform1 = cmsCreateTransform(dummy_profile, input_format,
                                                  dummy_profile, output_format,
                                                  intent, flags);
    
    if (transform1) {
        cmsUInt32Number in_fmt = cmsGetTransformInputFormat(transform1);
        cmsUInt32Number out_fmt = cmsGetTransformOutputFormat(transform1);
        
        if (remaining >= sizeof(cmsUInt32Number) * 8) {
            cmsUInt32Number pixels_per_line = extract_format(&cursor, &remaining);
            cmsUInt32Number line_count = extract_format(&cursor, &remaining);
            cmsUInt32Number bytes_per_line_in = extract_format(&cursor, &remaining);
            cmsUInt32Number bytes_per_line_out = extract_format(&cursor, &remaining);
            cmsUInt32Number bytes_per_plane_in = extract_format(&cursor, &remaining);
            cmsUInt32Number bytes_per_plane_out = extract_format(&cursor, &remaining);
            
            size_t buffer_size = bytes_per_line_in * line_count;
            if (buffer_size <= remaining && buffer_size > 0) {
                void* input_buffer = malloc(buffer_size);
                void* output_buffer = malloc(bytes_per_line_out * line_count);
                
                if (input_buffer && output_buffer) {
                    memcpy(input_buffer, cursor, buffer_size);
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
    
    cmsHTRANSFORM transform2 = cmsCreateProofingTransform(dummy_profile, input_format,
                                                         dummy_profile, output_format,
                                                         dummy_profile, intent,
                                                         proofing_intent, flags);
    if (transform2) {
        cmsDeleteTransform(transform2);
    }
    
    cmsHTRANSFORM transform3 = cmsCreateMultiprofileTransform(profiles, 3,
                                                             input_format, output_format,
                                                             intent, flags);
    if (transform3) {
        cmsDeleteTransform(transform3);
    }
    
    cmsCloseProfile(dummy_profile);
    return 0;
}