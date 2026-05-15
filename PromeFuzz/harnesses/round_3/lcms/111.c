// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsChangeBuffersFormat at cmsxform.c:1441:19 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsFormatterForColorspaceOfProfile at cmspack.c:4025:27 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsHPROFILE create_dummy_profile(const uint8_t *data, size_t size) {
    if (size < 128) {
        return cmsCreate_sRGBProfile();
    }
    
    FILE *file = fopen("./dummy_file", "wb");
    if (!file) {
        return cmsCreate_sRGBProfile();
    }
    
    fwrite(data, 1, size > 1024 ? 1024 : size, file);
    fclose(file);
    
    cmsHPROFILE profile = cmsOpenProfileFromFile("./dummy_file", "r");
    if (!profile) {
        profile = cmsCreate_sRGBProfile();
    }
    
    remove("./dummy_file");
    return profile;
}

static cmsHTRANSFORM create_dummy_transform(void) {
    cmsHPROFILE src = cmsCreate_sRGBProfile();
    cmsHPROFILE dst = cmsCreate_sRGBProfile();
    cmsHTRANSFORM transform = cmsCreateTransform(src, TYPE_RGB_8, dst, TYPE_RGB_8, INTENT_PERCEPTUAL, 0);
    cmsCloseProfile(src);
    cmsCloseProfile(dst);
    return transform;
}

int LLVMFuzzerTestOneInput_111(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Extract parameters from fuzzing input
    cmsUInt32Number param1 = *(cmsUInt32Number*)Data;
    cmsUInt32Number param2 = *(cmsUInt32Number*)(Data + 4 % Size);
    cmsUInt32Number param3 = *(cmsUInt32Number*)(Data + 8 % Size);
    cmsUInt32Number param4 = *(cmsUInt32Number*)(Data + 12 % Size);
    cmsUInt32Number param5 = *(cmsUInt32Number*)(Data + 16 % Size);
    cmsUInt32Number param6 = *(cmsUInt32Number*)(Data + 20 % Size);
    
    // Test cmsChangeBuffersFormat
    cmsHTRANSFORM transform1 = create_dummy_transform();
    if (transform1) {
        cmsChangeBuffersFormat(transform1, param1, param2);
        cmsDeleteTransform(transform1);
    }
    
    // Test cmsFormatterForColorspaceOfProfile
    cmsHPROFILE profile1 = create_dummy_profile(Data, Size);
    if (profile1) {
        cmsFormatterForColorspaceOfProfile(profile1, param3 % 3, param4 % 2);
        cmsCloseProfile(profile1);
    }
    
    // Test cmsCreateMultiprofileTransform
    cmsHPROFILE profiles[3];
    for (int i = 0; i < 3; i++) {
        profiles[i] = create_dummy_profile(Data + i * 10, Size > i * 10 ? Size - i * 10 : 0);
    }
    
    cmsUInt32Number nProfiles = param5 % 3 + 1;
    cmsHTRANSFORM transform2 = cmsCreateMultiprofileTransform(profiles, nProfiles, 
                                                              param1, param2, 
                                                              param3 % 4, param4);
    if (transform2) {
        cmsDeleteTransform(transform2);
    }
    
    for (int i = 0; i < 3; i++) {
        if (profiles[i]) cmsCloseProfile(profiles[i]);
    }
    
    // Test cmsCreateTransform
    cmsHPROFILE src_profile = create_dummy_profile(Data, Size);
    cmsHPROFILE dst_profile = create_dummy_profile(Data + 50, Size > 50 ? Size - 50 : 0);
    
    if (src_profile && dst_profile) {
        cmsHTRANSFORM transform3 = cmsCreateTransform(src_profile, param1, 
                                                     dst_profile, param2, 
                                                     param3 % 4, param4);
        if (transform3) {
            cmsDeleteTransform(transform3);
        }
    }
    
    if (src_profile) cmsCloseProfile(src_profile);
    if (dst_profile) cmsCloseProfile(dst_profile);
    
    // Test cmsCreateExtendedTransform
    cmsHPROFILE ext_profiles[2];
    cmsBool bpc[2] = {param5 % 2, param6 % 2};
    cmsUInt32Number intents[2] = {param1 % 4, param2 % 4};
    cmsFloat64Number adapt[2] = {param3 / 1000.0, param4 / 1000.0};
    
    for (int i = 0; i < 2; i++) {
        ext_profiles[i] = create_dummy_profile(Data + i * 20, Size > i * 20 ? Size - i * 20 : 0);
    }
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    cmsHPROFILE gamut_profile = create_dummy_profile(Data + 100, Size > 100 ? Size - 100 : 0);
    
    cmsHTRANSFORM transform4 = cmsCreateExtendedTransform(context, 2, ext_profiles, 
                                                         bpc, intents, adapt,
                                                         gamut_profile, param5 % 2,
                                                         param1, param2, param3);
    if (transform4) {
        cmsDeleteTransform(transform4);
    }
    
    for (int i = 0; i < 2; i++) {
        if (ext_profiles[i]) cmsCloseProfile(ext_profiles[i]);
    }
    if (gamut_profile) cmsCloseProfile(gamut_profile);
    if (context) cmsDeleteContext(context);
    
    // Test cmsDoTransformLineStride
    cmsHTRANSFORM transform5 = create_dummy_transform();
    if (transform5) {
        const size_t buffer_size = 1024;
        void *input_buffer = malloc(buffer_size);
        void *output_buffer = malloc(buffer_size);
        
        if (input_buffer && output_buffer && Size > 0) {
            size_t copy_size = Size > buffer_size ? buffer_size : Size;
            memcpy(input_buffer, Data, copy_size);
            
            cmsUInt32Number pixels = param1 % 100;
            cmsUInt32Number lines = param2 % 10;
            cmsUInt32Number bytes_in = param3 % 100;
            cmsUInt32Number bytes_out = param4 % 100;
            cmsUInt32Number plane_in = param5 % 50;
            cmsUInt32Number plane_out = param6 % 50;
            
            cmsDoTransformLineStride(transform5, input_buffer, output_buffer,
                                    pixels, lines, bytes_in, bytes_out,
                                    plane_in, plane_out);
        }
        
        free(input_buffer);
        free(output_buffer);
        cmsDeleteTransform(transform5);
    }
    
    return 0;
}