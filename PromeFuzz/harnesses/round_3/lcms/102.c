// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsFormatterForColorspaceOfProfile at cmspack.c:4025:27 in lcms2.h
// cmsFormatterForPCSOfProfile at cmspack.c:4044:27 in lcms2.h
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
// cmsCreateProofingTransformTHR at cmsxform.c:1361:25 in lcms2.h
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
        // Create a simple sRGB profile if data is too small
        return cmsCreate_sRGBProfile();
    }
    
    // Try to open profile from memory
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (hProfile != NULL) {
        return hProfile;
    }
    
    // Fallback to sRGB profile
    return cmsCreate_sRGBProfile();
}

static void cleanup_transform(cmsHTRANSFORM hTransform) {
    if (hTransform != NULL) {
        cmsDeleteTransform(hTransform);
    }
}

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Use the first byte to determine various parameters
    uint8_t control = Data[0];
    cmsUInt32Number nBytes = (control & 1) ? 2 : 1;
    cmsBool lIsFloat = (control & 2) ? TRUE : FALSE;
    cmsUInt32Number Intent = (control >> 2) & 3;
    cmsUInt32Number dwFlags = (control >> 4) & 0xF;
    
    // Create dummy profiles
    cmsHPROFILE hProfile1 = create_dummy_profile_from_data(Data, Size);
    cmsHPROFILE hProfile2 = create_dummy_profile_from_data(Data + (Size > 100 ? 100 : 0), 
                                                          Size > 100 ? Size - 100 : 0);
    cmsHPROFILE hProfile3 = create_dummy_profile_from_data(Data + (Size > 200 ? 200 : 0), 
                                                          Size > 200 ? Size - 200 : 0);
    
    // Test cmsFormatterForColorspaceOfProfile
    cmsUInt32Number formatter1 = cmsFormatterForColorspaceOfProfile(hProfile1, nBytes, lIsFloat);
    (void)formatter1; // Use result to avoid unused variable warning
    
    // Test cmsFormatterForPCSOfProfile
    cmsUInt32Number formatter2 = cmsFormatterForPCSOfProfile(hProfile1, nBytes, lIsFloat);
    (void)formatter2;
    
    // Test cmsCreateMultiprofileTransform
    cmsHPROFILE profiles1[3] = {hProfile1, hProfile2, hProfile3};
    cmsUInt32Number nProfiles1 = (Size % 3) + 1; // 1-3 profiles
    cmsUInt32Number InputFormat = TYPE_RGB_8;
    cmsUInt32Number OutputFormat = TYPE_CMYK_8;
    
    cmsHTRANSFORM hTransform1 = cmsCreateMultiprofileTransform(profiles1, nProfiles1, 
                                                              InputFormat, OutputFormat, 
                                                              Intent, dwFlags);
    cleanup_transform(hTransform1);
    
    // Test cmsCreateMultiprofileTransformTHR
    cmsContext context = cmsCreateContext(NULL, NULL);
    cmsHTRANSFORM hTransform2 = cmsCreateMultiprofileTransformTHR(context, profiles1, nProfiles1,
                                                                 InputFormat, OutputFormat,
                                                                 Intent, dwFlags);
    cleanup_transform(hTransform2);
    
    // Test cmsCreateExtendedTransform
    cmsBool BPC[3] = {FALSE, TRUE, FALSE};
    cmsUInt32Number Intents[3] = {Intent, Intent, Intent};
    cmsFloat64Number AdaptationStates[3] = {1.0, 1.0, 1.0};
    
    cmsHTRANSFORM hTransform3 = cmsCreateExtendedTransform(context, nProfiles1, profiles1,
                                                          BPC, Intents, AdaptationStates,
                                                          NULL, 0, InputFormat, OutputFormat,
                                                          dwFlags);
    cleanup_transform(hTransform3);
    
    // Test cmsCreateProofingTransformTHR
    cmsUInt32Number ProofingIntent = (control >> 6) & 3;
    cmsHTRANSFORM hTransform4 = cmsCreateProofingTransformTHR(context, hProfile1, InputFormat,
                                                             hProfile2, OutputFormat,
                                                             hProfile3, Intent, ProofingIntent,
                                                             dwFlags);
    cleanup_transform(hTransform4);
    
    // Cleanup
    cmsDeleteContext(context);
    cleanup_profile(hProfile1);
    cleanup_profile(hProfile2);
    cleanup_profile(hProfile3);
    
    return 0;
}