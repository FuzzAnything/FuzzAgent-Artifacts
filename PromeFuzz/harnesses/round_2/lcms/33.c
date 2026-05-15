// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIsIntentSupported at cmsio1.c:864:20 in lcms2.h
// cmsCreateProofingTransform at cmsxform.c:1393:25 in lcms2.h
// cmsGetSupportedIntents at cmscnvrt.c:1192:27 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
// cmsCreateXYZProfile at cmsvirt.c:617:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// _cmsDefaultICCintents at cmscnvrt.c:655:25 in lcms2_plugin.h
// cmsGetSupportedIntentsTHR at cmscnvrt.c:1157:27 in lcms2.h
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

static cmsHPROFILE create_dummy_profile_from_data(const uint8_t* data, size_t size) {
    if (size == 0) {
        return cmsCreate_sRGBProfile();
    }
    
    // Try to open as ICC profile from memory
    cmsHPROFILE profile = cmsOpenProfileFromMem(data, size);
    if (profile != NULL) {
        return profile;
    }
    
    // Fallback to built-in profiles
    static int counter = 0;
    switch (counter++ % 3) {
        case 0: return cmsCreate_sRGBProfile();
        case 1: return cmsCreateLab4Profile(NULL);
        case 2: return cmsCreateXYZProfile();
        default: return cmsCreate_sRGBProfile();
    }
}

static void cleanup_transform(cmsHTRANSFORM transform) {
    if (transform != NULL) {
        cmsDeleteTransform(transform);
    }
}

static void cleanup_pipeline(cmsPipeline* pipeline) {
    if (pipeline != NULL) {
        cmsPipelineFree(pipeline);
    }
}

static void cleanup_profile(cmsHPROFILE profile) {
    if (profile != NULL) {
        cmsCloseProfile(profile);
    }
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }
    
    // Use first bytes to seed deterministic behavior
    uint32_t seed = *(uint32_t*)Data;
    const uint8_t* profile_data = Data + 4;
    size_t profile_size = Size > 4 ? Size - 4 : 0;
    
    // Create dummy profiles
    cmsHPROFILE input_profile = create_dummy_profile_from_data(profile_data, profile_size / 3);
    cmsHPROFILE output_profile = create_dummy_profile_from_data(profile_data + profile_size / 3, profile_size / 3);
    cmsHPROFILE proofing_profile = create_dummy_profile_from_data(profile_data + 2 * profile_size / 3, profile_size / 3);
    
    // Test cmsCreateTransform
    cmsUInt32Number input_format = (seed >> 0) & 0xFF;
    cmsUInt32Number output_format = (seed >> 8) & 0xFF;
    cmsUInt32Number intent = (seed >> 16) & 0x3;
    cmsUInt32Number flags = (seed >> 24) & 0xFF;
    
    cmsHTRANSFORM transform1 = cmsCreateTransform(input_profile, input_format,
                                                  output_profile, output_format,
                                                  intent, flags);
    cleanup_transform(transform1);
    
    // Test _cmsDefaultICCintents
    cmsUInt32Number n_profiles = 2;
    cmsUInt32Number intents_arr[2] = {intent, (seed >> 18) & 0x3};
    cmsHPROFILE profiles_arr[2] = {input_profile, output_profile};
    cmsBool bpc_arr[2] = {(seed >> 20) & 0x1, (seed >> 21) & 0x1};
    cmsFloat64Number adaptation_arr[2] = {0.5, 1.0};
    
    cmsPipeline* pipeline = _cmsDefaultICCintents(NULL, n_profiles, intents_arr,
                                                  profiles_arr, bpc_arr,
                                                  adaptation_arr, flags);
    cleanup_pipeline(pipeline);
    
    // Test cmsGetSupportedIntentsTHR
    cmsUInt32Number max_intents = 10;
    cmsUInt32Number codes[10];
    char* descriptions[10];
    
    cmsUInt32Number count1 = cmsGetSupportedIntentsTHR(NULL, max_intents, codes, descriptions);
    if (count1 > 0) {
        for (cmsUInt32Number i = 0; i < count1 && i < max_intents; i++) {
            if (descriptions[i] != NULL) {
                free(descriptions[i]);
            }
        }
    }
    
    // Test cmsIsIntentSupported
    cmsUInt32Number used_direction = (seed >> 22) & 0x1;
    cmsBool supported = cmsIsIntentSupported(input_profile, intent, used_direction);
    (void)supported; // Suppress unused variable warning
    
    // Test cmsCreateProofingTransform
    cmsUInt32Number proofing_intent = (seed >> 18) & 0x3;
    cmsHTRANSFORM transform2 = cmsCreateProofingTransform(input_profile, input_format,
                                                         output_profile, output_format,
                                                         proofing_profile, intent,
                                                         proofing_intent, flags);
    cleanup_transform(transform2);
    
    // Test cmsGetSupportedIntents
    cmsUInt32Number count2 = cmsGetSupportedIntents(max_intents, codes, descriptions);
    if (count2 > 0) {
        for (cmsUInt32Number i = 0; i < count2 && i < max_intents; i++) {
            if (descriptions[i] != NULL) {
                free(descriptions[i]);
            }
        }
    }
    
    // Cleanup profiles
    cleanup_profile(input_profile);
    cleanup_profile(output_profile);
    cleanup_profile(proofing_profile);
    
    return 0;
}