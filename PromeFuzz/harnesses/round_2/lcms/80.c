// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsGetTransformInputFormat at cmsxform.c:1424:27 in lcms2.h
// cmsGetTransformOutputFormat at cmsxform.c:1432:27 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
// cmsGetTransformInputFormat at cmsxform.c:1424:27 in lcms2.h
// cmsGetTransformOutputFormat at cmsxform.c:1432:27 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
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

static cmsHPROFILE create_dummy_profile_from_data(const uint8_t* data, size_t size) {
    if (size < 128) {
        return cmsCreate_sRGBProfile();
    }
    
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) return cmsCreate_sRGBProfile();
    
    fwrite(data, 1, size > 4096 ? 4096 : size, fp);
    fclose(fp);
    
    cmsHPROFILE profile = cmsOpenProfileFromFile("./dummy_file", "r");
    if (!profile) {
        profile = cmsCreate_sRGBProfile();
    }
    
    remove("./dummy_file");
    return profile;
}

static void generate_test_image(void* buffer, size_t size, const uint8_t* data, size_t data_size) {
    size_t to_copy = size < data_size ? size : data_size;
    memcpy(buffer, data, to_copy);
    
    if (to_copy < size) {
        memset((uint8_t*)buffer + to_copy, 0, size - to_copy);
    }
}

int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsUInt32Number flags = 0;
    cmsUInt32Number intent = Data[0] % 4;
    cmsUInt32Number input_format = TYPE_RGB_8;
    cmsUInt32Number output_format = TYPE_RGB_8;
    
    if (Size > 1) {
        flags = Data[1] & 0xFF;
    }
    
    cmsHPROFILE input_profile = create_dummy_profile_from_data(Data, Size);
    cmsHPROFILE output_profile = create_dummy_profile_from_data(Data + Size/2, Size/2);
    cmsHPROFILE proofing_profile = create_dummy_profile_from_data(Data + Size/3, Size/3);
    
    if (!input_profile || !output_profile || !proofing_profile) {
        if (input_profile) cmsCloseProfile(input_profile);
        if (output_profile) cmsCloseProfile(output_profile);
        if (proofing_profile) cmsCloseProfile(proofing_profile);
        return 0;
    }
    
    cmsHTRANSFORM transform1 = cmsCreateTransform(input_profile, input_format,
                                                  output_profile, output_format,
                                                  intent, flags);
    
    if (transform1) {
        cmsUInt32Number retrieved_input = cmsGetTransformInputFormat(transform1);
        cmsUInt32Number retrieved_output = cmsGetTransformOutputFormat(transform1);
        
        cmsDeleteTransform(transform1);
    }
    
    cmsHPROFILE multi_profiles[2] = {input_profile, output_profile};
    cmsHTRANSFORM transform2 = cmsCreateMultiprofileTransform(multi_profiles, 2,
                                                              input_format, output_format,
                                                              intent, flags);
    
    if (transform2) {
        cmsUInt32Number retrieved_input = cmsGetTransformInputFormat(transform2);
        cmsUInt32Number retrieved_output = cmsGetTransformOutputFormat(transform2);
        
        cmsDeleteTransform(transform2);
    }
    
    cmsUInt32Number proofing_intent = (Data[2] % 4);
    cmsHTRANSFORM transform3 = cmsCreateProofingTransform(input_profile, input_format,
                                                          output_profile, output_format,
                                                          proofing_profile, intent,
                                                          proofing_intent, flags);
    
    if (transform3) {
        cmsUInt32Number retrieved_input = cmsGetTransformInputFormat(transform3);
        cmsUInt32Number retrieved_output = cmsGetTransformOutputFormat(transform3);
        
        if (Size > 100) {
            size_t buffer_size = 1024;
            void* input_buffer = malloc(buffer_size);
            void* output_buffer = malloc(buffer_size);
            
            if (input_buffer && output_buffer) {
                generate_test_image(input_buffer, buffer_size, Data, Size);
                
                cmsDoTransformLineStride(transform3, input_buffer, output_buffer,
                                        16, 4, 48, 48, 0, 0);
                
                free(input_buffer);
                free(output_buffer);
            }
        }
        
        cmsDeleteTransform(transform3);
    }
    
    cmsCloseProfile(input_profile);
    cmsCloseProfile(output_profile);
    cmsCloseProfile(proofing_profile);
    
    return 0;
}