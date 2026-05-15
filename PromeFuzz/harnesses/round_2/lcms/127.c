// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsFormatterForColorspaceOfProfile at cmspack.c:4025:27 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
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

static cmsHPROFILE create_dummy_profile_from_data(const uint8_t* data, size_t size) {
    if (size < 128) {
        return cmsCreate_sRGBProfile();
    }
    
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return cmsCreate_sRGBProfile();
    }
    
    fwrite(data, 1, size < 4096 ? size : 4096, fp);
    fclose(fp);
    
    cmsHPROFILE profile = cmsOpenProfileFromFile("./dummy_file", "r");
    if (!profile) {
        profile = cmsCreate_sRGBProfile();
    }
    
    remove("./dummy_file");
    return profile;
}

static cmsUInt32Number get_format_from_data(const uint8_t* data, size_t offset, size_t size) {
    if (offset + 4 > size) {
        return TYPE_RGB_8;
    }
    
    cmsUInt32Number format;
    memcpy(&format, data + offset, 4);
    return format;
}

static cmsUInt32Number get_intent_from_data(const uint8_t* data, size_t offset, size_t size) {
    if (offset + 4 > size) {
        return INTENT_PERCEPTUAL;
    }
    
    cmsUInt32Number intent;
    memcpy(&intent, data + offset, 4);
    return intent % 5;
}

static cmsBool get_bool_from_data(const uint8_t* data, size_t offset, size_t size) {
    if (offset >= size) {
        return FALSE;
    }
    return data[offset] % 2;
}

static cmsFloat64Number get_double_from_data(const uint8_t* data, size_t offset, size_t size) {
    if (offset + 8 > size) {
        return 1.0;
    }
    
    cmsFloat64Number value;
    memcpy(&value, data + offset, 8);
    return value;
}

int LLVMFuzzerTestOneInput_127(const uint8_t *Data, size_t Size) {
    if (Size < 16) {
        return 0;
    }
    
    cmsHPROFILE input_profile = create_dummy_profile_from_data(Data, Size);
    cmsHPROFILE output_profile = create_dummy_profile_from_data(Data + Size/2, Size/2);
    
    size_t offset = 0;
    
    cmsUInt32Number input_format = get_format_from_data(Data, offset, Size);
    offset += 4;
    
    cmsUInt32Number output_format = get_format_from_data(Data, offset, Size);
    offset += 4;
    
    cmsUInt32Number intent = get_intent_from_data(Data, offset, Size);
    offset += 4;
    
    cmsUInt32Number flags = get_format_from_data(Data, offset, Size);
    offset += 4;
    
    cmsHTRANSFORM transform = cmsCreateTransform(input_profile, input_format,
                                                output_profile, output_format,
                                                intent, flags);
    
    if (transform) {
        cmsChangeBuffersFormat(transform, input_format, output_format);
        
        cmsUInt32Number pixels_per_line = get_format_from_data(Data, offset, Size) % 100 + 1;
        offset += 4;
        
        cmsUInt32Number line_count = get_format_from_data(Data, offset, Size) % 10 + 1;
        offset += 4;
        
        cmsUInt32Number bytes_per_line_in = get_format_from_data(Data, offset, Size) % 1000 + 1;
        offset += 4;
        
        cmsUInt32Number bytes_per_line_out = get_format_from_data(Data, offset, Size) % 1000 + 1;
        offset += 4;
        
        cmsUInt32Number bytes_per_plane_in = get_format_from_data(Data, offset, Size) % 1000 + 1;
        offset += 4;
        
        cmsUInt32Number bytes_per_plane_out = get_format_from_data(Data, offset, Size) % 1000 + 1;
        offset += 4;
        
        size_t buffer_size = pixels_per_line * line_count * 4;
        if (buffer_size > 0 && buffer_size < 1000000) {
            void* input_buffer = malloc(buffer_size);
            void* output_buffer = malloc(buffer_size);
            
            if (input_buffer && output_buffer) {
                memset(input_buffer, 0, buffer_size);
                
                cmsDoTransformLineStride(transform, input_buffer, output_buffer,
                                        pixels_per_line, line_count,
                                        bytes_per_line_in, bytes_per_line_out,
                                        bytes_per_plane_in, bytes_per_plane_out);
                
                free(input_buffer);
                free(output_buffer);
            }
        }
        
        cmsDeleteTransform(transform);
    }
    
    cmsUInt32Number n_bytes = get_format_from_data(Data, offset, Size) % 2 + 1;
    offset += 4;
    
    cmsBool is_float = get_bool_from_data(Data, offset, Size);
    offset += 1;
    
    cmsFormatterForColorspaceOfProfile(input_profile, n_bytes, is_float);
    
    cmsUInt32Number n_profiles = get_format_from_data(Data, offset, Size) % 5 + 1;
    offset += 4;
    
    if (n_profiles > 0 && n_profiles <= 5) {
        cmsHPROFILE profiles[5];
        cmsBool bpc[5];
        cmsUInt32Number intents[5];
        cmsFloat64Number adaptation_states[5];
        
        for (cmsUInt32Number i = 0; i < n_profiles; i++) {
            profiles[i] = create_dummy_profile_from_data(Data + offset, Size - offset);
            bpc[i] = get_bool_from_data(Data, offset, Size);
            intents[i] = get_intent_from_data(Data, offset, Size);
            adaptation_states[i] = get_double_from_data(Data, offset, Size);
            offset += 13;
        }
        
        cmsHPROFILE gamut_profile = create_dummy_profile_from_data(Data + offset, Size - offset);
        offset += 1;
        
        cmsUInt32Number gamut_pcs_position = get_format_from_data(Data, offset, Size);
        offset += 4;
        
        cmsContext context = cmsCreateContext(NULL, NULL);
        if (context) {
            cmsHTRANSFORM ext_transform = cmsCreateExtendedTransform(context,
                                                                    n_profiles, profiles,
                                                                    bpc, intents, adaptation_states,
                                                                    gamut_profile, gamut_pcs_position,
                                                                    input_format, output_format,
                                                                    flags);
            
            if (ext_transform) {
                cmsDeleteTransform(ext_transform);
            }
            
            cmsDeleteContext(context);
        }
        
        cmsCloseProfile(gamut_profile);
        for (cmsUInt32Number i = 0; i < n_profiles; i++) {
            cmsCloseProfile(profiles[i]);
        }
        
        cmsHTRANSFORM multi_transform = cmsCreateMultiprofileTransform(profiles,
                                                                      n_profiles,
                                                                      input_format,
                                                                      output_format,
                                                                      intent,
                                                                      flags);
        
        if (multi_transform) {
            cmsDeleteTransform(multi_transform);
        }
    }
    
    cmsCloseProfile(input_profile);
    cmsCloseProfile(output_profile);
    
    return 0;
}