// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsChangeBuffersFormat at cmsxform.c:1441:19 in lcms2.h
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
// cmsFormatterForColorspaceOfProfile at cmspack.c:4025:27 in lcms2.h
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
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
    if (size == 0) {
        return cmsCreate_sRGBProfile();
    }
    
    FILE* f = fopen("./dummy_file", "wb");
    if (!f) return cmsCreate_sRGBProfile();
    
    size_t written = fwrite(data, 1, size, f);
    fclose(f);
    
    if (written != size) {
        return cmsCreate_sRGBProfile();
    }
    
    cmsHPROFILE profile = cmsOpenProfileFromFile("./dummy_file", "r");
    if (!profile) {
        profile = cmsCreate_sRGBProfile();
    }
    
    remove("./dummy_file");
    return profile;
}

static void cleanup_profile(cmsHPROFILE profile) {
    if (profile) {
        cmsCloseProfile(profile);
    }
}

static void cleanup_transform(cmsHTRANSFORM transform) {
    if (transform) {
        cmsDeleteTransform(transform);
    }
}

int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Create dummy profiles for testing
    size_t half_size = Size / 2;
    cmsHPROFILE profiles[2];
    profiles[0] = create_dummy_profile_from_data(Data, half_size);
    profiles[1] = create_dummy_profile_from_data(Data + half_size, Size - half_size);
    
    // Extract parameters from fuzz data
    cmsUInt32Number params[8];
    size_t params_needed = sizeof(params) / sizeof(params[0]);
    size_t params_to_read = params_needed * sizeof(cmsUInt32Number);
    
    if (Size >= params_to_read) {
        memcpy(params, Data, params_to_read);
    } else {
        // Use default values if not enough data
        for (size_t i = 0; i < params_needed; i++) {
            params[i] = i + 1;
        }
    }
    
    // Test cmsCreateMultiprofileTransform
    cmsUInt32Number nProfiles = (params[0] % 254) + 1; // 1-255
    cmsUInt32Number InputFormat = params[1];
    cmsUInt32Number OutputFormat = params[2];
    cmsUInt32Number Intent = params[3] % 4; // 0-3
    cmsUInt32Number dwFlags = params[4];
    
    cmsHTRANSFORM transform1 = cmsCreateMultiprofileTransform(profiles, nProfiles, 
                                                              InputFormat, OutputFormat, 
                                                              Intent, dwFlags);
    cleanup_transform(transform1);
    
    // Test cmsFormatterForColorspaceOfProfile
    for (int i = 0; i < 2; i++) {
        cmsUInt32Number nBytes = (params[5] % 2) + 1; // 1 or 2
        cmsBool lIsFloat = params[6] % 2;
        cmsUInt32Number formatter = cmsFormatterForColorspaceOfProfile(profiles[i], nBytes, lIsFloat);
        (void)formatter; // Use result to avoid unused variable warning
    }
    
    // Test cmsCreateExtendedTransform
    cmsContext context = NULL;
    cmsBool BPC[2] = {params[0] % 2, params[1] % 2};
    cmsUInt32Number Intents[2] = {params[2] % 4, params[3] % 4};
    cmsFloat64Number AdaptationStates[2] = {(double)(params[4] % 100) / 100.0, 
                                           (double)(params[5] % 100) / 100.0};
    cmsHPROFILE hGamutProfile = NULL;
    cmsUInt32Number nGamutPCSposition = params[6] % 3;
    
    cmsHTRANSFORM transform2 = cmsCreateExtendedTransform(context, 2, profiles, BPC, Intents,
                                                         AdaptationStates, hGamutProfile,
                                                         nGamutPCSposition, InputFormat,
                                                         OutputFormat, dwFlags);
    cleanup_transform(transform2);
    
    // Test cmsCreateTransform
    cmsHTRANSFORM transform3 = cmsCreateTransform(profiles[0], InputFormat,
                                                 profiles[1], OutputFormat,
                                                 Intent, dwFlags);
    
    // Test cmsChangeBuffersFormat if we have a valid transform
    if (transform3) {
        cmsUInt32Number newInputFormat = params[7];
        cmsUInt32Number newOutputFormat = params[0];
        cmsBool changed = cmsChangeBuffersFormat(transform3, newInputFormat, newOutputFormat);
        (void)changed;
        
        // Test cmsDoTransformLineStride
        if (Size >= 100) {
            size_t buffer_size = 100;
            void* input_buffer = malloc(buffer_size);
            void* output_buffer = malloc(buffer_size);
            
            if (input_buffer && output_buffer) {
                memcpy(input_buffer, Data, buffer_size < Size ? buffer_size : Size);
                
                cmsUInt32Number PixelsPerLine = params[1] % 100 + 1;
                cmsUInt32Number LineCount = params[2] % 10 + 1;
                cmsUInt32Number BytesPerLineIn = params[3] % 100 + 1;
                cmsUInt32Number BytesPerLineOut = params[4] % 100 + 1;
                cmsUInt32Number BytesPerPlaneIn = params[5] % 100 + 1;
                cmsUInt32Number BytesPerPlaneOut = params[6] % 100 + 1;
                
                cmsDoTransformLineStride(transform3, input_buffer, output_buffer,
                                        PixelsPerLine, LineCount, BytesPerLineIn,
                                        BytesPerLineOut, BytesPerPlaneIn, BytesPerPlaneOut);
            }
            
            free(input_buffer);
            free(output_buffer);
        }
    }
    
    cleanup_transform(transform3);
    
    // Cleanup profiles
    for (int i = 0; i < 2; i++) {
        cleanup_profile(profiles[i]);
    }
    
    return 0;
}